How to compile: 
    1)just type make and all the executables will be compiled

How to run:
    1) just type the name of the executable, for example ./neigh
    2)  
      .)
        neigh simply tests the basic nn functionality of the trees
      .)
        class evaluates a kd based nn classifier on small synthetic 2d data 
      .)
        time-benchmark is the same but for 4d data , the train and test size is FAR bigger 
        changing the define instructions you can tweek the test and train set size 
      .) 
        odysMAIN was used for experiments with high dimensional data 
    3) 
        you don't need any libraries, just a simple g++ compiler . 
        That being said if your cpu specs differ from ours , the multithreaded 
        search might not perform that well
 
Source Files:
    KDtree.hpp -> Definition of the KDtree data structure (Class ) and it's functions
    KStree.cpp -> Code 

