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
        int startPoint;
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
            //PrintIndividualSet(ParentSet);
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
            
            cout << "-----New generation(after Elite): "<< population.size()<<"/"<<percent<<"%("<<eliteNum <<")--------\n";
            
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

            //PrintIndividualSetWithInfo(population, populationInfo);
        }

        #pragma endregion

        //reproduction
        #pragma region Reproduction
        void Reproduction(){
            for(int i=0;i<ParentSet.size()/2;i++){
                Crossover(population[2*i].array, population[2*i+1].array);
            }
            cout<<"-----------New generation(after Reproduction)-----------\n";
            //PrintIndividualSetWithInfo(population, populationInfo);
            PrintIndividualSet(population);
            

        }

        void Crossover(vector<int> a, vector<int> b) {
            vector<int> a_child = a;
            vector<int> b_child = b;
            queue<int> a_range;//swap 전 해당 구간의 a 원소
            queue<int> b_range;//swap 전 해당 구간의 b 원소
            vector<int> a_exist; //b에 이미 존재했던 a_range의 원소의 index
            vector<int> b_exist; //a에 이미 존재했던 b_range의 원소의 index

            int start = 3; // 논문 예시처럼 하는 거라 이해하기 편하실 거에요
            int end = 6;

            cout << "strat: " << start << "end: " << end << "\n";

            cout << "a_array: ";
            for (int i = 0; i < a.size(); i++) {
                cout << a[i] << " ";
            }
            cout << "\n";

            cout << "b_array: ";
            for (int i = 0; i < b.size(); i++) {
                cout << b[i] << " ";
            }
            cout << "\n";


            //해당 구간에 대한 swaping
            for (int i = start; i < end; i++) {
                a_range.push(a_child[i]);
                b_range.push(b_child[i]);
                swap(a_child[i], b_child[i]);
            }

            while (a_range.size() != 0 || b_range.size() != 0) {
                int a_front = a_range.front();
                int b_front = b_range.front();
                a_range.pop();
                b_range.pop();

                for (int i = 0; i < deliveryLocationNum; i++) {
                    if (i >= start && i < end) continue; //교환한 구간의 원소는 건너뛰기
                    if (a_front == b_child[i]) {
                        a_exist.push_back(i);
                    }
                    if (b_front == a_child[i]) {
                        b_exist.push_back(i);
                    }
                }
            }
            // 기존의 ordering을 유지하기 위해 sort
            sort(a_exist.begin(), a_exist.end());
            sort(b_exist.begin(), b_exist.end());

            for (int i = 0; i < a_exist.size(); i++) {
                a_child[b_exist[i]] = b[a_exist[i]];
                b_child[a_exist[i]] = a[b_exist[i]];
            }

            cout << "a'_array: ";
            for (int i = 0; i < a.size(); i++) {
                cout << a_child[i] << " ";
            }
            cout << "\n";

            cout << "b'_array: ";
            for (int i = 0; i < b.size(); i++) {
                cout << b_child[i] << " ";
            }
            cout << "\n";

            //Probabilistic Mutations 
            


            // save new child to population(next generation)
            Individual c1, c2;
            IndividualInfo c1Info, c2Info;
                //child a
            c1.array = a_child;
            c1.fitness = CalcFitness(a_child);
            population.push_back(c1);
            c1Info.index = population.size()-1;
            c1Info.fitness = c1.fitness;
            populationInfo.push(c1Info);
                //child b
            c2.array = b_child;
            c2.fitness = CalcFitness(b_child);
            population.push_back(c2);
            c2Info.index = population.size()-1;
            c2Info.fitness = c1.fitness;
            populationInfo.push(c2Info);
        }

        void Mutation(vector<int> a){
            
        }
        #pragma endregion

    public:
        // generic main function
        void Genetic(int locNum, double **matrix, int start){
            deliveryLocationNum = locNum;
            adjMatrix = matrix;
            startPoint = start;
            //create initial population
            CreateInitialPopulation();
            //selection
            Selection();
            //reproduction
            Reproduction();

        }

};