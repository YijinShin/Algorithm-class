#include <iostream>
#include <queue>

using namespace std;

int main(){
    priority_queue<int> queue1;
    priority_queue<int> queue2;

    queue1.push(1);
    queue1.push(3);
    queue1.push(2);
    queue1.push(4);

    queue2 = queue1; // 둘이 주소값 다름. 다행

    //cout << &queue2.top()<<endl;
    //cout << &queue1.top();

    float a = 1;
    float b = 21;
    float result = a*(100/b)/100;
    cout << result;
}