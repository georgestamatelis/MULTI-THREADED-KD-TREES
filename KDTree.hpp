#pragma once
typedef struct tree_node TreeNode;
struct tree_node {
    char leaf_node; // Leaf node is 1, otherwise it's 0.
    int median_element; // Only meaningful for non-leaf nodes
    int* point; // Only for leaf nodes. Point is an array: int[dimensions].
    TreeNode* Left;
    TreeNode* Right;
};

class Tree {
private:
    TreeNode* Root;
    int dimensions; // Last dimension of points is label so the array is n_points x (dimensions+1)
    TreeNode* BuildKDtree(int** PointsArray, int depth, int n_points);
    void print_recursive(TreeNode* root, int depth);
    void tree_destructor_recursive(TreeNode* root);
    static int comparePoints(int *point1, int *point2);
public:
//builds a "balanced KD tree according to the slides"
    Tree(int **points, int n_points, int dimensions);
//nearest neighbor search on the tree acording to the slides
    int NN(int *point, int **curBest);
//same as before, but parallelised for speed up
    int ThreadSearchWrapper(int **SearchPointsArray, int n_points, int **ReturnPointsArray, int n_threads);
    //user doesn't need to call these two
    static int nearestNeighborThreads(TreeNode *root, int *point, int k, int depth, int*& curBest, int &distance);
    
    int nearestNeighbor(TreeNode *root, int *point, int k, int depth, int **curBest, int &distance);
    void print();
    ~Tree();
};