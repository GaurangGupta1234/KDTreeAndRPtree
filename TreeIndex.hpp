#include"VectorDataset.hpp"
#include <algorithm> // for std::numeric_limits
#include <cmath>     // for std::abs
#include <set>
#include <random>
#include <utility>
//constants
const long long max_rand= 1e9;
//TreeIndex class
class TreeIndex{
    protected:
    vector<DataVector> data_treeindex;
    //Default constructor
    TreeIndex() {
        // cout << data_treeindex.size() << endl;
    }
    //Parameterized constructor
    TreeIndex(vector<DataVector>& cur_data);
    //Destructor
    ~TreeIndex() {} 
    //Static instance of TreeIndex
    static TreeIndex* par_instance;
    //Brute force k nearest neighbours
    void knnbruteforce(vector<DataVector>& cur_nodepoints,DataVector& query,int k,set<pair<double,DataVector>>& s){
        int total = cur_nodepoints.size();
        vector<pair<double,int>> dist;
        for (int i = 0; i < total; i++)
        {
            dist.push_back({query.distance(cur_nodepoints[i]), i});
        }
        sort(dist.begin(), dist.end());
        for (int i = 0; i < total; i++)
        {
            if(s.size() < k)
            {
                s.insert({dist[i].first,cur_nodepoints[dist[i].second]});
            }
            else if(s.size() == k)
            {
                if(dist[i].first < s.rbegin()->first)
                {
                    s.erase(*s.rbegin());
                    s.insert({dist[i].first,cur_nodepoints[dist[i].second]});
                }
            }
        }

    }
    public:
    //Static method to get instance of TreeIndex
    static TreeIndex& GetInstance(vector<DataVector>& cur_data);
    //Function to add a DataVector to the index
    void addDataVector(DataVector &a);
    //Function to remove a DataVector from the index
    void removeDataVector(int idx);
    
};
//KDTreeIndex Node,derieved from TreeIndex
typedef struct KDNode {
    struct KDNode* left;
    struct KDNode* right;
    vector<DataVector> nearest_points;
    int axis;
    double median;
    
}KDNode;
//KDTreeIndex class
class KDTreeIndex : public  TreeIndex{
    private: 
    //Parameterized constructor
    KDTreeIndex(vector<DataVector>& data_v); 
    //Destructor
    ~KDTreeIndex() {} 
    //Static instance of KDTreeIndex
    static KDTreeIndex* instance;
    //Root of the KDTree
    KDNode* root;
    //Recursive function to build the KDTree
    KDNode* BuildTreeRec(vector<DataVector>& dataset,int start,int end,int depth){
        if (end-start<100) return NULL;
        KDNode* node = new KDNode;
        for(auto it = dataset.begin() + start; it != dataset.begin() + end; it++)
        {
            node->nearest_points.push_back(*it);
        }
        int axis = chooseAxis(dataset, start, end, depth);
        int medianIdx = (start + end) / 2;

        sort(dataset.begin() + start, dataset.begin() + end, [axis](const DataVector& a, const DataVector& b) {
            int pt_a = a.getElem(axis);
            int pt_b = b.getElem(axis);
            return pt_a < pt_b;
        });
        
        
        node->axis = axis;
        node->median = dataset[medianIdx].getElem(axis);
        
        // Recursively build left and right subtrees
        node->left = BuildTreeRec(dataset, start, medianIdx, depth + 1);
        node->right = BuildTreeRec(dataset, medianIdx + 1, end, depth + 1);

        return node;

    }
    //Function to choose the axis using max spread
    int chooseAxis(vector<DataVector>& data_v, int start, int end, int depth){
        int all_dimensions = data_v[0].getDimension();
        vector<int> spread(all_dimensions, 0);
        for (int i = 0; i < all_dimensions; ++i) {
            int min_val = 10000;
            int max_val = -10000;
            for (int j = start; j < end; ++j) {
                min_val = min(min_val, data_v[j].getElem(i));
                max_val = max(max_val, data_v[j].getElem(i));
            }
            spread[i] = max_val - min_val;
        }

        // Choose axis with largest spread
        int max_spread_index = 0;
        for (int i = 1; i < all_dimensions; ++i) {
            if (spread[i] > spread[max_spread_index]) {
                max_spread_index = i;
            }
        }

        return max_spread_index;

    }
    //Recursive function to search the KDTree
    void SearchTreeRecursive(KDNode* node, DataVector& query, int k,set<pair<double,DataVector>> &s)
    {
        if(node == NULL) return;
        if(query.getElem(node->axis) <= node->median )
        {
            SearchTreeRecursive(node->left,query,k,s);
            // current nearest neighbours are not enough
            if(s.size() < k)
            {
                knnbruteforce(node->nearest_points,query,k,s);
            }  
            else{              
                double check_dist_hyperplane = fabs(query.getElem(node->axis) - node->median);
                if(check_dist_hyperplane < s.rbegin()->first)
                {
                    knnbruteforce(node->right->nearest_points,query,k,s);
                }
                else return;
            }
        }
        else
        {
            SearchTreeRecursive(node->right,query,k,s);
            // current nearest neighbours are not enough
            if(s.size() < k)
            {
                knnbruteforce(node->nearest_points,query,k,s);
            }  
            else{              
                double check_dist_hyperplane = fabs(query.getElem(node->axis) - node->median);
                if(check_dist_hyperplane < s.rbegin()->first)
                {
                    knnbruteforce(node->left->nearest_points,query,k,s);
                }
                else return;
            }
        }
    }
        KDNode* freeTree(  KDNode* node) {
            if (node == nullptr) {
            return NULL;
        }

        freeTree(node->left);
        freeTree(node->right);

        delete node;
        return NULL;
        }  
    public:
    //Static method to get instance of KDTreeIndex
    static KDTreeIndex& GetInstance(vector<DataVector>& cur_data);

    //Function to make the KDTree
    KDNode* MakeTree(vector<DataVector> data_v);
    //Function to search the KDTree
    set<pair<double,DataVector>> search(DataVector& query, int k);
    //Function to free the KDTree
     
    //Function to add a DataVector to the index
    void addDataVector(DataVector &a);
    //Function to remove a DataVector from the index
    void removeDataVector(int idx);
    
};
//RPTreeIndex Node,derieved from TreeIndex
typedef struct RPNode{
    struct RPNode* left;
    struct RPNode* right;
    vector<DataVector> nearest_points;
    DataVector axis;
    double median;
    double error;
}RPNode;
class RPTreeIndex : public  TreeIndex{
    private: 
    //Default constructor
    RPTreeIndex() {}
    //Parameterized constructor
    RPTreeIndex(vector<DataVector>& data_v);
    //Static instance of RPTreeIndex
    static RPTreeIndex* instance;
    RPNode* root;
    //Recursive function to build the RPTree
    void ChooseRule(RPNode& node){
        //generating random unit vector
        double sum_square = 0;
        vector<double> axis_temp;
        int D = node.axis.getDimension();
        for(int i=0;i<node.axis.getDimension();i++)
        {
            double temp = (double)rand() / RAND_MAX;
            axis_temp.push_back(temp);
            sum_square += temp*temp;
        }
        sum_square = sqrtf(sum_square);
        for(int i=0;i<node.axis.getDimension();i++)
        {
            axis_temp[i] = axis_temp[i]/sum_square;
        }
        node.axis.set(axis_temp);
        //generating random error term
        
        double max_dis = 0;
        for(int i=0;i<node.nearest_points.size();i++)
        {
            max_dis = max(max_dis,node.nearest_points[i].distance(node.nearest_points[0]));
        }
        double range = 0.06*max_dis/sqrt(D);
        uniform_real_distribution<double> unif(-1.0*range,range);
        default_random_engine re;
        node.error = unif(re);
        vector<double> dot_products;
        

        for(int i=0;i<node.nearest_points.size();i++)
        {
            dot_products.push_back(node.nearest_points[i]*node.axis);
        }
        sort(dot_products.begin(),dot_products.end());
        node.median = dot_products[dot_products.size()/2];
        return;
        
        
        // int D = node.nearest_points[1].getDimension();
        
        
        // node.unitvector.setDimension(D);
        
        // vector<double> temp(D);
        

        // for(int i=0;i<D;i++){
        //     double random_num =(double)rand()/RAND_MAX;
        //     temp[i] = random_num;
        //     sum_square += (random_num*random_num);
        // }
        
        // sum_square = sqrtf(sum_square);
        // for(int i=0;i<D;i++)temp[i]/=sum_square;

        // //finding random error term 
        // double max_dis = 0;
        // for(int i=0;i<node.nearest_points.size();i++)
        // {
        //     max_dis = max(max_dis,node.nearest_points[i].distance(node.nearest_points[0]));
        // }
        // double range = 6*max_dis/sqrt(D);

        // uniform_real_distribution<double> unif(-1.0*range,range);
        // default_random_engine re;
        // node.error = unif(re);
        // node.unitvector.set(temp);

        // //finding the median of dot products of vectors in dataset with random unit vector
        // vector<double> temp2;
        // for(int i=0;i<node.nearest_points.size();i++){
        //     temp2.push_back((node.nearest_points[i])*(node.unitvector));
        // }
        // sort(temp2.begin(),temp2.end());
        // int n = temp2.size();
        // node.median = (temp2[(n-1)/2]+temp2[n/2])/2.0;
        
    }
    RPNode* BuildTreeRec(vector<DataVector>& dataset,int depth,int start,int end){
        
        if(dataset.size()<500) return NULL;
        RPNode* cur = new RPNode();
        cur->axis.setDimension(dataset[0].getDimension());
        for(auto it = dataset.begin() + start; it != dataset.begin() + end; it++)
        {
            cur->nearest_points.push_back(*it);
        }
        
        ChooseRule(*cur);
        
        vector<DataVector> left,right;
        // cout<<left.size() << " " << right.size() << endl;
        for(int i=start;i<end;i++)
        {
            if((dataset[i]*cur->axis) > cur->median + cur->error)
            {
                right.push_back(dataset[i]);
            }
            else
            {
                left.push_back(dataset[i]);
            }
        }
        int split_idx = start + left.size();
        int ptr = start;
        for(int i=0;i<left.size();i++)
        {
            dataset[ptr] = left[i];
            ptr++;
        }
        for(int i=0;i<right.size();i++)
        {
            dataset[ptr] = right[i];
            ptr++;
        }
        left.clear();   
        right.clear();
        // cout << left.size() << " " << right.size() << endl;
        
        cur->left = BuildTreeRec(left,depth+1,start,split_idx);
        cur->right = BuildTreeRec(right,depth+1,split_idx,end);

        return cur;

    }
    //Function to choose the rule(Generating a random unit vector and a random error term for the hyperplane)
    
    //Recursive function to search the RPTree
    void SearchTreeRecursive(RPNode* node, DataVector& query, int k,set<pair<double,DataVector>> &s)
    {   if(node == NULL) return;
        //Base case
        double dot_product = query*node->axis;
        if(dot_product > node->median + node->error)
        {
            SearchTreeRecursive(node->left,query,k,s);
            // current nearest neighbours are not enough
            if(s.size() < k)
            {
                knnbruteforce(node->nearest_points,query,k,s);
            }  
            else{              
                double check_dist_hyperplane = abs(dot_product - node->median);
                if(check_dist_hyperplane < s.rbegin()->first)
                {
                    knnbruteforce(node->right->nearest_points,query,k,s);
                }
                else return;
            }
        }
        else
        {
            SearchTreeRecursive(node->right,query,k,s);
            // current nearest neighbours are not enough
            if(s.size() < k)
            {
                knnbruteforce(node->nearest_points,query,k,s);
            }  
            else{              
                double check_dist_hyperplane = abs(dot_product - node->median);
                if(check_dist_hyperplane < s.rbegin()->first)
                {
                    knnbruteforce(node->left->nearest_points,query,k,s);
                }
                else return;
            }
        }

        // if(node->left==NULL){
        //     knnbruteforce(node->nearest_points,query,k,s);
        // }
        // if(query*node->unitvector<=node->median+node->error){
        //     //go left while moving down the recursion tree
        //     SearchTreeRecursive(node->left,query,k,s);

        //     //check dividing hyperplane distance 
        //     double dist = fabs(query*node->unitvector-(node->median+node->error));
        //     if(s.size()<k){
        //         knnbruteforce(node->right->nearest_points,query,k,s);
        //     }
        //     else{
        //         if(dist<(*(--s.end())).first){
        //             knnbruteforce(node->right->nearest_points,query,k,s);
        //         }
        //         else return;
        //     }
        // }
        // else{
        //     //go right while moving down the recursion tree
        //     knnbruteforce(node->right->nearest_points,query,k,s);

        //     //check if sibling region needs to be searched
        //     double dist = fabs(query*node->axis-(node->median+node->error));
        //     if(s.size()<k){
        //         knnbruteforce(node->left->nearest_points,query,k,s);
        //     }
        //     else{
        //         if(dist<(*(--s.end())).first){
        //             knnbruteforce(node->left->nearest_points,query,k,s);
        //         }
        //         else return;
        //     }
        // }
    }
    RPNode* freeTree(RPNode* node) {
    if (node == nullptr) {
        return NULL;
    }

    freeTree(node->left);
    freeTree(node->right);

    delete node;
    return NULL;
    }
    public:
    RPNode* MakeTree(vector<DataVector>& data_v);
    //Static method to get instance of RPTreeIndex
    static RPTreeIndex& GetInstance(vector<DataVector>& cur_data);
    //Function to search the RPTree
    set<pair<double,DataVector>> search(DataVector& query, int k);
    //Function to free the RPTree
    
    //Function to add a DataVector to the index
        void addDataVector(DataVector &a);
        //Function to remove a DataVector from the index
        void removeDataVector(int idx);
    }; 
