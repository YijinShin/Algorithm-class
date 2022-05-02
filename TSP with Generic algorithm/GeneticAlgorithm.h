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
        int deliveryLocationNum;
        int startPoint;
        vector<Individual> parentSet;
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

        void voidPrintArray(vector<int> array){
            for (int i = 0; i < array.size(); i++) {
                cout << array[i] << " ";
            }
            cout<<"\n";
        }

        #pragma endregion

        #pragma region Initialization
        void CreateInitialPopulation() {
            int initPopulationSize = 500000;
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
            for(int i=0;i<population.size();i++){
                c1 = rand() % population.size();
                c2 = rand() % population.size();
                swap(indexList[c1], indexList[c2]);
            }
            //repeat tournament
            for(int i=0;i<repeatNum;i++){
                    //cout <<indexList[2*i] <<"  vs  "<<indexList[2*i+1]<<endl;
                //compete (small fitness is winner)
                if(population[indexList[2*i]].fitness < population[indexList[2*i+1]].fitness) // i is winner
                    parentSet.push_back(population[indexList[2*i]]);
                else // i+1 is winner
                    parentSet.push_back(population[indexList[2*i+1]]); 
            }
            //cout << "------- Parent set("<<parentSet.size()<<")--------\n";
            //PrintIndividualSet(parentSet);
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
                populationInfo.pop();
            }
            population.clear();
            // change current generation population and elite population
            eliteSet.swap(population); // swap delete previous vector. 
            swap(populationInfo, eliteInfo);

            //cout << "----Elite: "<< population.size()<<"/"<<percent<<"%("<<eliteNum <<")----\n";
            //PrintIndividualSet(population);
        }
        #pragma endregion

        #pragma region Reproduction
        void Reproduction(){
            for(int i=0;i < parentSet.size()/2;i++){
                //cout <<"(crossover: "<<i<<"): "<<2*i<<","<<2*i+1<<endl;
                Crossover(parentSet[2*i].array, parentSet[2*i+1].array);
            }
            // clear parenet set
            parentSet.clear();
            //cout<<"-----------New generation(after Reproduction)("<<population.size()<<")-----------\n";
            //PrintIndividualSetWithInfo(population, populationInfo);
            //PrintIndividualSet(population);
        }
        void Crossover(vector<int> a, vector<int> b) {
            vector<int> a_child = a;
            vector<int> b_child = b;
            queue<int> a_swapRange;//swap 전 해당 구간의 a 원소
            queue<int> b_swapRange;//swap 전 해당 구간의 b 원소
            priority_queue<int> a_needChange; //a에 이미 존재했던 b_range의 원소의 index
            priority_queue<int> b_needChange; //b에 이미 존재했던 a_range의 원소의 index
            int mutationProbability = 30;
            int mutationRandomnum;

            int start = 3; // 논문 예시처럼 하는 거라 이해하기 편하실 거에요
            int end = 6;

            /*
            cout << "a_parent: ";   
            voidPrintArray(a);
            cout << "b_parent: ";
            voidPrintArray(b);
            */
            
            //swaping
            for (int i = start; i <= end; i++) {
                a_swapRange.push(a_child[i]);
                b_swapRange.push(b_child[i]);
                swap(a_child[i], b_child[i]);
            }
            //check if array has overlapping elememts
            for(int i=0 ; i<=end-start ; i++){ // repeate rangeNum
                int a_swapElement = a_swapRange.front();
                int b_swapElement = b_swapRange.front();
                b_swapRange.pop();
                a_swapRange.pop();

                for (int j = 0; j < deliveryLocationNum; j++) {
                    if (j >= start && j <= end) continue; //교환한 구간의 원소는 건너뛰기
                    //cout << "comp: "<<a_swapElement<<","<<b_child[j]<<endl;
                    if (a_swapElement == b_child[j]) {
                        //cout << "*\n";
                        b_needChange.push(j);
                    }
                    //cout << "comp: "<<b_swapElement<<","<<a_child[j]<<endl;
                    if (b_swapElement == a_child[j]) {
                       //cout << "*\n";
                        a_needChange.push(j);
                    }
                }
            }
            //change overleapping elements
            //cout <<"겹침 size:"<<b_needChange.size()<<endl;
            //cout <<"겹침 size:"<<a_needChange.size()<<endl;
            int changeCnt = b_needChange.size();
            for(int i=0;i<changeCnt;i++){
                swap(a_child[a_needChange.top()], b_child[b_needChange.top()]);
                a_needChange.pop();
                b_needChange.pop();
            }
            /*
            cout << "a_child_array: ";
            voidPrintArray(a_child);
            cout << "b_child_array: ";
            voidPrintArray(b_child);
            */

            //Probabilistic Mutations 
            mutationRandomnum = rand()%100;
            if(mutationRandomnum < mutationProbability) a_child = Mutation(a_child);
            mutationRandomnum = rand()%100;
            if(mutationRandomnum < mutationProbability) b_child = Mutation(b_child);
            
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

        vector<int> Mutation(vector<int> a){
            vector<int> a_mutation = a;

            //Reverse Sequence Mutation(RSM)
            //start,end 구간 랜덤 설정
            int start = rand() % deliveryLocationNum;
            int end = rand() % deliveryLocationNum;
            while(start == end){
                end = rand() % deliveryLocationNum;
            }
            if (start > end) swap(start, end);
            //cout << "strat: " << start << "end: " << end << "\n";

            while (start < end) {
                swap(a_mutation[start], a_mutation[end]);
                start++;
                end--;
            }
            return a_mutation;
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
            
            for(int i=0;i<20;i++){
                cout << endl<<"Iteration["<<i<<"]"<<endl;
                //selection
                Selection();
                //reproduction
                Reproduction();
            }
            cout<<"\n\n-----------Result sol -------------\n";
            PrintIndividualSetWithInfo(population, populationInfo);
        }

};