#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
//using namespace cv;

cv::Point click;
int is_click = 0;

void on_mouse(int event, int x, int y, int flags, void *ustc) {
    // Mat &image = *(cv::Mat *) ustc;//这样就可以传递Mat信息了，很机智
    switch (event) {
        case cv::EVENT_LBUTTONDOWN: // click left button;
        {
            click = cv::Point(x, y); // for points, (x, y) means (column, row);
            is_click = 1;
            break;
        }
        case cv::EVENT_RBUTTONDOWN: // click right button;
        {
            click = cv::Point(x, y);
            is_click = 2;
            break;
        }
        default:
            break;
    }
}

int main() {
    const char object_mask_path[512] = "../basketball/seg/%04d.png";
    const char res_mask_path[512] = "../basketball/mask/%04d.png";
    const int number = 23; // number of cameras

    vector<cv::Vec3b> vec_fgr_value;   // BGR value of left click position;
    vector<cv::Vec3b> vec_bgr_value;  // BGR value of right click position;

    namedWindow("mask", cv::WINDOW_NORMAL);
    cv::setMouseCallback("mask", on_mouse);
    
    vector<cv::Mat> original_img; // original images;
    vector<cv::Mat> result_mask; // result images;

    for (int track_index = 0; track_index < number; track_index++) {
        char path_temp[512];
        // use "track_index" to format "object_mask_path" which will be saved into "path_temp";
        sprintf(path_temp, object_mask_path, track_index);

        cv::Mat mask = cv::imread(path_temp);
        original_img.push_back(mask);  // save object image into masks(vector);

        cv::Mat res = cv::Mat::zeros(mask.rows, mask.cols, CV_8UC1); // 8UC1: single channel for mask image;
        result_mask.push_back(res);
    }

    int next = 0;
    bool update = false;
    bool finish = false;
    char key = 0;
    while (key != 'q') {
        key = cv::waitKey(1); // waitKey: return the code of pressed key;

        // matting automatically;
        if (key == 's') {
            cout << "finish\n";
            finish = true;
        }

        if (key == 'd' || finish) {
            cout << "next\n";
            next++;
            update = true; // signal to process image
        }

        if (next >= number)
            break;

        // left button - signal foreground area
        if (is_click == 1) {
            is_click = 0; // initial value;
            // use Vec3b instead of uchar? uchar is for CV_8UC1, Vec3b is for CV_8UC3(RGB:3 channels).
            // access pixel's value, while its location is row(click.y), column(click.x);
            // return vec3b: a vector with 3 value <B G R>
            cv::Vec3b temp = original_img[next].ptr<cv::Vec3b>(click.y)[click.x];
            vec_fgr_value.push_back(temp);
            cout << "1" << endl;
            update = true; // begin to process after user's click;
        }

        // right button - signal background area
        if (is_click == 2) {
            is_click = 0;
            cv::Vec3b temp = original_img[next].ptr<cv::Vec3b>(click.y)[click.x];
            vec_bgr_value.push_back(temp);
            cout << "2" << endl;
            update = true; // begin to process after user's click;
        }

        // processing image;
        if (update) {
            for (int i = 0; i < original_img[next].rows; i++) {
                for (int j = 0; j < original_img[next].cols; j++) {
                    for (const auto & k : vec_fgr_value) {
                        //
                        if (original_img[next].ptr<cv::Vec3b>(i)[j] == k) {
                            original_img[next].ptr<cv::Vec3b>(i)[j] = cv::Vec3b(255, 255, 255); // foreground area;
                            result_mask[next].ptr<uchar>(i)[j] = 255;
                        }
                    }
                    for (const auto & k : vec_bgr_value) {
                        if (original_img[next].ptr<cv::Vec3b>(i)[j] == k) {
                            original_img[next].ptr<cv::Vec3b>(i)[j] = cv::Vec3b(0, 0, 0); // background area;
                            result_mask[next].ptr<uchar>(i)[j] = 200; // gray area in result image;
                        }
                    }
                }
            }
            update = false;
        }
        imshow("mask", original_img[next]);
    }
    // save matting results;
    for (int track_index = 0; track_index < number; track_index++) {
        char path_temp[512];
        sprintf(path_temp, res_mask_path, track_index);
        imwrite(path_temp, result_mask[track_index]);
    }

    return 0;
}
