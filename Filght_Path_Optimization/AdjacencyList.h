#pragma once 
#include <iostream>
#include <vector>
#include "Queue.h"
using namespace std;
using namespace Que;

// Un-driected, Weighted graph

struct Node{
    vector<int,int> location; // 공항 위치 (경도, 위도)
    string name; // 공항 이름
    string type; // 공항 타입 
    int id; // 공항 번호 
    float weight; // edge weight
    struct Node *next;
};

class AdjList{
    private:
        vector<Node>* adjList;
        int V; // vertex num
        int E; // edge num
        bool *visited; // 탐색에서 사용
        Queue queue; // 최단 경로 저장 

    public:
        AdjList(){}
        ~AdjList(){}
        
        void AdjListSetting(Node* airportList, int count){ 
            // adjList에 공항 수만큼 할당(node 수 만큼)
                // V 갱신 
                // 공항 수 만큼 adjList 생성  
                adjList = new vector<Node>[count+1];
                // adjList의 각 백터 첫번째 node에 공항 정보 추가 

                    // for (int i=0;i<count;i++) adjList[i]
                // visited = new bool[count+1];
                // visited false로 초기화 
        }

        void AddEdge(string airport_a, string airport_b, float weight){    
            // 공항간 path 추가 (Undirected 이기 때문에 두번해줘야함)
                // add edge a > b
                // add edge b < a

        }

};