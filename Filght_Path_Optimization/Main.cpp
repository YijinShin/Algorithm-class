#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include "AdjacencyList.h"
using namespace std;

#define BUFSIZE 128

struct Airport{ // airport info 
    int id;
    string name;
    string type;
    string IATA;
    string ICAO;
    vector<double> location;
};

struct Section{ // danger section info
    vector<double> points;
};

int ShowMenu();
int ReadData(string filename, Airport*& airportList);
int FindAirport_NametoId(string name, Airport* airportList, int airportCnt);
int FindAirport_IdtoName(int id, Airport* airportList, int airportCnt);
void GetEdge(AdjList* adjList,Airport* airportList, int airportCnt);
int GetSection(Section*& sectionList);
double CalcWeight(vector<double> start, vector<double> end);
void ShowAirportList(Airport* airportList, int airportCnt);
void ShowSectionList(Section* sectionList, int sectionCnt);
void ShowQueue(queue<int> que, Airport* airportList, int airportCnt);

int main(){
    int menu;
    AdjList adjList;
    int airportCnt; // airport cnt
    Airport* airportList; // airport list
    int sectionCnt; // danger section cnt
    Section* sectionList; // danger section list 
    
    queue<int> shortestPath;
    queue<int> shortestPath_weather;
    string start_name,end_name;
    int start_id, end_id;

    // Get csv data  
    airportCnt = ReadData("South_Korea_airport_toy_example.csv",airportList);
    ShowAirportList(airportList,airportCnt);
    
    // Create Adj list 
    adjList.AdjListSetting(airportCnt);
    
    // Get edge 
    GetEdge(&adjList, airportList, airportCnt);

    // Get start, end point

    // main
    while(1){
        cin.ignore();
        menu = ShowMenu();
        if(menu == 1){
            // get start, end point
            cout <<  "Start point: ";
            cin >> start_name;
            cout <<  "End point: ";
            cin >> end_name;
            start_id = FindAirport_NametoId(start_name, airportList, airportCnt);
            end_id = FindAirport_NametoId(end_name, airportList, airportCnt);
            // get input of section
            sectionCnt = GetSection(sectionList);
            ShowSectionList(sectionList, sectionCnt);
            // Dij: without weather
            shortestPath =  adjList.Dijkstra(start_id, end_id);
            // print result
            cout << "Shortest path (not consider weather): ";
            ShowQueue(shortestPath, airportList, airportCnt);
            // edge checking
            
            // Dij: with weather
            shortestPath_weather =  adjList.Dijkstra(start_id, end_id);
            // print result
            cout << "Shortest path (consider weather): ";
            ShowQueue(shortestPath_weather, airportList, airportCnt);
        }else if(menu == 2){
            adjList.ShowList();
        }else if(menu == 3){
            break;
        }
    }
    
    delete[] airportList;
    return 0;
}


int ShowMenu(){
    int menu;
    cout << "\n1.Shortest path\n2.Print Graph\n3.Quit\n"; 
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

void GetEdge(AdjList* adjList, Airport* airportList, int airportCnt){
    int edgeCnt;
    string start_name, end_name;
    int start_id, end_id;
    double weight = 0;

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
        weight = CalcWeight(airportList[start_id].location,airportList[end_id].location);
        
        // Add edge in graph
        adjList->AddEdge(start_id, end_id, weight);// a>b
        adjList->AddEdge(end_id, start_id, weight);// b<a
    }
}

int GetSection(Section*& sectionList){
    int sectionCnt;
    double x, y;
    cout << "How many danger section: ";
    cin >> sectionCnt;
    sectionList = new Section[sectionCnt];

    for(int i=0;i<sectionCnt;i++){
        // Get point
        for(int j=0;j<4;j++){
            cout << "point"<<i<<" (double double): ";   
            scanf("%lf %lf",&x,&y);
            sectionList[i].points.push_back(x);
            sectionList[i].points.push_back(y);
        }
    }
    return sectionCnt;
}

double CalcWeight(vector<double> start, vector<double> end){
    double weight;
    // Haversine Formula
    double radius = 6371; // earth radius (km)
    double toRadian = M_PI / 180;

    double deltaLatitude = abs(start[0] - end[0]);
    double deltaLongitude = abs(start[1] - end[1]) * toRadian;

    double sinDeltaLat = sin(deltaLatitude / 2);
    double sinDeltaLng = sin(deltaLongitude / 2);
    double squareRoot = sqrt( sinDeltaLat * sinDeltaLat + cos(start[0] * toRadian) * cos(end[0] * toRadian) * sinDeltaLng * sinDeltaLng);

    weight = 2 * radius * asin(squareRoot);

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

void ShowSectionList(Section* sectionList, int sectionCnt){
    for(int i=0;i<sectionCnt;i++){
        for(int j=0;j<=6;j=j+2){
            cout <<"(" << sectionList[i].points[j] << ","<< sectionList[i].points[j+1] <<")";
        }
        cout <<endl;
    }
}

void ShowQueue(queue<int> que, Airport* airportList, int airportCnt){
    int id;
    string name;
    while(!que.empty()){
        id = que.front();
        que.pop();
        name = FindAirport_IdtoName(id, airportList, airportCnt);
        cout << name << " - ";
    }
    cout << endl;
}