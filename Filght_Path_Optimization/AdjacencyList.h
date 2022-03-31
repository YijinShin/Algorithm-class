#pragma once 
#include <iostream>
#include <vector>
#include "Queue.h"
using namespace std;
using namespace Que;

// Un-driected, Weighted graph

struct Node{
    int airport_id;
    float weight;
};

class AdjList{
    private:
        vector<Node>* adjList;
        int V; // vertex num
        int E; // edge num
        bool *visited; // 탐색에서 사용
        Queue queue; // 최단 경로 저장 

    public:
        AdjList(){
            V = 0;
            E = 0;
        }
        ~AdjList(){}
        
        void AdjListSetting(int vertexNum){ 
            // adjList에 공항 수만큼 할당(node 수 만큼)
                // V 갱신 
                V = vertexNum;
                // 공항 수 만큼 adjList 생성  
                adjList = new vector<Node>[V+1];
                // visited array setting
                visited = new bool[V+1];
                for(int i=0;i<V+1;i++) visited[i] = false;
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

};