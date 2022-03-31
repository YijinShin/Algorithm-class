#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

#pragma warning(disable: 4996) // c++에서 fopen, strtok 사용오류 해결
using namespace std;

#define BUFSIZE 128

typedef struct {
    char Name [40];
    int Type;
    char IATA[40];
    char ICAO[40];
    float Longitude;
    float Latitude;
} info_plane;

void read_data(const char* filename, info_plane **info, int* num_line);
void print_data(info_plane* info, int num_line);

int main() {
    info_plane* info;
    int num_line;
    read_data("South_Korea_airport_toy_example.csv", &info, &num_line);
    print_data(info, num_line);
}

void read_data(const char* filename, info_plane **info, int* num_line)
{
    int file_lines = 0; // 카테고리 제외한 csv 행의 수
    
    // read file
    FILE* fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("File not Found!\n");
        exit(1);
    }
    
    char line[BUFSIZE]; // 문자열을 한줄씩 읽어와서 저장
    
    fgets(line, BUFSIZE, fp); // 파일내 카테고리 건너뛰고 데이터부터
    while (fgets(line, BUFSIZE, fp)) {
        file_lines++;
    }
    *num_line = file_lines; // 카테고리 제외한 csv 행의 수

    *info = (info_plane*)malloc(sizeof(info_plane)* (file_lines+2));
    
    fseek(fp, 0, SEEK_SET); // 읽고 있는 곳을 파일의 처음으로 초기화
    fgets(line, BUFSIZE, fp); // 파일내 카테고리 건너뛰고 데이터부터
    int nth_line = 0; // 읽고 있는 line의 번호
    //데이터 저장
    while (fgets(line, BUFSIZE, fp)) {
        
        char* token;
        token = strtok(line, ",");
        strcpy((*info)[nth_line].Name, token);

        token = strtok(NULL, ",");
        if (token[0] == 'I') (*info)[nth_line].Type = 1; // 국제선 = 1
        else (*info)[nth_line].Type = 2; // 국내선 = 2

        token = strtok(NULL, ",");
        strcpy((*info)[nth_line].IATA, token);

        token = strtok(NULL, ",");
        strcpy((*info)[nth_line].ICAO, token);

        token = strtok(NULL, ",");
        (*info)[nth_line].Longitude = atof(token);

        token = strtok(NULL, ",");
        (*info)[nth_line].Latitude = atof(token);

        nth_line++;
    }
    fclose(fp);
}

void print_data(info_plane* info, int num_line){
    printf("%-10s   %-10s   %-10s   %-10s   %-20s   %-20s \n", "Name", "Type", "IATA", "ICAO", "Longitude(deg)", "Latitude(deg)");
    for (int k = 0; k < num_line; k++) {
        printf("--------------");
    }
    printf("\n");
    for (int i = 0; i < num_line; i++)
    {
        printf("%-10s   %-10d   %-10s   %-10s   %-20.4f   %-20.4f \n", info[i].Name, info[i].Type, info[i].IATA, info[i].ICAO, info[i].Longitude, info[i].Latitude);
        
    }
}