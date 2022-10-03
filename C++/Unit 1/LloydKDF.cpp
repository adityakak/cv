#include <fstream>
#include <vector>
#include <bits/stdc++.h>
#include <functional>
#include <chrono>
#include <math.h>
#include <string>
#include <array>
#include <limits>
#include <cmath>

using namespace std;

const int d = 2;
const int pointsNumber = 100;
const int k = 5;
const int maxValue = numeric_limits<int>::max();
const int minValue = numeric_limits<int>::min();
auto result = new int[800][800];

void printSet(unordered_set<double> smth)
{
    for (auto a : smth)
        std::cout << a << ", ";
    std::cout << endl;
}
void printVector(vector<double> v)
{
    for (auto a : v)
    {
        std::cout << a << " ";
    }
}
void printDictionary(unordered_map<int, vector<double>> d)
{
    for (auto it : d)
    {
        std::cout << it.first << ": ";
        printVector(it.second);
        std::cout << endl;
    }
    std::cout << endl;
}
void print2Vector(vector<vector<double>> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            std::cout << v[i][j];
        }
        std::cout << endl;
    }
    std::cout << endl;
}
void print2Dictionary(unordered_map<int, vector<vector<double>>> d)
{
    for (auto it : d)
    {
        std::cout << it.first << ": ";
        print2Vector(it.second);
    }
    std::cout << endl;
}

class Node
{
    vector<double> data;
    vector<pair<double, double>> boundary;
    Node *leftChild;
    Node *rightChild;
    vector<double> com;
    int comCount;

public:
    Node(vector<double>);
    void setLeftChild(Node *);
    void setRightChild(Node *);
    Node *getLeftChild()
    {
        return leftChild;
    }
    Node *getRightChild()
    {
        return rightChild;
    }
    vector<double> getDataValues()
    {
        return data;
    }
    vector<pair<double, double>> getBoundary()
    {
        return boundary;
    }
    void setBoundary(vector<pair<double, double>> newBoundary)
    {
        boundary = newBoundary;
    }
    vector<double> getCOM()
    {
        return com;
    }
    void setCOM(vector<double> val)
    {
        com = val;
    }
    int getCOMCount()
    {
        return comCount;
    }
    void setCOMCount(int c)
    {
        comCount = c;
    }
};
void Node::setLeftChild(Node *child)
{
    leftChild = child;
}
void Node::setRightChild(Node *child)
{
    rightChild = child;
}
Node::Node(vector<double> inputValues)
{
    leftChild = NULL;
    rightChild = NULL;
    data = inputValues;
    for (int x = 0; x < d; x++)
    {
        boundary.push_back(make_pair(data.at(x), data.at(x))); //Min First Max Second
    }
    for (auto values : data)
    {
        com.push_back(values);
    }
    comCount = 1;
}

void printNode(Node *pointer)
{
    auto data = pointer->getDataValues();
    printVector(data);
    std::cout << endl;
}
void printVectorNodes(vector<vector<Node*>> theVector) {
    for(auto list: theVector) {
        for(auto list2: list) {
            auto data = list2->getDataValues();
            for(auto values: data) {
                std::cout << values << ", ";
            }
            std::cout << " - ";
        }
        std::cout << '#' << '\n';
    }
}
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
vector<Node *> insert(Node &root, Node &newKey, vector<Node *> pointers)
{
    Node x = root;
    Node y(vector<double>{0});

    int usedDimension = 0;
    int count = 0;
    while (x.getLeftChild() != NULL || x.getRightChild() != NULL)
    {
        y = x;
        usedDimension = count % d;
        double keyData = newKey.getDataValues().at(usedDimension);
        double xData = x.getDataValues().at(usedDimension);
        if (keyData < xData)
        {
            if (x.getLeftChild() != NULL)
            {
                x = *x.getLeftChild();
            }
            else
            {
                break;
            }
        }
        else
        {
            if (x.getRightChild() != NULL)
            {
                x = *x.getRightChild();
            }
            else
            {
                break;
            }
        }
        count++;
    }
    usedDimension = count % d;
    Node *nodePointer = &newKey;

    //std::cout << "xData: ";
    //printVector(x.getDataValues());
    //std::cout << "NewKey Data: ";
    //printVector(newKey.getDataValues());
    //std::cout << endl;

    pointers.push_back(nodePointer);

    auto newData = newKey.getDataValues();
    auto xBoundaryData = x.getBoundary();
    count = 0;
    for (auto values : newData)
    {
        auto pairVals = xBoundaryData.at(count);
        double xMin = pairVals.first;
        double xMax = pairVals.second;
        if (values > xMax)
            xMax = values;
        else if (values < xMin)
            xMin = values;
        xBoundaryData.at(count) = make_pair(xMin, xMax);
        count++;
    }
    x.setBoundary(xBoundaryData);

    auto xData = x.getCOM();
    auto xCount = x.getCOMCount();
    count = 0;

    for (auto values : xData)
    {
        values += newData.at(count);
        values /= xCount;
        xData.at(count) = values;
    }
    xCount += 1;
    x.setCOM(xData);
    x.setCOMCount(xCount);

    if (newKey.getDataValues().at(usedDimension) < x.getDataValues().at(usedDimension))
    {
        x.setLeftChild(nodePointer);
    }
    else
    {
        x.setRightChild(nodePointer);
    }
    return pointers; //Return a vector of pointers to prevent deletion, try not returning Node root and see what diffirence is made
}
Node buildkdTree(unordered_map<int, vector<double>> points)
{
    //printDictionary(points);
    //std::cout << endl;
    auto val = points.at(0);
    //printVector(val);
    //std::cout << endl;
    Node root(points.at(0));
    Node *rootPoint = &root;
    vector<Node *> pointerStore;
    for (auto values : points)
    {
        int count = 0;
        if (values.second != rootPoint->getDataValues())
        {
            Node newKey(values.second);
            Node *x = rootPoint;

            int usedDimension = 0;
            int count = 0;
            while (x->getLeftChild() != NULL || x->getRightChild() != NULL)
            {
                //y = x;
                usedDimension = count % d;
                double keyData = newKey.getDataValues().at(usedDimension);
                double xData = x->getDataValues().at(usedDimension);
                if (keyData < xData)
                {
                    if (x->getLeftChild() != NULL)
                    {
                        x = x->getLeftChild();
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if (x->getRightChild() != NULL)
                    {
                        x = x->getRightChild();
                    }
                    else
                    {
                        break;
                    }
                }
                count++;
            }
            usedDimension = count % d;
            auto newData = newKey.getDataValues();
            auto xBoundaryData = x->getBoundary();
            count = 0;
            for (auto values : newData)
            {
                auto pairVals = xBoundaryData.at(count);
                double xMin = pairVals.first;
                double xMax = pairVals.second;
                if (values > xMax)
                    xMax = values;
                else if (values < xMin)
                    xMin = values;
                xBoundaryData.at(count) = make_pair(xMin, xMax);
                count++;
            }
            x->setBoundary(xBoundaryData);

            auto xData = x->getCOM();
            auto xCount = x->getCOMCount();
            count = 0;

            for (auto values : xData)
            {
                values += newData.at(count);
                values /= xCount;
                xData.at(count) = values;
            }
            xCount += 1;
            x->setCOM(xData);
            x->setCOMCount(xCount);

            if (newKey.getDataValues().at(usedDimension) < x->getDataValues().at(usedDimension))
            {
                //std::cout << "Parent: ";
                //printVector(x->getDataValues());
                //std::cout << "Left Child: ";
                //printVector(newKey.getDataValues());
                //std::cout << endl;
                x->setLeftChild(new Node(values.second));
            }
            else
            {
                //std::cout << "Parent: ";
                //printVector(x->getDataValues());
                //std::cout << "Right Child: ";
                //printVector(newKey.getDataValues());
                //std::cout << endl;
                x->setRightChild(new Node(values.second));
            }
        }
    }
    return root;
}
vector<double> findMean(vector<vector<double>> points)
{
    vector<double> averageVals(d, 0);
    for (auto a : points)
    {
        for (int b = 0; b < d; b++)
        {
            averageVals.at(b) += a.at(b);
        }
    }
    for (int i = 0; i < d; i++)
    {
        averageVals.at(i) /= points.size();
    }
    return averageVals;
}
bool isInVector(double value, vector<double> entity)
{
    for (double v : entity)
    {
        if (v == value)
            return true;
    }
    return false;
}
double hyperRectangleDistance(Node *point, vector<pair<double, double>> bounds)
{
    auto pointData = point->getDataValues();

    unordered_set<double> k;
    vector<pair<double, int>> j0;
    vector<pair<double, int>> j1;

    int count = 0;
    for (pair<double, double> dimension : bounds)
    {
        if (dimension.first < pointData.at(count) && dimension.second > pointData.at(count))
        {
            k.emplace(pointData.at(count));
        }
        count++;
    }

    count = 0;
    for (pair<double, double> dimension : bounds)
    {
        if ((abs(dimension.first - pointData.at(count)) > abs(dimension.second - pointData.at(count))) && k.count(pointData.at(count) == 0))
        {
            j0.push_back(make_pair(pointData.at(count), count));
        }
        if ((abs(dimension.second - pointData.at(count)) > abs(dimension.first - pointData.at(count))) && k.count(pointData.at(count) == 0))
        {
            j1.push_back(make_pair(pointData.at(count), count));
        }
        count++;
    }
    double totalj0 = 0;
    double totalj1 = 0;
    double total = 0;
    for (auto value : j0)
    {
        double data = value.first;
        int dimension = value.second;
        totalj0 += pow(data - bounds.at(dimension).second, 2);
    }
    for (auto value : j1)
    {
        double data = value.first;
        int dimension = value.second;
        totalj1 += pow(data - bounds.at(dimension).first, 2);
    }
    total = totalj0 + totalj1;
    total = sqrt(total);
    return total;
}
vector<vector<double>> update(Node *h, vector<vector<double>> centers, vector<vector<Node *>> associate, vector<double> counters)
{
    if (h->getLeftChild() == NULL && h->getRightChild() == NULL)
    {
        auto data = h->getDataValues();
        double minDist = INT16_MAX;
        int posHolder, count = 0;
        for (auto val : centers)
        {
            double dist = distance(data, val);
            if (dist < minDist)
            {
                posHolder = count;
                minDist = dist;
            }
            count++;
        }
        int tempCount = 0;
        for(auto value: data) {
            counters.at(posHolder) = counters.at(posHolder) + 1;
            centers.at(posHolder).at(tempCount) = ((centers.at(posHolder).at(tempCount) + value) / counters.at(posHolder));
            tempCount++;
        }
        return centers;
    }
    else
    {
        int pos;
        int count = 0;
        double min = INT16_MAX;
        for (auto values : centers)
        {
            Node *tempC = new Node(values);
            double dist = hyperRectangleDistance(tempC, h->getBoundary());
            if (dist < min)
            {
                min = dist;
                pos = count;
            }
            count++;
        }
        bool domination = true;
        auto bounds = h->getBoundary();
        vector<double> newPoint;
        for (auto otherCenters : centers)
        {
            if (otherCenters != centers.at(pos))
            {
                for (int dimen = 0; dimen < d; dimen++)
                {
                    if (otherCenters.at(dimen) > centers.at(pos).at(dimen))
                    {
                        newPoint.push_back(bounds.at(dimen).second);
                    }
                    else
                    {
                        newPoint.push_back(bounds.at(dimen).first);
                    }
                }
                double dist1 = distance(newPoint, centers.at(pos));
                double dist2 = distance(newPoint, otherCenters);

                if (dist2 < dist1)
                {
                    domination = false;
                }
            }
            if (domination == false)
            {
                break;
            }
            newPoint.clear();
        }
        if (domination == true)
        {
            auto data = h->getDataValues();
            int tempCount = 0;
            for(auto values: data) {
                counters.at(pos) = counters.at(pos) + 1;
                centers.at(pos).at(tempCount) = ((centers.at(pos).at(tempCount) + values) / counters.at(pos));
                tempCount++;
            }
            return centers;
        }
        if(h->getLeftChild() != NULL) {
            centers = update(h->getLeftChild(), centers, associate, counters);
        }
        if(h->getRightChild() != NULL) {
            centers = update(h->getRightChild(), centers, associate, counters);
        }
        return centers;
    }
}
vector<vector<double>> lloyds(unordered_map<int, vector<double>> points, Node tree)
{
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
    }
    for (int i = 0; i < 10; i++)
    {
        cout << i << endl;
        vector<vector<Node *>> initialAssociate(k);
        vector<double> counters(k, 1);
        centers = update(&tree, centers, initialAssociate, counters);
        auto debugger = &tree;
        if (i == 149)
        {
            break;
        }
        initialAssociate.clear();
        counters.clear();
    }
    cout << "Exited" << endl;
    return centers;
}

double variance(unordered_map<int, vector<vector<double>>> dict)
{
    double totalVariance = 0;
    for (auto it : dict)
    {
        double total = 0;
        auto result = findMean(it.second);
        for (auto item : it.second)
        {
            total += pow(distance(result, item), 2);
        }
        total /= it.second.size();
        totalVariance += total;
    }
    return totalVariance;
}
void drawLineDAXNeg(pair<int, int> a, pair<int, int> b, int color)
{
    std::cout << "DAXNEG" << endl;
    int dX = b.first - a.first;
    int dY = b.second - a.second;
    int y = a.second;
    int error = dX - dY;
    for (int i = a.first; i <= b.first - 1; i++)
    {
        setPixel(i, y, color);
        if (error > 0)
        {
            y -= 1;
            error -= dX;
        }
        error += abs(dY);
    }
}

void drawLineDAXPos(pair<int, int> a, pair<int, int> b, int color)
{
    std::cout << "DAXPOS" << endl;
    int dX = b.first - a.first;
    int dY = b.second - a.second;
    int y = a.second;
    int error = dY - dX;

    for (int i = a.first; i <= b.first - 1; i++)
    {
        //setPixel(i, y, color);
        //cout << i << ",";
        if (error > 0)
        {
            y += 1;
            error -= dX;
        }
        error += dY;
    }
    int mx = 0;
}

void drawLineDAYNeg(pair<int, int> a, pair<int, int> b, int color)
{
    std::cout << "DAYNEG" << endl;
    int dY = b.second - a.second;
    int dX = b.first - a.first;
    int error = dX - dY;
    int x = a.first;
    for (int i = a.second; i <= b.second - 1; i++)
    {
        setPixel(x, i, color);
        if (error > 0)
        {
            x -= 1;
            error -= dY;
        }
        error += abs(dX);
    }
}

void drawLineDAYPos(pair<int, int> a, pair<int, int> b, int color)
{
    std::cout << "DAYPOS" << endl;
    int dY = b.second - a.second;
    int dX = b.first - a.first;
    int error = dX - dY;
    int x = a.first;
    for (int i = a.second; i <= b.second - 1; i++)
    {
        setPixel(x, i, color);
        if (error > 0)
        {
            x += 1;
            error -= dY;
        }
        error += dX;
    }
}

void drawLine(pair<double, double> a, pair<double, double> b, int color)
{
    pair<int, int> aInt = make_pair((int)(a.first * 800), (int)(a.second * 800));
    pair<int, int> bInt = make_pair((int)(b.first * 800), (int)(b.second * 800));
    //std::cout << endl;
    //std::cout << aInt.first << ", " << aInt.second << endl;
    //std::cout << bInt.first << ", " << bInt.second << endl;
    if (bInt.first - aInt.first >= bInt.second - aInt.second)
    {
        if (bInt.second - aInt.second < 0 && bInt.first - aInt.first < 0)
        {
            drawLineDAYPos(bInt, aInt, color);
        }
        else if (bInt.second - aInt.second < 0)
        {
            if (abs(bInt.first - aInt.first) > abs(bInt.second - aInt.second))
            {
                drawLineDAXNeg(aInt, bInt, color);
            }
            else
            {
                drawLineDAYNeg(bInt, aInt, color);
            }
        }
        else
        {
            drawLineDAXPos(aInt, bInt, color);
        }
    }
    else if (bInt.second - aInt.second < 0 && bInt.first - aInt.first < 0)
    {
        drawLineDAXPos(bInt, aInt, color);
    }
    else if (bInt.first - aInt.first < 0)
    {
        if (abs(bInt.first - aInt.first) < abs(bInt.second - aInt.second))
        {
            drawLineDAYNeg(aInt, bInt, color);
        }
        else
        {
            drawLineDAXNeg(bInt, aInt, color);
        }
    }
    else
    {
        drawLineDAYPos(aInt, bInt, color);
    }
}
void preOrderTraversal(Node *root, int color)
{
    auto data = root->getDataValues();
    auto drawData = make_pair(data.at(0), data.at(1));

    drawCircle(drawData, .005, color);

    auto left = root->getLeftChild();
    auto right = root->getRightChild();

    if (left != NULL)
    {
        preOrderTraversal(left, color);
    }
    if (right != NULL)
    {
        preOrderTraversal(right, color);
    }
}
void printTree(Node *root, int space)
{
    if (root == NULL)
        return;
    space += 10;
    printTree(root->getRightChild(), space);
    std::cout << endl;
    for (int i = 10; i < space; i++)
        std::cout << "\t";
    printVector(root->getDataValues());
    std::cout << "\n";
    printTree(root->getLeftChild(), space);
}
int main()
{
    ofstream fout("output7.ppm");
    if (fout.fail())
        return -1;
    fout << "P3\n"
         << 800 << " "
         << "800\n"
         << "255\n";
    //srand(54344);
    srand((unsigned)time(NULL));
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
        for (int x = 0; x < d; x++)
        {
            double value = ((double)rand() / (RAND_MAX + 1.0));
            while (avoidRepeat[x].find(value) != avoidRepeat[x].end())
            {
                value = ((double)rand() / (RAND_MAX + 1.0));
            }
            points.at(i).push_back(value);
        }
    }
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    auto createdTree = buildkdTree(points);
    //std::cout << endl;
    vector<vector<double>> answer = lloyds(points, createdTree);
    int count = 1;
    for(auto element: points) {
        auto data = element.second;
        double min = INT32_MAX;
        int tempCount = 1;
        int pos = 1;
        for(auto value: answer) {
            double dist = distance(data, value);
            if(dist < min) {
                min = dist;
                pos = tempCount;
            }
            tempCount++;
        }
        drawCircle(make_pair(data.at(0), data.at(1)), .005, pos);
    }
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    cout << endl;
    cout << "Time taken by brute is : " << time_span.count() << " sec " << endl;
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