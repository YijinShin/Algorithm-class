#include <iostream>
#include <fstream>
#include "DataPreprocessing.h"
using namespace std;

int main(int argc, char** argv){ // execute: ./a.out filename.csv
    Preprocessing preprocessing;

    //get data and do pre-processing
    preprocessing.ReadData(argv[0]); 
    double *adjMatrix = preprocessing.ConstructAdjMatrix();

    //Greedy

    //Generic

    return 0;
}
