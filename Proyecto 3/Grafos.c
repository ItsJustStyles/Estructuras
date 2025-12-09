#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define FILAS 11
#define COLUMNAS 21
#define INF INT_MAX

typedef struct { int r,c; } Punto;

int maze[FILAS][COLUMNAS];
int id_map[FILAS][COLUMNAS];
int node_r[FILAS*COLUMNAS];
int node_c[FILAS*COLUMNAS];

int *adj_matrix = NULL;
int nodes = 0;
int *distancias = NULL;
int *predecesor = NULL;

int dentro(int r,int c){ return r>=0 && r<FILAS && c>=0 && c<COLUMNAS; }

void inicializar(){
    for(int i=0;i<FILAS;i++)
        for(int j=0;j<COLUMNAS;j++){
            maze[i][j]=1;
            id_map[i][j]=-1;
        }
}

void shuffle4(int a[4]){
    for(int i=0;i<4;i++){
        int j = rand()%4;
        int t=a[i]; a[i]=a[j]; a[j]=t;
    }
}

void dfs_carve(int r,int c){
    maze[r][c]=0;
    int dirs[4][2]={{-1,0},{1,0},{0,1},{0,-1}};
    int ord[4]={0,1,2,3};
    shuffle4(ord);
    for(int i=0;i<4;i++){
        int d=ord[i];
        int nr=r+dirs[d][0]*2;
        int nc=c+dirs[d][1]*2;
        if(dentro(nr,nc) && maze[nr][nc]==1){
            maze[r+dirs[d][0]][c+dirs[d][1]]=0;
            dfs_carve(nr,nc);
        }
    }
}

void generar_laberinto(){
    inicializar();
    dfs_carve(1,1);
    maze[1][0]=0;
    maze[FILAS-2][COLUMNAS-1]=0;
}

void construir_grafo(){
    nodes=0;
    for(int i=0;i<FILAS;i++)
        for(int j=0;j<COLUMNAS;j++)
            if(maze[i][j]==0){
                id_map[i][j]=nodes;
                node_r[nodes]=i;
                node_c[nodes]=j;
                nodes++;
            }

    adj_matrix = malloc(nodes * nodes * sizeof(int));
    for(int i=0;i<nodes;i++)
        for(int j=0;j<nodes;j++)
            adj_matrix[i*nodes+j]=INF;

    for(int i=0;i<nodes;i++)
        adj_matrix[i*nodes+i]=0;

    int dr[4]={-1,1,0,0}, dc[4]={0,0,1,-1};
    for(int i=0;i<nodes;i++){
        int r=node_r[i], c=node_c[i];
        for(int k=0;k<4;k++){
            int rr=r+dr[k], cc=c+dc[k];
            if(dentro(rr,cc) && maze[rr][cc]==0){
                int j=id_map[rr][cc];
                adj_matrix[i*nodes+j]=1;
            }
        }
    }

    distancias = malloc(nodes * sizeof(int));
    predecesor = malloc(nodes * sizeof(int));
}

void dijkstra(int start,int goal){
    for(int i=0;i<nodes;i++){
        distancias[i]=INF;
        predecesor[i]=-1;
    }
    distancias[start]=0;

    int used[nodes];
    for(int i=0;i<nodes;i++) used[i]=0;

    for(int it=0; it<nodes; it++){
        int u=-1, best=INF;
        for(int v=0; v<nodes; v++)
            if(!used[v] && distancias[v]<best){
                best=distancias[v]; u=v;
            }
        if(u==-1) break;
        if(u==goal) break;

        used[u]=1;

        for(int v=0; v<nodes; v++){
            int w=adj_matrix[u*nodes+v];
            if(w!=INF && !used[v]){
                if(distancias[u]+w < distancias[v]){
                    distancias[v]=distancias[u]+w;
                    predecesor[v]=u;
                }
            }
        }
    }
}

int reconstruir(int start,int goal,int path[]){
    if(predecesor[goal]==-1) return 0;
    int cur=goal,k=0;
    while(cur!=-1){
        path[k++]=cur;
        if(cur==start) break;
        cur=predecesor[cur];
    }
    for(int i=0;i<k/2;i++){
        int t=path[i];
        path[i]=path[k-1-i];
        path[k-1-i]=t;
    }
    return k;
}

void imprimir_maze(int path[], int k){
    int mark[FILAS][COLUMNAS]={0};

    for(int i=0;i<k;i++){
        int n=path[i];
        mark[node_r[n]][node_c[n]]=1;
    }

    for(int i=0;i<FILAS;i++){
        for(int j=0;j<COLUMNAS;j++){
            if(i==1 && j==0){ printf("E"); continue; }
            if(i==FILAS-2 && j==COLUMNAS-1){ printf("S"); continue; }
            if(maze[i][j]==1) printf("█");
            else if(mark[i][j]) printf(".");
            else printf(" ");
        }
        printf("\n");
    }
}


int main(){
    srand(time(NULL));
    generar_laberinto();
    construir_grafo();

    int inicio=id_map[1][0];
    int fin=id_map[FILAS-2][COLUMNAS-1];

    if(inicio<0 || fin<0){
        printf("ERROR entrada o salida\n");
        return 1;
    }

    dijkstra(inicio,fin);

    int path[nodes];
    int L=reconstruir(inicio,fin,path);

    if(L>0){
        printf("Camino encontrado: %d nodos\n\n",L);
        imprimir_maze(path,L);
        printf("\nDistancia: %d\n",distancias[fin]);
    } else {
        printf("No se encontró camino\n\n");
        imprimir_maze(NULL,0);
    }

    free(adj_matrix);
    free(distancias);
    free(predecesor);
    return 0;
}

