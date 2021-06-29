#include <iostream>
#include "KDTree.hpp"
#include <algorithm>
#include <pthread.h>
//USED TO PASS ARGUMENTS TO THREADS
struct thread_arguments {
    int** SearchPointsArray; // Each thread has its own share of the input array
    int** ReturnPointsArray; // and puts the nearest element in this output array
    int n_points;
    int dimensions;
    TreeNode* root;
};
//USED BY STD:N-TH ELEMENT
int temp_dimension;
int Tree::comparePoints(int *point1, int *point2) {
    if (point1[temp_dimension] < point2[temp_dimension])
        return 1;
    else
        return 0;
}

int EuclideanDistance(int *d1, int *d2, int k) {
    if (d1 == NULL || d2 == NULL)
        return 999999;
    int sum = 0;
    for (int i = 0; i < k; i++)
        sum = sum + (d1[i]-d2[i])*(d1[i]-d2[i]);
    return sum;
}
Tree::Tree(int **PointsArray, int n_points, int dimensions) {
    // Tree root is at depth 0.
    this->dimensions = dimensions;
    this->Root = this->BuildKDtree(PointsArray, 0, n_points);
}

TreeNode* Tree::BuildKDtree(int** PointsArray, int depth, int n_points) {
    if (n_points == 0)
        return NULL;
    else if (n_points == 1) {
        TreeNode* leaf_node_to_return = new TreeNode;
        leaf_node_to_return->leaf_node = 1;
        leaf_node_to_return->median_element = 0; // Irrelevant for leaf nodes
        leaf_node_to_return->point = PointsArray[0];
        leaf_node_to_return->Left = NULL;
        leaf_node_to_return->Right = NULL;
        return leaf_node_to_return;
    }
    else {
        temp_dimension = depth % this->dimensions; // This global variable is only used to tell the comparePoints function which dimension to compare by
        std::nth_element(PointsArray, PointsArray + n_points/2, PointsArray + n_points, this->comparePoints);
        int n_points_for_P1 = n_points / 2; // If n_points is odd, the middle one goes right
        int n_points_for_P2 = n_points - n_points_for_P1;

        // Make non-leaf node
        TreeNode* non_leaf_node = new TreeNode;
        non_leaf_node->leaf_node = 0;
        non_leaf_node->median_element = PointsArray[n_points_for_P1][depth % dimensions];
        non_leaf_node->point = NULL;
        non_leaf_node->Left = this->BuildKDtree(PointsArray, depth + 1, n_points_for_P1);
        non_leaf_node->Right = this->BuildKDtree(PointsArray + n_points_for_P1, depth + 1, n_points_for_P2);
        return non_leaf_node;
    }
}
///////////////////////////// THREADS STUFF
int Tree::nearestNeighborThreads(TreeNode *root, int *point, int k, int depth, int*& curBest, int &distance) {
    if (root == NULL)
        return distance;
    if ((root->Right == NULL) && (root->Left == NULL)) {
        int eucl = EuclideanDistance(root->point, point, k);
        if (eucl < distance) {
            curBest = root->point;
            distance = eucl; // For the parents to check
            return eucl;
        }
        return distance;
    }
    else { // Internal node
        int splitValue = point[depth%k];
        if (splitValue < root->median_element) {
            int leftDist = nearestNeighborThreads(root->Left, point, k, depth+1, curBest, distance);
            if (splitValue + distance > root->median_element)
                int rightValue = nearestNeighborThreads(root->Right, point, k, depth+1, curBest, distance);
        }
        else {
            int rightValue = nearestNeighborThreads(root->Right, point, k, depth+1, curBest, distance);
            if (splitValue - distance <= root->median_element)
                int leftValue = nearestNeighborThreads(root->Left, point, k, depth+1, curBest, distance);
        }
    }
    return distance;
}
void* thread_f(void* thread_argz) {
    int** SearchPointsArray = ((struct thread_arguments*)thread_argz)->SearchPointsArray; // Each thread has its own share of the input array
    int** ReturnPointsArray = ((struct thread_arguments*)thread_argz)->ReturnPointsArray; // and modifies this output array to point to the closest training set point in the tree.
    int n_points = ((struct thread_arguments*)thread_argz)->n_points;
    int dimensions = ((struct thread_arguments*)thread_argz)->dimensions;
    TreeNode* root = ((struct thread_arguments*)thread_argz)->root;
    // std::cout << "THREAD points: " << n_points << std::endl;

    int best;
    for (int i = 0; i < n_points; ++i) { // Call recursive search for every search point
        best = 999999;
        Tree::nearestNeighborThreads(root, SearchPointsArray[i], dimensions, 0, ReturnPointsArray[i], best);
    }
    pthread_exit(NULL);
}
int Tree::ThreadSearchWrapper(int **SearchPointsArray, int n_points, int **ReturnPointsArray, int n_threads) {
// I took George's search function and made it multithreaded.
// Arguments are: An array of every point we want to search, the number of points, an array of NULL pointers, and the number of threads.
// ReturnPointsArray is the array of NULL pointers. I the end, each pointer will point to an existing point in the tree that is closest to the corresponding SearchPoint.
// SearchPointsArray and ReturnPointsArray are split evenly between the threads, and every thread is only responsible for calculating its part.
// There is no return value. Threads read and modify their share of the 2 arrays.
    int n_points_for_each_thread = n_points / n_threads;
    pthread_t* threadz = new pthread_t[n_threads];
    int retourn;
    struct thread_arguments *thread_argz = new struct thread_arguments[n_threads];
    for (int i = 0; i < n_threads; ++i) {
        thread_argz[i].SearchPointsArray = SearchPointsArray + i*n_points_for_each_thread;
        thread_argz[i].ReturnPointsArray = ReturnPointsArray + i*n_points_for_each_thread;
        thread_argz[i].n_points = n_points_for_each_thread;
        if (i == n_threads - 1)
            thread_argz[i].n_points = n_points - i*n_points_for_each_thread; // The last thread may take some more points because n_points is not divided perfectly by N_THREADS
        thread_argz[i].dimensions = this->dimensions;
        thread_argz[i].root = this->Root;
        retourn = pthread_create(threadz+i, NULL, thread_f, thread_argz+i);
    }
    void *r;
    for (int i = 0; i < n_threads; ++i) {
        retourn = pthread_join(threadz[i], &r);
    }
    delete[] threadz;
    delete[] thread_argz;
    return 0;
}
///////////////////////////// END OF THREADS STUFF
int Tree::NN(int *point, int **curBest) {
    int best = 999999;
    return nearestNeighbor(this->Root, point, this->dimensions, 0, curBest, best);
}
int Tree::nearestNeighbor(TreeNode *root, int *point, int k, int depth, int** curBest, int &distance) {
    if (root == NULL)
        return distance;
    if ((root->Right == NULL) && (root->Left == NULL)) {
        //std::cout<<"eucl="<<EuclideanDistance(root->point,point,k)<<" distance="<<distance<<"\n";
        int eucl = EuclideanDistance(root->point, point, k);
        if (eucl < distance) {
            *curBest = root->point;
            distance = eucl; // For the parents to check
            return eucl;
        }
        return distance;
    }
    else { // Internal node
        int splitValue = point[depth%k];
        if (splitValue < root->median_element) {
            int leftDist = nearestNeighbor(root->Left, point, k, depth+1, curBest, distance);
            //std::cout<<"leftDist="<<leftDist<<" best="<<distance<<"\n";
            if (splitValue + distance > root->median_element)
                int rightValue = nearestNeighbor(root->Right, point, k, depth+1, curBest, distance);
        }
        else {
            int rightValue = nearestNeighbor(root->Right, point, k, depth+1, curBest, distance);
            if (splitValue - distance <= root->median_element)
                int leftValue = nearestNeighbor(root->Left, point, k, depth+1, curBest, distance);
        }
    }
    return distance;
}
void Tree::print() {
    // std::cout << "Tree dimensions: " << this->dimensions << std::endl;
    this->print_recursive(this->Root, 0);
}
void Tree::print_recursive(TreeNode* root, int depth) {
    if (root == NULL)
        return;
    if (root->Left != NULL)
        this->print_recursive(root->Left, depth + 1);
    if (root->leaf_node == 0)
        std::cout << "Depth " << depth << ": Non leaf node median element = " << root->median_element << std::endl;
    else {
        std::cout << "Depth " << depth << ": LEAF node point: ";
        for (int i = 0; i < this->dimensions; ++i)
            std::cout << root->point[i] << " ";
        std::cout << "- label: " << root->point[this->dimensions];
        std::cout << std::endl;
    }
    if (root->Right != NULL)
        this->print_recursive(root->Right, depth + 1);
}
void Tree::tree_destructor_recursive(TreeNode* root) {
    if (root == NULL)
        return;
    if (root->Left != NULL)
        tree_destructor_recursive(root->Left);
    if (root->Right != NULL)
        tree_destructor_recursive(root->Right);
    if (root->leaf_node == 1)
        delete[] root->point;
    delete root;
}
Tree::~Tree() {
    this->tree_destructor_recursive((this->Root));
}