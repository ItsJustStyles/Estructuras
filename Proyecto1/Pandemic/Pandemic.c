#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>

//Funcion para los jugadores
struct jugadores{
    char nombre[20];
    char paisActual[25];
};

struct jugadores* crear_jugador(const char* nombre, const char* pais){
    struct jugadores* jugador = calloc(1, sizeof(struct jugadores));
    if(jugador == NULL){
        return NULL;
    }
    strncpy(jugador -> nombre, nombre, 20);
    jugador -> nombre[20] = '\0';

    strncpy(jugador -> paisActual, pais, 25);
    jugador -> paisActual[25] = '\0';
    return jugador;
}

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

    printf("\n--- PAÍSES Y PROBLEMÁTICAS ---\n");

    printf("\n%-3s| %-35s| %-5s| %-5s\n", "#", "PAÍS", "A1", "A2");
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

int inicializar_jugadores(struct jugadores** jugador1, struct jugadores** jugador2, struct Dlista* listaPaises){
    srand(time(NULL)); // La inicialización de rand() está bien aquí.
    struct Dnodo* actual = listaPaises -> inicio;
    char nombre[20];
    
    int indice_aleatorio1 = rand() % 9; 
    int indice_aleatorio2 = rand() % 9;

    printf("\nIngrese el nombre del jugador 1: ");
    scanf("%s", nombre);
    
    actual = listaPaises->inicio;
    for(int i = 0; i < indice_aleatorio1; i++){
        if (actual -> sigt == NULL) break; 
        actual = actual -> sigt;
    }

    *jugador1 = crear_jugador(nombre, actual -> pais); 

    printf("Ingrese el nombre del jugador 2: ");
    scanf("%s", nombre);

    actual = listaPaises->inicio;
    for(int i = 0; i < indice_aleatorio2; i++){
        if (actual -> sigt == NULL) break;
        actual = actual -> sigt;
    }

    *jugador2 = crear_jugador(nombre, actual -> pais);
    
    printf("\nBienvenidos jugadores %s y %s\n", (*jugador1)->nombre, (*jugador2)->nombre);
    
    return 0;
}

//Liberar memoria:
void liberar_jugador(struct jugadores* jugador) {
    if (jugador != NULL) {
        free(jugador);
    }
}

void liberar_lista(struct Dlista* lista) {
    if (lista == NULL) {
        return;
    }

    struct Dnodo* actual = lista->inicio;
    struct Dnodo* siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->sigt;
        free(actual);            
        actual = siguiente;
    }
    
    free(lista);
}

//Función principal, aquí se ejecutara el juego:
int main(){
    printf("--- Bienvenido a Pandemic ---\n");

    //Insertar paises:
    struct Dlista* juego = crear_lista();
    const char* archivo_paises = "../Documentos/Países de América Latina.txt";
    crear_tablero(juego, archivo_paises);

    //Crear jugadores:
    struct jugadores* jugador1;
    struct jugadores* jugador2;
    inicializar_jugadores(&jugador1, &jugador2, juego);

    //Crear problematicas del juego:
    creacion_problematicas(juego);

    //Jugadores y su posicion en el mapa:
    printf("\n--- JUGADORES ---\n");
    printf("\nEl jugador %s ha aparecido en el pais: %s\nEl jugador %s ha aparecido en el pais: %s\n", jugador1 -> nombre, jugador1 -> paisActual, jugador2 -> nombre, jugador2 -> paisActual);

    //Liberación de memoria:
    liberar_lista(juego);
    liberar_jugador(jugador1);
    liberar_jugador(jugador2);
    return 0;
}
