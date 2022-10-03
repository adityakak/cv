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

const int k = 6;
const int d = 2;
const int pointsNumber = 10;
//unordered_set<double> xLimits;
//unordered_set<double> yLimits;
auto result = new int[800][800];

void printSet(unordered_set<double> smth)
{
    for (auto a : smth)
        cout << a << ", ";
    cout << endl;
}
void printVector(vector<double> v)
{
    for (auto a : v)
    {
        cout << a << " ";
    }
}
void printDictionary(unordered_map<int, vector<double>> d)
{
    for (auto it : d)
    {
        cout << it.first << ": ";
        printVector(it.second);
        cout << endl;
    }
    cout << endl;
}
void print2Vector(vector<vector<double>> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v[i].size(); j++)
        {
            cout << v[i][j];
        }
        cout << endl;
    }
    cout << endl;
}
void print2Dictionary(unordered_map<int, vector<vector<double>>> d)
{
    for (auto it : d)
    {
        cout << it.first << ": ";
        print2Vector(it.second);
    }
    cout << endl;
}

class Node
{
    vector<double> data;
    Node *leftChild;
    Node *rightChild;

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
    //printVector(inputValues);
    //cout << endl;
    leftChild = NULL;
    rightChild = NULL;
    data = inputValues;
}

void printNode(Node *pointer)
{
    auto data = pointer->getDataValues();
    printVector(data);
    cout << endl;
}
void setPixel(int x, int y, int col)
{
    if (x < 800 && x > 0 && y < 800 && y > 0)
        result[x][y] = col;
    //cout << y << ", ";
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

    cout << "xData: ";
    printVector(x.getDataValues());
    cout << "NewKey Data: ";
    printVector(newKey.getDataValues());
    cout << endl;

    pointers.push_back(nodePointer);
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
    printDictionary(points);
    cout << endl;
    auto val = points.at(0);
    printVector(val);
    cout << endl;
    Node root(points.at(0));
    Node *rootPoint = &root;
    vector<Node *> pointerStore;
    for (auto values : points)
    {
        int count = 0;
        if (values.second != rootPoint->getDataValues())
        {
            Node newKey(values.second);

            /*
            cout << "Root Vals: ";
            printVector(root.getDataValues());
            cout << endl;

            cout << "New Key Vals: ";
            printVector(values.second);
            cout << endl;
            */

            Node *x = rootPoint;
            //Node y(vector<double>{0});

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
            //Node *nodePointer = &newKey;

            //pointerStore.push_back(nodePointer);
            if (newKey.getDataValues().at(usedDimension) < x->getDataValues().at(usedDimension))
            {
                cout << "Parent: ";
                printVector(x->getDataValues());
                cout << "Left Child: ";
                printVector(newKey.getDataValues());
                cout << endl;
                x->setLeftChild(new Node(values.second));
            }
            else
            {
                cout << "Parent: ";
                printVector(x->getDataValues());
                cout << "Right Child: ";
                printVector(newKey.getDataValues());
                cout << endl;
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
        for (int x = 0; x < k; x++)
        {
            centers.at(x) = findMean(dict.at(x));
        }
        dictCopy = dict;
        for (auto it : dict)
        {
            it.second.clear();
        }
    } while (dict != dictCopy);
    return dict;
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
    cout << "DAXNEG" << endl;
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
    cout << "DAXPOS" << endl;
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
    cout << "DAYNEG" << endl;
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
    cout << "DAYPOS" << endl;
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
    cout << endl;
    cout << aInt.first << ", " << aInt.second << endl;
    cout << bInt.first << ", " << bInt.second << endl;
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
void drawMap(Node *root, int depth, double parentDimensionLimit, bool greater, unordered_set<int> *xLimits, unordered_set<int> *yLimits)
{
    int usedDimension = depth % d;
    auto data = root->getDataValues();
    unordered_set<int> xLimitsR = *xLimits;
    unordered_set<int> yLimitsR = *yLimits;
    if (usedDimension == 0)
    {
        if (parentDimensionLimit == numeric_limits<double>::min())
        {
            drawLine(make_pair(1, data.at(0)), make_pair(0, data.at(0)), 1);
            xLimitsR.emplace(data.at(0));
        }
        else
        {
            if (greater)
            {
                double bottomHandLimit = 1;
                for (auto value : yLimitsR)
                {
                    if (value < 1 && value > data.at(1) && value < bottomHandLimit)
                    {
                        bottomHandLimit = value;
                    }
                }
                cout << endl;
                cout << "Vertical Line from " << data.at(0) * 800 << ", " << parentDimensionLimit * 800 << " to " << bottomHandLimit << ", " << data.at(0) * 800 << endl;
                drawLine(make_pair(bottomHandLimit, data.at(0)), make_pair(parentDimensionLimit, data.at(0)), 1);
                xLimitsR.emplace(data.at(0));
            }
            else
            {
                double upperHandLimit = 0;
                for (auto value : yLimitsR)
                {
                    if (value > 0 && value < data.at(1) && value > upperHandLimit)
                    {
                        upperHandLimit = value;
                    }
                }
                cout << endl;
                cout << "Vertical Line from " << data.at(0) * 800 << ", " << parentDimensionLimit * 800 << " to " << 0 << ", " << data.at(0) * 800 << endl;
                if (abs(parentDimensionLimit - upperHandLimit) > .02)
                {
                    drawLine(make_pair(parentDimensionLimit, data.at(0)), make_pair(upperHandLimit, data.at(0)), 1);
                    xLimitsR.emplace(data.at(0));
                }
            }
        }
    }
    else
    {
        if (greater)
        {
            double rightHandLimit = 1;
            for (auto value : xLimitsR)
            {
                if (value < 1 && value > data.at(0) && rightHandLimit > value)
                    rightHandLimit = value;
            }
            cout << endl;
            cout << "Horizontal Line from " << data.at(1) * 800 << ", " << parentDimensionLimit * 800 << " to " << rightHandLimit * 800 << ", " << data.at(1) * 800 << endl;
            if (abs(parentDimensionLimit - rightHandLimit) > .02)
            {
                drawLine(make_pair(data.at(1), rightHandLimit), make_pair(data.at(1), parentDimensionLimit), 1);
                yLimitsR.emplace(data.at(1));
            }
        }
        else
        {
            double leftHandLimit = 0;
            for (auto value : xLimitsR)
            {
                if (value > 0 && value < data.at(0) && leftHandLimit < value)
                    leftHandLimit = value;
            }
            cout << endl;
            cout << "Horizontal Line from " << data.at(1) * 800 << ", " << parentDimensionLimit * 800 << " to " << leftHandLimit * 800 << ", " << data.at(1) * 800 << endl;
            if (abs(parentDimensionLimit - leftHandLimit) > .02)
            {
                drawLine(make_pair(data.at(1), parentDimensionLimit), make_pair(data.at(1), leftHandLimit), 1);
                yLimitsR.emplace(data.at(1));
            }
        }
    }
}
void preOrderTraversal(Node *parent, Node root, int depth, bool greater, unordered_set<int> xLimits, unordered_set<int> yLimits)
{
    Node *nodes = &root;
    double pUse;
    if (parent == NULL)
    {
        pUse = numeric_limits<double>::min();
    }
    else
    {
        pUse = parent->getDataValues().at((depth - 1) % d);
    }
    Node *rightChild = nodes->getRightChild();
    Node *leftChild = nodes->getLeftChild();
    auto data = nodes->getDataValues();
    drawCircle(make_pair(data.at(1), data.at(0)), .01, 1);
    if (!(rightChild == NULL && leftChild == NULL))
    {
        drawMap(nodes, depth, pUse, greater, &xLimits, &yLimits);
        if (rightChild != NULL)
            preOrderTraversal(nodes, *rightChild, depth + 1, true, xLimits, yLimits);
        if (leftChild != NULL)
            preOrderTraversal(nodes, *leftChild, depth + 1, false, xLimits, yLimits);
    }
}
void printTree(Node *root, int space)
{
    if (root == NULL)
        return;
    space += 10;
    printTree(root->getRightChild(), space);
    cout << endl;
    for (int i = 10; i < space; i++)
        cout << "\t";
    printVector(root->getDataValues());
    cout << "\n";
    printTree(root->getLeftChild(), space);
}
int main()
{
    ofstream fout("output6.ppm");
    if (fout.fail())
        return -1;
    fout << "P3\n"
         << 800 << " "
         << "800\n"
         << "1\n";
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
    auto createdTree = buildkdTree(points);
    cout << endl;
    /*
    Node *ptrToTree = &createdTree;
    printTree(ptrToTree, 0);
    
    Node* createdTree = new Node(vector<double> {.45125, .2925}); 

    createdTree->setLeftChild(new Node(vector<double> {.335, .26}));
    createdTree->getLeftChild()->setRightChild(new Node(vector<double> {.18375, .49385}));

    createdTree->setRightChild(new Node(vector<double> {.55125, .3825}));
 

    unordered_set<int> xLimits, yLimits;
    preOrderTraversal(NULL, createdTree, 0, false, xLimits, yLimits);
    */

    //printDictionary(points);
    
    double minVariance = INT_FAST16_MAX;
    unordered_map<int, vector<vector<double>>> minGroup;
    for (int i = 0; i < 100; i++)
    {
        auto answer = lloyds(points);
        double tempVariance = variance(answer);
        if (tempVariance < minVariance)
        {
            minVariance = tempVariance;
            minGroup = answer;
        }
    }
    
    //print2Dictionary(minGroup);
    for (auto items : minGroup)
    {
        int colorNum = items.first;
        if (colorNum == 0)
            colorNum = 6;
        for (auto color : items.second)
        {
            drawCircle(make_pair(color.at(0), color.at(1)), .005, colorNum);
        }
    }
    
    for (int x = 0; x < 800; x++)
    {
        for (int y = 0; y < 800; y++)
        {
            if (result[x][y] == 1)
                fout << 0 << " " << 0 << " " << 0 << " ";
            else
            {
                fout << 1 << " " << 1 << " " << 1 << " ";
            }
        }
    }
}