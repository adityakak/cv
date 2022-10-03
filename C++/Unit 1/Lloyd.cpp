#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <functional>
#include <chrono>
#include <math.h>
#include <string>
#include <array>

using namespace std;

const int k = 160;
const int d = 32;
auto result = new int[800][800];

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
inline double distance(vector<double> a, vector<double> b)
{
    double value = 0;
    for (int x = 0; x < d; x++)
    {
        value += pow((b.at(x) - a.at(x)), 2);
    }
    return sqrt(value);
}
void printVector(vector<double> v) {
    for(auto a: v) {
        cout << a << " ";
    }
}
void printDictionary(unordered_map<int, vector<double>> d) {
    for(auto it: d) {
        cout << it.first << ": ";
        printVector(it.second);
        cout << endl;
    }
    cout << endl;
}
void print2Vector(vector<vector<double>> v) {
    for(int i = 0; i < v.size(); i++) {
        for(int j = 0; j < v[i].size(); j++) {
            cout << v[i][j];
        }
        cout << endl;
    }
    cout << endl;
}
void print2Dictionary(unordered_map<int, vector<vector<double>>> d) {
    for(auto it: d) {
        cout << it.first << ": ";
        print2Vector(it.second);
    }
    cout << endl;
}
bool checkIn(vector<vector<double>> stuff, vector<double> check)
{
    for (int x = 0; x < k; x++)
    {
        for (int y = 0; y < d; y++)
        {
            if (stuff.at(x).at(y) != check.at(y))
            {
                break;
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}
vector<double> findMean(vector<vector<double>> points) {
    vector<double> averageVals(d, 0);
   for(auto a: points) {
       for(int b = 0; b < d; b++) {
           averageVals.at(b) += a.at(b);
       }
   }
    for(int i = 0; i < d; i++) {
        averageVals.at(i) /= points.size();
    }
    return averageVals;
}
unordered_map<int, vector<vector<double>>> lloyds(unordered_map<int, vector<double>> points)
{
    unordered_map<int, vector<vector<double>>> dict;
    unordered_set<int> repAvoider;
    vector<vector<double>> centers;
    for (int x = 0; x < k; x++)
    {
        int randomNum = rand() % points.size();
        while (repAvoider.find(randomNum) != repAvoider.end())
        {
            randomNum = rand() % points.size();
        }
        repAvoider.insert(randomNum);
        centers.push_back(points.at(randomNum));
        dict.emplace(x, vector<vector<double>>{});
    }
    unordered_map<int, vector<vector<double>>> dictCopy = dict;
    do
    {
        for (auto it : points)
        {
            int centerPos = 0;
            double minDistance = INT_FAST16_MAX;
            for (int x = 0; x < k; x++)
                {
                    double tempDistance = distance(centers.at(x), it.second);
                    if (tempDistance < minDistance)
                    {
                        minDistance = tempDistance;
                        centerPos = x;
                    }
                }
            dict.at(centerPos).push_back(it.second);
        }
        for(int x = 0; x < k; x++) {
            centers.at(x) = findMean(dict.at(x));
        }
        dictCopy = dict;
        for(auto it: dict) {
            it.second.clear();
        }
    } while (dict != dictCopy);
    return dict;
}

double variance(unordered_map<int, vector<vector<double>>> dict) {
    double totalVariance = 0;
    for(auto it: dict) {
        double total = 0;
        auto result = findMean(it.second);
        for(auto item: it.second) {
            total += pow(distance(result, item), 2);
        }
        total /= it.second.size();
        totalVariance += total;
    }
    return totalVariance;
}
int main()
{
    ofstream fout("output6.ppm");
    if (fout.fail())
        return -1;
    fout << "P3\n"
         << 800 << " "
         << "800\n"
         << "255\n";
    int pointsNumber = 1600;

    //unordered_map<int, point> points;
    unordered_map<int, vector<double>> points;
    unordered_set<double> avoidRepeat[d];
    for (int x = 0; x < d; x++)
    {
        avoidRepeat[x] = unordered_set<double>{};
    }

    for (int i = 0; i <= pointsNumber; i++)
    {
        //cout << i << endl;
        points.emplace(i, vector<double>{});
        for(int x = 0; x < d; x++)
        {
            double value = ((double)rand() / (RAND_MAX + 1.0));
            while (avoidRepeat[x].find(value) != avoidRepeat[x].end())
            {
                value = ((double)rand() / (RAND_MAX + 1.0));
            }
            points.at(i).push_back(value);
        }
    }
    //printDictionary(points);
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    double minVariance = INT_FAST16_MAX;
    unordered_map<int, vector<vector<double>>> minGroup;
    for(int i = 0; i < 100; i++) {
        auto answer = lloyds(points);
        double tempVariance = variance(answer);
        if(tempVariance < minVariance) {
            minVariance = tempVariance;
            minGroup = answer;
        }
    }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    cout << endl;
    cout << "Time taken by brute is : " << time_span.count() << " sec " << endl;
    //print2Dictionary(minGroup);
    for(auto items: minGroup) {
        int colorNum = items.first;
        if(colorNum == 0)
            colorNum = 6;
        for(auto color: items.second) {
            drawCircle(make_pair(color.at(0), color.at(1)), .005, colorNum);
        }
    }
    for(int x = 0; x < 800; x++) {
        for(int y = 0; y < 800; y++) {
            if(result[x][y] == 0) {
                fout << 255 << " " << 255 << " " << 255 << " ";
            }
            if(result[x][y] == 1) {
                fout << 0 << " " << 0 << " " << 255 << " ";
            }
            if(result[x][y] == 2) {
                fout << 255 << " " << 0 << " " << 0 << " ";
            }
            if(result[x][y] == 3) {
                fout << 0 << " " << 255 << " " << 0 << " ";
            }
            if(result[x][y] == 4) {
                fout << 255 << " " << 105 << " " << 180 << " ";
            }
            if(result[x][y] == 5) {
                fout << 128 << " " << 128 << " " << 0 << " ";
            }
            if(result[x][y] == 6) {
                fout << 0 << " " << 191 << " " << 225 << " ";
            }
        }
    }
}