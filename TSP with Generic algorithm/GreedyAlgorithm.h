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
        int locCnt;
        double** adjMatrix;        
        //cheack all node visitied
        bool CheckAllVisited(bool *visitied, int deliveryLocationNum){
            for(int i=0;i<deliveryLocationNum;i++){
                if(!visitied[i]) return false;
            }
            return true;
        }
        double CalcFitness(int *solution){
            double fitness=0;
            for(int i=0;i<locCnt-1;i++){
                fitness += adjMatrix[solution[i]][solution[i+1]];
            }
            fitness += adjMatrix[solution[0]][solution[locCnt-1]];
            return fitness;
        }

       
    public:
        //  matrix 받아서 greedy 알고리즘 실행 
        void Greedy(double **matrix, int startPoint, int deliveryLocationNum){
            bool visited[deliveryLocationNum];
            locCnt = deliveryLocationNum;
            adjMatrix = matrix;

            for(int i=0;i<deliveryLocationNum;i++) visited[i] = false;
            
            int currentPoint = startPoint;
            int minWeight = currentPoint;
            
            int solution[deliveryLocationNum];
            int solutionIndex = 0;
            
            double fitness = 0;

            while(1){
                visited[currentPoint] = true;
                solution[solutionIndex] = currentPoint;
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
            fitness = CalcFitness(solution);
            cout << "[Greedy soluetion] : ";    
            for(int i=0;i<deliveryLocationNum;i++) cout<< solution[i]<<", ";
            cout << endl;
            cout <<"fitness:"<<fitness<<endl;
        }
};      