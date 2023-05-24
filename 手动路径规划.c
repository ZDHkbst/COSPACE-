#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define MAXN 7 

typedef struct{
    char type; 
    int deadline;
    int score;
} Station;

int GetScore(Station station, int time, int stationIndex) {
    if(stationIndex == 0) 
    return 0; 
    if(time <= (float)station.deadline){
        return station.score;
    } else {
        switch(station.type){
            case 'C':
                return station.score - (time - station.deadline) * 3;
            case 'U':
                return station.score - (time - station.deadline) * 2;
            case 'N':
                return station.score - (time - station.deadline);
            case 'F':
                return 20;
            default:
                return 0;
        }
    }
}

void swap(int* x, int* y) {
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void CopyArray(int* src, int* dest, int len) {
    for (int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
}

void GetMaxScore(int* a, int l, int r, float graph[MAXN][MAXN], int* max_path, int* max_score, Station* stations, int n) {
   int i;
   if (l == r) {
       int current_score = 0;
       float time = 0;
       for (i = 0; i < r; i++) { 
           time += graph[a[i]][a[i+1]];
           current_score += GetScore(stations[a[i+1]], time, a[i+1]);
       }
       if (current_score > *max_score) {
           *max_score = current_score;
           CopyArray(a, max_path, n);
       }
   }
   else { 
       for (i = l; i <= r; i++) {
          swap((a+l), (a+i));
          GetMaxScore(a, l+1, r, graph, max_path, max_score, stations, n);
          swap((a+l), (a+i)); 
       }
   }
}

int main() {
    int n; 
    printf("请输入站点的数量（不包括S0）：");
    scanf("%d", &n);
    n++;
    float graph[MAXN][MAXN] = {0}; 
    for(int i=0; i<n; i++){
        for(int j=i+1; j<n; j++){ 
            printf("请输入S%d到S%d所用时间：", i, j);
            scanf("%f", &graph[i][j]);
            graph[j][i] = graph[i][j]; 
        }
    }
    printf("请输入站点的类型(C/U/N/F)，大小写均可或者直接输入分数(90/60/30/20)，然后输入deadline。\n例如：C 100 或 90 100\n");
    Station stations[MAXN];
    stations[0].type = 'F';
    stations[0].score = 0;
    stations[0].deadline = 0;

    for(int i=1; i<n; i++){
        printf("请输入S%d的信息：", i);
        char input[10];
        int deadline;
        scanf("%s", input);
        scanf("%d", &deadline);
        stations[i].deadline = deadline;
        if(input[0] >= '0' && input[0] <= '9') {
            stations[i].score = atoi(input);
            switch(stations[i].score){ 
                case 90:
                   stations[i].type = 'C';
                    break;
                case 60:
                    stations[i].type = 'U';
                    break;
                case 30:
                    stations[i].type = 'N';
                    break;
                case 20:
                    stations[i].type = 'F';
                    break;
                default:
                    printf("输入的分数无效，请重新输入S%d的信息。\n", i+1);
                    i--;
            }
           
        } else {
            char type = toupper(input[0]);  
            stations[i].type = type;
            switch(type){
                case 'C':
                    stations[i].score = 90;
                    break;
                case 'U':
                    stations[i].score = 60;
                    break;
                case 'N':
                    stations[i].score = 30;
                    break;
                case 'F':
                    stations[i].score = 20;
                    break;
                default:
                    printf("输入的类型无效，请重新输入。\n");
                    i--;
            }
        }
    }
    int a[MAXN]; 
    for (int i = 0; i < n; i++) {
        a[i] = i;
    }
    int max_path[MAXN]; 
    int max_score = 0; 

    GetMaxScore(a, 0, n-1, graph, max_path, &max_score, stations, n);

    char* names[] = {"S0", "S1", "S2", "S3", "S4", "S5", "S6", "S7"}; 
    printf("分数最高的路径是: ");
    for (int i = 0; i < n; i++) {
        printf("%s ", names[max_path[i]]);
        if (i < n - 1) {
            printf("→ ");
        }
    }
    printf("\n预计分数是：%d\n", max_score);
    getchar();
    getchar();
    return 0;
}
