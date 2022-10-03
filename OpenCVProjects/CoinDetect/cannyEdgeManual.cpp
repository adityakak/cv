#include <iostream>

#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;
const int rows = 3024; //Second Number in JPG
const int cols = 4032; //First Number in JPG

vector<int> greyscaled(Mat img)
{
    vector<int> greyed((cols * rows) + 1);
    
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            Vec3b colour = img.at<Vec3b>(r, c);
            int greyScaledVal = (int)((colour.val[0] + colour.val[1] + colour.val[2]) / 3);
            greyed[(r * cols) + c] = greyScaledVal;
        }
    }
    return greyed;
}


vector<int> sobelGradient(vector<int> greyed) {
    vector<int> {1, 0, -1, 2, 0, -2, 1, 0, -1};
    vector<int> gradientX((cols * rows) + 1);
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(r == 0 || r == rows - 1 || c == 0 || c == cols - 1) {
                gradientX[(r * cols) + c] = 0;
            }
            else {
                int topLeft = ((r - 1) * cols) + (c - 1);
                int middleLeft = (r * cols) + (c - 1);
                int bottomLeft = ((r + 1) * cols) + (c - 1);

                int topRight = ((r - 1) * cols) + (c + 1);
                int middleRight = (r * cols) + (c + 1);
                int bottomRight = ((r + 1) * cols) + (c + 1);

                gradientX[(r * cols) + c] = greyed[topLeft] + (2 * greyed[middleLeft]) + greyed[bottomLeft] + (-1 * greyed[topRight]) + (-2 * greyed[middleRight]) + (-1 * greyed[bottomRight]);
            }
        }
    }
    vector<int> gradientY((cols * rows) + 1);
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(r == 0 || r == rows - 1 || c == 0 || c == cols - 1) {
                gradientX[(r * cols) + c] = 0;
            }
            else {
                int topLeft = ((r - 1) * cols) + (c - 1);
                int bottomLeft = ((r + 1) * cols) + (c - 1);

                int top = ((r - 1) * cols) + c;
                int bottom = ((r + 1) * cols) + c;

                int topRight = ((r - 1) * cols) + (c + 1);
                int bottomRight = ((r + 1) * cols) + (c + 1);

                gradientY[(r * cols) + c] = greyed[topLeft] + (2 * greyed[top]) + greyed[topRight] + (-1 * greyed[bottomLeft]) + (-2 * greyed[bottom]) + (-1 * greyed[bottomRight]);
            }
        }
    }
    vector<int> gradient((cols * rows) + 1);
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            gradient[(r * cols) + c] = ceil(sqrt(pow(gradientX[(r * cols) + c], 2) + pow(gradientY[(r * cols) + c], 2)));
        }
    }
    return gradient;
}


int main()
{
    ofstream fout("cvManual.ppm");
    if (fout.fail())
        return -1;
    fout << "P3\n"
         << cols << " "
         << rows << '\n'
         << "255\n";
    string img = "coinsEasy3.jpg";
    //string img = "lenna.jpg";
    //string img = "colorimage.jpg";
    //string img = "Parrots.png";
    Mat srcImg = imread(img);
    if (!srcImg.data)
    {
        return 1;
    }
    //imshow("srcImage", srcImg);
    //waitKey(0);

    vector<int> greyedOut = greyscaled(srcImg);
    vector<int> outlined = sobelGradient(greyedOut);
    int iterate = 0;
    for(int greyScaleVals: outlined) {
        if(iterate != 0 && iterate % cols == 0) {
            fout << '\n';
        }
        if(greyScaleVals < 185) {
            fout << 0 << " "  << 0 << " " << 0 << " ";
        }
        else {
            //fout << greyScaleVals << " "  << greyScaleVals << " " << greyScaleVals << " ";
            fout << 255 << " "  << 255 << " " << 255 << " ";
        }
        iterate++;
    }

    return 0;
}