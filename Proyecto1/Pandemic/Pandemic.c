#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>

//Funciones para manejo de datos;

//Lista doblemente enlazada:

//Dnodo es por el doblemente enlazada, para diferenciar de la no enlazada y de los nodos de árboles, al igual que Dlista
struct Dnodo{
    char pais[25]; //Se puede hacer mejor xd

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

    strncpy(nn->pais, NombrePais, 24);
    nn->pais[24] = '\0';

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
    int total_paises = 0;

    int indices_elegidos[9] = {0};
    char paises_archivo[30][30];

    if (archivo == NULL) {
        perror("Error al abrir el archivo de países");
        return -1;
    }

    int total_paises_archivo = contar_paises(nombre_archivo);
    if(total_paises_archivo < 9){
        fprintf(stderr, "Error: No hay suficientes países cargados para el inicio del juego.\n");
        return -1;
    }
    
    while (fgets(linea, 30, archivo) != NULL && total_paises < 30) {
        size_t len = strlen(linea);

        while (len > 0 && (linea[len - 1] == ' ' || linea[len - 1] == '\t')) {
            linea[len - 1] = '\0'; // Reemplaza espacio/tab por nulo
            len--;
        }
        
        if (len > 0 && linea[len-1] == '\n') {
            linea[len-1] = '\0'; // Reemplaza '\n' por el terminador nulo '\0'
        }

        if (strlen(linea) > 0) {
            strncpy(paises_archivo[total_paises], linea, 29);
            paises_archivo[total_paises][29] = '\0';
            total_paises++;
        }
    }
    //Cerrar el archivo
    fclose(archivo);
    srand(time(NULL));
    int contador = 0;
    while(contador < 9){
        int indice_aleatorio = rand() % total_paises;
        int ya_elegido = 0;
        
        for (int i = 0; i < contador; i++) {
            if (indices_elegidos[i] == indice_aleatorio) {
                ya_elegido = 1;
                break;
            }
        }
        
        if (!ya_elegido) {
            indices_elegidos[contador] = indice_aleatorio;
            contador++;
        }
    }

    for (int i = 0; i < 9; i++) {
        int indice = indices_elegidos[i];
        const char* pais = paises_archivo[indice];

        if (insertar_inicio(lista, pais) != 0) {
            fprintf(stderr, "Error al insertar el país seleccionado: %s\n", pais);
            return -1;
        }
    }

    return 0;
}


int contar_ancho_visual_utf8(const char *s) {
    int ancho_visual = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        // Si el byte NO está en el rango 0x80 a 0xBF, es el inicio de un carácter
        // (ASCII estándar o inicio de secuencia multibyte).
        if ((s[i] & 0xC0) != 0x80) {
            ancho_visual++;
        }
    }
    return ancho_visual;
}


int creacion_problematicas(struct Dlista* listaPaises){
    if (listaPaises == NULL || listaPaises->inicio == NULL){
        return -1;
    }

    struct Dnodo* actual = listaPaises->inicio;
    int contador = 1;

    while (actual != NULL && contador <= 9){
        if (contador <= 3){
            actual->aspecto1 = 3;
            actual->aspecto2 = 2;
        } 
        else if (contador <= 6){
            actual->aspecto1 = 2;
            actual->aspecto2 = 1;
        } 
        else { 
            actual->aspecto1 = 1;
            actual->aspecto2 = 1;
        }

        actual = actual->sigt;
        contador++;
    }

        // 1. Imprimir encabezado de la tabla (con anchos fijos y limpios)
    printf("\n--- PAÍSES Y PROBLEMÁTICAS ---\n");
    printf("\n");
    // Formato: Contador (3) | País (30) | A1 (5) | A2 (5)
    printf("%-3s| %-35s| %-5s| %-5s\n", "#", "PAÍS", "A1", "A2");
    printf("---------------------------------------------------\n"); 

    actual = listaPaises->inicio;
    contador = 1;
    
   while (actual != NULL) {
        int ancho_visual = contar_ancho_visual_utf8(actual->pais);
        int padding = 34 - ancho_visual;
        
        if (padding < 0) padding = 1; 
        printf("%-3d| %s", contador++, actual->pais); 

        for (int i = 0; i < padding; i++) {
            printf(" ");
        }
        
        printf("| %-5d| %-5d\n", 
               actual->aspecto1, 
               actual->aspecto2); 
               
        actual = actual->sigt;
    }

    printf("---------------------------------------------------\n");
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
    crear_tablero(juego, archivo_paises);
    creacion_problematicas(juego);
    return 0;
}
