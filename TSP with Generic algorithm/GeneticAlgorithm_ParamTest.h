#pragma once 
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <string>
#include <queue>
#include <time.h>
#include <fstream>
#include <sstream>
using namespace std;

/* This file contains tools for conducting parameter tests and authorization convergence tests.
    output
    - Iteration_Info_List.csv 
*/

template<typename T>
std::ostream& write_typed_data(std::ostream& stream, const T& value) {
    return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

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

struct Rank{
    float index;
    float rank;
    double fitness;
    float SP; // select probability
    float sliceSize;
};

// 각 iteration 의 정보를 저장함 (for test)
struct IterInfo{
    int iterNum;
    double minimumValue;
    int populationSize;
};

struct Result{
    int iteration;
    float converValue;
};

class GeneticAlgorithm{
    private:
        int deliveryLocationNum;
        int startPoint;
        int initPopulationSize;
        vector<Individual> parentSet;
        vector<Individual> population; 
        priority_queue<IndividualInfo, vector<IndividualInfo>, cmp> populationInfo;
        double** adjMatrix;   
        int iter = 0;     
        int elitismPercent;
        int mutationProbability;
        // for test
        vector<IterInfo> iterInfoList;

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
            srand(time(NULL));

            //setting standard array
            double fitness;
            Individual individual;
            IndividualInfo info;
            
            for(int i=0;i<deliveryLocationNum;i++) individual.array.push_back(i);
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
            //RankRoultteWheel();
            // Elitism
            Elitism();
        }

        // Tournament
        void Tournament(int size){
            int repeatNum = population.size()/size;            
            int c1, c2; // contestant sol index
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
                //compete (small fitness is winner)
                if(population[indexList[2*i]].fitness < population[indexList[2*i+1]].fitness) // i is winner
                    parentSet.push_back(population[indexList[2*i]]);
                else // i+1 is winner
                    parentSet.push_back(population[indexList[2*i+1]]); 
            }
        }

        // Rank based roultte wheel 
        void RankRoultteWheel(){
            float RankSum = 0;
            priority_queue<IndividualInfo, vector<IndividualInfo>, cmp> copyPopulationInfo;
            Rank currentIndividual;
            vector<Rank> SPList;
            int rank = 0;

            // Modify as needed
            int MIN = 0;
            int MAX = 100;
            double f;
            double roultteSelector;
            float Amax = 1.2; //normal
            float Amin = 2-Amax; //normal
            
            //get sum of rank
            for(float i=1;i<=population.size();i++) RankSum = RankSum + i;
            //copy populationInfo 
            copyPopulationInfo = populationInfo;
            // calc SP of each individuals
            while(!copyPopulationInfo.empty()){
                //currentIndividual = new Rank;
                rank ++;
                currentIndividual.index = copyPopulationInfo.top().index;
                currentIndividual.fitness = copyPopulationInfo.top().fitness; // fitness based priority queue라서 필요함. 
                currentIndividual.rank = rank;
                //currentIndividual.SP = (rank * (100/RankSum));
                currentIndividual.SP = (Amax - (Amax-Amin)*(rank-1)/(RankSum-1))*1/RankSum;
                SPList.push_back(currentIndividual);
                if(rank == 1)currentIndividual.sliceSize = currentIndividual.SP;
                else if(rank> 1) currentIndividual.sliceSize = SPList[rank-1].sliceSize + currentIndividual.SP;
                copyPopulationInfo.pop();
            }
            /*
            for(int i=0;i<SPList.size();i++){
                cout << SPList[i].index << "    " << SPList[i].rank << "    " << SPList[i].SP << "......"<<SPList[i].sliceSize << "\n";
            }
            */
            int index = 0;
            bool selectSuccess = false;
            Individual selectedInd;
            for(int i=0;i<population.size()* 3/4;i++){
                selectSuccess = false;
                //while(!selectSuccess){
                    f = (double)rand() / RAND_MAX;
                    roultteSelector = MIN + f * (MAX - MIN);
                    //cout << roultteSelector<<" choose\n";
                    while(index < SPList.size()){
                        if(roultteSelector< SPList[index].sliceSize){
                            //cout << "selected: "<<roultteSelector<<" ------- ";
                            //cout << SPList[index].index <<" , "<< SPList[index].rank <<" , "<< SPList[index].SP <<" , "<< SPList[index].sliceSize<<"\n";
                            selectedInd = population[SPList[index].index];
                            parentSet.push_back(selectedInd);
                            SPList.erase(SPList.begin() + index);
                  //          selectSuccess = true;
                            break;
                        }
                        index++;
                    }
                //}
            }
        }
        

        // Elitism
        void Elitism(){
            int eliteNum = population.size()*elitismPercent/100;
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

        }
        #pragma endregion

        #pragma region Reproduction
        void Reproduction(){
            for(int i=0;i < parentSet.size()/2;i++){
                Crossover(parentSet[2*i].array, parentSet[2*i+1].array);
            }
            // clear parenet set
            parentSet.clear();
        }
        void Crossover(vector<int> a, vector<int> b) {
            vector<int> a_child = a;
            vector<int> b_child = b;
            queue<int> a_swapRange;//swap 전 해당 구간의 a 원소
            queue<int> b_swapRange;//swap 전 해당 구간의 b 원소
            priority_queue<int> a_needChange; //a에 이미 존재했던 b_range의 원소의 index
            priority_queue<int> b_needChange; //b에 이미 존재했던 a_range의 원소의 index
            int mutationRandomnum;

            int start = rand() % deliveryLocationNum;
            int end = rand() % deliveryLocationNum;

            //start,end 구간 랜덤 설정
            //end-start = size이면 사실상 부모꺼 전체가 하나씩 생기기 때문
            while (start == end || abs(end-start) == deliveryLocationNum-1) {
                start = rand() % deliveryLocationNum;
                end = rand() % deliveryLocationNum;
            }
            if (start > end) swap(start, end); 
            
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
                    if (j >= start && j <= end) continue;
                    if (a_swapElement == b_child[j]) {
                        b_needChange.push(j);
                    }
                    if (b_swapElement == a_child[j]) {
                        a_needChange.push(j);
                    }
                }
            }
            //change overleapping elements
            int changeCnt = b_needChange.size();
            for(int i=0;i<changeCnt;i++){
                swap(a_child[a_needChange.top()], b_child[b_needChange.top()]);
                a_needChange.pop();
                b_needChange.pop();
            }

            //Crossover로 만든 children 먼저 추가하기
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

            //(Crossover + Mutation)로 만든 children 추가하기 (mutation 발생시에만 추가)
            //Probabilistic Mutations 
            mutationRandomnum = rand()%100;
            if(mutationRandomnum < mutationProbability){
                a_child = Mutation(a_child);
                //child a
                c1.array = a_child;
                c1.fitness = CalcFitness(a_child);
                population.push_back(c1);
                c1Info.index = population.size()-1;
                c1Info.fitness = c1.fitness;
                populationInfo.push(c1Info);
            }
            mutationRandomnum = rand()%100;
            if(mutationRandomnum < mutationProbability){
                b_child = Mutation(b_child);
                //child b
                c2.array = b_child;
                c2.fitness = CalcFitness(b_child);
                population.push_back(c2);
                c2Info.index = population.size()-1;
                c2Info.fitness = c1.fitness;
                populationInfo.push(c2Info);
            }
        }

        vector<int> Mutation(vector<int> a){
            vector<int> a_mutation = a;

            //Reverse Sequence Mutation(RSM)
            int start = rand() % deliveryLocationNum;
            int end = rand() % deliveryLocationNum;
            while(start == end){
                end = rand() % deliveryLocationNum;
            }
            if (start > end) swap(start, end);

            while (start < end) {
                swap(a_mutation[start], a_mutation[end]);
                start++;
                end--;
            }
            return a_mutation;
        }

        #pragma endregion

        #pragma region Iteration convergence test tools
        void SaveIterInfo(int num, int size, double value){
            IterInfo info;
            info.iterNum = num;
            info.populationSize = size;
            info.minimumValue = value;
            iterInfoList.push_back(info);
        }

        void Save_csv(vector<IterInfo> iterInfoList){
            string f_name = "Iteration_Info_List.csv";
            ofstream outfile(f_name);

            int listSize = iterInfoList.size();
            int id;
            IterInfo item;
            
            outfile << "iterNum,populationSize,minimumValue\n";
            
            for(int i=0; i<listSize; i++){
                item = iterInfoList[i];
                outfile << item.iterNum << ","<< item.populationSize << ","<< item.minimumValue << "\n";
            }
            outfile.close();
        }
        #pragma endregion

    public:
        // generic main function
        Result Genetic(int locNum, double **matrix, int start, int iniPopulationCnt, int elitism, int mutation){
            Result result;
            deliveryLocationNum = locNum;
            adjMatrix = matrix;
            mutationProbability = mutation;
            startPoint = start;
            initPopulationSize = iniPopulationCnt;
            elitismPercent = elitism;
            //create initial population
            CreateInitialPopulation();
            
            int stopCnt = 0;
            float PreviousMinValue;
            //for(int i=0;i<iterationCnt;i++){
            while(population.size()>20){
            //while(stopCnt < 20 || population.size()>20){
                iter++;
                cout << endl<<"Iteration["<<iter<<"]" << "mutation_rate: "<< mutationProbability << "min value: "<< populationInfo.top().fitness<<"\n";
                 // path print
                for(int i=0;i<locNum;i++){
                    cout << population[populationInfo.top().index].array[i]<<", ";
                }
                cout << "\n";
                
                //selection
                Selection();
                //reproduction
                Reproduction();
                /*
                if(iter!= 0){
                    if(PreviousMinValue == populationInfo.top().fitness) stopCnt++;
                    else stopCnt = 0;
                }
                PreviousMinValue = populationInfo.top().fitness;
                */
                //save iteration information
                SaveIterInfo(iter, population.size(), populationInfo.top().fitness);
                //Reduce mutationProbability
                if(iter%3==0) mutationProbability = mutationProbability*0.95;
            }
            result.converValue = populationInfo.top().fitness;
            result.iteration =  iter;
            Save_csv(iterInfoList);
            
            return result;
        }

};