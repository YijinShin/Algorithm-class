#pragma once 
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <stack>
using namespace std;

// Un-driected, Weighted graph

struct Node{
    int airport_id;
    double weight;
};

struct Distance{
    int airport_id;
    double distance;
    int from;
};

struct cmp{
    bool operator()(Distance a, Distance b){
        return a.distance > b.distance;
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
        stack<int> shortestPath;
        

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
                    distance[i].from = 0;
                }
        }

        void AddEdge(int start, int end, float weight){
            Node newEdge;
            newEdge.airport_id = end;
            newEdge.weight = weight;
            adjList[start].push_back(newEdge);
            E++;
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

        void Dijkstra(int start, int end){
            priority_queue<Distance, vector<Distance>, cmp> p_que;

            //visitied initialize
            for(int i=0;i<=V;i++){
                visited[i] = false;
            }

            //distance initialize
            for(int i=1;i<V+1;i++){
                distance[i].distance = INT_MAX;
            }  
            distance[start].distance = 0;

            // put all V in p_que (작은 distance를 우선순위)
            for(int i=1;i<V+1;i++){
                p_que.push(distance[i]);
            }
            //cout <<  "p_que setting:" <<p_que.size()<<endl;
            
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
                for(auto n : adjList[curr_id]){ // 인접리스트로 연결된 경로 하나 씩 빼서
                    int next=n.airport_id; 
                    //자기가 온곳에 대해서는 고려 x
                    if(next == distance[curr_id].from) continue;
                    double curr_weight = n.weight;
                    cout << "curr_id: " << curr_id << " next: " << n.airport_id << " distance[curr_id].distance: "<< distance[curr_id].distance <<" curr_weight: " << curr_weight << "\n";

                    if(distance[next].distance>distance[curr_id].distance+curr_weight){
                        distance[next].distance = distance[curr_id].distance+curr_weight;
                        distance[next].from = curr_id; // can find out about the previous path of next.
                        p_que.push(distance[next]);
                        cout << next << " id: " << curr_id << "\n";
                    }
                }
                cout << "next\n";
            }
        }   

        queue<int> path_queue(int start, int end){
            queue<int> shortpath_queue;
            //push {end, from[end-1] ... from[start+1], start}
            shortestPath.push(end); // push end
            trace_path(start, end); // push {from[end-1] ... from[start+1], start}
            
            //change stack to queue (for other function)
            while(!shortestPath.empty()){
                shortpath_queue.push(shortestPath.top());
                shortestPath.pop();
            }
            return shortpath_queue;
        }

        void trace_path(int start, int curr_id){
            if(curr_id != start){
                shortestPath.push(distance[curr_id].from);
                trace_path(start, distance[curr_id].from);
            }
        }

        void ChangeWeight(int a, int b){
            // edge a->b
            for(int i=0;i<adjList[a].size();i++){
                if(adjList[a].at(i).airport_id ==  b){ 
                        adjList[a].at(i).weight = 1000000;
                }
            }
            // edge b->a
            for(int i=0;i<adjList[b].size();i++){
                if(adjList[b].at(i).airport_id ==  a){ 
                        adjList[b].at(i).weight = 1000000;
                }
            }
        }
};