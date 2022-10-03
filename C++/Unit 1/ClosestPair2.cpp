#include <time.h>
#include <cmath>
#include <tuple>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <functional>
#include <chrono>
#include <iostream>
#include <ctime>
#include <ratio>
#include <chrono>
using namespace std;

auto result = new int[800][800];
int i = 0;

void setPixel(int x, int y, int col)
{
    if (x < 800 && x > 0 && y < 800 && y > 0)
        result[x][y] = col;
}

void drawCircle(pair<double, double> vals, double radius, int col)
{
    pair<double, double> center = vals;
    pair<int, int> centerInt = make_pair((int)(center.first * 800), (int)(center.second * 800));
    radius *= 800;
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
        setPixel(x + centerInt.first, y + centerInt.second, col);
        setPixel(x + centerInt.first, -y + centerInt.second, col);
        setPixel(-x + centerInt.first, y + centerInt.second, col);
        setPixel(-x + centerInt.first, -y + centerInt.second, col);
        setPixel(y + centerInt.first, x + centerInt.second, col);
        setPixel(y + centerInt.first, -x + centerInt.second, col);
        setPixel(-y + centerInt.first, x + centerInt.second, col);
        setPixel(-y + centerInt.first, -x + centerInt.second, col);
        y2_new -= (2 * x) - 3;
    }
}

double distance(pair<double, double> a, pair<double, double> b)
{
    double value = pow((b.first - a.first), 2.0) + pow((b.second - a.second), 2.0);
    return sqrt(value);
}

void brute(vector<pair<double, double>> coor)
{
    double minDist = INT_MAX;
    pair<double, double> save;
    pair<double, double> save2;
    for (pair<double, double> a : coor)
    {
        for (pair<double, double> b : coor)
        {
            if (distance(a, b) < minDist && a != b)
            {
                minDist = distance(a, b);
                save = a;
                save2 = b;
            }
        }
        drawCircle(a, .002, 1);
    }
    cout << minDist * 800 << endl;
}

void printVector(vector<pair<double, double>> vect) {
    for(pair<double, double> a: vect) {
        cout << a.first << " " << a.second << endl;
    }
}

tuple<double, pair<double, double>, pair<double,double>> recurAlgo(vector<pair<double, double>> coorX, vector<pair<double, double>> coorY)
{
    if(coorX.size() <= 3) {
        double minDist = INT_MAX;
        pair<double, double> save;
        pair<double, double> save2;
        for(pair<double, double> a : coorX)
            for(pair<double, double> b: coorX)
                if (distance(a, b) < minDist && a != b)
                {
                    minDist = distance(a, b);
                    save = a;
                    save2 = b;
                }
        auto returnVals = make_tuple(minDist, save, save2);
        return returnVals;
    }
    else {
        int length = coorX.size();
        double vLine = coorX.at(length / 2).first;
        vector<pair<double, double>> vX, vX2, vY, vY2;

        for(int i = 0; i < coorX.size(); i++) {
            if(coorX.at(i).first <= vLine) {
                vX.push_back(coorX.at(i));
            }
            else {
                vX2.push_back(coorX.at(i));
            }
            if(coorY.at(i).first <= vLine) {
                vY.push_back(coorY.at(i));
            }
            else {
                vY2.push_back(coorY.at(i));
            }
        }    
        double val, val1, finalVal;
        pair<double, double> p1, p2, p3, p4, fp1, fp2;
        tie(val, p1, p2) = recurAlgo(vX, vY);
        tie(val1, p3, p4) = recurAlgo(vX2, vY2);
        if(val < val1) {
            finalVal = val;
            fp1 = p1;
            fp2 = p2;
        }
        else {
            finalVal = val1;
            fp1 = p3;
            fp2 = p4;
        }
        
        vector<pair<double, double>> yPrime;
        for(int x = 0; x < coorY.size(); x++) {
            if(coorY.at(x).first >= finalVal - vLine && coorY.at(x).first <= finalVal + vLine)
                yPrime.push_back(coorY.at(x));
        }
        double minimum = INT_MAX;
        pair<double, double> save;
        pair<double, double> save2;
        for(int x = 0; x < coorY.size(); x++) {
            for(int y = x + 1; y < x + 8; y++) {
                if(y < coorY.size())
                    if (distance(coorY.at(x), coorY.at(y)) < minimum && coorY.at(x) != coorY.at(y))
                    {
                    minimum = distance(coorY.at(x), coorY.at(y));
                    save = coorY.at(x);
                    save2 = coorY.at(y);
                    }
            }
        }
        if (minimum < finalVal)
            return make_tuple(minimum, save, save2);
        else
            return make_tuple(finalVal, fp1, fp2);
    }
}

int main()
{
    ofstream fout("output4.ppm");
    if (fout.fail())
        return -1;
    fout << "P3\n"
         << 800 << " "
         << "800\n"
         << "255\n";

    srand((unsigned)time(NULL));
    vector<pair<double, double>> vect;
    vector<pair<double, double>> vect2;
    vector<pair<double, double>> vect3;
    double x, y;
    unordered_set<double> xVals, yVals;
    for (int i = 0; i <= 16000; i++)
    {
        x = ((double)rand() / (RAND_MAX + 1.0));
        y = ((double)rand() / (RAND_MAX + 1.0));
        if(xVals.find(x) == xVals.end() && yVals.find(y) == yVals.end()) {
            xVals.insert(x);
            yVals.insert(y);
            vect.push_back(make_pair(x, y));
            vect2.push_back(make_pair(x, y));
            vect3.push_back(make_pair(x, y));
        }
    }
    

    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    brute(vect);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    cout << "Time taken by brute is : " << time_span.count() << " sec " << endl;


    std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
    sort(vect2.begin(), vect2.end());
    sort(vect3.begin(), vect3.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });
    double save;
    pair<double, double> p1, p2;
    tie(save, p1, p2) = recurAlgo(vect2, vect3);
    std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
    cout << save * 800 << endl;
    std::chrono::duration<double> time_span2 = std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3);
    cout << "Time taken by recur is : " << time_span2.count() << " sec " << endl;

    drawCircle(p1, .01, 255);
    drawCircle(p2, .01, 255);


    for (int x = 0; x < 800; x++)
    {
        for (int y = 0; y < 800; y++)
        {
            if (result[x][y] == 1)
            {
                fout << 0 << " " << 0 << " " << 0 << " ";
            }
            else if (result[x][y] == 255)
            {
                fout << 255 << " " << 0 << " " << 0 << " ";
            }
            else
            {
                fout << 255 << " " << 255 << " " << 255 << " ";
            }
        }
    }
}