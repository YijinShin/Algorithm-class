#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <stack>
#include "AdjacencyList.h"

using namespace std;

// M_PI는 cmath 헤더 안에 미리 선언되어있습니다:)

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
string FindAirport_IdtoName(int id, Airport* airportList, int airportCnt);
void GetEdge(AdjList* adjList,Airport* airportList, int airportCnt);
int GetSection(Section*& sectionList);
int ccw(pair<int, int> a, pair<int, int> b, pair<int, int> c);
bool isIntersect(Section* sectionList, int sectionCnt);
void Intersect_weight(AdjList* adjList);
double CalcWeight(vector<double> start, vector<double> end);
void ShowAirportList(Airport* airportList, int airportCnt);
void ShowSectionList(Section* sectionList, int sectionCnt);
void Save_csv(queue<int> que, Airport* airportList, int airportCnt, bool weather);
void ShowQueue(queue<int> que, Airport* airportList, int airportCnt);

int main(){
    int menu;
    AdjList adjList;
    int airportCnt; // airport cnt
    Airport* airportList; // airport list
    int sectionCnt; // danger section cnt
    Section* sectionList; // danger section list 
    
    queue<int> shortestPath_without_weather;
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
            cout << "start_id: " << start_id << " " << "end_id: " << end_id << "\n";
            // get input of section
            sectionCnt = GetSection(sectionList);
            ShowSectionList(sectionList, sectionCnt);
            
            // 1. Dijkstra: without weather
                //Dijkstra
                adjList.Dijkstra(start_id, end_id);
                // print result
                cout << "Shortest path (not consider weather): ";
                shortestPath_without_weather=adjList.path_queue(start_id, end_id);
                //Save_csv(shortestPath_without_weather, airportList, airportCnt, 0);
                ShowQueue(shortestPath_without_weather, airportList, airportCnt);
            
            // 2. Dijkstra: with weather
                // edge weight fix 
                    // intersection 확인 > 있으면 해당 edge weight 수정 
                // Dijkstra
                adjList.Dijkstra(start_id, end_id);
                // print result
                cout << "Shortest path (consider weather): ";
                shortestPath_weather = adjList.path_queue(start_id, end_id);
                //Save_csv(shortestPath, airportList, airportCnt, 1);
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
        cout << i+1 << "th "<<"Edge(start, end): ";
        cin >> start_name >> end_name;
        start_id = FindAirport_NametoId(start_name, airportList, airportCnt);
        end_id = FindAirport_NametoId(end_name, airportList, airportCnt);
        
        // Calc edge weight
        weight = CalcWeight(airportList[start_id].location,airportList[end_id].location);
        
        // Add edge in graph
        adjList->AddEdge(start_id, end_id, weight);// a->b
        adjList->AddEdge(end_id, start_id, weight);// b<-a
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
            cout << "point"<<j<<" (double double): ";
            scanf("%lf %lf",&x,&y);
            sectionList[i].points.push_back(x);
            sectionList[i].points.push_back(y);
        }
    }
    return sectionCnt;
}

int ccw(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
    int op = a.first*b.second + b.first*c.second + c.first*a.second;
    op -= (a.second*b.first + b.second*c.first + c.second*a.first);
    if (op > 0)return 1;
    else if (op == 0)return 0;
    else return -1;
}

bool isIntersect(Section* sectionList, int sectionCnt){
    for(int i=0;i<sectionCnt;i++){
        //vertices between edges

        // get point location of bad weather section (사각형에만 한정되므로 수정함. )
        pair<int, int> a = {sectionList[i].points[0], sectionList[i].points[1]};
        pair<int, int> b = {sectionList[i].points[2], sectionList[i].points[3]};
        pair<int, int> c = {sectionList[i].points[4], sectionList[i].points[5]};
        pair<int, int> d = {sectionList[i].points[6], sectionList[i].points[7]};
        for(int j=0;j<sectionList[i].points.size();j=j+2);
        
        // ab, bc, cb, da


        int ab = ccw(a, b, c)*ccw(a, b, d);
        int cd = ccw(c, d, a)*ccw(c, d, b);
        if (ab == 0 && cd == 0){
            if (a > b)swap(a, b);
            if (c > d)swap(c, d);
            return (c <= b&&a <= d);
        }
        return (ab <= 0 && cd <= 0);
    }
    return false;
}

void Intersect_weight(AdjList* adjList){
    //교차한 vertex에 큰 weight 부하
    
}


double CalcWeight(vector<double> start, vector<double> end){
    double weight;
    // Haversine Formula
    double radius = 6371; // earth radius (km)  
    double toRadian = M_PI / 180;

    double deltaLatitude = abs(start[0] - end[0]) * toRadian;
    double deltaLongitude = abs(start[1] - end[1]) * toRadian;

    double sinDeltaLat = sin(deltaLatitude / 2);
    double sinDeltaLng = sin(deltaLongitude / 2);
    double squareRoot = sqrt( sinDeltaLat * sinDeltaLat + cos(start[0] * toRadian) * cos(end[0] * toRadian) * sinDeltaLng * sinDeltaLng);

    weight = 2 * radius * asin(squareRoot);

    return weight;
}

string FindAirport_IdtoName(int id, Airport* airportList, int airportCnt){
    for(int i=1;i<=airportCnt;i++){
        if(airportList[i].id == id){
            return airportList[i].name;
        }
    }
    return "not_found";
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
    cout << "\nbad weather section\n";
    for(int i=0;i<sectionCnt;i++){
        cout << i+1 << "th section: ";
        for(int j=0;j<=6;j=j+2){
            cout <<"(" << sectionList[i].points[j] << ","<< sectionList[i].points[j+1] <<")";
        }
        cout <<endl;
    }
    cout << "\n";
}

void Save_csv(queue<int> que, Airport* airportList, int airportCnt, bool weather){
    string f_name;
    // if weather is 0, not consider weather
    if(weather) f_name = "shortest_path_weather.csv";
    else f_name = "shortest_path.csv";
    ofstream outfile(f_name);
    int qsize = que.size();
    int id;
    for(int i=0; i<qsize; i++){
        id = que.front();
        outfile << id;
        if(i!=qsize-1) outfile << ",";
        que.pop();
        que.push(id);
    }
    outfile.close();
}

void ShowQueue(queue<int> que, Airport* airportList, int airportCnt){
    int id;
    string name;
    while(!que.empty()){
        id = que.front();
        que.pop();
        name = FindAirport_IdtoName(id, airportList, airportCnt);
        if(que.size()!=0)cout << name << " -> ";
        else cout << name;
    }
    cout << endl;
}