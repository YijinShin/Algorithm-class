#include <iostream>
#include <fstream>
#include <string>
#include "DataPreprocessing.h"
#include "GreedyAlgorithm.h"
#include "GeneticAlgorithm.h"
using namespace std;

int main(int argc, char** argv){ // execute: ./a.out filename.csv
    int deliveryLocationNum = 0;
    int initPopulationSize;
    int iterationCnt;
    string fileName;
    Preprocessing preprocessing;
    GreedyAlgorithm greedy;
    GeneticAlgorithm genetic;

    cout << "File name: ";
    cin >> fileName;
    //get data and do pre-processing
    deliveryLocationNum = preprocessing.ReadData(fileName); 
    double **adjMatrix = preprocessing.ConstructAdjMatrix();
    //preprocessing.ShowAdjMatrix(adjMatrix);

    //Greedy
    greedy.Greedy(adjMatrix, 0, deliveryLocationNum);

    //Generic
    cout << "initial PopulationSize: ";
    cin >> initPopulationSize;
    cout << "Iteration: ";
    cin >> iterationCnt;
    genetic.Genetic(deliveryLocationNum,adjMatrix,0, initPopulationSize,iterationCnt);

    return 0;
}
