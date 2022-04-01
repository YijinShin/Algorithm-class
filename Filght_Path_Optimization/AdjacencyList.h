#pragma once 
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

// Un-driected, Weighted graph

struct Node{
    int airport_id;
    double weight;
};

struct Distance{
    int airport_id;
    double distance;
};

struct cmp{
    bool operator()(Distance a, Distance b){
        if(a.distance >= b.distance){
            return true;
        }else return false;
    }
};

class AdjList{
    private:
        vector<Node>* adjList;
        int V; // vertex num
        int E; // edge num
        bool* visited; // Dijkstar에서 사용 
        //int* distance; // Dijkstar에서 사용 
        Distance* distance;
        queue<int> que; // 최단 경로 저장 

    public:
        AdjList(){
            V = 0;
            E = 0;
        }
        ~AdjList(){
            delete[] visited;
            delete[] adjList;
        }
        
        void AdjListSetting(int vertexNum){ 
            // adjList에 공항 수만큼 할당(node 수 만큼)
                // V 갱신 
                V = vertexNum;
                // 공항 수 만큼 adjList 생성  
                adjList = new vector<Node>[V+1];
                // visited array setting
                visited = new bool[V+1];
                for(int i=1;i<=V;i++) visited[i] = false;
                // distance array setting
                distance = new Distance[V+1];
                for(int i=1;i<=V;i++){
                    distance[i].airport_id = i;
                }
        }

        void AddEdge(int start, int end, float weight){    
            Node newEdge;
            newEdge.airport_id = end;
            newEdge.weight = weight;
            adjList[start].push_back(newEdge); 
        }

        void ShowList(){
            cout<<"<Edge List>"<<endl;
            for(int i=1;i<=V;i++){             
                for(int j=0;j<adjList[i].size();j++){
                    cout << "["<<i<<","<<adjList[i].at(j).airport_id<<"]: "<<adjList[i].at(j).weight<<endl;
                }
                cout << endl;
            }
        }

        queue<int> Dijkstra(int start, int end){
            queue<int> shortestPath;
            priority_queue<Distance, vector<Distance>, cmp> p_que;
            queue<int> S;
        
            for(int i=0;i<V+1;i++) distance[i].distance = numeric_limits<int>::infinity();
            distance[start].distance = 0;
            
            // put all V in p_que (weight가 기준)
            for(int i=1;i<=V;i++){
                p_que.push(distance[i]);
            }
            cout <<  "p_que setting:" <<p_que.size()<<endl;
            // while p_que is not empty
                // pop front 
                // check visited
                // dist calc (if already visited, then pass)
                // heapify
            return shortestPath;
        }   
};