# Project01_Flight Path Optimization using Dijkstra’s Algorithm
-------------
* notion: https://www.notion.so/Flight-Path-Optimization-52e4cd771bda4b47ab325f1245b1f2f7


## Program Introduction
This program find the shortest path between six airports in Korea by using Dijkstra algorithm.

## Data of South_Korea_airport_toy_example.csv  
<img width="389" alt="스크린샷 2022-04-08 오후 3 40 55" src="https://user-images.githubusercontent.com/49178058/162379005-69d0de2c-ac4b-4d78-ad74-65ba10c3bc1e.png">


## How to use
1. Complie. "g++ -std=c++11 Main.cpp -o a.out"
2. Enter the number of paths between airports.
3. Enter the path between the airports.   
   Enter the name of the airport at the start and the name of the airport at the end point.   
   (ex_ Jeju Yangyang)
4. Select the menu you want.
5. If you select shortest path menu,   
   then you can enter start airport, end airport,the number of bad weather regions and the points of regions.
   The result will be a shortest path that does not take the weather into account and a shortest path that takes the weather into account.
6. If you select show graph menu, You can see all edges in graph.
7. If you want to quit, choose menu 3.

## Example of execution
* Input   
<img width="455" alt="스크린샷 2022-04-08 오전 4 42 13" src="https://user-images.githubusercontent.com/49178058/162283555-55cd73dc-f938-4993-a14c-0037b9b2ee2c.png">   
   
* Result   
<img width="523" alt="스크린샷 2022-04-08 오전 4 42 32" src="https://user-images.githubusercontent.com/49178058/162283568-83c968e0-0a39-42df-a883-e5f17df6d915.png">
<img width="494" alt="스크린샷 2022-04-08 오전 4 42 37" src="https://user-images.githubusercontent.com/49178058/162283579-0b4c76a3-9925-4e7a-95ef-7977468c2c7f.png">


## Program Configuration
 * Main.cpp   
 Main file of this program
 * AdjasencyList.h   
 An adjacency list class is implemented. You can create a undirected, weighted graph by declaring a class object.
 * AL_project1_visualize_plot.ipynb   
 A visualization task of the resulting graph for the graph of initial conditions or the shortest path is implemented.
 * input.txt   
 Example of input. You can copy and use the program when you try it.
 * South_Korea_airport_toy_example.csv   
 csv data file. Data of airports.
 
## Distributor
* 김주호 21600144
* 신의진 21800408
