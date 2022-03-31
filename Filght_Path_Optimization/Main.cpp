#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include "AdjacencyList.h"
using namespace std;

#define BUFSIZE 128

struct Airport{ // airport info 
    int id;
    string name;
    string type;
    string IATA;
    string ICAO;
    vector<float> location;
};

struct Section{ // danger section info
    vector<float> point_one;
};

int ShowMenu();
int ReadData(string filename, Airport*& airportList);
int FindAirport_NametoId(string name, Airport* airportList, int airportCnt);
int FindAirport_IdtoName(int id, Airport* airportList, int airportCnt);
void GetEdge(AdjList* adjList,Airport* airportList, int airportCnt);
void ShowAirportList(Airport* airportList, int airportCnt);

int main(){
    int menu;
    AdjList adjList;
    int airportCnt; // 공항 cnt
    Airport* airportList;

    // Get csv data  
    airportCnt = ReadData("South_Korea_airport_toy_example.csv",airportList);
    ShowAirportList(airportList,airportCnt);
    
    // Create Adj list 
    adjList.AdjListSetting(airportCnt);
    
    // Get edge 
    GetEdge(&adjList, airportList, airportCnt);

    // main
    while(1){
        menu = ShowMenu();
        if(menu == 1){
            // Dij
            // reseult print
        }else if(menu == 2){
            // get input of section
            // edge check 
            // Dij
            // reseult print
        }else if(menu == 3){
            adjList.ShowList();
        }else if(menu == 4){
            break;
        }
    }
    return 0;
}


int ShowMenu(){
    int menu;
    cout << "\n1. Shortest path: with out weather\n2. Shortest path: with weather\n3.Print Graph\n4.Quit\n"; 
    cin >> menu;
    return menu;
}


int ReadData(string filename, Airport*& airportList)
{
    int file_lines = 0;
    fstream fs; 
    vector<string> lines;
    string line;

    fs.open(filename, ios::in);
    if(!fs.is_open()) cout << "Not found"<<endl;
    
    // Read csv by line
    getline(fs, line); 
    while(getline(fs, line)){
        lines.push_back(line); // cut by '\n'
        file_lines++;
    }
    cout << "index: "<< file_lines<< endl;

    // Create and setting node list
    airportList = new Airport[file_lines+1]; // make Node list, 0은 사용하지 x
    for(int i=1;i<=file_lines;i++){ // cut by ',' and make Node
        istringstream ss(lines[i-1]);
        airportList[i].id = i;
        getline(ss, airportList[i].name, ','); 
        getline(ss, airportList[i].type, ',');
        getline(ss, airportList[i].IATA, ',');
        getline(ss, airportList[i].ICAO, ',');
        getline(ss, line, ',');
        airportList[i].location.push_back(stof(line));
        getline(ss, line, ',');
        airportList[i].location.push_back(stof(line));
    }
    cout << endl;
    fs.close();
    return file_lines;
}
/*
Airport* ReadData(string filename, int* airportCnt)
{
    int file_lines = 0;
    fstream fs; 
    vector<string> lines;
    string line;

    fs.open(filename, ios::in);
    if(!fs.is_open()) cout << "Not found"<<endl;
    
    // Read csv by line
    getline(fs, line); 
    while(getline(fs, line)){
        lines.push_back(line); // cut by '\n'
        file_lines++;
    }
    airportCnt = file_lines;
    cout << "index: "<< file_lines<< endl;

    // Create and setting node list
    Airport* airportList; // make Node list, 0은 사용하지 x
    airportList = new Airport[file_lines+1];
    for(int i=1;i<=file_lines;i++){ // cut by ',' and make Node
        istringstream ss(lines[i-1]);
        airportList[i].id = i;
        getline(ss, airportList[i].name, ','); 
        getline(ss, airportList[i].type, ',');
        getline(ss, airportList[i].IATA, ',');
        getline(ss, airportList[i].ICAO, ',');
        getline(ss, line, ',');
        airportList[i].location.push_back(stof(line));
        getline(ss, line, ',');
        airportList[i].location.push_back(stof(line));
    }
    
    cout << endl;
    fs.close();
    return airportList;
} 
*/
void GetEdge(AdjList* adjList,Airport* airportList, int airportCnt){
    int edgeCnt;
    string start_name, end_name;
    int start_id, end_id;
    float weight = 0;

    // Get Input 
    cout << "How many edges: ";
    cin >> edgeCnt;
    for(int i=0;i<edgeCnt;i++){
        // Get strat, end point
        cout << "Edge(start,end): ";
        cin >> start_name >> end_name;
        start_id = FindAirport_NametoId(start_name, airportList, airportCnt);
        end_id = FindAirport_NametoId(end_name, airportList, airportCnt);
        
        // Calc edge weight

        // Add edge in graph
        adjList->AddEdge(start_id, end_id, weight);// a>b
        adjList->AddEdge(end_id, start_id, weight);// b<a
    }
}

float CalcWeight(){
    float weight;
    return weight;
}

int FindAirport_IdtoName(int id, Airport* airportList, int airportCnt){
    for(int i=1;i<=airportCnt;i++){
        if(airportList[i].id == id){
            return i;
        }
    }
    return -1;
}

int FindAirport_NametoId(string name, Airport* airportList, int airportCnt){
    for(int i=1;i<=airportCnt;i++){
        if(airportList[i].name == name){
            return i;
        }
    }
    return -1;
}

void ShowAirportList(Airport* airportList, int airportCnt){
    for(int i=1;i<=airportCnt;i++) cout << airportList[i].id << " / "<< airportList[i].name << " / " << airportList[i].type << " / "<< airportList[i].IATA << " / "<< airportList[i].ICAO << " / "<< airportList[i].location[0] << " / "<< airportList[i].location[1] << endl;
}