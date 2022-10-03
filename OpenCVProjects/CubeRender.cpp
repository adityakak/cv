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

using namespace std;
using namespace cv;

Mat projection(Mat rotation, Mat translation) {
    vector<Mat> pointsW;
    for(int i = 0; i < 8; i++) {
        pointsW.push_back(Mat::zeros(4, 1, CV_64F));
    }
    pointsW[0].at<double>(1, 0) = 1; //0, 1, 0
    pointsW[0].at<double>(3, 0) = 1;

    pointsW[1].at<double>(1, 0) = 1; //0, 1, 1
    pointsW[1].at<double>(2, 0) = 1;
    pointsW[1].at<double>(3, 0) = 1;

    pointsW[2].at<double>(0, 0) = 1; //1, 1, 1
    pointsW[2].at<double>(1, 0) = 1;
    pointsW[2].at<double>(2, 0) = 1;
    pointsW[2].at<double>(3, 0) = 1;

    pointsW[3].at<double>(0, 0) = 1; //1, 1, 0
    pointsW[3].at<double>(1, 0) = 1;
    pointsW[3].at<double>(3, 0) = 1;

    pointsW[4].at<double>(3, 0) = 1; //0, 0, 0

    pointsW[5].at<double>(0, 0) = 1; //1, 0, 0
    pointsW[5].at<double>(3, 0) = 1;

    pointsW[6].at<double>(0, 0) = 1; //1, 0, 1
    pointsW[6].at<double>(2, 0) = 1;
    pointsW[6].at<double>(3, 0) = 1;

    pointsW[7].at<double>(2, 0) = 1; //0, 0, 1
    pointsW[7].at<double>(3, 0) = 1;

    Mat p;
    hconcat(rotation, translation, p);

    Mat iden = Mat::eye(3, 3, CV_64F);
    iden.at<double>(0, 0) = 2;
    iden.at<double>(1, 1) = 1;
    iden.at<double>(0, 2) = .5;
    iden.at<double>(0, 2) = .5;

    vector<Mat> imageCoordinates;
    int index = 0;
    for(int j = 0; j < pointsW.size(); j++) {
        pointsW[index] = p * pointsW[index];
        pointsW[index] = iden * pointsW[index];
        Mat pointMat = Mat::zeros(2, 1, CV_64F);
        if (pointsW[index].at<double>(2, 0) != 0) {
            pointMat.at<double>(0, 0) = (pointsW[index].at<double>(0, 0) / pointsW[index].at<double>(2, 0)) * 200;
            pointMat.at<double>(1, 0) = (pointsW[index].at<double>(1, 0) / pointsW[index].at<double>(2, 0)) * 200;
        }
        else {
            pointMat.at<double>(0, 0) = pointsW[index].at<double>(0, 0) * 200;
            pointMat.at<double>(1, 0) = pointsW[index].at<double>(1, 0) * 200;
        }
        imageCoordinates.push_back(pointMat);
        index += 1;
    }
    Mat image = Mat::zeros(Size(800, 400), CV_8UC1);
    vector<Point> corLoc;
    for(int i = 0; i < imageCoordinates.size(); i++) {
        circle(image, Point(imageCoordinates.at(i).at<double>(0, 0), imageCoordinates.at(i).at<double>(0, 1)), 5, Scalar(255, 125 , 125), 2);
        corLoc.push_back(Point(imageCoordinates.at(i).at<double>(0, 0), imageCoordinates.at(i).at<double>(0, 1)));
    }
    line(image, corLoc[7], corLoc[1], Scalar(255, 125 , 125), 2, LINE_AA);
    line(image, corLoc[2], corLoc[6], Scalar(255, 125 , 125), 2, LINE_AA);
    line(image, corLoc[7], corLoc[6], Scalar(255, 125 , 125), 2, LINE_AA);
    line(image, corLoc[1], corLoc[2], Scalar(255, 125 , 125), 2, LINE_AA); //Front Face

    line(image, corLoc[4], corLoc[0], Scalar(255, 125 , 125), 2, LINE_AA);
    line(image, corLoc[5], corLoc[3], Scalar(255, 125 , 125), 2, LINE_AA);
    line(image, corLoc[0], corLoc[3], Scalar(255, 125 , 125), 2, LINE_AA);
    line(image, corLoc[4], corLoc[5], Scalar(255, 125 , 125), 2, LINE_AA); //Back Face

    line(image, corLoc[7], corLoc[4], Scalar(255, 125 , 125), 2, LINE_AA);
    line(image, corLoc[1], corLoc[0], Scalar(255, 125 , 125), 2, LINE_AA);
    line(image, corLoc[6], corLoc[5], Scalar(255, 125 , 125), 2, LINE_AA);
    line(image, corLoc[2], corLoc[3], Scalar(255, 125 , 125), 2, LINE_AA);
    return image;
}

int main() {
    vector<Mat> images;
    double xTheta = 0, yTheta = 0, zTheta = 0;
    while(yTheta <= 360) {
        double xThetaRad = (xTheta * M_PI) / 180, yThetaRad = (yTheta * M_PI) / 180, zThetaRad = (zTheta * M_PI) / 180;

        Mat rotX = Mat::zeros(3, 3, CV_64F);
        rotX.at<double>(0, 0) = 1;
        rotX.at<double>(1, 1) = cos(xThetaRad);
        rotX.at<double>(1, 2) = -sin(xThetaRad);
        rotX.at<double>(2, 1) = sin(xThetaRad);
        rotX.at<double>(2, 2) = cos(xThetaRad);

        Mat rotY = Mat::zeros(3, 3, CV_64F);
        rotY.at<double>(0, 0) = cos(yThetaRad);
        rotY.at<double>(0, 2) = sin(yThetaRad);
        rotY.at<double>(1, 1) = 1;
        rotY.at<double>(2, 0) = -sin(yThetaRad);
        rotY.at<double>(2, 2) = cos(yThetaRad);

        Mat rotZ = Mat::zeros(3, 3, CV_64F);
        rotZ.at<double>(0, 0) = cos(zThetaRad);
        rotZ.at<double>(0, 1) = -sin(zThetaRad);
        rotZ.at<double>(1, 0) = sin(zThetaRad);
        rotZ.at<double>(1, 1) = cos(zThetaRad);
        rotZ.at<double>(2, 2) = 1;

        Mat rotation = (rotX * rotY) * rotZ;

        Mat translation = Mat::zeros(3, 1, CV_64F);
        translation.at<double>(0, 0) = 2;
        translation.at<double>(1, 0) = 2;
        translation.at<double>(2, 0) = 3;
        cout << yTheta << endl;
        images.push_back(projection(rotation, translation));
        yTheta += .5;
        //xTheta += .5;
        zTheta += .5;
    }
    //VideoWriter video("rotation.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 120, Size(800, 400), true);
    mkdir("KakRotationJPEGs");
    int number = 0;
    for(auto item: images) {
        //video << item;
        string output = string("KakRotationJPEGs/") + "ImageRotation" + to_string(number) + ".jpg";
        imwrite(output, item);
        number += 1;
    }
    //video.release();
}
