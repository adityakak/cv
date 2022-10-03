#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <math.h>
#include <unordered_set>
#include <algorithm>
#include <tuple>

using namespace std;
using namespace cv;
const int rows = 724; //Second Number in JPG
const int cols = 869; //First Number in JPG

void printVector(vector<double> v)
{
    for (auto a : v)
    {
        std::cout << a << " ";
    }
}

double distance(pair<double, double> a, pair<double, double> b)
{
    double value = pow((b.first - a.first), 2) + pow((b.second - a.second), 2.0);
    return sqrt(value);
}

vector<int> drawCircle(pair<double, double> vals, double radius)
{
    vector<int> toChange;
    pair<double, double> center = vals;
    pair<int, int> centerInt = make_pair((int)(center.first * rows), (int)(center.second * cols));
    //radius *= (rows + cols) / 2;
    int radius2 = (int)(radius);
    int xmax = (int)(radius * (sqrt(2) / 2));
    int y = radius;
    int y2 = y * y;
    int ty = (2 * y) - 1;
    int y2_new = y2;
    for (int x = 0; x <= xmax; x++)
    {
        if ((y2 - y2_new) >= ty)
        {
            y2 -= ty;
            y -= 1;
            ty -= 2;
        }
        toChange.push_back(x + centerInt.first + ((y + centerInt.second) * cols));
        toChange.push_back(x + centerInt.first + ((-y + centerInt.second) * cols));
        toChange.push_back(-x + centerInt.first + ((y + centerInt.second) * cols));
        toChange.push_back(-x + centerInt.first + ((-y + centerInt.second) * cols));
        toChange.push_back(y + centerInt.first + ((x + centerInt.second) * cols));
        toChange.push_back(y + centerInt.first + ((-x + centerInt.second) * cols));
        toChange.push_back(-y + centerInt.first + ((x + centerInt.second) * cols));
        toChange.push_back(-y + centerInt.first + ((-x + centerInt.second) * cols));
        //setPixel(x + centerInt.first, y + centerInt.second);
        //setPixel(x + centerInt.first, -y + centerInt.second);
        //setPixel(-x + centerInt.first, y + centerInt.second);
        //setPixel(-x + centerInt.first, -y + centerInt.second);
        //setPixel(y + centerInt.first, x + centerInt.second);
        //setPixel(y + centerInt.first, -x + centerInt.second);
        //setPixel(-y + centerInt.first, x + centerInt.second);
        //setPixel(-y + centerInt.first, -x + centerInt.second);
        y2_new -= (2 * x) - 3;
    }
    return toChange;
}

vector<int> greyscaled(Mat img)
{
    vector<int> greyed((cols * rows) + 1);

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            Vec3b colour = img.at<Vec3b>(r, c);
            int greyScaledVal = (int)((colour.val[0] + colour.val[1] + colour.val[2]) / 3);
            greyed[(r * cols) + c] = greyScaledVal;
        }
    }
    return greyed;
}

vector<int> sobelGradient(vector<int> greyed, Mat img)
{
    vector<int>{1, 0, -1, 2, 0, -2, 1, 0, -1};
    vector<int> gradientX((cols * rows) + 1);
    //std::cout << "Calculating X-Gradient\n";
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (r == 0 || r == rows - 1 || c == 0 || c == cols - 1)
            {
                gradientX[(r * cols) + c] = 0;
            }
            else
            {
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
    //std::cout << "Calculating Y-Gradient\n";
    vector<int> gradientY((cols * rows) + 1);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (r == 0 || r == rows - 1 || c == 0 || c == cols - 1)
            {
                gradientX[(r * cols) + c] = 0;
            }
            else
            {
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
    //std::cout << "Calculating Total-Gradient\n";
    int maxGradientValue = INT_MIN;
    vector<int> gradient((cols * rows) + 1);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int currValue = ceil(sqrt(pow(gradientX[(r * cols) + c], 2) + pow(gradientY[(r * cols) + c], 2)));
            gradient[(r * cols) + c] = currValue;
            if (currValue > maxGradientValue)
            {
                maxGradientValue = currValue;
            }
        }
    }
    //std::cout << "Max Value In Gradient is: " << maxGradientValue << "\n";
    //std::cout << "Calculating Angles\n";
    vector<double> angles((cols * rows) + 1);
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            angles[(r * cols) + c] = (atan2(gradientY[(r * cols) + c], gradientX[(r * cols) + c]) * 180) / M_PI;
        }
    }

    //std::cout << "Applying Non-Max Supression\n";
    double maxSort = .14 * maxGradientValue;
    double minSort = .05 * maxSort;
    //std::cout << "Max Sort Value is " << maxSort << endl;
    unordered_set<int> strong;
    vector<int> weak;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (r == 0 || r == rows - 1 || c == 0 || c == cols - 1)
            {
                gradient[(r * cols) + c] = 0;
            }
            else
            {
                double angleValue = angles[(r * cols) + c];
                int gradientValue = gradient[(r * cols) + c];
                if (((-22.5 <= angleValue) && (angleValue < 22.5)) || (((-180 <= angleValue) && (angleValue < -157.5)) || ((157.5 < angleValue) && (angleValue <= 180))))
                {
                    //Horizontal
                    int middleLeft = (r * cols) + (c - 1);
                    int middleRight = (r * cols) + (c + 1);
                    if (gradient[middleLeft] > gradientValue || gradient[middleRight] > gradientValue)
                    {
                        gradient[(r * cols) + c] = 0;
                    }
                }
                else if (((22.5 <= angleValue) && (angleValue < 67.5)) || ((-157.5 <= angleValue) && (angleValue < -112.5)))
                {
                    //North East - South West
                    int topRight = ((r - 1) * cols) + (c + 1);
                    int bottomLeft = ((r + 1) * cols) + (c - 1);
                    if (gradient[topRight] > gradientValue || gradient[bottomLeft] > gradientValue)
                    {
                        gradient[(r * cols) + c] = 0;
                    }
                }
                else if (((67.5 <= angleValue) && (angleValue < 112.5)) || ((-112.5 <= angleValue) && (angleValue < -67.5)))
                {
                    //Up Down
                    int top = ((r - 1) * cols) + c;
                    int bottom = ((r + 1) * cols) + c;
                    if (gradient[top] > gradientValue || gradient[bottom] > gradientValue)
                    {
                        gradient[(r * cols) + c] = 0;
                    }
                }
                else
                {
                    //North West - South East
                    int topLeft = ((r - 1) * cols) + (c - 1);
                    int bottomRight = ((r + 1) * cols) + (c + 1);
                    if (gradient[topLeft] > gradientValue || gradient[bottomRight] > gradientValue)
                    {
                        gradient[(r * cols) + c] = 0;
                    }
                }
                if (gradient[(r * cols) + c] > maxSort)
                {
                    strong.emplace((r * cols) + c);
                }
                else if ((minSort <= gradient[(r * cols) + c]) && (gradient[(r * cols) + c] < maxSort))
                {
                    weak.push_back((r * cols) + c);
                }
            }
        }
    }

    //std::cout << "Applying Hysteresis\n";
    //std::cout << "Weak Pixel Length: " << weak.size() << "\n";
    unordered_set<int> toAdd;
    for (auto values : weak)
    {
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
            (yPos * cols) + (xPos + 1)};
        for (auto toCheck : places)
        {
            if (strong.find(toCheck) != strong.end())
            {
                toAdd.emplace(values);
                toHide = false;
                break;
            }
        }
        if (toHide)
        {
            gradient[values] = 0;
        }
    }
    strong.insert(toAdd.begin(), toAdd.end());

    vector<int>().swap(gradient);
    vector<int>().swap(gradientX);
    vector<int>().swap(gradientY);
    vector<int>().swap(weak);
    vector<double>().swap(angles);

    //std::cout << "Finding Circles\n";

    //double radius = 23.5;
    unordered_map<int, tuple<double, int>> centerLocations; //Pos, <Radius, AccumCount>
    for (double radius = 20; radius < 35; radius += .2)
    {
        //cout << radius << endl;
        vector<int> accum((cols * rows) + 1, 0);
        for (auto values : strong)
        {
            int xPos = values % cols;
            int yPos = values / cols;
            int newX, newY, checkPos;
            for (int j = 0; j < 360; j += 1)
            {
                newX = xPos - (cos((j * M_PI) / 180.0) * radius);
                newY = yPos - (sin((j * M_PI) / 180.0) * radius);
                checkPos = (newY * cols) + newX;
                if (checkPos > 0 && checkPos < (cols * rows))
                {
                    accum[checkPos] += 1;
                }
            }
        }
        int pos = 0;
        unordered_map<int, int> tempCenterPos; //Pos, AccumCount
        for (int findLarge : accum)
        {
            if (findLarge > 75)
            {
                int posX = pos % cols;
                int posY = pos / cols;
                bool center = true;
                vector<int> toRemove;
                for (auto values : tempCenterPos)
                {
                    double dist = distance(make_pair(posX, posY), make_pair(values.first % cols, values.first / cols));
                    if (dist < (radius * 1.5))
                    {
                        if (findLarge < values.second)
                            center = false;
                        else
                        {
                            //tempCenterPos.erase(values.first);
                            toRemove.push_back(values.first);
                        }
                    }
                }
                for (int remove : toRemove)
                    tempCenterPos.erase(remove);
                if (center)
                    tempCenterPos.emplace(pos, findLarge);
            }
            pos++;
        }
        vector<int> toRemove;
        vector<int> toAdditon;
        bool toMultiStep = true;
        for (auto temps : tempCenterPos)
        {
            for (auto currSave : centerLocations)
            {
                double dist = distance(make_pair(currSave.first % cols, currSave.first / cols), make_pair(temps.first % cols, temps.first / cols));
                if (dist < (radius * 1.5))
                {
                    toMultiStep = false;
                    if (get<1>(currSave.second) < temps.second)
                    {
                        toRemove.push_back(currSave.first);
                        toAdditon.push_back(temps.first);
                    }
                }
            }
        }
        if (toMultiStep)
        {
            for (auto items : tempCenterPos)
            {
                centerLocations.emplace(items.first, make_tuple(radius, items.second));
            }
        }
        else
        {
            if (toRemove.size() > 0)
            {
                for (int i = 0; i < toRemove.size(); i++)
                {
                    centerLocations.erase(toRemove.at(i));
                    centerLocations.emplace(toAdditon.at(i), make_tuple(radius, tempCenterPos.at(toAdditon.at(i))));
                }
            }
        }
    }
    cout << "Removing Overlap" << endl;
    unordered_set<int> toRemove;
    for (auto posCenters : centerLocations)
    {
        for (auto toCheck : centerLocations)
        {
            if (posCenters != toCheck)
            {
                double dist = distance(make_pair(posCenters.first % cols, posCenters.first / cols), make_pair(toCheck.first % cols, toCheck.first / cols));
                if (dist < (25 * 1.5))
                {
                    if (get<1>(posCenters.second) < get<1>(toCheck.second))
                    {
                        toRemove.emplace(posCenters.first);
                    }
                    else
                    {
                        toRemove.emplace(toCheck.first);
                    }
                }
            }
        }
    }
    for (auto items : toRemove)
    {
        centerLocations.erase(items);
    }

    double priceTotal = 0;
    vector<int> final((cols * rows) + 1, 0);
    for (auto maxPos : centerLocations)
    {
        int xPos = maxPos.first % cols;
        int yPos = maxPos.first / cols;
        double radius = get<0>(maxPos.second);

        vector<vector<int>> toAverage(10, vector<int>(3, 0));
        for (int i = 0; i < 10; i++)
        {
            int createdRandomX = (rand() % (int)((radius / 4) - 1)) + xPos;
            int createdRandomY = (rand() % (int)((radius / 4) - 1)) + yPos;
            Vec3b colour = img.at<Vec3b>(createdRandomX, createdRandomY);
            toAverage.at(i).push_back(colour.val[0]);
            toAverage.at(i).push_back(colour.val[1]);
            toAverage.at(i).push_back(colour.val[2]);
        }
        int avgRed, avgGreen, avgBlue = 0;
        for (int i = 0; i < 10; i++)
        {
            avgRed += toAverage.at(i).at(0);
            avgGreen += toAverage.at(i).at(1);
            avgBlue += toAverage.at(i).at(2);
        }
        avgRed /= 10;
        avgGreen /= 10;
        avgBlue /= 10;
        double distFromPenny = abs(avgRed - 100) + abs(avgGreen - 76) + abs(avgBlue - 60);
        double distFromSilver = abs(avgRed - 134) + abs(avgGreen - 134) + abs(avgBlue - 185);

        if (radius < 24.2)
        {
            priceTotal += .01;
        }
        else if (radius >= 24.2 && radius < 26.8)
        {
            priceTotal += .05;
        }
        else
        {
            priceTotal += .25;
        }
        int newX, newY, checkPos;
        for (int j = 0; j < 360; j += 1)
        {
            newX = xPos - (cos((j * M_PI) / 180.0) * radius);
            newY = yPos - (sin((j * M_PI) / 180.0) * radius);
            checkPos = (newY * cols) + newX;
            if (checkPos > 0 && checkPos < (cols * rows))
            {
                if (radius < 24.2)
                    final[checkPos] = 1;
                else if (radius >= 24.2 && radius < 26.8)
                    final[checkPos] = 2;
                else if (radius >= 26.8)
                    final[checkPos] = 3;
            }
        }
    }
    //std::cout << "Circle Finding Completed\n";
    std::cout << "Total Value of Coins: " << priceTotal << endl;
    //cin.get();
    return final;
}
vector<vector<int>> combine(vector<int> circlesOutline, Mat img)
{
    //std::cout << "Starting Merging\n";
    vector<vector<int>> final((cols * rows) + 1, vector<int>(3, 0));
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            Vec3b colour = img.at<Vec3b>(r, c);
            int circleColour = circlesOutline[(r * cols) + c];
            if (circleColour == 1)
            {
                vector<int> a{255, 0, 0};
                final[(r * cols) + c] = a;
            }
            else if (circleColour == 2)
            {
                vector<int> a{0, 255, 0};
                final[(r * cols) + c] = a;
            }
            else if (circleColour == 3)
            {
                vector<int> a{0, 0, 255};
                final[(r * cols) + c] = a;
            }
            else
            {
                //vector<int> a {colour.val[0], colour.val[1], colour.val[2]};
                vector<int> a{colour.val[2], colour.val[1], colour.val[0]};
                final[(r * cols) + c] = a;
            }
        }
    }
    //std::cout << "Merging Complete\n";
    //cin.get();
    return final;
}

int main()
{
    ofstream fout("cvCoinDetect.ppm");
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

    vector<int> greyedOut = greyscaled(srcImg);
    vector<int> outlinedCircles = sobelGradient(greyedOut, srcImg);
    vector<vector<int>> finalOutlined = combine(outlinedCircles, srcImg);
    int iterate = 0;
    for (vector<int> rgbValues : finalOutlined)
    {
        if (iterate != 0 && iterate % cols == 0)
        {
            fout << '\n';
        }
        fout << rgbValues.at(0) << " " << rgbValues.at(1) << " " << rgbValues.at(2) << " ";
        /*
        if(greyScaleVals == 3175) {
            fout << greyScaleVals - 3000 << " "  << 0 << " " << 0 << " ";
        }
        else if(greyScaleVals < 165) {
            fout << 0 << " "  << 0 << " " << 0 << " ";
        }
        else {
            fout << 255 << " "  << 255 << " " << 255 << " ";
        }
        */
        iterate++;
    }

    return 0;
}