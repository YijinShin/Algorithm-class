#include <iostream>
#include <fstream>
#include <string>
#include "DataPreprocessing.h"
#include "GreedyAlgorithm.h"
#include "GeneticAlgorithm.h"
using namespace std;

int main(int argc, char** argv){ // execute: ./a.out filename.csv
    int deliveryLocationNum = 0;
    Preprocessing preprocessing;
    GreedyAlgorithm greedy;
    GeneticAlgorithm genetic;

    //get data and do pre-processing
    deliveryLocationNum = preprocessing.ReadData("Cincinnati.csv"); 
    double **adjMatrix = preprocessing.ConstructAdjMatrix();
    //preprocessing.ShowAdjMatrix(adjMatrix);

    //Greedy
    greedy.Greedy(adjMatrix, 0, deliveryLocationNum);

    //Generic
    genetic.Genetic(deliveryLocationNum);

    return 0;
}
