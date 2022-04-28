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
    public:
        // read csv()
        int ReadData(string filename)
        {
            int file_lines = 0;
            fstream fs; 
            vector<string> lines;
            string line;

            fs.open(filename, ios::in);
            if(!fs.is_open()) cout << "Not found"<<endl;
            
            // Read csv by line
            getline(fs, line); 
            while(getline(fs, line)){
                lines.push_back(line); // cut by '\n'
                file_lines++;
            }
            cout << "index: "<< file_lines<< endl;

            // Create and setting node list
            DeliveryLocationList = new DeliveryLocation[file_lines+1]; // make Node list, 0은 사용하지 x
            for(int i=1;i<=file_lines;i++){ // cut by ',' and make Node
                istringstream ss(lines[i-1]);
                //DeliveryLocationList[i].id = i;
                DeliveryLocationList[i].id = (stof(line));
                getline(ss, line, ',');
                DeliveryLocationList[i].location.first = (stof(line));
                getline(ss, line, ',');
                DeliveryLocationList[i].location.second = (stof(line));
            }
            cout << endl;
            fs.close();
            return file_lines;
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
        double* ConstructAdjMatrix(){
            double adjMatrix[deliveryLocationNum][deliveryLocationNum];
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
            return (*adjMatrix);
        }
};