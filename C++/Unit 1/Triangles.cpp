#include<iostream>
#include<time.h>
#include<cmath>
#include<tuple>
#include<fstream>
#include<vector>
#include<algorithm>
#include<bits/stdc++.h> 
#include<functional>
using namespace std;
vector<pair<int, int>> vect;

double distance(pair<double, double> a, pair<double, double> b){
    double value = pow((b.first - a.first), 2) + pow((b.second - a.second), 2.0);
    return sqrt(value);
}

double semiP(double d1, double d2, double d3){
    return 0.5 * (d1 + d2 + d3);
}

double rInScribed(double p, double a, double b, double c){
    return sqrt(((p - a) * (p - b) * (p - c)) / p);
}

double rCircumScribed(double p, double r, double a, double b, double c) {
    return ((a * b * c) / (4 * r * p));
}

pair<double, double> midpoint(pair<double, double> a, pair<double, double> b) {
    pair<double, double> midpoint = make_pair(((a.first + b.first) / 2), ((a.second + b.second) / 2));
    return midpoint;
}

double slope(pair<double, double> a, pair<double, double> b) {
    return ((b.second - a.second) / (b.first - a.first));
}

tuple<pair<double, double>, pair<double, double>, pair<double, double>> circleCenters(pair<double, double> a, pair<double, double> b, pair<double,double> c, double semiPer, double s1, double s2, double s3){
    //Start of Circum - Center
    pair<double, double> midpoint1 = midpoint(a, b);
    pair<double, double> midpoint2 = midpoint(b, c);
    double slope1 = slope(a, b);
    double slope2 = slope(b, c);
    double slope3 = slope(a, c);
    double perpSlope1 =  -1 / slope1;
    double perpSlope2 =  -1 / slope2;
    double perpSlope3 = -1 / slope3;
    double intercept1 = midpoint1.second - (perpSlope1 * midpoint1.first);
    double intercept2 = midpoint2.second - (perpSlope2 * midpoint2.first);
    double xcoor = (intercept2 - intercept1) / (perpSlope1 - perpSlope2);
    double ycoor = (perpSlope1 * xcoor) + intercept1;
     
    //InCenter
    double xcoor2 = (a.first * s3 + b.first * s2 + c.first * s1) / (s1 + s2 + s3);
    double ycoor2 = (a.second * s3 + b.second * s2 + c.second * s1) / (s1 + s2 + s3);

    //OrthoCenter
    double p1 = c.second - c.first * perpSlope1;
    double p2 = b.second - b.first * perpSlope3;

    double xcoorTemp = (p2 - p1) / (perpSlope1 - perpSlope3);
    double ycoorTemp = xcoorTemp * perpSlope1 + p1;
    double xcoor3 = (xcoorTemp + xcoor) / 2;
    double ycoor3 = (ycoorTemp + ycoor) / 2; 

    return make_tuple(make_pair(xcoor, ycoor), make_pair(xcoor2, ycoor2), make_pair(xcoor3, ycoor3));
}

void setPixel(int x, int y) {
    if(find(vect.begin(), vect.end(), make_pair(x, y)) == vect.end())
        if(x < 800 && x > 0 && y < 800 && y > 0)
            vect.push_back(make_pair(x, y));
}
void drawCircle(tuple<pair<double, double>> vals, double radius) {
    pair<double, double> center = get<0>(vals);
    pair<int, int> centerInt = make_pair((int)(center.first * 800), (int)(center.second * 800));
    radius *= 800;
    int radius2 = (int)(radius);
    int xmax = (int)(radius * (sqrt(2) / 2));
    int y = radius;
    int y2 = y * y;
    int ty = (2 * y) - 1;
    int y2_new = y2;
    for(int x = 0; x <= xmax; x++) {
        if((y2- y2_new) >= ty) {
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
void drawLineDAXNeg(pair<int, int> a, pair<int,int> b) {
    int dX = b.first - a.first;
    int dY = b.second - a.second;
    int y = a.second;
    int error = dX - dY;
    for(int i = a.first; i <= b.first - 1; i++) {
        setPixel(i, y);
        if(error > 0) {
            y -= 1;
            error -= dX;
        }
        error += abs(dY);
    }
}

void drawLineDAXPos(pair<int, int> a, pair<int,int> b) {
    int dX = b.first - a.first;
    int dY = b.second - a.second;
    int y = a.second;
    int error = dY - dX;

    for(int i = a.first; i <= b.first - 1; i++) {
        setPixel(i, y);
        if(error > 0) {
            y += 1;
            error -= dX;
        }
        error += dY;
    }
}

void drawLineDAYNeg(pair<int, int> a, pair<int, int> b) {
    int dY = b.second - a.second;
    int dX = b.first - a.first;
    int error = dX - dY;
    int x = a.first;
    for(int i = a.second; i <= b.second - 1; i++) {
        setPixel(x, i);
        if(error > 0) {
            x -= 1;
            error -= dY;
        }
        error += abs(dX);
    }
}


void drawLineDAYPos(pair<int, int> a, pair<int, int> b) {
    int dY = b.second - a.second;
    int dX = b.first - a.first;
    int error = dX - dY;
    int x = a.first;
    for(int i = a.second; i <= b.second - 1; i++) {
        setPixel(x, i);
        if(error > 0) {
            x += 1;
            error -= dY;
        }
        error += dX;
    }
}

void drawLine(pair<double, double> a, pair<double, double> b) {
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
    else
        if(bInt.second - aInt.second < 0 && bInt.first - aInt.first < 0) 
            drawLineDAXPos(bInt, aInt);
        else if(bInt.first - aInt.first < 0)
            if(abs(bInt.first - aInt.first) < abs(bInt.second - aInt.second))
                drawLineDAYNeg(aInt, bInt);
            else
                drawLineDAXNeg(bInt, aInt);
        else
            drawLineDAYPos(aInt, bInt);         
}

int main(){
    ofstream fout("output.ppm");
    if(fout.fail()) return -1;
    fout << "P3\n" << 800 << " " <<"800\n" << "1\n";
    
    pair <double, double> a;
    pair <double, double> b;    
    pair <double, double> c;

    srand((unsigned)time(NULL));
    a = make_pair (((double) rand()/(RAND_MAX + 1.0)), ((double) rand()/(RAND_MAX + 1.0)));
    b = make_pair (((double) rand()/(RAND_MAX + 1.0)), ((double) rand()/(RAND_MAX + 1.0)));
    c = make_pair (((double) rand()/(RAND_MAX + 1.0)), ((double) rand()/(RAND_MAX + 1.0)));
    double side1 = distance(a, b);
    double side2 = distance(a, c);
    double side3 = distance(b, c);
    double semiPerimeter = semiP(side1, side2, side3);
    
    double radiiIn = rInScribed(semiPerimeter, side1, side2, side3);
    double radiiOut = rCircumScribed(semiPerimeter, radiiIn, side1, side2, side3);
    auto centers = circleCenters(a, b, c, semiPerimeter, side1, side2, side3);
    
    drawCircle(get<0>(centers), radiiOut);
    drawCircle(get<1>(centers), radiiIn);
    drawCircle(get<2>(centers), radiiOut / 2);
    drawLine(a, b);
    drawLine(b, c); 
    drawLine(a, c);
    auto result = new int[800][800];
    for(int x = 0; x < 800; x++) 
        for(int y = 0; y < 800; y++)
            result[x][y] = 0;
            
    for(pair<int, int> i : vect) 
        if(!(i.first < 0 || i.second < 0))
            result[i.first][i.second] = 1;
    
    for(int x = 0; x < 800; x++)
        for(int y = 0; y < 800; y++) 
            if(result[x][y] == 1) 
              fout << 0 << " " << 0 << " " << 0 << " ";  
            else
                fout << 1 << " " << 1 << " " << 1 << " ";        
    fout.close();
}

fout << "\n"