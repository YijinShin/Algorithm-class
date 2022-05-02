#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
void crossover(vector<int> a, vector<int> b, int size);
void mutation(vector<int> a, int size);
vector<vector<int>> population;

int main()
{
    int init_num = 30; //생성하려는 랜덤 배열 개수
    int size = 8; //도시 개수
    vector<int> stand_arr;
    vector<vector<int>> init_arr;
    srand(time(NULL));

    // 1~size 까지의 수가 포함된 배열 생성
    for (int i = 1; i <= size; i++) {
        stand_arr.push_back(i);
    }

    //예제 시도해본것--------------------------------
    vector<int> ex1 = { 3,4,8,2,7,1,6,5 };
    vector<int> ex2 = { 4,2,5,1,6,8,3,7 };

    crossover(ex1, ex2, size);
    mutation(ex1, size);
    //-----------------------------------------------

    return 0;
}


void crossover(vector<int> a, vector<int> b, int size) {
    vector<int> a_child = a;
    vector<int> b_child = b;
    queue<int> a_range;//swap 전 해당 구간의 a 원소
    queue<int> b_range;//swap 전 해당 구간의 b 원소
    vector<int> a_exist; //b에 이미 존재했던 a_range의 원소의 index
    vector<int> b_exist; //a에 이미 존재했던 b_range의 원소의 index

    int start = 3; // 논문 예시처럼 하는 거라 이해하기 편하실 거에요
    int end = 6;
    //int start = rand() % size;
    //int end = rand() % size;

    //start,end 구간 랜덤 설정
    // end-start = size-1이면 사실상 부모꺼 전체가 하나씩 생기기 때문
    //while (start == end || abs(end-start) == size-1) {
    //    int start = rand() % size;
    //    int end = rand() % size;
    //}
    //if (start > end) swap(start, end); 

    cout << "strat: " << start << "end: " << end << "\n";

    cout << "a_array: ";
    for (int i = 0; i < a.size(); i++) {
        cout << a[i] << " ";
    }
    cout << "\n";

    cout << "b_array: ";
    for (int i = 0; i < b.size(); i++) {
        cout << b[i] << " ";
    }
    cout << "\n";


    //해당 구간에 대한 swaping
    for (int i = start; i < end; i++) {
        a_range.push(a_child[i]);
        b_range.push(b_child[i]);
        swap(a_child[i], b_child[i]);
    }

    while (a_range.size() != 0 || b_range.size() != 0) {
        int a_front = a_range.front();
        int b_front = b_range.front();
        a_range.pop();
        b_range.pop();

        for (int i = 0; i < size; i++) {
            if (i >= start && i < end) continue; //교환한 구간의 원소는 건너뛰기
            if (a_front == b_child[i]) {
                a_exist.push_back(i);
            }
            if (b_front == a_child[i]) {
                b_exist.push_back(i);
            }
        }
    }
    // 기존의 ordering을 유지하기 위해 sort
    sort(a_exist.begin(), a_exist.end());
    sort(b_exist.begin(), b_exist.end());

    for (int i = 0; i < a_exist.size(); i++) {
        a_child[b_exist[i]] = b[a_exist[i]];
        b_child[a_exist[i]] = a[b_exist[i]];
    }

    cout << "a'_array: ";
    for (int i = 0; i < a.size(); i++) {
        cout << a_child[i] << " ";
    }
    cout << "\n";

    cout << "b'_array: ";
    for (int i = 0; i < b.size(); i++) {
        cout << b_child[i] << " ";
    }
    cout << "\n";

    population.push_back(a_child);
    population.push_back(b_child);
}

void mutation(vector<int> a, int size) {
    //Reverse Sequence Mutation(RSM)
    vector<int> a_child = a;
    int start = rand() % size;
    int end = rand() % size;
    
    //start,end 구간 랜덤 설정
    while (start == end) {
        int start = rand() % size;
        int end = rand() % size;
    }
    if (start > end) swap(start, end);
    cout << "strat: " << start << "end: " << end << "\n";

    while (start < end) {
        swap(a_child[start], a_child[end]);
        start++;
        end--;
    }

    cout << "a'_array: ";
    for (int i = 0; i < a.size(); i++) {
        cout << a_child[i] << " ";
    }
    cout << "\n";

    population.push_back(a_child);
}