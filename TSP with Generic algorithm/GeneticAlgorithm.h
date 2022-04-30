#pragma once 
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <queue>
#include <time.h>
using namespace std;

struct IndividualInfo{
    int index;
    double fitness;
};

struct Individual{
    vector<int> array;
    double fitness;
};

struct cmp{
    bool operator()(IndividualInfo a, IndividualInfo b){
        return a.fitness > b.fitness;
    }
};

class GeneticAlgorithm{
    private:
        int deliveryLocationNum = 0;
        vector<Individual> ParentSet;
        vector<Individual> population; 
        priority_queue<IndividualInfo, vector<IndividualInfo>, cmp> populationInfo;
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
        
        void PrintIndividualSetWithInfo(vector<Individual> popSet, priority_queue<IndividualInfo, vector<IndividualInfo>, cmp>  popInfo){ // the function pop all elements from the queue. 
            for(int i=0;i<popSet.size();i++){
                cout << popInfo.top().index << ": ";
                for(int j=0;j<deliveryLocationNum;j++) cout << popSet[popInfo.top().index].array[j]<<" ";
                cout <<popInfo.top().fitness << "\n";
                popInfo.pop();
            }
        }

        void PrintIndividualSet(vector<Individual> set){
            for(int i=0;i<set.size();i++){
                for(int j=0;j<deliveryLocationNum;j++) cout << set[i].array[j]<<" ";
                cout <<"\n";
            }
        }
        #pragma endregion

        #pragma region Initialization
        void CreateInitialPopulation() {
            int initPopulationSize = 30;
            srand(time(NULL));

            //setting standard array
            double fitness;
            Individual individual;
            IndividualInfo info;
            
            for(int i=0;i<deliveryLocationNum;i++) individual.array.push_back(i);
            cout << "-------- Initial population --------\n";
            for (int i = 0; i < initPopulationSize; i++) {
                // get solution, push individual into population
                individual.array = CreateRandomSolution(individual.array);
                fitness = CalcFitness(individual.array);
                individual.fitness = fitness;
                population.push_back(individual);

                // get fitness and index of individual, push individual info to populationinfo
                info.fitness = fitness;
                info.index = population.size()-1;
                populationInfo.push(info);

                cout << info.index<< ": ";
                for(int j=0;j<individual.array.size();j++) cout << individual.array[j]<<" ";
                cout << info.fitness<< "\n";
            }            
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
            //Tournament
            Tournament(2);
            // Elitism
            Elitism(10);
        }
        
        // Tournament
        void Tournament(int size){
            int repeatNum = population.size()/size;            
            int c1, c2; // contestant sol index
            Individual winner;
            int indexList[population.size()];

            for(int i=0;i<population.size();i++) indexList[i] = i;

            // randomly listup individuals
            for(int i=0;i<repeatNum;i++){
                c1 = rand() % population.size();
                c2 = rand() % population.size();
                swap(indexList[c1], indexList[c2]);
            }
            //repeat tournament
            for(int i=0;i<repeatNum;i++){
                    //cout <<indexList[2*i] <<"  vs  "<<indexList[2*i+1]<<endl;
                //compete (small fitness is winner)
                if(population[indexList[i]].fitness < population[indexList[i+1]].fitness) // i is winner
                    ParentSet.push_back(population[indexList[i]]);
                else // i+1 is winner
                    ParentSet.push_back(population[indexList[i+1]]); 
            }
            cout << "------- Parent set--------\n";
            PrintIndividualSet(ParentSet);
        }
        
        // Roulette wheel Selection 


        // Elitism
        void Elitism(int percent){
            int eliteNum = population.size()*percent/100;
            vector<Individual> eliteSet;
            Individual elite;
            priority_queue<IndividualInfo, vector<IndividualInfo>, cmp> eliteInfo;
            int eliteIndex;
            IndividualInfo info;
            
            cout << "----- new generation(elite only): "<< population.size()<<"/"<<percent<<"%("<<eliteNum <<")--------\n";
            
            for(int i=0;i<eliteNum;i++){
                // save elite solution to elite vector
                eliteIndex = populationInfo.top().index;
                    // save elite soluation 
                elite.array = population[eliteIndex].array;
                elite.fitness = populationInfo.top().fitness;
                eliteSet.push_back(elite);
                    // save elite soluation info
                info.index = eliteSet.size()-1;
                info.fitness = elite.fitness;                
                eliteInfo.push(info);
                // delete elite solution from current population 
                population.erase(population.begin() + eliteIndex);
                populationInfo.pop();
                //cout <<"elite(index,fitness): "<<info.index<<","<<info.fitness<<"\n";
            }
            // change current generation population and elite population
            population.clear();
            eliteSet.swap(population); // swap delete previous vector. 
            swap(populationInfo, eliteInfo);

            PrintIndividualSetWithInfo(population, populationInfo);
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
            //selection
            Selection();
        }

};