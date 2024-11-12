#include <limits.h>
#include <stdbool.h>
#include <stdio.h>


#define MAX 1000
#define V 4  


int minDistance(int dist[], bool sptSet[])
{
    int min = MAX, min_index;


    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;


    return min_index;
}


void printSolution(int dist[], int src)
{
    printf("Source Vertex: %d\n", src);
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < V; i++)
        printf("%d \t\t\t\t %d\n", i, dist[i]);
    printf("\n");
}


void dijkstra(int graph[V][V], int src)
{
    int dist[V];
    bool sptSet[V];


    for (int i = 0; i < V; i++)
        dist[i] = MAX, sptSet[i] = false;


    dist[src] = 0;


    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dist, sptSet);


        sptSet[u] = true;
        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != MAX && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }


    printSolution(dist, src);
}


int main()
{
    int graph[V][V] = {{0, 1, 4, MAX},
                       {1, 0, 2, 6},
                       {4, 2, 0, 3},  
                       {MAX, 6, 3, 0}};


    for (int src = 0; src < V; src++) {
        dijkstra(graph, src);
    }


    return 0;
}
