#include <iostream>
#include <time.h>
#include <cmath>
#include <tuple>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>
#include <functional>
using namespace std;

auto result = new bool[800][800];

void setPixel(int x, int y)
{
    //cout << x << " " << y << endl;
    if (x < 800 && x > 0 && y < 800 && y > 0)
        result[x][y] = true;
}

pair<double, double> midpoint(pair<double, double> a, pair<double, double> b)
{
    pair<double, double> midpoint = make_pair(((a.first + b.first) / 2), ((a.second + b.second) / 2));
    return midpoint;
}

double distance(pair<double, double> a, pair<double, double> b)
{
    double value = pow((b.first - a.first), 2) + pow((b.second - a.second), 2.0);
    return sqrt(value);
}

double slope(pair<double, double> a, pair<double, double> b)
{
    return ((b.second - a.second) / (b.first - a.first));
}
void drawLineDAXNeg(pair<int, int> a, pair<int, int> b){
    cout << "DAXNEG";
    int dX = b.first - a.first;
    int dY = b.second - a.second;
    int y = a.second;
    int error = dX - dY;
    for (int i = a.first; i <= b.first - 1; i++){
        setPixel(i, y);
        if (error > 0){
            y -= 1;
            error -= dX;
        }
        error += abs(dY);
    }
}

void drawLineDAXPos(pair<int, int> a, pair<int, int> b){
    cout << "DAXPOS";
    int dX = b.first - a.first;
    int dY = b.second - a.second;
    int y = a.second;
    int error = dY - dX;

    for (int i = a.first; i <= b.first - 1; i++){
        setPixel(i, y);
        cout << i << ",";
        if (error > 0){
            y += 1;
            error -= dX;
        }
        error += dY;
    }
    int mx = 0;
}

void drawLineDAYNeg(pair<int, int> a, pair<int, int> b){
    cout << "DAYNEG";
    int dY = b.second - a.second;
    int dX = b.first - a.first;
    int error = dX - dY;
    int x = a.first;
    for (int i = a.second; i <= b.second - 1; i++){
        setPixel(x, i);
        if (error > 0){
            x -= 1;
            error -= dY;
        }
        error += abs(dX);
    }
}

void drawLineDAYPos(pair<int, int> a, pair<int, int> b){
    cout << "DAYPOS";
    int dY = b.second - a.second;
    int dX = b.first - a.first;
    int error = dX - dY;
    int x = a.first;
    for (int i = a.second; i <= b.second - 1; i++){
        setPixel(x, i);
        if (error > 0){
            x += 1;
            error -= dY;
        }
        error += dX;
    }
}

void drawLine(pair<double, double> a, pair<double, double> b)
{
    pair<int, int> aInt = make_pair((int)(a.first * 800), (int)(a.second * 800));
    pair<int, int> bInt = make_pair((int)(b.first * 800), (int)(b.second * 800));
    if(bInt.first - aInt.first >= bInt.second - aInt.second)
        if(bInt.second - aInt.second < 0 && bInt.first - aInt.first < 0)
            drawLineDAYPos(bInt, aInt);
        else if(bInt.second - aInt.second < 0)
            if(abs(bInt.first - aInt.first) > abs(bInt.second - aInt.second))
                drawLineDAXNeg(aInt, bInt);
            else
                drawLineDAYNeg(bInt, aInt);
        else
            drawLineDAXPos(aInt, bInt);
    else if(bInt.second - aInt.second < 0 && bInt.first - aInt.first < 0)
        drawLineDAXPos(bInt, aInt);
    else if(bInt.first - aInt.first < 0)
        if(abs(bInt.first - aInt.first) < abs(bInt.second - aInt.second))
            drawLineDAYNeg(aInt, bInt);
        else
            drawLineDAXNeg(bInt, aInt);
    else
        drawLineDAYPos(aInt, bInt);
}

bool triangleTest(pair<double, double> test, pair<double, double> a, pair<double, double> b, pair<double, double> c)
{
    double tx = test.first - a.first;
    double ty = test.second - a.second;

    bool tab = (b.first - a.first) * ty - (b.second - a.second) * tx > 0;

    if ((c.first - a.first) * ty - (c.second - a.second) * tx > 0 == tab)
        return false;

    if ((c.first - b.first) * (test.second - b.second) - (c.second - b.second) * (test.first - b.first) > 0 != tab)
        return false;

    return true;
}
void drawCircle(pair<double, double> vals, double radius)
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
        setPixel(x + centerInt.first, y + centerInt.second);
        setPixel(x + centerInt.first, -y + centerInt.second);
        setPixel(-x + centerInt.first, y + centerInt.second);
        setPixel(-x + centerInt.first, -y + centerInt.second);
        setPixel(y + centerInt.first, x + centerInt.second);
        setPixel(y + centerInt.first, -x + centerInt.second);
        setPixel(-y + centerInt.first, x + centerInt.second);
        setPixel(-y + centerInt.first, -x + centerInt.second);
        y2_new -= (2 * x) - 3;
    }
}

bool calcCircle(pair<double, double> a, pair<double, double> b, pair<double, double> c, pair<double, double> d, double ab, double bc, double cd, double ad)
{
    bool test = triangleTest(a, b, c, d);
    bool test2 = triangleTest(b, a, c, d);
    bool test3 = triangleTest(c, a, b, d);
    bool test4 = triangleTest(d, a, b, c);

    if (test || test2 || test3 || test4)
        return false;

    pair<double, double> midpoint1 = midpoint(a, b);
    pair<double, double> midpoint2 = midpoint(c, d);
    double radius1 = ab / 2;
    double radius2 = cd / 2;

    double slope1 = (-1.0 / slope(a, b));
    double slope2 = (-1.0 / slope(c, d));
    double b1 = midpoint1.second - (slope1 * midpoint1.first);
    double b2 = midpoint2.second - (slope2 * midpoint2.first);

    double num = pow(radius1, 2) * (1 + pow(slope1, 2)) - pow((midpoint1.second - (slope1 * midpoint1.first) - b1), 2);
    double interx1 = (midpoint1.first + (midpoint1.second * slope1) - (b1 * slope1) + sqrt(num)) / (1 + pow(slope1, 2));
    double interx2 = (midpoint1.first + (midpoint1.second * slope1) - (b1 * slope1) - sqrt(num)) / (1 + pow(slope1, 2));
    double intery1 = (b1 + (midpoint1.first * slope1) + (midpoint1.second * pow(slope1, 2)) + slope1 * sqrt(num)) / (1 + pow(slope1, 2));
    double intery2 = (b1 + (midpoint1.first * slope1) + (midpoint1.second * pow(slope1, 2)) - slope1 * sqrt(num)) / (1 + pow(slope1, 2));
    pair<double, double> intersectionc1 = make_pair(interx1, intery1);
    pair<double, double> intersectionc12 = make_pair(interx2, intery2);

    double num2 = pow(radius2, 2) * (1 + pow(slope2, 2)) - pow((midpoint2.second - (slope2 * midpoint2.first) - b2), 2);
    double inter2x1 = (midpoint2.first + (midpoint2.second * slope2) - (b2 * slope2) + sqrt(num2)) / (1 + pow(slope2, 2));
    double inter2x2 = (midpoint2.first + (midpoint2.second * slope2) - (b2 * slope2) - sqrt(num2)) / (1 + pow(slope2, 2));
    double inter2y1 = (b2 + (midpoint2.first * slope2) + (midpoint2.second * pow(slope2, 2)) + slope2 * sqrt(num2)) / (1 + pow(slope2, 2));
    double inter2y2 = (b2 + (midpoint2.first * slope2) + (midpoint2.second * pow(slope2, 2)) - slope2 * sqrt(num2)) / (1 + pow(slope2, 2));
    pair<double, double> intersectionc2 = make_pair(inter2x1, inter2y1);
    pair<double, double> intersectionc21 = make_pair(inter2x2, inter2y2);

    pair<double, double> mainP1 = intersectionc12;
    pair<double, double> mainP2 = intersectionc2;
    double temp = distance(intersectionc12, intersectionc2);
    if (temp > distance(intersectionc12, intersectionc21))
    {
        mainP1 = intersectionc12;
        mainP2 = intersectionc21;
        temp = distance(intersectionc12, intersectionc21);
    }
    if (temp > distance(intersectionc1, intersectionc2))
    {
        mainP1 = intersectionc1;
        mainP2 = intersectionc2;
        temp = distance(intersectionc1, intersectionc2);
    }
    if (temp > distance(intersectionc1, intersectionc21))
    {
        mainP1 = intersectionc1;
        mainP2 = intersectionc21;
    }

    double slope3 = slope(mainP1, mainP2);
    double b3 = mainP1.second - (slope3 * mainP1.first);

    double num3 = pow(radius1, 2) * (1 + pow(slope3, 2)) - pow((midpoint1.second - (slope3 * midpoint1.first) - b3), 2);
    double interxC = (midpoint1.first + (midpoint1.second * slope3) - (b3 * slope3) + sqrt(num3)) / (1 + pow(slope3, 2));
    double interxC2 = (midpoint1.first + (midpoint1.second * slope3) - (b3 * slope3) - sqrt(num3)) / (1 + pow(slope3, 2));
    double interyC = (b3 + (midpoint1.first * slope3) + (midpoint1.second * pow(slope3, 2)) + slope3 * sqrt(num3)) / (1 + pow(slope3, 2));
    double interyC2 = (b3 + (midpoint1.first * slope3) + (midpoint1.second * pow(slope3, 2)) - slope3 * sqrt(num3)) / (1 + pow(slope3, 2));
    if (distance(make_pair(interxC, interyC), mainP1) < distance(make_pair(interxC2, interyC2), mainP1))
    {
        interxC = interxC2;
        interyC = interyC2;
    }
    pair<double, double> pI = make_pair(interxC, interyC);

    double num4 = pow(radius2, 2) * (1 + pow(slope3, 2)) - pow((midpoint2.second - (slope3 * midpoint2.first) - b3), 2);
    double interxC3 = (midpoint2.first + (midpoint2.second * slope3) - (b3 * slope3) + sqrt(num4)) / (1 + pow(slope3, 2));
    double interxC4 = (midpoint2.first + (midpoint2.second * slope3) - (b3 * slope3) - sqrt(num4)) / (1 + pow(slope3, 2));
    double interyC3 = (b3 + (midpoint2.first * slope3) + (midpoint2.second * pow(slope3, 2)) + slope3 * sqrt(num4)) / (1 + pow(slope3, 2));
    double interyC4 = (b3 + (midpoint2.first * slope3) + (midpoint2.second * pow(slope3, 2)) - slope3 * sqrt(num4)) / (1 + pow(slope3, 2));
    if (distance(make_pair(interxC3, interyC3), mainP2) < distance(make_pair(interxC4, interyC4), mainP2))
    {
        interxC3 = interxC4;
        interyC3 = interyC4;
    }
    pair<double, double> pK = make_pair(interxC3, interyC3);

    if (pK.first < 0 || pK.second < 0 || pI.first < 0 || pI.second < 0)
        return false;

    double slopeS1;
    double slopeS2;
    double slopeS3;
    double slopeS4;

    slopeS1 = slope(pI, b);
    slopeS2 = slope(pI, a);
    slopeS3 = slope(pK, c);
    slopeS4 = slope(pK, d);


    double bS1 = b.second - (b.first * slopeS1);
    double bS2 = a.second - (a.first * slopeS2);
    double bS3 = c.second - (c.first * slopeS3);
    double bS4 = d.second - (d.first * slopeS4);

    //check for negatives and add in that case to prevent 0 slope
    pair<double, double> pL;
    pair<double, double> pJ;

    if(abs(slopeS1 - slopeS3) < .01 || abs(slopeS2 - slopeS4) < .01)
    {

        double lX = (bS4 - bS1) / (slopeS1 - slopeS4);
        pL = make_pair(lX, (slopeS1 * lX) + bS1);

        double jX = (bS3 - bS2) / (slopeS2 - slopeS3);
        pJ = make_pair(jX, (slopeS2 * jX) + bS2);

    }
    else
    {

        double lX = (bS4 - bS2) / (slopeS2 - slopeS4);
        pL = make_pair(lX, (slopeS2 * lX) + bS2);

        double jX = (bS3 - bS1) / (slopeS1 - slopeS3);
        pJ = make_pair(jX, (slopeS3 * jX) + bS3);
    }

    drawCircle(a, .01);
    drawCircle(b, .01);
    drawCircle(c, .01);
    drawCircle(d, .01);

    drawLine(pI, pJ);
    drawLine(pJ, pK);
    drawLine(pK, pL);
    drawLine(pL, pI);

    return true;
}

int main()
{
    ofstream fout("output2.ppm");
    if (fout.fail())
        return -1;
    fout << "P3\n"
         << 800 << " "
         << "800\n"
         << "1\n";
    srand((unsigned)time(NULL));

    pair<double, double> a = make_pair(((double)rand() / (RAND_MAX + 1.0)), ((double)rand() / (RAND_MAX + 1.0)));
    pair<double, double> b = make_pair(((double)rand() / (RAND_MAX + 1.0)), ((double)rand() / (RAND_MAX + 1.0)));
    pair<double, double> c = make_pair(((double)rand() / (RAND_MAX + 1.0)), ((double)rand() / (RAND_MAX + 1.0)));
    pair<double, double> d = make_pair(((double)rand() / (RAND_MAX + 1.0)), ((double)rand() / (RAND_MAX + 1.0)));

    
    double ab = distance(a, b);
    double bc = distance(b, c);
    double cd = distance(c, d);
    double ad = distance(a, d);

    bool check = calcCircle(a, b, c, d, ab, bc, cd, ad);

    
    if (!check)
    {
        do
        {
            a = make_pair(((double)rand() / (RAND_MAX + 1.0)), ((double)rand() / (RAND_MAX + 1.0)));
            b = make_pair(((double)rand() / (RAND_MAX + 1.0)), ((double)rand() / (RAND_MAX + 1.0)));
            c = make_pair(((double)rand() / (RAND_MAX + 1.0)), ((double)rand() / (RAND_MAX + 1.0)));
            d = make_pair(((double)rand() / (RAND_MAX + 1.0)), ((double)rand() / (RAND_MAX + 1.0)));

            ab = distance(a, b);
            bc = distance(b, c);
            cd = distance(c, d);
            ad = distance(a, d);
        } while (!calcCircle(a, b, c, d, ab, bc, cd, ad));
    }
    
    //drawLine(make_pair(0, .45), make_pair(1, .45));
    cout << endl << result[10][15] << endl;

    for (int x = 0; x < 800; x++)
        for (int y = 0; y < 800; y++)
            if (result[x][y] == true)
                fout << 0 << " " << 0 << " " << 0 << " ";
            else
                fout << 1 << " " << 1 << " " << 1 << " ";
}



