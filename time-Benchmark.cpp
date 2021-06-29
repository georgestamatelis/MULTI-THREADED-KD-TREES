#include "KDTree.hpp"
#include <iostream>
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */
#include <sys/time.h>
#include <string>
//IF YOU WANT TO TEST FOR DIFFERENT DIMENSIONS , JUST CHANGE THE DEFINES AND HIT MAKE AGAIN
#define TRAINSIZE 1000000
#define TESTSIZE 1000000
int EuclideanDistance(int *d1,int *d2,int k);
int **makeTrainSet() {
    int **points = new int *[TRAINSIZE];
    for(int i=0;i<TRAINSIZE/2;i++) {
        points[i] = new int[5];
        points[i][0]=rand() % 10 + 1;
        points[i][1]=rand() % 10 + 1;
        points[i][2]=rand() % 10 + 1;
        points[i][3]=rand() % 10 + 1;
        points[i][4]=0; //LABEL
    }
    for(int i=TRAINSIZE/2;i<TRAINSIZE;i++) {
        points[i] = new int[5];
        points[i][0]=rand() % 10 + 60;
        points[i][1]=rand() % 10 + 60;
        points[i][2]=rand() % 10 + 60;
        points[i][3]=rand() % 10 + 60;
        points[i][4]=1; //LABEL
   }
    return points;
}
int **makeTestSet() {
    int **points = new int *[TESTSIZE];
    for(int i=0;i<TESTSIZE/2;i++) {
        points[i] = new int[5];
        points[i][0]=rand() % 10 + 1;
        points[i][1]=rand() % 10 + 1;
        points[i][2]=rand() % 10 + 1;
        points[i][3]=rand() % 10 + 1;
        points[i][4]=0; //LABEL
    }
    for(int i=TESTSIZE/2;i<TESTSIZE;i++) {
        points[i] = new int[5];
        points[i][0]=rand() % 10 + 60;
        points[i][1]=rand() % 10 + 60;
        points[i][2]=rand() % 10 + 60;
        points[i][3]=rand() % 10 + 60;
        points[i][4]=1; //LABEL
   }
    return points;
}
int main() {
    struct timeval tv;
    struct tm *info;
    char buffer[100];
    clock_t t;
    srand (time(NULL));
    //MAKE TRAIN AND TEST SET
    int **trainData=makeTrainSet();
    int **testData=makeTestSet();
    std::cout<<"Train and test set initialised\n";
    //BUILD THE KD TREE
    t = clock();
    Tree *kd=new Tree(trainData,TRAINSIZE,4);
    t = clock()-t;
    std::cout<<"Points inserted in kd tree in "<< ((float)t)/CLOCKS_PER_SEC <<" seconds\n";
    std::cout<<"CLASSIFICATION BEGINS\n";
    //TRADITIONAL NEAREST NEIGHBOR SEARCH JUST LIKE THE SLIDES/BOOK
    gettimeofday(&tv, NULL);
    info = localtime(&(tv.tv_sec));
    strftime (buffer, sizeof(buffer), "Starting 1 thread search --------- %I:%M:%S", info);
    printf("%s:%ld\n", buffer, tv.tv_usec);
    
    clock_t t2 = clock();
    int totalMistakes=0;
    for(int i=0;i<TESTSIZE;i++) {
        int label;
        int *currentBest=NULL;
        int data[4];
        data[0]=testData[i][0];
        data[1]=testData[i][1];
        data[2]=testData[i][2];
        data[3]=testData[i][3];
        kd->NN(data,&currentBest);
        label=currentBest[4];
        if(label!=testData[i][4]) {
            std::cout<<"MISSCLASIFIED "<<data[0]<<","<<data[1]<<"\n";
            std::cout<<"NEIGHBOR IS "<<currentBest[0]<<","<<currentBest[1]<<"\n";
            std::cout<<"--------------------------------------------\n";
            totalMistakes++;
        }
    }
    // printf("SINGLE THREADED SEARCH DONE\n");

    gettimeofday(&tv, NULL);
    info = localtime(&(tv.tv_sec));
    //MULTI THREADED SEARCH
    strftime (buffer, sizeof(buffer), "Completed. Starting many threads - %I:%M:%S", info);
    printf("%s:%ld\n", buffer, tv.tv_usec);

    ////////////////////////////////
    totalMistakes=0;
    int **currentBest2 = new int*[TESTSIZE];
    for (int i = 0; i < TESTSIZE; ++i) {
        currentBest2[i] = NULL;
    }
    int label;
    int search_threads = 12;
    kd->ThreadSearchWrapper(testData, TESTSIZE, currentBest2, search_threads);
    for(int i=0;i<TESTSIZE;i++) {
        label=currentBest2[i][4];
        if(label!=testData[i][4]) {
            std::cout<<"MISSCLASIFIED "<<testData[i][0]<<","<<testData[i][1]<<"\n";
            std::cout<<"NEIGHBOR IS "<<currentBest2[i][0]<<","<<currentBest2[i][1]<<"\n";
            std::cout<<"--------------------------------------------\n";
            totalMistakes++;
        }
    }
    ///////////////////////////////////////////
    t2=clock() -t2 ;
    std::cout<<"TOTAL MISTAKES "<<totalMistakes << " IN "<< ((float)t2)/CLOCKS_PER_SEC <<" seconds \n";

    gettimeofday(&tv, NULL);
    info = localtime(&(tv.tv_sec));
    strftime (buffer, sizeof(buffer), "Many threads complete ------------ %I:%M:%S", info);
    printf("%s:%ld\n", buffer, tv.tv_usec);
    
    std::cout<<"#OF TRAIN DATA="<<TRAINSIZE<<" NUMBER OF TEST DATA = "<<TESTSIZE<<"\n";
    //and now for brute force 
    clock_t t3=clock();


    for(int i=0;i<TESTSIZE ;i++)//andnow for brute force .very slow
    {

        int minDist=9999999;
        int bestLabel=-1 ;
        for (int j=0;j< TRAINSIZE;j++)

        {
            int curDist=EuclideanDistance(testData[i]  ,trainData[j],4);
            if(curDist<  minDist)
            {
                bestLabel =trainData[i][4];       //keep  smallest distance
            }
            

        }
    }
    t3=clock()-t3;
    std::cout<<"BRUTE FORCE TOOK "<<((float)t3)/CLOCKS_PER_SEC<<" SECONDS\n";
    //CLEAN UP SPACE
    for (int j = 0; j < TESTSIZE; ++j) {
        delete[] testData[j];
    }
    delete[] trainData;
    delete[] testData;
    delete kd;
    delete[] currentBest2;
    return 0;
}