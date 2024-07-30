#include"DataVector.hpp"
#include <fstream>
#include <sstream>

using namespace std;
class VectorDataset{
    vector<DataVector> dataset;
    public:
    VectorDataset(){
        dataset.resize(0);
    }
    ~VectorDataset(){
        dataset.clear();
    }
    void addDataVector(DataVector &v){
        dataset.push_back(v);
    }
    vector<DataVector>& getDataset(){
        return dataset;
    }
    void ReadDataset(const string& filename) {
        ifstream file(filename);

        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            vector<double> row;
            istringstream iss(line);
            string value;

            while (getline(iss, value, ',')) {
                row.push_back(stod(value));
            }
            DataVector temp;
            temp.set(row);  
            dataset.push_back(temp);
        }
        file.close();
    }
};
