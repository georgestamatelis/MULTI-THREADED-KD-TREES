#include "KDTree.hpp"
#include <iostream>
#include <string>
/*
this is a nn classifier for a small train set and a small test set
1)we insert some points of two classes into the tree , the dataset is pretty simple
 the two classes are far from each other 
2)Then we evaluate the classifier on some other points
*/
int main()
{
    //2d train set points
    int x1Train[]={0,3,0,4,2,3,6,-1,-2,5,/**/11,15,14,11,12,29,17};
    int x2Train[]={3,2,-1,3,4,4,5,4,3,0,/**/0,1,-2,-1,4,5,-3};
    int labels[]={0,0,0,0,0,0,0,0,0,0,/**/1,1,1,1,1,1,1};//and their labels

    int trainLengh=17;
    int **points=new int *[trainLengh];
    for(int i=0;i<trainLengh;i++)
    {
     
        points[i]=new int[3];
        points[i][0]=x1Train[i];
        points[i][1]=x2Train[i];
        points[i][2]=labels[i];

    }
    //add the train set to the tree
    Tree *kd=new Tree(points,trainLengh,2);
    //BUILD THE TEST SET
    int testLen=9;
    int x1Test[]={0,2,1,5,3,/**/10,13,15,21};
    int x2Test[]={1,3,0,0,1,/**/-1,-3,5,7};
    int labelsTest[]={0,0,0,0,0,/**/1,1,1,1};
    //EVALUATE THE CLASSIFIER
    int totalMistakes=0;
    for(int i=0;i<testLen;i++)
    {   
        /*
        Important note: Each data point of k dimensions is represented by an array 
        of k+1 integers. The integers from 0-k are the dimensions and the last is the label
        */
        int label;
        int *currentBest=NULL;
        int data[2];
        data[0]=x1Test[i];
        data[1]=x2Test[i];
        kd->NN(data,&currentBest);
        label=currentBest[2]; 
        if(label!=labelsTest[i])
        {
            std::cout<<"MISSCLASIFIED "<<data[0]<<","<<data[1]<<"\n";
            std::cout<<"NEIGHBOR IS "<<currentBest[0]<<","<<currentBest[1]<<"\n";
            std::cout<<"--------------------------------------------\n";
            totalMistakes++;
        }
    }
    printf("Total mistakes made by the classifier=%d\n",totalMistakes);
    delete[] points;
    delete kd;
}