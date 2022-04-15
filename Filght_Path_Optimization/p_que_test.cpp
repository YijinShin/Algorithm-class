#include <iostream>
#include <queue>
using namespace std;


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

int main(){
    priority_queue<Distance, vector<Distance>, cmp> p_que;
    Distance d[3];
    for(int i=0;i<3;i++){
        d[i].airport_id = 10-i;
        d[i].distance = i*10;
        d[i].from = i*20;
        p_que.push(d[i]);
    }
    while(!p_que.empty()){
        cout << p_que.top().distance << " ";
        p_que.pop();

    }
}