#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <math.h>
#include <unordered_set>
#include <algorithm>
#include <tuple>

using namespace std;
using namespace cv;

int main() {
    string img = "coinsEasy3.jpg";
    Mat src = imread(img);

    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, 5);

    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, gray.rows/16, 100, 30, 70, 135);

    double total = 0;
    for(int i = 0; i < circles.size(); i++) {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        int radius = c[2];
        //cout << radius << ": "  << center.x << " " << center.y << endl;
        if(radius <= 95) {
            circle(src, center, radius, Scalar(255, 0, 0), 3, LINE_AA); //BGR
            total += .01;
        }
        else if (radius > 95 && radius < 105) {
            circle(src, center, radius, Scalar(0, 255, 0), 3, LINE_AA);
            total += .05;
        }
        else {
            circle(src, center, radius, Scalar(0, 0, 255), 3, LINE_AA);
            total += .25;
        }
    }
    cout << total;
    imwrite("OpenCvOutput.jpg", src);
}