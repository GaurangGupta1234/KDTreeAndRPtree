#include"TreeIndex.hpp"
using namespace std;
#include<bits/stdc++.h>
//Tree Index Functions 
//Parameterized constructor
   TreeIndex::  TreeIndex(vector<DataVector>& cur_data) : data_treeindex(cur_data){
        // cout  << data_treeindex.size() << endl; 
        cout << "TreeIndex Constructor called\n";
    } 

     TreeIndex& TreeIndex:: GetInstance(vector<DataVector>& cur_data)
    {
        if(par_instance == 0)
        {
            par_instance = new TreeIndex(cur_data);
        }
        return *par_instance;
    }
    void TreeIndex:: addDataVector(DataVector &a)
    {
        for(auto &v : data_treeindex)
        {
        if(v == a) return;
        }
    data_treeindex.push_back(a);
    }
    void TreeIndex:: removeDataVector(int idx)
    {
        if(idx < 0 || idx >= data_treeindex.size()) return;
        data_treeindex.erase(data_treeindex.begin() + idx);
    }



//KDTreeIndex Functions
KDTreeIndex:: KDTreeIndex(vector<DataVector>& data_v) :TreeIndex(data_v) {
        cout << "KDTreeIndex Constructor\n";
        root=MakeTree(data_v);
    } 
KDTreeIndex& KDTreeIndex:: GetInstance(vector<DataVector>& cur_data){
        if(instance == 0){
            instance = new KDTreeIndex(cur_data);
            }
        return *instance;
    };
KDNode* KDTreeIndex:: MakeTree(vector<DataVector> data_v){
        if (data_v.empty()) return NULL;
        root = BuildTreeRec(data_v, 0, data_v.size(), 0);
        return root;
};
set<pair<double,DataVector>> KDTreeIndex:: search(DataVector& query, int k)
    {
    set<pair<double,DataVector>> s;
    SearchTreeRecursive(root, query, k, s);
    return s;
    }
    void KDTreeIndex:: addDataVector(DataVector &a)
    {
        TreeIndex:: GetInstance(data_treeindex).addDataVector(a);
        TreeIndex:: addDataVector(a);
        freeTree(root);
        root = MakeTree(data_treeindex);
    }
    //Function to remove a DataVector from the index
    void KDTreeIndex:: removeDataVector(int idx)
    {
        TreeIndex:: GetInstance(data_treeindex).removeDataVector(idx);
        TreeIndex:: removeDataVector(idx);
        freeTree(root);
        root = MakeTree(data_treeindex);
    }

    

//RPTreeIndex Functions
    RPTreeIndex:: RPTreeIndex(vector<DataVector>& data_v) : TreeIndex(data_v){
    cout << "RPTreeIndex Constructor\n";
    // cout << data_v.size() << endl;
    root = MakeTree(data_v);
    }
    RPNode* RPTreeIndex:: MakeTree(vector<DataVector>& data_v){
        
        if (data_v.empty()) return NULL;
        
        root = BuildTreeRec(data_v, 0,0,data_v.size());
        return root;
    };
     RPTreeIndex& RPTreeIndex:: GetInstance(vector<DataVector>& cur_data){
        
    if(instance == 0)

    {   
        
        instance = new RPTreeIndex(cur_data);
    }
    return *instance;
    }

    set<pair<double,DataVector>> RPTreeIndex:: search(DataVector& query, int k)
    {
    set<pair<double,DataVector>> s;
    SearchTreeRecursive(root, query, k, s);
    return s;
    }
    void RPTreeIndex:: addDataVector(DataVector &a)
        {
            TreeIndex :: GetInstance(data_treeindex).addDataVector(a); // adds its to base instance "tree"
            TreeIndex :: addDataVector(a); // adds it to derived instance "rptree"
            freeTree(root);
            root = MakeTree(data_treeindex);
        }
    void RPTreeIndex:: removeDataVector(int idx)
        {
            TreeIndex :: GetInstance(data_treeindex).removeDataVector(idx); // adds its to base instance "tree"
            TreeIndex :: removeDataVector(idx); // adds it to derived instance "rptree"
            freeTree(root);
            root = MakeTree(data_treeindex);
        }






TreeIndex* TreeIndex::par_instance = 0;
KDTreeIndex* KDTreeIndex::instance = 0;
RPTreeIndex* RPTreeIndex::instance = 0; 

int main(){
    
    VectorDataset dataset;
    dataset.ReadDataset("fmnist-test.csv");
    vector<DataVector> data = dataset.getDataset();
   

    TreeIndex& tree = TreeIndex::GetInstance(data);
    KDTreeIndex& kd = KDTreeIndex::GetInstance(data);
    RPTreeIndex& rp = RPTreeIndex::GetInstance(data);
    cout<< "KDTreeIndex\n";
    // cout<<"In terms of distance\n";
    for(auto &pr : kd.search(data[13], 10))
    {
        cout << pr.first << " ";
    } 
    cout << endl;
    cout<< "RPTreeIndex\n";
    // cout<<"In terms of distance\n";
    for(auto &pr : rp.search(data[13], 10))
    {
        cout << pr.first << " ";
    }
    cout << endl;
    //Generating a random point
    //Verify add and remove
    vector<double> rand(784, 0);
    for(int i = 0; i < 784; i++)
    {
        rand[i] = i*2.0 + 10;
    }
    DataVector pt;
    pt.set(rand);
    kd.addDataVector(pt);
    //Added additional point in dataset randomly generated 
    cout<<"KDTreeIndex with randomly added Point\n";
    for(auto &pr : kd.search(pt, 10))
    {
        cout << pr.first << " ";
    }   
    cout << endl;
    //Removed the additional point in dataset randomly generated
    kd.removeDataVector(10001);
    cout<<"KDTreeIndex with  removed Point previously added \n";
    for(auto &pr : kd.search(pt, 10))
    {
        cout << pr.first << " ";
    }
    cout << endl;
    cout<<"RPTreeIndex \n";
    for(auto &pr : rp.search(pt, 10))
    {
        cout << pr.first << " ";
    }
    cout << endl;

    

    
    
    
    
    



}