#include <iostream>
#include <string>
#include "KDTree.hpp"
using namespace std;

/*in these examples we don't classify data points , all points have a label 0*/
/*
All we do is insert some points and then check the neihgbors of some other points , it is done to make sure
the tree is build/ works correctly

*/

int main(int argc, char const *argv[]) {
    int** PointsArray = new int*[9];
    for (int i = 0; i < 9; ++i)
    {
        PointsArray[i] = new int[4];
        PointsArray[i][3]=0;//don't care we only want neighbors
    }
    PointsArray[0][0] = 8; PointsArray[0][1] = 10; PointsArray[0][2] = 4;
    PointsArray[1][0] = 3; PointsArray[1][1] = 20; PointsArray[1][2] = 11;
    PointsArray[2][0] = 7; PointsArray[2][1] = 8; PointsArray[2][2] = 6;
    PointsArray[3][0] = 15; PointsArray[3][1] = 12; PointsArray[3][2] = 1;
    PointsArray[4][0] = 21; PointsArray[4][1] = 17; PointsArray[4][2] = 14;
    PointsArray[5][0] = 19; PointsArray[5][1] = 13; PointsArray[5][2] = 5;
    PointsArray[6][0] = 16; PointsArray[6][1] = 15; PointsArray[6][2] = 2;
    PointsArray[7][0] = 3; PointsArray[7][1] = 1; PointsArray[7][2] = 4;
    PointsArray[8][0] = 16; PointsArray[8][1] = 7; PointsArray[8][2] = 9;
    Tree* tri = new Tree(PointsArray,9,3);
    tri->print();//print the tree for debugging purposes 
    /*NOW TIME FOR THE NN SEARCHES*/
    int *point=new int[3];
    point[0]=9;
    point[1]=11;
    point[2]=4;
    int *curBest=NULL;
    int dist=tri->NN(point,&curBest);
    std::cout<<"CLOSET DISTANCE FOR 9,11,4 IS: "<<dist<<"\n";
    std::cout<<"Nearest neighbor: "<<curBest[0]<<","<<curBest[1]<<","<<curBest[2]<<"\n";
    point[0]=3; point[1]=-1;point[2]=0;
    dist=tri->NN(point,&curBest);
    std::cout<<"CLOSET DISTANCE FOR 3,-1,0 IS: "<<dist<<"\n";
    std::cout<<"Nearest neighbor: "<<curBest[0]<<","<<curBest[1]<<","<<curBest[2]<<"\n";
    point[0]=25; point[1]=25;point[2]=25;
    dist=tri->NN(point,&curBest);
    std::cout<<"CLOSET DISTANCE FOR 25,25,25 IS: "<<dist<<"\n";
    std::cout<<"Nearest neighbor: "<<curBest[0]<<","<<curBest[1]<<","<<curBest[2]<<"\n";
    point[0]=0; point[1]=0;point[2]=0;
    dist=tri->NN(point,&curBest);
    std::cout<<"CLOSET DISTANCE FOR 0,0,0 IS: "<<dist<<"\n";
    std::cout<<"Nearest neighbor: "<<curBest[0]<<","<<curBest[1]<<","<<curBest[2]<<"\n";
    point[0]=25; point[1]=7;point[2]=-15;
    dist=tri->NN(point,&curBest);
    std::cout<<"CLOSET DISTANCE FOR 25,7,-15 IS: "<<dist<<"\n";
    std::cout<<"Nearest neighbor: "<<curBest[0]<<","<<curBest[1]<<","<<curBest[2]<<"\n";
    point[0]=5; point[1]=6;point[2]=7;
    dist=tri->NN(point,&curBest);
    std::cout<<"CLOSET DISTANCE FOR 5,6,7 IS: "<<dist<<"\n";
    std::cout<<"Nearest neighbor: "<<curBest[0]<<","<<curBest[1]<<","<<curBest[2]<<"\n";
    point[0]=15; point[1]=12;point[2]=1;
    dist=tri->NN(point,&curBest);
    std::cout<<"CLOSET DISTANCE FOR 15,12,1 IS: "<<dist<<"\n";
    std::cout<<"Nearest neighbor: "<<curBest[0]<<","<<curBest[1]<<","<<curBest[2]<<"\n";
    point[0]=16; point[1]=7;point[2]=9;
    dist=tri->NN(point,&curBest);
    std::cout<<"CLOSET DISTANCE FOR 16,7,9 IS: "<<dist<<"\n";
    std::cout<<"Nearest neighbor: "<<curBest[0]<<","<<curBest[1]<<","<<curBest[2]<<"\n";
    //CLEAN UP SPACE , NO MEMORY LEAKS
    delete tri;
    for (int i = 0; i < 9; ++i) 
    {
        delete[] PointsArray[i];
    }
    delete[] PointsArray;
    delete[] point;
    return 0;
}