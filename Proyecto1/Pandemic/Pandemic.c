#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

//Funciones para manejo de datos;

//Lista doblemente enlazada:

//Dnodo es por el doblemente enlazada, para diferenciar de la no enlazada y de los nodos de árboles, al igual que Dlista
struct Dnodo{
    char pais[30]; //Se puede hacer mejor xd

    //Las problematicas empiezan en 0 por defecto xd
    int aspecto1;
    int aspecto2;

    struct Dnodo* sigt; //Vecino1 del pais (Creo xd)
    struct Dnodo* ante; //Vecino2 del pais (Creo xd)
};

struct Dlista{
    struct Dnodo* inicio;
};

struct Dlista* crear_lista(){
    struct Dlista* nueva_lista = calloc(1, sizeof(struct Dlista));
    if(nueva_lista == NULL){
        return NULL;
    }
    nueva_lista->inicio = NULL;
    return nueva_lista;
}

struct Dnodo* crear_nodo(const char* NombrePais){
    struct Dnodo* nn = calloc(1, sizeof(struct Dnodo));
    if(nn == NULL){
        return NULL;
    }

    strncpy(nn->pais, NombrePais, 29);
    nn->pais[29] = '\0';

    nn -> aspecto1 = 0;
    nn -> aspecto2 = 0;

    nn -> ante = NULL;
    nn -> sigt = NULL;
    return nn;
}

int insertar_inicio(struct Dlista* lista, const char* NombrePais){
    if(lista == NULL){
        return -1;
    }
    struct Dnodo* nn = crear_nodo(NombrePais);
    if(nn == NULL){
        return -1;
    }
    nn -> sigt = lista -> inicio;

    if(lista -> inicio != NULL){
        lista -> inicio -> ante = nn;
    }
    lista -> inicio = nn;
    return 0;
}

int contar_paises(const char* nombre_archivo){
    FILE *archivo = fopen(nombre_archivo, "r");
    char linea[60];
    int contador = 0;

    if(archivo == NULL){
        return -1;
    }

    while(fgets(linea, 60, archivo) != NULL){
        size_t len = strlen(linea);
        if (len > 0 && linea[len-1] == '\n') {
            linea[len-1] = '\0';
            len--;
        }
        if (len > 0) {
            contador++;
        }
    }
    fclose(archivo);
    return contador;
}



int crear_tablero(struct Dlista* lista, const char* nombre_archivo){
    char linea[60];
    FILE *archivo = fopen(nombre_archivo, "r");

    if (archivo == NULL) {
        perror("Error al abrir el archivo de países");
        return -1;
    }
    
    while (fgets(linea, 60, archivo) != NULL) {
        size_t len = strlen(linea);
        if (len > 0 && linea[len-1] == '\n') {
            linea[len-1] = '\0'; // Reemplaza '\n' por el terminador nulo '\0'
        }
        
        if (strlen(linea) == 0) {
            continue;
        }

        // Insertar el país en la lista doblemente enlazada
        if (insertar_inicio(lista, linea) != 0) {
            fprintf(stderr, "Error al insertar el país: %s\n", linea);
            return -1;
        } else {
            // Comprobación de que se insertó
            // printf("Insertado: %s\n", linea);
        }
    }
    //Cerrar el archivo
    fclose(archivo);
    //printf("Carga de países finalizada.\n");
    return 0;
}

int creacion_problematicas(struct Dlista* listaPaises){
    return 0;
}

int colocacion_jugadores(){
    char jugador1[30];
    char jugador2[30];

    printf("Ingrese el nombre del jugador 1: ");
    scanf("%s", jugador1);

    printf("Ingrese el nombre del jugador 2: ");
    scanf("%s", jugador2);

    printf("Bienvenidos jugadores %s y %s\n", jugador1, jugador2);

    //Falta la colocacion de los jugadores xd
    return 0;
}

//Función principal, aquí se ejecutara el juego:
int main(){
    printf("--- Bienvenido a Pandemic ---\n");
    //colocacion_jugadores();
    struct Dlista* juego = crear_lista();
    const char* archivo_paises = "../Documentos/Países de América Latina.txt";
    printf("%d\n", contar_paises(archivo_paises));
    crear_tablero(juego, archivo_paises);

        //Comprobación del tablero
        int indicePais = 1;
        if(juego != NULL){
            printf("Países cargados:\n");
            struct Dnodo* actual = juego -> inicio;
            while(actual != NULL){
                printf("Pais %d: %s\n", indicePais++, actual -> pais);
                actual = actual -> sigt;
            }
        }
    return 0;
}