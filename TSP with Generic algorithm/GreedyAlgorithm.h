#pragma once 
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;


class GreedyAlgorithm{
    private:
        //cheack all node visitied
        bool CheckAllVisited(bool *visitied, int deliveryLocationNum){
            for(int i=0;i<deliveryLocationNum;i++){
                if(!visitied[i]) return false;
            }
            return true;
        }
    public:
        //  matrix 받아서 greedy 알고리즘 실행 
        void Greedy(double **adjMatrix, int startPoint, int deliveryLocationNum){
            bool visited[deliveryLocationNum];
            for(int i=0;i<deliveryLocationNum;i++) visited[i] = false;
            
            int currentPoint = startPoint;
            int minWeight = currentPoint;
            
            int solution[deliveryLocationNum];
            int solutionIndex = 0;

            while(1){
                visited[currentPoint] = true;
                    //cout << "Visited["<< currentPoint<<"]"<<endl;
                solution[solutionIndex] = currentPoint;
                    //cout << "solution["<< solutionIndex<<"]: "<<currentPoint<<endl;
                if(CheckAllVisited(visited, deliveryLocationNum))break;
                
                minWeight = currentPoint;
                solutionIndex++;
                for(int i=0;i<deliveryLocationNum;i++){
                    if(!visited[i]){ // not visited
                        if(adjMatrix[currentPoint][minWeight] == 0) minWeight = i;
                        else if(adjMatrix[currentPoint][i] < adjMatrix[currentPoint][minWeight]) minWeight = i;
                    }
                }
                currentPoint = minWeight;
            }

            cout << "[Greedy soluetion] : ";    
            for(int i=0;i<deliveryLocationNum;i++) cout<< solution[i]<<", ";
            cout << endl <<endl;
        }
};      