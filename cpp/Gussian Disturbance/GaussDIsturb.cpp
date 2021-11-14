#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <cstring>
#include <iostream>
#include <opencv2/core/eigen.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace Eigen;

int main() {
  // xml_1 for save result
  string mkdir_command = "mkdir ../res_xml";
  system(mkdir_command.c_str());
  for (int i = 0; i < 50; i++) {
    // 1. read xml
    cout << "Processing XML: " << to_string(i) << endl;

    string xml_path = "../xml/%d.xml";
    char path_temp[512];
    sprintf(path_temp, xml_path.c_str(), i);
    FileStorage fs(path_temp, FileStorage::READ);

    Mat RT, DC;
    fs["RT"] >> RT;
    fs["DistortionCam"] >> DC;

    // 2. extract R & T
    Mat R = Mat_<float>(3, 3);
    Mat T = Mat_<float>(3, 1);

    for (int r = 0; r < 3; r++)
      for (int c = 0; c < 3; c++)
        R.at<float>(r, c) = RT.at<float>(r, c);

    for (int m = 0; m < 3; m++)
      T.at<float>(m, 0) = RT.at<float>(m, 3);

    // 3. transfer R to Euler
    Matrix3f E_R;
    cv2eigen(R, E_R);

    Vector3f euler_angles = E_R.eulerAngles(2, 1, 0);
    cout << "yaw(z) pitch(y) roll(x) = " << euler_angles.transpose() << endl;

    // 4. add gauss noise, both Euler_R and matrix T
    // Define random generator with Gaussian distribution
    const float mean = 0.0;
    const float stddev = 0.1;
    default_random_engine generator;
    normal_distribution<float> dist(mean, stddev);

    for (auto &x : euler_angles)
      x += dist(generator);
    cout << "Add Gauss Noise: yaw(z) pitch(y) roll(x) = "
         << euler_angles.transpose() << endl;

    Mat T_new = T;

    for (int r = 0; r < 3; r++)
      T_new.at<float>(r, 1) += dist(generator);

    // 5. Euler to R;
    Eigen::Matrix3f rotation_matrix;
    rotation_matrix =
        Eigen::AngleAxisf(euler_angles[0], Eigen::Vector3f::UnitZ()) *
        Eigen::AngleAxisf(euler_angles[1], Eigen::Vector3f::UnitY()) *
        Eigen::AngleAxisf(euler_angles[2], Eigen::Vector3f::UnitX());

    Mat R_new;
    eigen2cv(rotation_matrix, R_new);

    // 6. R_new + T_new = RT_new
    Mat RT_new;
    hconcat(R_new, T_new, RT_new);

    Mat tmp = (Mat_<float>(1, 4) << 0, 0, 0, 1);
    vconcat(RT_new, tmp, RT_new);

    // 7. read K and reshape it from 3x3 to 4x4;
    Mat K;
    fs["IntrinsicCam"] >> K;
    Mat tmp_zero = (Mat_<float>(3, 1) << 0, 0, 0);

    Mat K_4x4;
    hconcat(K, tmp_zero, K_4x4);
    vconcat(K_4x4, tmp, K_4x4);

    // 8. generate P = K_4x4 * RT_new
    Mat P_new = K_4x4 * RT_new;

    // 9. write back result
    string new_xml_path = "../res_xml/%d.xml";

    char new_path_temp[512];
    sprintf(new_path_temp, new_xml_path.c_str(), i);
    FileStorage new_fs(new_path_temp, FileStorage::WRITE);

    new_fs << "P" << P_new;
    new_fs << "RT" << RT_new;
    new_fs << "IntrinsicCam" << K;
    new_fs << "DistortionCam" << DC;

    fs.release();
    new_fs.release();
  }

  return 0;
}
