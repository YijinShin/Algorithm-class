#include <iostream>
#include <fstream>
#include <string>
#include "DataPreprocessing.h"
#include "GreedyAlgorithm.h"
#include "GeneticAlgorithm2.h"
using namespace std;

double CalcFitness(int deliveryLocationNum, double **adjMatrix, vector<int> solutionArray);

int main(int argc, char** argv){ // execute: ./a.out filename.csv
    int deliveryLocationNum = 0;
    int initPopulationSize = 10000;
    int iterationCnt;
    int elitism = 40;  // 기존 45
    int mutation = 50; // 기존 20
    string fileName;
    Preprocessing preprocessing;
    GreedyAlgorithm greedy;
    GeneticAlgorithm genetic;

    //cout << "File name: ";
    //cin >> fileName;
    
    //get data and do pre-processing
        //deliveryLocationNum = preprocessing.ReadData(fileName); 
    deliveryLocationNum = preprocessing.ReadData("Atlanta.csv"); 
    double **adjMatrix = preprocessing.ConstructAdjMatrix();

    //Greedy
    greedy.Greedy(adjMatrix, 0, deliveryLocationNum);

    //Generic
    /*
    cout << "initial PopulationSize: ";
    cin >> initPopulationSize;
    cout << "Elitism percent: ";
    cin >> elitism;
    */
    genetic.Genetic(deliveryLocationNum,adjMatrix,0, initPopulationSize, elitism,mutation);

    //calc specific solution
    
    vector<int> solution = {5,9,15,3,18,14,7,13,17,0,6,1,2,16,12,8,11,10,4,19};
    double value = CalcFitness(deliveryLocationNum,adjMatrix, solution);
    cout << "\n ex_generic value:"<<value<<endl;

    solution = {17, 16, 2, 1, 0, 6, 12, 8, 11, 10, 4, 19, 5, 9, 15, 3, 18, 14, 7, 13};
    value = CalcFitness(deliveryLocationNum,adjMatrix, solution);
    cout << "\n ex_generic value:"<<value<<endl;
    

    return 0;
}
double CalcFitness(int deliveryLocationNum, double **adjMatrix, vector<int> solutionArray){
    double fitness = 0.0;

    for(int i=0;i<deliveryLocationNum-1;i++){
        fitness += adjMatrix[solutionArray[i]][solutionArray[i+1]];
    }
    fitness += adjMatrix[solutionArray[0]][solutionArray[deliveryLocationNum-1]];
    
    return fitness;  
}
