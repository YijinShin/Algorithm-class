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
        vector<vector<int>> Population;
        priority_queue<SolutionInfo, vector<SolutionInfo>, cmp> PopulationInfo;
        double** adjMatrix;

        double CalcFitness(vector<int> solutionArray){
            double fitness = 0.0;

            for(int i=0;i<deliveryLocationNum-1;i++){
                fitness += adjMatrix[solutionArray[i]][solutionArray[i+1]];
            }
            fitness += adjMatrix[solutionArray[0]][solutionArray[deliveryLocationNum-1]];
            
            return fitness;  
        }
        
        void PrintPopulationInfo(){ // the function pop all elements from the queue. 
            cout << "-----[print solution + info]--------\n";
            for(int i=0;i<Population.size();i++){
                cout << PopulationInfo.top().index << ": ";
                for(int j=0;j<deliveryLocationNum;j++) cout << Population[PopulationInfo.top().index][j]<<" ";
                cout <<PopulationInfo.top().fitness << "\n";
                PopulationInfo.pop();
            }
        }

        #pragma region Initialization
        void CreateInitialPopulation() {
            int initPopulationSize = 30;
            srand(time(NULL));

            //setting standard array
            vector<int> stand_arr;
            SolutionInfo solutionInfo;

            for(int i=0;i<deliveryLocationNum;i++) stand_arr.push_back(i);

            for (int i = 0; i < initPopulationSize; i++) {
                // get solution, push solution into population
                stand_arr = CreateRandomSolution(stand_arr);
                Population.push_back(stand_arr);

                // get fitness and index of solution, push solution info to populationinfo
                solutionInfo.fitness = CalcFitness(stand_arr);
                solutionInfo.index = Population.size()-1;
                PopulationInfo.push(solutionInfo);

                cout << solutionInfo.index<< ": ";
                for(int j=0;j<stand_arr.size();j++) cout << stand_arr[j]<<" ";
                cout << solutionInfo.fitness<< "\n";
            }

            //print population
            PrintPopulationInfo();
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

        }
        // elitism
        void Elitism(){
            
        }
        // Roulette wheel Selection 
        // Sorting population
        void SortingPopulation(){

        }
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
        }

};