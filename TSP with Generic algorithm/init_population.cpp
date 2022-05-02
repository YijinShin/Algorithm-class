#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>

using namespace std;
vector<int>make_population(int size, vector<int> stand_arr);

int main()
{
    int init_num = 30; //생성하려는 랜덤 배열 개수
    int size = 10; //도시 개수
    vector<int> stand_arr;
    vector<vector<int>> init_arr;
    srand(time(NULL));
    // 1~size 까지의 수가 포함된 배열 생성
    for (int i = 1; i <= size; i++) {
        stand_arr.push_back(i);
    }

    for (int i = 0; i < init_num; i++) {
        stand_arr = make_population(size, stand_arr);
        init_arr.push_back(stand_arr);
    }
    return 0;
}

vector<int> make_population(int size, vector<int> stand_arr) {
    int iter = size;
    for (int i = 0; i < iter; i++) {
        int rn1 = rand() % size;
        int rn2 = rand() % size;
        swap(stand_arr[rn1], stand_arr[rn2]);
    }
    for (int i = 0; i < stand_arr.size(); i++) {
        cout << stand_arr[i] << " ";
    }
    cout << "\n";
    return stand_arr;
}