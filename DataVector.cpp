#include<iostream>
#include<vector>
#include<cmath>
#include"DataVector.hpp"
using namespace std;
DataVector :: DataVector(int dimension)
{
    this->v.resize(dimension,0.0);
    this->dimension = dimension;
}
DataVector :: DataVector(const DataVector& other)
{
    this->v = other.v;
    this->dimension = other.dimension;
}
DataVector :: ~DataVector()
{
    this->v.clear();
    this->dimension = 0;
}
DataVector& DataVector :: operator=(const DataVector &other)
{
    if(this != &other) 
    {
        this->v = other.v;
        this->dimension = other.dimension;
    }
    return *(this);
}
DataVector DataVector :: operator+(const DataVector &other)
{
    int sz1 = this->v.size();
    int sz2 = other.v.size();
    DataVector ans;
    if(sz1 == sz2)
    {
        ans.setDimension(sz1);
        ans.dimension = sz1;
        for (int i = 0; i < sz1; i++)
        {
            ans.v[i] = this->v[i] + other.v[i];
        }
    }
    else
    {
        cout << "DIMENSION MISMATCH\n";
        cout << "Vector addition not defined.\n";
        exit(0);
    }
    return ans;
}
DataVector DataVector :: operator-(const DataVector &other)
{
    int sz1 = this->v.size();
    int sz2 = other.v.size();
    DataVector ans;
    if(sz1 == sz2)
    {
        ans.setDimension(sz1);
        ans.dimension = sz1;
        for (int i = 0; i < sz1; i++)
        {
            ans.v[i] = this->v[i] - other.v[i];
        }
    }
    else
    {
        cout << "DIMENSION MISMATCH\n";
        cout << "Vector subtraction not defined.\n";
        exit(0);
    }
    return ans;
}
double DataVector :: operator*(const DataVector &other)
{
    double ans = 0;
    if (this->v.size()==other.v.size()) 
    {
        for (int i = 0; i < this->v.size(); i++)
        {
            ans = ans + (this->v[i])*(other.v[i]);
        }
        return ans;
    }
    else
    {
        cout << "DIMENSION MISMATCH\n";
        cout << "Dot product not defined\n";
        exit(0);
    }
    return 0;
}
bool DataVector :: operator<(const DataVector &other) const
{
    if(this->v.size() != other.v.size())
    {
        cout << "DIMENSION MISMATCH\n";
        cout << "Comparison not defined\n";
        exit(0);
    }
    for (int i = 0; i < this->v.size(); i++)
    {
        if(this->v[i] >= other.v[i])
        {
            return false;
        }
    }
    return true;
}

double DataVector :: norm()
{
    double norm_sq = this->operator*(*this);
    double ans = sqrtf(norm_sq);
    return ans;
}
double DataVector :: distance(const DataVector &other)
{
    // dist is norm of diff vector
    DataVector ans = *this - other;
    return ans.norm();
}
void DataVector :: set(vector<double>& temp)
{
    this->v = temp;
    this->dimension = temp.size();
}
void DataVector :: setDimension(int dimension)
{
    this->v.resize(dimension,0.0);
    this->dimension = dimension;
}
