#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <math.h>
#include <unordered_set>

using namespace std;
using namespace cv;
const int rows = 456; //Second Number in JPG
const int cols = 855; //First Number in JPG

void printVector(vector<double> v)
{
    for (auto a : v)
    {
        std::cout << a << " ";
    }
}


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
    std::cout << "Calculating X-Gradient\n";
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
    std::cout << "Calculating Y-Gradient\n";
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
    std::cout << "Calculating Total-Gradient\n";
    int maxGradientValue = INT_MIN;
    vector<int> gradient((cols * rows) + 1);
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            int currValue = ceil(sqrt(pow(gradientX[(r * cols) + c], 2) + pow(gradientY[(r * cols) + c], 2)));
            gradient[(r * cols) + c] = currValue;
            if(currValue > maxGradientValue) {
                maxGradientValue = currValue;
            }
        }
    }
    std::cout << "Max Value In Gradient is: " << maxGradientValue << "\n";
    std::cout << "Calculating Angles\n";
    vector<double> angles((cols * rows) + 1);
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            angles[(r * cols) + c] = (atan2(gradientY[(r * cols) + c], gradientX[(r * cols) + c]) * 180) / M_PI;
        }
    }
    
    std::cout << "Applying Non-Max Supression\n";
    double maxSort = .09 * maxGradientValue;
    double minSort = .05 * maxSort;
    unordered_set<int> strong;
    vector<int> weak;
    for(int r = 0; r < rows; r++) {
        for(int c = 0; c < cols; c++) {
            if(r == 0 || r == rows - 1 || c == 0 || c == cols - 1) {
                gradient[(r * cols) + c] = 0;
            }
            else {
                double angleValue = angles[(r * cols) + c];
                int gradientValue = gradient[(r * cols) + c];
                if (((-22.5 <= angleValue) && (angleValue < 22.5)) || (((-180 <= angleValue) && (angleValue < -157.5)) || ((157.5 < angleValue) && (angleValue <= 180)))) {
                    //Horizontal
                    int middleLeft = (r * cols) + (c - 1);
                    int middleRight = (r * cols) + (c + 1);
                    if (gradient[middleLeft] > gradientValue || gradient[middleRight] > gradientValue) {
                        gradient[(r * cols) + c] = 0;
                    }
                }
                else if (((22.5 <= angleValue) && (angleValue < 67.5)) || ((-157.5 <= angleValue) && (angleValue < -112.5))) {
                    //North East - South West
                    int topRight = ((r - 1) * cols) + (c + 1);
                    int bottomLeft = ((r + 1) * cols) + (c - 1);
                    if (gradient[topRight] > gradientValue || gradient[bottomLeft] > gradientValue) {
                        gradient[(r * cols) + c] = 0;
                    }
                }
                else if (((67.5 <= angleValue) && (angleValue < 112.5)) || ((-112.5 <= angleValue) && (angleValue < -67.5))) {
                    //Up Down
                    int top = ((r - 1) * cols) + c;
                    int bottom = ((r + 1) * cols) + c;
                    if (gradient[top] > gradientValue || gradient[bottom] > gradientValue) {
                        gradient[(r * cols) + c] = 0;
                    }
                }
                else {
                    //North West - South East
                    int topLeft = ((r - 1) * cols) + (c - 1);
                    int bottomRight = ((r + 1) * cols) + (c + 1);
                    if (gradient[topLeft] > gradientValue || gradient[bottomRight] > gradientValue) {
                        gradient[(r * cols) + c] = 0;
                    }
                }
                /*
                double sortingValues = gradient[(r * cols) + c] / maxGradientValue;
                std::cout << gradient[(r * cols) + c] << "\n";
                if(sortingValues >= .85) {
                    strong.emplace((r * cols) + c);
                }
                else if(.5 <= sortingValues < .85) {
                    weak.push_back((r * cols) + c);
                }
                */
                if(gradient[(r * cols) + c] > maxSort) {
                    strong.emplace((r * cols) + c);
                }
                else if((minSort <= gradient[(r * cols) + c]) && (gradient[(r * cols) + c] < maxSort)) {
                    weak.push_back((r * cols) + c);
                }
            }
        }
    }
    
    std::cout << "Applying Hysteresis\n";
    std::cout << "Weak Pixel Length: " << weak.size() << "\n";
    unordered_set<int> toAdd;
    for(auto values: weak) {
        bool toHide = true;
        int xPos = values % cols;
        int yPos = values / cols;

        vector<int> places{
            ((yPos - 1) * cols) + (xPos - 1),
            ((yPos + 1) * cols) + (xPos - 1),
            ((yPos - 1) * cols) + xPos,
            ((yPos + 1) * cols) + xPos,
            ((yPos - 1) * cols) + (xPos + 1),
            ((yPos + 1) * cols) + (xPos + 1),
            (yPos * cols) + (xPos - 1),
            (yPos * cols) + (xPos + 1)
        };
        for(auto toCheck: places) {
            if(strong.find(toCheck) != strong.end()) {
                toAdd.emplace(values);
                toHide = false;
                break;
            }
         }
        if(toHide) {
            gradient[values] = 0;
        }
    }
    
    //printVector(angles);
    return gradient;
}


int main()
{
    ofstream fout("cvManualSupressed.ppm");
    if (fout.fail())
        return -1;
    fout << "P3\n"
         << cols << " "
         << rows << '\n'
         << "255\n";
    //string img = "coinsEasy3.jpg";
    //string img = "lenna.jpg";
    //string img = "colorimage.jpg";
    //string img = "HariCanny.png";
    string img = "CoinTest.JPG";
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
        if(greyScaleVals < 165) {
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