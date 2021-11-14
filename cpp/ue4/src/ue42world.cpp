#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace std;
using namespace cv;

int main() {
    string para_path = "../basketball/para/cameraParams%d.xml";
    string xml_path = "../basketball/xml/%d.xml";

    int nums = 2;
    for (int i = 0; i < nums; i++) {
        char src_path[256] = {0};
        char dst_path[256] = {0};
        // path.c_str(): string -> "const char *" which can be used in sprintf;
        sprintf(src_path, para_path.c_str(), i);
        sprintf(dst_path, xml_path.c_str(), i);
        cout << "read " << src_path << endl;
        cout << "read " << dst_path << endl;

        // read "cameraParams?.xml"'s content;
        cv::FileStorage fs(src_path, cv::FileStorage::READ);
        if (!fs.isOpened()) {
            std::cerr << "Can't open file " << src_path << std::endl;
        }
        cv::Mat KRt44;
        cv::Mat Rt44;
        cv::Mat K33;

        cv::Mat P, IntrinsicCam, DistortionCam, RT;
        cv::Mat R, t;

        fs["transProjection"] >> P;
        fs["IntrinsicCam"] >> IntrinsicCam;
        fs["DistortionCam"] >> DistortionCam;
        fs["transR"] >> R;
        fs["transT"] >> t;
        fs.release();

        P.convertTo(P, CV_32FC1);
        IntrinsicCam.convertTo(IntrinsicCam, CV_32FC1);

        hconcat(R, t, RT); // RT = [R, T]; horizontal concatenation;
        RT.convertTo(RT, CV_32FC1);
        Mat temp = (Mat_<float>(1, 4) << 0, 0, 0, 1.0);
        vconcat(RT, temp, RT); // vertical concatenation;

        Mat IntrinsicCam_copy = IntrinsicCam.clone();
        Mat tmp = (Mat_<float>(3, 1) << 0, 0, 0);
        cout << "temp_IntrinsicCam " << IntrinsicCam_copy << endl;
        hconcat(IntrinsicCam_copy, tmp, IntrinsicCam_copy);
        vconcat(IntrinsicCam_copy, temp, IntrinsicCam_copy);

        // (x, y, z) -> (y, x, -z);
        Mat sRt1 = (Mat_<float>(4, 4) << 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 1);
        P = IntrinsicCam_copy * RT * sRt1;
        // camera <- UE4world * UE4world <- realworld;
        RT = RT * sRt1;

        cv::FileStorage fd(dst_path, cv::FileStorage::WRITE); //填入写操作

        P.convertTo(P, CV_64F);
        IntrinsicCam.convertTo(IntrinsicCam, CV_64F);
        DistortionCam.convertTo(DistortionCam, CV_64F);
        fd << "P" << P;
        fd << "IntrinsicCam" << IntrinsicCam;
        fd << "DistortionCam" << DistortionCam;

        fd.release();
    }
    system("pause");

    return 0;
}
