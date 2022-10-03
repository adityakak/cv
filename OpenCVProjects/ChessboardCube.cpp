#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <math.h>
#include <unordered_set>
#include <algorithm>
#include <tuple>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <dir.h>
#include <filesystem>
#include <opencv2/videoio.hpp>
#include <opencv2/core/types_c.h>

using namespace std;
using namespace cv;

void drawCube(Mat img, vector<Point2f> corners, vector<vector<Point2f>> imgpts) {
    auto cornerPos = corners[0];
    line(img, cornerPos, imgpts[0][cornersFound.width - 1], Scalar(255, 0, 0));
    line(img, cornerPos, imgpts[0][cornersFound.width * (cornersFound.height - 1)], Scalar(255, 0, 0));
    line(img, cornerPos, imgpts[0][1], Scalar(255, 0, 0));
    cv::imshow("Frame", img);
    cv::waitKey(5);
}

int main() {
    VideoCapture cap("ChessboardCube.mov");
    
    Size outputSize;
    vector<Mat> output;
    int frameCount = 0;
    Mat cameraMat;
    Mat distMat;
    Mat rMat;
    Mat tMat;
    while(1) {
        Mat frame;
        cap >> frame;
        outputSize = frame.size();
        flip(frame, frame, 0);
        flip(frame, frame, ROTATE_180);
        if (frame.empty())
            break;
        if(frameCount == 0) {
            vector<Point3f> obj;
            for(int i = 0; i < 7; i++) {
                for(int j = 0; j < 7; j++) {
                    obj.push_back(Point3f(j, i, 0));
                }
            }
            vector<vector<Point3f>> objPoints;
            objPoints.push_back(obj);
            Mat gray = frame;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            
            vector<Point2f> corners;
            auto found = findChessboardCorners(gray, Size(7, 7), corners);
            if(found) {
                cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.001));
            }
            drawChessboardCorners(frame, Size(7, 7), Mat(corners), found);
            vector<vector<Point2f>> imgPoints;
            imgPoints.push_back(corners);
            calibrateCamera(objPoints, imgPoints, frame.size(), cameraMat, distMat, rMat, tMat);
            drawCube(frame, corners, imgPoints);
        }
        else {
            Rect* roi = NULL;
            Mat newCamera = getOptimalNewCameraMatrix(cameraMat, distMat, frame.size(), 0, frame.size(), roi);
            Mat output;
            undistort(frame, output, cameraMat, distMat, newCamera);

            Mat gray = frame;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            vector<Point2f> corners;
            bool found = false;
            int index = 0;
            vector<Point> toTry{
                Point(7, 7),
                Point(6, 7),
                Point(7, 6),
                Point(6, 6),
                Point(6, 5),
                Point(5, 6),
                Point(5, 5),
                Point(5, 4),
                Point(4, 5),
                Point(4, 4),
                Point(3, 4),
                Point(4, 3),
                Point(3, 3),
            };
            while(!found) {
                found = findChessboardCorners(gray, Size(toTry[index].x, toTry[index].y), corners);
                if(!found)
                    index += 1;
            }
            found = findChessboardCorners(gray, Size(toTry[index].x, toTry[index].y), corners);
            drawChessboardCorners(frame, Size(toTry[index].x, toTry[index].y), Mat(corners), found);
        }

        cv::imshow("Frame", frame);
        cv::waitKey(5);
        output.push_back(frame);
        frameCount += 1;
    }
    VideoWriter video("out.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, outputSize, true);
    for(auto frames: output) {
        video << frames;
    }

}