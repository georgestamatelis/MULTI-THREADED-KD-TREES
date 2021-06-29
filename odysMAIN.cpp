#include "KDTree.hpp"
#include <iostream>
#include <sys/time.h>
#include <cstring>
//IF YOU WANT TO TRY SOMETHING DIFFERRENT JUST CHANGE THE DEFINE CALLS 
//AND HIT MAKE AGAIN
#define TRAINSIZE 10000
#define TESTSIZE 20000
#define DIMENSIONS 8 //we tried with 8,16,32
using namespace std;

inline void print_time(timeval &tv1, timeval &tv2, char *message) {
    int seconds = tv2.tv_sec - tv1.tv_sec;
    int useconds = tv2.tv_usec - tv1.tv_usec;
    int minutes = 0;
    if (useconds < 0) {
        seconds--;
        useconds = useconds + 1000000;
    }
    if (seconds >= 60) {
        minutes = seconds / 60;
        seconds = seconds % 60;
    }
    printf("%s %d:%02d.%06d\n", message, minutes, seconds, useconds);
}

int EuclideanDistance(int *d1,int *d2,int k);
int **makeDataset(int size) { // Stamatelis function for creating datasets
    int **points = new int *[size];
    for (int i = 0; i < size/2; i++) {
        points[i] = new int[DIMENSIONS + 1];
        for (int j = 0; j < DIMENSIONS; ++j)
            points[i][j] = rand() % 10 + 1;
        points[i][DIMENSIONS] = 0; // Label
    }
    for (int i = size/2; i < size; i++) {
        points[i] = new int[DIMENSIONS + 1];
        for (int j = 0; j < DIMENSIONS; ++j)
            points[i][j] = rand() % 10 + 60;
        points[i][DIMENSIONS] = 1; // Label
    }
    return points;
}

int main(int argc, char const *argv[]) {
    struct timeval tv1, tv2, tv3, tv4, tv5, tv6;
    char message[100];
    srand(22350);

    int** PointsArray = makeDataset(TRAINSIZE);
    int** SearchPointsArray = makeDataset(TESTSIZE);
    int** NearestPointPointers = new int*[TESTSIZE];
    for (int i = 0; i < TESTSIZE; ++i)
        NearestPointPointers[i] = NULL; // This is just a bunch of pointers that point to the nearest point. The nearest points themselves already exist in PointsArray.

/////////////////////////////////////////////////////////////////////
    gettimeofday(&tv1, NULL);

    Tree* tri = new Tree(PointsArray, TRAINSIZE, DIMENSIONS);

    gettimeofday(&tv2, NULL);
    strcpy(message, "Time to build tree         ");
    print_time(tv1, tv2, message);

    for (int i = 0; i < TESTSIZE; i++) {
        int minDist = 9999999;
        int bestLabel = -1;
        for (int j = 0; j < TRAINSIZE; j++) {
            int curDist = EuclideanDistance(PointsArray[j], SearchPointsArray[i], DIMENSIONS);
            if (curDist < minDist)
                bestLabel = PointsArray[j][DIMENSIONS];
        }
    }

    gettimeofday(&tv3, NULL);
    strcpy(message, "Time for brute force       ");
    print_time(tv2, tv3, message);

    int* currentBest;
    for (int i = 0; i < TESTSIZE; ++i) {
        tri->NN(SearchPointsArray[i], &currentBest);
        // for (int j = 0; j < DIMENSIONS; ++j) {
        //     if (currentBest[j] != NearestPointPointers[i][j])
        //         printf("MISTAKE\n");
        // }
    }

    gettimeofday(&tv4, NULL);
    strcpy(message, "Time for single threaded NN");
    print_time(tv3, tv4, message);

    tri->ThreadSearchWrapper(SearchPointsArray, TESTSIZE, NearestPointPointers, 6);

    gettimeofday(&tv5, NULL);
    strcpy(message, "Time for 6 threads         ");
    print_time(tv4, tv5, message);

    tri->ThreadSearchWrapper(SearchPointsArray, TESTSIZE, NearestPointPointers, 12);

    gettimeofday(&tv6, NULL);
    strcpy(message, "Time for 12 threads        ");
    print_time(tv5, tv6, message);
/////////////////////////////////////////////////////////////////////

    delete tri;
    for (int i = 0; i < TESTSIZE; ++i)
        delete[] SearchPointsArray[i];
    delete[] NearestPointPointers;
    delete[] PointsArray;
    delete[] SearchPointsArray;
    return 0;
} 