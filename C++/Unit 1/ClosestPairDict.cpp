#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <functional>
#include <chrono>
#include <math.h>  
#include <string>

using namespace std;
typedef pair<double, double> point;
typedef vector<point> pointStore;
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

inline double distance(point a, point b)
{
    double value = pow((b.first - a.first), 2.0) + pow((b.second - a.second), 2.0);
    return sqrt(value);
}

void brute(pointStore coor)
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
    }
    cout << save.first << "," << save.second << endl;
    cout << save2.first << "," << save2.second << endl; 
    cout << minDist << endl;
}

pointStore knuthShuffle(pointStore vect) {
    for(int i = vect.size() - 1; i > 1; i--) {
        int randomNum = rand() % (i + 1);
        point temp = vect.at(randomNum);
        vect.at(randomNum) = vect.at(i);
        vect.at(i) = temp;
    }
    return vect;
}
inline vector<int> validSubSquares(int x, int y, int gridWidth) {   
    vector<int> returnVals{
    x + (y * gridWidth), x + ((y + 1) * gridWidth), x + ((y - 1) * gridWidth), x + ((y + 2) * gridWidth), x + ((y - 2) * gridWidth),
    (x + 1) + (y * gridWidth), (x + 1) + ((y + 1) * gridWidth), (x + 1) + ((y - 1) * gridWidth), (x + 1) + ((y + 2) * gridWidth), (x + 1) + ((y - 2) * gridWidth),
    (x + 2) + (y * gridWidth), (x + 2) + ((y + 1) * gridWidth), (x + 2) + ((y - 1) * gridWidth), (x + 2) + ((y + 2) * gridWidth), (x + 2) + ((y - 2) * gridWidth),
    (x - 1) + (y * gridWidth), (x - 1) + ((y + 1) * gridWidth), (x - 1) + ((y - 1) * gridWidth), (x - 1) + ((y + 2) * gridWidth), (x - 1) + ((y - 2) * gridWidth),
    (x - 2) + (y * gridWidth), (x - 2) + ((y + 1) * gridWidth), (x - 2) + ((y - 1) * gridWidth), (x - 2) + ((y + 2) * gridWidth), (x - 2) + ((y - 2) * gridWidth),
    };
    return returnVals;
}

void linearClosestPair(pointStore vect) {
    unsigned int x, y, posPoint, gridWidth, pos;
    double delta, minDistance;
    point min1, min2, min3;

    delta = distance(vect.at(0), vect.at(1));
    min1 = vect.at(0);
    min2 = vect.at(1);
    gridWidth = ceil(1 / (delta / 2));
    unordered_map<int, vector<point>> dict;

    for(unsigned int i = 0; i < vect.size(); i++) {
        point a = vect.at(i);
        drawCircle(a, .002, 1);
        x = floor(a.first / (delta / 2));
        y = floor(a.second /(delta / 2));
        posPoint = (x + (y * gridWidth));
        minDistance = INT_FAST16_MAX;
        for(int checker: validSubSquares(x, y, gridWidth)) {
            auto tempVector = dict.find(checker);
            if(tempVector != dict.end()) {
                for(point j: tempVector->second) {
                    double tempD = distance(a, j);
                    if(tempD < minDistance) {
                        minDistance = tempD;
                        min2 = j;
                    }
                }
            }
        }
        if(minDistance < delta) {
            min1 = a;
            min3 = min2;
            delta = minDistance;
            gridWidth = ceil(1 / (delta / 2));
            dict.clear();
            for(unsigned int j = 0; j < i; j++) {
                point add = vect.at(j);
                x = floor(add.first / (delta / 2));
                y = floor(add.second /(delta / 2));
                pos = (x + (y * gridWidth));
                if(dict.find(pos) == dict.end())
                    dict.emplace(pos, vector<point>{});
                dict.at(pos).push_back(add);
            }
        }
        else {
            if(dict.find(posPoint) == dict.end())
                dict.emplace(posPoint, vector<point>{});
            dict.at(posPoint).push_back(a);
        }
    }
    
    drawCircle(min3, .01, 255);
    drawCircle(min1, .01, 255);
    
    /*
    cout << min3.first << "," << min3.second << endl; 
    cout << min1.first << "," << min1.second << endl;
    cout << delta << endl;
    */
}

int main() {
    ofstream fout("output5.ppm");
    if (fout.fail())
        return -1;
    fout << "P3\n"
         << 800 << " "
         << "800\n"
         << "255\n";

    srand((unsigned)time(NULL));
 
    pointStore vect;
    unordered_set<double> xVals, yVals;
    
    
    for (int i = 0; i <= 35; i++)
    {
        double x = ((double)rand() / (RAND_MAX + 1.0));
        double y = ((double)rand() / (RAND_MAX + 1.0));
        if(xVals.find(x) == xVals.end() && yVals.find(y) == yVals.end()) {
            xVals.insert(x);
            yVals.insert(y);
            vect.push_back(make_pair(x, y));
        }
    }
    
    vect = knuthShuffle(vect);

    /*
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    cout << "Time taken by brute is : " << time_span.count() << " sec " << endl;
    */
    //brute(vect);
    //std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
    linearClosestPair(vect);
    //std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> time_span2 = std::chrono::duration_cast<std::chrono::duration<double>>(t4 - t3);
    //cout << "Time taken by dictionary is : " << time_span2.count() << " sec " << endl;

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