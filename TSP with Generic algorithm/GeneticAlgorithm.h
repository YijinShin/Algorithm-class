#pragma once 
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <queue>
#include <time.h>
using namespace std;

struct SolutionInfo{
    int index;
    double fitness;
};

struct cmp{
    bool operator()(SolutionInfo a, SolutionInfo b){
        return a.fitness > b.fitness;
    }
};

class GeneticAlgorithm{
    private:
        int deliveryLocationNum = 0;
        vector<vector<int>> ParentSet;
        vector<vector<int>> population;
        priority_queue<SolutionInfo, vector<SolutionInfo>, cmp> populationInfo;
        double** adjMatrix;

        #pragma region etc
        double CalcFitness(vector<int> solutionArray){
            double fitness = 0.0;

            for(int i=0;i<deliveryLocationNum-1;i++){
                fitness += adjMatrix[solutionArray[i]][solutionArray[i+1]];
            }
            fitness += adjMatrix[solutionArray[0]][solutionArray[deliveryLocationNum-1]];
            
            return fitness;  
        }
        
        void PrintSolutionInfo(vector<vector<int>> solSet, priority_queue<SolutionInfo, vector<SolutionInfo>, cmp>  solInfo){ // the function pop all elements from the queue. 
            for(int i=0;i<solSet.size();i++){
                cout << solInfo.top().index << ": ";
                for(int j=0;j<deliveryLocationNum;j++) cout << solSet[solInfo.top().index][j]<<" ";
                cout <<solInfo.top().fitness << "\n";
                solInfo.pop();
            }
        }
        #pragma endregion

        #pragma region Initialization
        void CreateInitialPopulation() {
            int initPopulationSize = 30;
            srand(time(NULL));

            //setting standard array
            vector<int> stand_arr;
            SolutionInfo solutionInfo;

            for(int i=0;i<deliveryLocationNum;i++) stand_arr.push_back(i);
            cout << "-------- Initial population --------\n";
            for (int i = 0; i < initPopulationSize; i++) {
                // get solution, push solution into population
                stand_arr = CreateRandomSolution(stand_arr);
                population.push_back(stand_arr);

                // get fitness and index of solution, push solution info to populationinfo
                solutionInfo.fitness = CalcFitness(stand_arr);
                solutionInfo.index = population.size()-1;
                populationInfo.push(solutionInfo);

                cout << solutionInfo.index<< ": ";
                for(int j=0;j<stand_arr.size();j++) cout << stand_arr[j]<<" ";
                cout << solutionInfo.fitness<< "\n";
            }

            /*print population 
            (if you use this, you have to stop program because this func pop all elements from priority queue)*/
            //PrintPopulationInfo();
        }

        // create one random solution
        vector<int> CreateRandomSolution(vector<int> stand_arr){

            for (int i = 0; i < deliveryLocationNum; i++) {
                int rn1 = rand() % deliveryLocationNum;
                int rn2 = rand() % deliveryLocationNum;
                swap(stand_arr[rn1], stand_arr[rn2]);
            }
            return stand_arr;
        }
        #pragma endregion

        #pragma region Selection
        void Selection(){
            vector<vector<int>> ParentSet;
            

            // Elitism
            Elitism(10);
        }
        // Elitism
        void Elitism(int percent){
            int eliteNum = population.size()*percent/100;
            vector<vector<int>> elite;
            priority_queue<SolutionInfo, vector<SolutionInfo>, cmp> eliteInfo;
            int eliteIndex;
            SolutionInfo info;
            
            cout << "----- new generation(elite only): "<< population.size()<<"/"<<percent<<"%("<<eliteNum <<")--------\n";
            
            for(int i=0;i<eliteNum;i++){
                // save elite solution to elite vector
                eliteIndex = populationInfo.top().index;
                    // save elite soluation 
                elite.push_back(population[eliteIndex]); 
                    // save elite soluation info
                info.index = elite.size()-1;
                info.fitness = populationInfo.top().fitness;
                eliteInfo.push(info);
                // delete elite solution from current population 
                population.erase(population.begin() + eliteIndex);
                populationInfo.pop();
                cout <<"elite(index,fitness): "<<info.index<<","<<info.fitness<<"\n";
            }

            // change current generation population and elite population
            population.clear();
            elite.swap(population); // swap delete previous vector. 
            swap(populationInfo, eliteInfo);

            //PrintSolutionInfo(population, populationInfo);
        }
        // Roulette wheel Selection 

        #pragma endregion

        //reproduction
        //replacement

    public:
        // generic main function
        void Genetic(int locNum, double **matrix){
            deliveryLocationNum = locNum;
            adjMatrix = matrix;
            //create initial population
            CreateInitialPopulation();
            //selection
            Selection();
        }

};