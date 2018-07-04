#include <stdio.h>
#include <limits.h>
 
const int n=9;
 
int minDistance(int dist[], bool sptSet[])
{
    
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < n; v++)
        if (sptSet[v] == false &&
                   dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}
 
void printPath(int parent[], int j)
{
    
    if (parent[j] == - 1)
        return;
 
    printPath(parent, parent[j]);
 
    printf("%d ", j);
}

int printSolution(int dist[], int parent[],int src)
{
    for (int i = src+1; i < n; i++)
    {
        printf("\n%d -> %d \t\t %d\t\t%d ",
                      src, i, dist[i], src);
        printPath(parent, i);
    }
}
 
void dijkstra(int graph[][105], int src)
{
    
    int dist[n]; 

    bool sptSet[n];
 
    int parent[n];
 
    for (int i = 0; i < n; i++)
    {
        parent[src] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++)
    {
        int u = minDistance(dist, sptSet);
 
        sptSet[u] = true;
 
        for (int v = 0; v < n; v++)
 
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            } 
    }
 
    printSolution(dist,parent,src);
}
 
int main()
{
	int pro[100][100];
	
    freopen ("dokument.txt","r",stdin);
	
   for(int i=0; i<V; i++)
   {
   		for(int j=0; j<V; j++)
   		{
   			scanf("%d",&pro[i][j]);
   			pro[i][j]=-pro[i][j];
   		}
   }
    
 	for(int i=0; i<n; i++)
 	{
    	dijkstra(pro, i);
	}
    return 0;
}
