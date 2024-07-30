// Class DataVector IMPLEMENTATION
// *******ATTRIBUTES*******
// - vector<double> v, used to store the vector of doubles
// - int dimension, used to store the dimension of the vector
// *******CONSTRUCTOR and DESTRUCTOR*******
// - a constructor that takes an integer argument and initializes a vector of doubles of the given size
// - a destructor
// - a copy constructor
// - an assignment operator
// *******METHODS*******
// - a method set that takes a vector of doubles and sets the internal vector to the given vector
// - a method setDimension that takes an integer argument and sets the dimension of the internal vector to the given integer
// - a method operator+ that takes a DataVector argument and returns a new DataVector that is the sum of the internal vector and the given DataVector's internal vector
// - a method operator- that takes a DataVector argument and returns a new DataVector that is the difference of the internal vector and the given DataVector's internal vector
// - a method operator* that takes a DataVector argument and returns the dot product of the internal vector and the given DataVector's internal vector
// - a method norm that returns the norm of the internal vector
// - a method dist that takes a DataVector argument and returns the Euclidean distance between the internal vector and the given DataVector's internal vector
#include <iostream>
#include <vector>
using namespace std;
class DataVector {
    vector<double> v;
    int dimension;
    public:
    DataVector(int dimension=0);
    ~DataVector();
    DataVector(const DataVector& other);
    DataVector & operator=(const DataVector &other);
    void set(vector<double> &a);
    void setDimension(int dimension);
    int getDimension() const { 
        
        return v.size();
    }
    int getElem(int i) const
    {
        return v[i];
    }
    DataVector operator+(const DataVector &other);
    DataVector operator-(const DataVector &other);
    double operator*(const DataVector &other);
    bool operator<(const DataVector &other) const;
    double norm();
    double distance(const DataVector &other);
    void print() const {
    
        for(int i=0;i<v.size();i++)
        {
            cout << v[i] << " ";
        }
        cout << endl;
    }
    bool  operator==(const DataVector &other) const
    {
    if(this->v.size() != other.v.size())
    {
        cout << "DIMENSION MISMATCH\n";
        cout << "Comparison not defined\n";
        exit(0);
    }
    for (int i = 0; i < this->v.size(); i++)
    {
        if(this->v[i] != other.v[i])
        {
            return false;
        }
    }
    return true;
    }
};