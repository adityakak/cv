#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types_c.h>

using namespace std;
using namespace cv;
RNG rng(12345);

Mat draw(Mat img, Mat cubeCoords) {
    //cout << cubeCoords << endl;

    vector<Point> base;
    vector<vector<Point>> based;
    for(int i = 0; i < 4; i++) {
        float x = cubeCoords.at<float>(i, 0);
        float y = cubeCoords.at<float>(i, 1);
        base.push_back(Point(int(x), int(y)));
    }
    based.push_back(base);
    drawContours(img, based, -1, Scalar(255, 0, 0), 4);

    vector<Point> pillarPoints{Point(0, 4), Point(1, 5), Point(2, 6), Point(3, 7)};
    for(auto zipped: pillarPoints) {
        line(img, Point(int(cubeCoords.at<float>(zipped.x, 0)), int(cubeCoords.at<float>(zipped.x, 1))), 
        Point(int(cubeCoords.at<float>(zipped.y, 0)), int(cubeCoords.at<float>(zipped.y, 1))), Scalar(0, 255, 0), 4);
    }
    vector<Point> top;
    vector<vector<Point>> topped;
    for(int i = 4; i < 8; i++) {
        float x = cubeCoords.at<float>(i, 0);
        float y = cubeCoords.at<float>(i, 1);
        top.push_back(Point(int(x), int(y)));
    }
    topped.push_back(top);
    drawContours(img, topped, -1, Scalar(0, 0, 255), 4);
    return img;
}

int main() {
    VideoCapture cap("noChessBoard.mov");
    int frameCount = 0;
    vector<Mat> videoOuput;
    while(1) {
        Mat frame, gray, cannyOut, output;
        cap >> frame;
        flip(frame, frame, 0);
        flip(frame, frame, ROTATE_180);
        if(frameCount % 6 == 0 && frameCount < 365) {
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            vector<Point3f> axis{Point3f(0, 0, 0), Point3f(0, 3, 0), Point3f(3, 3, 0), Point3f(3, 0, 0), Point3f(0, 0, -3), Point3f(0, 3, -3), Point3f(3, 3, -3), Point3f(3, 0, -3)};
            vector<float> rotation {-0.326392, 0.913686, 2.31035};
            vector<float> translation {5.69355, -9.88411, 22.212};
            vector<float> camMat {1471.521725309487, 0, 800.053034144515, 0, 1163.293357551302, 1152.432265324065, 0, 0, 1};
            vector<float> distortion {0.047040275838388, -4.266196704381556, -0.3452438607704736, 0.02008757746841202, 6.216382832928919};
            Mat cam = Mat (camMat).reshape(0, 3);
            Mat dist = Mat(distortion);
            projectPoints(axis, rotation, translation, cam, dist, output);
            frame = draw(frame, output);
            videoOuput.push_back(frame);

        }
        frameCount += 1;
        //cout << frameCount << endl;
        if(frameCount >= 365) {
            break;
        }
    }
    VideoWriter video("outcpp2.avi", VideoWriter::fourcc('M','J','P','G'), 10, Size(1920, 1080));
    for(auto imageOut: videoOuput) {
        video.write(imageOut);
    }
    //cout << "Done";
}
