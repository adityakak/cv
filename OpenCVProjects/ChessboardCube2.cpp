#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/types_c.h>

using namespace std;
using namespace cv;

int chessboard[2]{7, 7};

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
    vector<vector<Point3f>> objPoints;
    vector<vector<Point2f>> imgPoints;
    vector<Mat> videoOuput;

    vector<Point3f> obj;
    for(int i{0}; i < chessboard[1]; i++) {
        for(int j{0}; j < chessboard[0]; j++) {
            obj.push_back(Point3f(j, i, 0));
        }
    }
    vector<Point3f> axis{Point3f(0, 0, 0), Point3f(0, 3, 0), Point3f(3, 3, 0), Point3f(3, 0, 0), Point3f(0, 0, -3), Point3f(0, 3, -3), Point3f(3, 3, -3), Point3f(3, 0, -3)};

    VideoCapture cap("ChessboardCube.mov");
    TermCriteria criteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.001);
    int frameCount = 0;
    while(1) {
        Mat frame, gray;
        Mat camMat, dist, rot, trans, output;
        vector<Point2f> corners;
        bool found;
        cap >> frame;
        flip(frame, frame, 0);
        flip(frame, frame, ROTATE_180);
        if (frame.empty())
            break;
        if(frameCount % 12 == 0) {
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            found = findChessboardCorners(gray, Size(chessboard[0], chessboard[1]), corners);
            if(found) {
                cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), criteria);
                auto corners2 = corners;

                objPoints.push_back(obj);
                imgPoints.push_back(corners);
                calibrateCamera(objPoints, imgPoints, Size(gray.rows, gray.cols), camMat, dist, rot, trans);
                vector<float> rotation;
                vector<float> translation;
                for(int i = 0; i < 3; i++) {
                    rotation.push_back(rot.at<double>(0, i));
                    translation.push_back(trans.at<double>(0, i));
                }    

                //cout << rot << endl;
                projectPoints(axis, rotation, translation, camMat, dist, output);

                cout << axis << endl;
                for(auto i: rotation) {
                    cout << i << ", ";
                }
                cout << endl;
                for(auto i: translation) {
                    cout << i << ", ";
                }
                cout << camMat << endl;
                cout << dist << endl;
                cout << endl;
                cout << endl;
                frame = draw(frame, output);
                imshow("Img", frame);
                waitKey(0);
                videoOuput.push_back(frame);
            }
        }
        frameCount += 1;
        cout << frameCount;
        //imshow("Img", frame);
        //waitKey(10);
        //cout << endl;
    }
    VideoWriter video("outcpp.avi", VideoWriter::fourcc('M','J','P','G'), 10, Size(1920, 1080));
    for(auto imageOut: videoOuput) {
        video.write(imageOut);
    }
}