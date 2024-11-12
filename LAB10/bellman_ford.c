#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>


#define MAX 1000
#define MAX_VERTICES 10
struct RoutingTable{
    int Table[MAX_VERTICES];
    int nextHop[MAX_VERTICES];
    int vertex;
};


int Graph[MAX_VERTICES][MAX_VERTICES] = {{0, 1, 4, MAX}, {1, 0, 2, 6}, {4, 2, 0, 3}, {MAX, 6, 3, 0}};
int num_vertices = 4;
struct RoutingTable tables[4];


//0 - A, 1 - B, 2 - C, 3 - D.


void init_RT(){
    for(int i = 0; i < num_vertices; i++){
        for(int j = 0; j < num_vertices; j++){
            tables[i].Table[j] = Graph[i][j];
            tables[i].nextHop[j] = j;
            tables[i].vertex = i;
        }
    }
}


void printRT(){
    for(int i = 0;i < num_vertices; i++){
        printf("Routing Table-%d:\n", i);
        printf("   D  N_h\n");
        for(int j = 0;j < num_vertices; j++){
            printf("%d  %d  %d\n", j, tables[i].Table[j], tables[i].nextHop[j]);
        }
        printf("\n");
    }


    printf("\n\n");
}




void BellmanFord(){
    int num_it = 1;
    int num_iter = num_vertices - 1;


    while(num_iter--){
        printf("Iteration - %d\n", num_it++);
        for(int i = 0; i < num_vertices; i++){
            for(int j = 0; j < num_vertices; j++){
                for(int k = 0; k < num_vertices; k++){
                    if(tables[i].Table[j] > tables[k].Table[j] + Graph[i][k]){
                        tables[i].Table[j] = tables[k].Table[j] + Graph[i][k];
                        tables[i].nextHop[j] = k;
                    }
                }
            }
        }


        printRT();
    }
}




int main(){


    init_RT();


    BellmanFord();


    // printRT();
}
