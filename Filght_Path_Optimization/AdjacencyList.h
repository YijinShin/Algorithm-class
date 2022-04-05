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
};

struct cmp{
    bool operator()(Distance a, Distance b){
        if(a.distance > b.distance){
            return true;
        }else return false;
    }
};

struct Section{ // danger section info
    vector<double> points;
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
        vector<int>* from;
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
                }
                // from array setting
                from = new vector<int>(V+1);
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
        
            for(int i=1;i<V+1;i++){
                distance[i].distance = INT_MAX;
            }
            //시작점
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
                    double curr_weight = n.weight;  
                    //cout << "curr_id: " << curr_id << "next: " << n.airport_id << "curr_weight: " << curr_weight << "\n";
                    if(distance[next].distance>distance[curr_id].distance+curr_weight){
                        distance[next].distance = distance[curr_id].distance+curr_weight;
                        
                        (*from)[next] = curr_id; // can find out about the previous path of next.
                        cout << "form next("<<next<<"):"<<curr_id<<endl;
                    }
                }
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
                shortestPath.push((*from)[curr_id]);
                trace_path(start,(*from)[curr_id]);
            }
        }
        
        int ccw(pair<int, int> a, pair<int, int> b, pair<int, int> c) {
            double external = (b.first - a.first)*(c.second - a.second) - (b.second - a.second)*(c.first - a.first);
            if (external > 0)return 1;
            else if (external == 0)return 0;
            else return -1;
        }

        bool isIntersect(pair<int, int> x, pair<int, int> y, pair<int, int> z, pair<int, int> r){
            // check intersection between to line (x,y) (z,r)
            int xy = ccw(x,y,z)*ccw(x,y,r);
            int zr = ccw(z,r,x)*ccw(z,r,y);
            if(xy && zr == 0){
                // 평항한데 겹치는 경우 reutrn true;
                // 평행하고 안겹치는 경우 return false;
            }else if( xy<0 && zr<0 ) return true;
            else return false;
        }  

        // We have to visit all edges
        void CheckEdgeAvailable(Section* sectionList, int sectionCnt){
            // for 1 ~ edge num
            for(int i=0;i<E;i++){
                
                
                // for 1 ~ lines forming a polygon
                for(int j=0;j<sectionCnt;j++){
                    // make lines 
                    pair<int, int> a = {sectionList[j].points[0], sectionList[j].points[1]};
                    pair<int, int> b = {sectionList[j].points[2], sectionList[j].points[3]};
                    pair<int, int> c = {sectionList[j].points[4], sectionList[j].points[5]};
                    pair<int, int> d = {sectionList[j].points[6], sectionList[j].points[7]};
                    // check intersection 
                    //if(isIntersect(a,b,)) cout << "Edge()can't use"<<endl; 
                }
            }
        }

        /*
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
        
        queue<int> path_queue(int start, int end, vector<int>* from){
            queue<int> shortestpath;
            // start
            shortestpath.push(start);
            // shortest path between start and end
            shortestpath = trace_path(start, end, from, shortestpath);
            // end
            shortestpath.push(end);
            return shortestpath;
        }
        */
};