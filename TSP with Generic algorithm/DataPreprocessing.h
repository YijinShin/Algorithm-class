#pragma once 
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

struct DeliveryLocation{
    int id;
    pair<double, double> location;
};

class Preprocessing{
    private:
        int deliveryLocationNum = 0;
        DeliveryLocation* DeliveryLocationList;
        double** adjMatrix;
    public:
        // read csv()
        int ReadData(string filename)
        {
            fstream fs; 
            vector<string> lines;
            string line;

            fs.open(filename, ios::in);
            if(!fs.is_open()) cout << "Not found"<<endl;
            
            // Read csv by line
            getline(fs, line); 
            while(getline(fs, line)){
                lines.push_back(line); // cut by '\n'
                deliveryLocationNum++;
            }
            cout << "index: "<< deliveryLocationNum<< endl;


            // Create and setting node list
            DeliveryLocationList = new DeliveryLocation[deliveryLocationNum]; // make Node list, 0은 사용하지 x
            for(int i=0;i<deliveryLocationNum;i++){ // cut by ',' and make Node
                istringstream ss(lines[i]);
                getline(ss, line, ',');
                DeliveryLocationList[i].id = (stof(line));
                getline(ss, line, ',');
                DeliveryLocationList[i].location.first = (stof(line));
                getline(ss, line, ',');
                DeliveryLocationList[i].location.second = (stof(line));
                //cout << DeliveryLocationList[i].id << "," << DeliveryLocationList[i].location.first << ","<<DeliveryLocationList[i].location.second<<endl;
            }
            cout << endl;
            fs.close();
            return deliveryLocationNum;
        }

        // save csv()  
        

        // calc weight()
        double CalcWeight(pair<double,double> start, pair<double,double> end){
            // Haversine Formula
            double weight;
            double radius = 6371; // earth radius (km)  
            double toRadian = M_PI / 180;

            double deltaLatitude = abs(start.first - end.first) * toRadian;
            double deltaLongitude = abs(start.second - end.second) * toRadian;

            double sinDeltaLat = sin(deltaLatitude / 2);
            double sinDeltaLng = sin(deltaLongitude / 2);
            double squareRoot = sqrt( sinDeltaLat * sinDeltaLat + cos(start.first * toRadian) 
                                * cos(end.first * toRadian) * sinDeltaLng * sinDeltaLng);

            weight = 2 * radius * asin(squareRoot);
            return weight;
        }

        // make adj matrix(), return adj matrix 
        double** ConstructAdjMatrix(){
            //double adjMatrix[deliveryLocationNum][deliveryLocationNum];
            adjMatrix = new double*[deliveryLocationNum];
            for(int i=0;i<deliveryLocationNum;i++){adjMatrix[i] = new double[deliveryLocationNum];}
            // calc weight of each edges
            for(int i=0;i<deliveryLocationNum;i++){
                for(int j=0;j<deliveryLocationNum;j++){
                    if(i==j) adjMatrix[i][j] =  0; 
                    else{
                        double weight = CalcWeight(DeliveryLocationList[i].location,DeliveryLocationList[j].location);
                        adjMatrix[i][j] =  weight;
                        adjMatrix[j][i] =  weight;
                    }
                }   
            }
            return adjMatrix;
        }

        // show AdjMatrix
        void ShowAdjMatrix(double **matrix){
            for(int i=0;i<deliveryLocationNum;i++){
                for(int j=0;j<deliveryLocationNum;j++){
                    cout << matrix[i][j] << "  /  ";
                }    
                cout <<endl;
            }
        }   
};