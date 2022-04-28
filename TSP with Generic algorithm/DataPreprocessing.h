#pragma once 
#include <iostream>
using namespace std;

struct DeliveryLocation{
    int id;
    pair<double, double> location;
};

class Preprocessing{
    private:
        int deliveryLocationNum = 0;
    public:
        // read csv()
        // save csv()
        // calc weight()
        // make adj matrix() > return adj matrix 
        double** ConstructAdjMatrix(){
            double[deliveryLocationNum][deliveryLocationNum] adjMatrix;
            return adjMatrix;
        }
};