#pragma once 
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
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
        if(a.distance > b.distance){
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

        vector<int>* Dijkstra(int start, int end){
            vector<int>* from = new vector<int>(V+1);
            priority_queue<Distance, vector<Distance>, cmp> p_que;
        
            for(int i=1;i<V+1;i++){
                distance[i].distance = INT_MAX;
            }
            //시작점
            distance[start].distance = 0;

            // put all V in p_que (작은 distance를 우선순위)
            for(int i=1;i<V+1;i++){
                p_que.push(distance[i]);
            }
            cout <<  "p_que setting:" <<p_que.size()<<endl;
            
            // while p_que is not empty
            while(!p_que.empty()){
                int curr_id= p_que.top().airport_id;
                // pop front
                p_que.pop();
                // if already visited, then pass
                if(visited[curr_id]) continue;
                // check visited
                visited[curr_id] =true;
                // heapify
                // curr_id의 인접리스트로 연결된 경로
                for(auto n : adjList[curr_id]){
                    int next=n.airport_id;
                    double curr_weight = n.weight;
                    //cout << "curr_id: " << curr_id << "next: " << n.airport_id << "curr_weight: " << curr_weight << "\n";
                    if(distance[next].distance>distance[curr_id].distance+curr_weight){
                        distance[next].distance = distance[curr_id].distance+curr_weight;
                        p_que.push(distance[next]);
                        (*from)[next] = curr_id; // can find out about the previous path of next.
                    }
                }
            }
            return from;
        }   

        queue<int> trace_path(int start, int end, vector<int>* from, queue<int> shortestpath){
            //if the start and the end are the same
            if((*from)[end]==start){
                return shortestpath;
            }
            //recursion for the path to the vertex before vertex 'end'
            trace_path(start, (*from)[end], from, shortestpath);
            shortestpath.push((*from)[end]);
            return shortestpath;
        }

        queue<int> path_queue(int start, int end, vector<int>* from, queue<int> shortestpath){
            // start
            shortestpath.push(start);
            // shortest path between start and end
            shortestpath = trace_path(start, end, from, shortestpath);
            // end
            shortestpath.push(end);
            return shortestpath;
        }
};