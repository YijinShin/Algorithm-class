#include <iostream>
#include <fstream>
#include <string>
#include "DataPreprocessing.h"
#include "Greedy.h"
using namespace std;

int main(int argc, char** argv){ // execute: ./a.out filename.csv
    int deliveryLocationNum = 0;
    Preprocessing preprocessing;
    Greedy greedy;

    //get data and do pre-processing
    deliveryLocationNum = preprocessing.ReadData("Cincinnati.csv"); 
    double **adjMatrix = preprocessing.ConstructAdjMatrix();
    preprocessing.ShowAdjMatrix(adjMatrix);

    //Greedy
    greedy.GreedyAlgorithm(adjMatrix, 0, deliveryLocationNum);


    //Generic

    return 0;
}
