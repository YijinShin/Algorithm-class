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
    int init_num = 30; //�����Ϸ��� ���� �迭 ����
    int size = 8; //���� ����
    vector<int> stand_arr;
    vector<vector<int>> init_arr;
    srand(time(NULL));

    // 1~size ������ ���� ���Ե� �迭 ����
    for (int i = 1; i <= size; i++) {
        stand_arr.push_back(i);
    }

    //���� �õ��غ���--------------------------------
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
    queue<int> a_range;//swap �� �ش� ������ a ����
    queue<int> b_range;//swap �� �ش� ������ b ����
    vector<int> a_exist; //b�� �̹� �����ߴ� a_range�� ������ index
    vector<int> b_exist; //a�� �̹� �����ߴ� b_range�� ������ index

    int start = 3; // �� ����ó�� �ϴ� �Ŷ� �����ϱ� ���Ͻ� �ſ���
    int end = 6;
    //int start = rand() % size;
    //int end = rand() % size;

    //start,end ���� ���� ����
    // end-start = size-1�̸� ��ǻ� �θ� ��ü�� �ϳ��� ����� ����
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


    //�ش� ������ ���� swaping
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
            if (i >= start && i < end) continue; //��ȯ�� ������ ���Ҵ� �ǳʶٱ�
            if (a_front == b_child[i]) {
                a_exist.push_back(i);
            }
            if (b_front == a_child[i]) {
                b_exist.push_back(i);
            }
        }
    }
    // ������ ordering�� �����ϱ� ���� sort
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
    
    //start,end ���� ���� ����
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