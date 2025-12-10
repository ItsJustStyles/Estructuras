#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

// - Definiciones de constantes
#define FILAS 11
#define COLUMNAS 21
#define INF INT_MAX // ? Representa el infinito (la distancia máxima o no alcanzable).

//- Estructura para representar una coordenada (Punto)
typedef struct { int r,c; } Punto;

int maze[FILAS][COLUMNAS];
int id_map[FILAS][COLUMNAS];
int node_r[FILAS*COLUMNAS];
int node_c[FILAS*COLUMNAS];

// - Variables globales para el Grafo
int *adj_matrix = NULL;
int nodes = 0;
int *distancias = NULL; // Almacena la distancia más corta desde el inicio.
int *predecesor = NULL; // Almacena el nodo anterior en la ruta más corta (para reconstruir el camino).

// - Función auxiliar para verificar los limites de la matriz
int dentro(int r,int c){ return r>=0 && r<FILAS && c>=0 && c<COLUMNAS; }

//* Generación del laberinto

void inicializar(){
    for(int i=0;i<FILAS;i++)
        for(int j=0;j<COLUMNAS;j++){
            maze[i][j]=1;
            id_map[i][j]=-1;
        }
}

// -Función encargada de elegir una direccionar aleatoria para usarse en la creación del laberinto
void shuffle4(int a[4]){
    for(int i=0;i<4;i++){
        int j = rand()%4;
        int t=a[i]; a[i]=a[j]; a[j]=t;
    }
}

// - Algoritmo para crear (carve = tallar xd) el laberinto
void dfs_carve(int r,int c){
    
    maze[r][c]=0; // Convierte la celda actual en un pasillo

    // Direcciones posibles
    int dirs[4][2]={{-1,0},{1,0},{0,1},{0,-1}};
    int ord[4]={0,1,2,3};
    //Escoge una al azar (desordena o revuelve)
    shuffle4(ord);

    for(int i=0;i<4;i++){
        int d=ord[i];
        int nr=r+dirs[d][0]*2;
        int nc=c+dirs[d][1]*2;

        //Verifica si la celda ésta en los limites y si es un muro
        if(dentro(nr,nc) && maze[nr][nc]==1){
            // Talla el muro intermedio para conectar (1 posición)
            maze[r+dirs[d][0]][c+dirs[d][1]]=0;
            dfs_carve(nr,nc);
        }
    }
}

void generar_laberinto(){
    inicializar();
    dfs_carve(1,1); // Crea o talla desde la posición (1,1)
    maze[1][0]=0; // Crea la entrada
    maze[FILAS-2][COLUMNAS-1]=0; // Crea la salida
}

//* Contrucción del grafo

void construir_grafo(){
    nodes=0;
    // Mapea las celdas del laberinto en nodos
    for(int i=0;i<FILAS;i++)
        for(int j=0;j<COLUMNAS;j++)
            if(maze[i][j]==0){ // Solo las celdas que son pasillos
                id_map[i][j]=nodes;
                node_r[nodes]=i;
                node_c[nodes]=j;
                nodes++;
            }

    // Asigna memoria para la matriz de adyacencia 
    adj_matrix = malloc(nodes * nodes * sizeof(int));
    for(int i=0;i<nodes;i++)
        for(int j=0;j<nodes;j++)
            adj_matrix[i*nodes+j]=INF;

    for(int i=0;i<nodes;i++)
        adj_matrix[i*nodes+i]=0;
    
    // Conectar nodos vecinos
    int dr[4]={-1,1,0,0}, dc[4]={0,0,1,-1};
    for(int i=0;i<nodes;i++){
        int r=node_r[i], c=node_c[i]; // Coordenadas del nodo actual
        for(int k=0;k<4;k++){
            int rr=r+dr[k], cc=c+dc[k]; // Coordenadas del vecino
            if(dentro(rr,cc) && maze[rr][cc]==0){ 
                int j=id_map[rr][cc];
                adj_matrix[i*nodes+j]=1; // Establece el peso de la arista en 1
            }
        }
    }

    // Asigna la memoria para las estructuras
    distancias = malloc(nodes * sizeof(int));
    predecesor = malloc(nodes * sizeof(int));
}

/*
u : source: Donde se empieza
v: Vertex: El destino
w: Weight: El peso, costo o distancia
*/
void dijkstra(int start,int goal){

    // Inicializa distancias y predecesores para todos los nodos
    for(int i=0;i<nodes;i++){
        distancias[i]=INF;
        predecesor[i]=-1;
    }
    distancias[start]=0;

    // Array para rastrear qué nodos han sido "finalizados" (visitados definitivamente)
    int used[nodes];
    for(int i=0;i<nodes;i++) used[i]=0;

    //Bucle principal
    for(int it=0; it<nodes; it++){

        int u=-1, best=INF;
        //Selección del nodo con menor distancia
        for(int v=0; v<nodes; v++)
            //Busca entre nodos que NO han sido finalizados
            //Y además tienen una menor distancia que la encontrada hasta ahora
            if(!used[v] && distancias[v]<best){
                best=distancias[v]; u=v;
            }
        if(u==-1) break; // Si no se encontro ningun nodo accesible termina
        if(u==goal) break; // Si ya llegamos a la meta también termina

        used[u]=1;
        
        //Actualización de aristas
        for(int v=0; v<nodes; v++){
            int w=adj_matrix[u*nodes+v];
            if(w!=INF && !used[v]){
                //Comprueba si el nuevo camino es menor al mejor conocido
                if(distancias[u]+w < distancias[v]){
                    distancias[v]=distancias[u]+w;
                    predecesor[v]=u;
                }
            }
        }
    }
}

void bfs(int start, int goal){

    // Inicializa distancias y predecesores para todos los nodos
    for(int i = 0; i < nodes; i++){
        distancias[i] = INF;
        predecesor[i] = -1;
    }
    distancias[start] = 0;

    //Simulación de la cola:
    int queue[nodes];
    int head = 0; //Indice para entraer
    int tail = 0;  //Indice para insertar

    queue[tail++] = start;
    
    while(head < tail){
        int u = queue[head++];
        if(u == goal) break;

        for(int v = 0; v < nodes; v++){
            int w = adj_matrix[u * nodes + v];

            if(w == 1 && distancias[v] == INF){
                distancias[v] = distancias[u] + 1;
                predecesor[v] = u;
                queue[tail++] = v;
            }
        }
    }
}

//* Reconstrucción y visualización

int reconstruir(int start,int goal,int path[]){
    //Verifica si la meta fue alcanzada
    
    if(predecesor[goal]==-1) return 0;
    int cur=goal,k=0;
    // Recorrer hacia atrás desde la meta hasta el inicio
    while(cur!=-1){
        path[k++]=cur;
        if(cur==start) break;
        cur=predecesor[cur];
    }

    //Invertir el array para que vaya de inicio a fin
    for(int i=0;i<k/2;i++){
        int t=path[i];
        path[i]=path[k-1-i];
        path[k-1-i]=t;
    }
    return k; // Retorna la longitud del camino
}

void imprimir_maze(int path[], int k){
    int mark[FILAS][COLUMNAS]={0};

    // Marca las coordenadas de los nodos que forman la ruta
    for(int i=0;i<k;i++){
        int n=path[i];
        mark[node_r[n]][node_c[n]]=1;
    }

    // Imprime el laberinto
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

    printf("========== ESCOGER METODO DE BUSQUEDA ==========\n");
    printf("1. Dijkstra\n");
    printf("2. Bfs\n");

    int opcion;
    while(opcion != 1 && opcion != 2){
        scanf("%d", &opcion);
        if(opcion == 1){
            dijkstra(inicio,fin);
        }else if(opcion == 2){
            bfs(inicio, fin);
        }else{
            printf("Opcion invalida, por favor intente de nuevo: \n");
        }
    }   
    

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

