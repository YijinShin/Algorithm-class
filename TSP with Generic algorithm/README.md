# Project02_TSP with generic algorithm
-------------
* notion: https://www.notion.so/Algorithm-project-52e4cd771bda4b47ab325f1245b1f2f7


## Program Introduction
This program uses genetic algorithms to solve  Traveling Salesman Problem(TSP).    


## Data 
- Cincinnati.csv
- Boston.csv
- Denver.csv
- Atlanta.csv
- New_York.csv
- Philadelphia.csv
- Can_Francisco.csv  


## How to use
 * Normal execute    
 Complie. "g++ -std=c++11 Main.cpp -o a.out"    
 In Main.cpp file, you can change data file, initial population size, percent of elitism and mutation.    
 * Execute and save the result to csv    
 Change header file "GeneticAlgorithm.h" to "GeneticAlgorithm_ParamTest.h"    
 Change genetic part with comment "GA with testing"    
 Complie. "g++ -std=c++11 Main.cpp -o a.out"    
 

## Example of execution
<img width="361" alt="스크린샷 2022-05-17 오전 2 39 32" src="https://user-images.githubusercontent.com/49178058/168653737-9284f924-d2ed-4ff8-a52d-6a20f9c1530c.png">



## Program Configuration
 * Main.cpp   
 Main file of this program
 * DataPreprocessing.h   
 Read csv file and construct adjacency matrix with csv data. 
 * GreedyAlgorithm.h
 Solve TSP problem using greedy algorithm.
 * GeneticAlgorithm.h
 Solve TSP problem using genetic algorithm.
 * GenericAlgorithm_ParamTest.h
 Solve TSP problem using genetic algorithm and save the result to csv file.
 * IterationGraph.ipynb
 Draw Graph of Minimum value per iteration with Iteration_Info_List.csv data.
 * AL_project2_visualize_plot.ipynb  

 * Iteration_Info_List.csv   
 Save data of all iteration and the minimum distence of each iteration. 
 * Result_List.csv
 Save data of iteration count, minimum distence and the path of each execution.
 
 
 
## Distributor
* 김주호 21600144
* 신의진 21800408
