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

    struct Dnodo* sigt; //Vecino1 del pais (Creo xd)(sera?)
    struct Dnodo* ante; //Vecino2 del pais (Creo xd)(sera?)
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

//Funcion para diccionarios o tablas hash (Aqui van los proyectos para imprementar)

//Maicol, no se si esta vara funcione xd

#define TAMANO_INICIAL 13 //Primo para mejor rendimiento xd
#define FACTOR_CARGA_MAXIMO 0.75 //El valor de carga para si se pasa redimencionar el diccionario xd

struct tablaHash{
    struct Dlista** buckets;
    int tamano;
    int cantidad_elementos;
};

unsigned int funcion_hash(const char* clave, int tamano){
    unsigned int hash = 0;
    for(int i = 0; clave[i] != '\0'; i++){
        hash = hash * 31 + clave[i];
    }
    return hash % tamano;
}

struct tablaHash* crear_tabla_hash(){
    struct tablaHash* tabla = calloc(1, sizeof(struct tablaHash));
    if(tabla == NULL){
        return NULL;
    }
    tabla -> buckets = calloc(TAMANO_INICIAL, sizeof(struct Dlista*));
    if(tabla -> buckets == NULL){
        free(tabla);
        return NULL;
    }

    tabla -> tamano = TAMANO_INICIAL;
    tabla -> cantidad_elementos = 0; //Por que cuando se crea no tiene nada adentro xd

    for(int i = 0; i < tabla -> tamano; i++){
        tabla -> buckets[i] = crear_lista();
        if(tabla -> buckets[i] == NULL){
            return tabla;
        }
    }
    return tabla;
}


double calcular_factor_de_carga(const struct tablaHash* tabla){
    if(tabla == NULL || tabla -> tamano == 0){
        return 0.0;
    }
    return (double)tabla->cantidad_elementos / tabla -> tamano;
}

//Funcion para los jugadores
struct jugadores{
    char nombre[20];
    struct Dnodo* paisActual;
};

struct jugadores* crear_jugador(const char* nombre, struct Dnodo* pais){
    struct jugadores* jugador = calloc(1, sizeof(struct jugadores));
    if(jugador == NULL){
        return NULL;
    }
    strncpy(jugador -> nombre, nombre, 20);
    jugador -> nombre[19] = '\0';

    jugador -> paisActual = pais;
    return jugador;
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

void imprimir_paises(struct Dlista* listaPaises){
    printf("\n--- PAÍSES Y PROBLEMÁTICAS ---\n");

    printf("\n%-3s| %-35s| %-5s| %-5s\n", "#", "PAÍS", "A1", "A2");
    printf("---------------------------------------------------\n"); 

    struct Dnodo* actual = listaPaises->inicio;
    int contador = 1;
    
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

    imprimir_paises(listaPaises);
    return 0;
}

int inicializar_jugadores(struct jugadores** jugador1, struct jugadores** jugador2, struct Dlista* listaPaises){
    srand(time(NULL));
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

    *jugador1 = crear_jugador(nombre, actual); 

    printf("Ingrese el nombre del jugador 2: ");
    scanf("%s", nombre);

    actual = listaPaises->inicio;
    for(int i = 0; i < indice_aleatorio2; i++){
        if (actual -> sigt == NULL) break;
        actual = actual -> sigt;
    }

    *jugador2 = crear_jugador(nombre, actual);
    
    printf("\nBienvenidos jugadores %s y %s\n", (*jugador1)->nombre, (*jugador2)->nombre);
    
    return 0;
}

//Aqui van las accioness que podra hacer el jugador en el turno correspondiente:

int desplazarse_pais(struct jugadores** jugador, struct Dlista* paises) {
    
    struct Dnodo* actual = (*jugador)->paisActual;

    int opcion;
    printf("¿A dónde deseas moverte?\n");
    printf("1. País siguiente (->)\n");
    printf("2. País anterior (<-)\n");
    printf("Opción: ");
    scanf("%d", &opcion);
    // Lo voy a dejar asi momentaneamente, hasta que usted me diga como se hace de verdad xd
    switch (opcion) {
        case 1:
            if (actual->sigt != NULL) {
                (*jugador)->paisActual = actual->sigt;
                printf("Te has desplazado a: %s\n", (*jugador)->paisActual->pais);
                return 1; // Movimiento realizado
            } else {
                printf("fin del camino papu\n");
                return 0;
            }

        case 2:
            if (actual->ante != NULL) {
                (*jugador)->paisActual = actual->ante;
                printf("Te has desplazado a: %s\n", (*jugador)->paisActual->pais);
                return 1;// Movimiento realizado
            } else {
                printf("no hay terreno a la izquierda bro \n");
                return 0;
            }
        default:
            printf("Ete setch \n");
            return 0;
    }
}

// Eliminar elemento
int eliminar_paises(struct Dlista* paises){
    if (paises == NULL || paises->inicio == NULL) 
        return -1;

    // eliminar nodos al inicio que coincidan
    while (paises->inicio != NULL && (paises->inicio-> aspecto1 == 3 && paises->inicio-> aspecto2 == 3)) {
        struct Dnodo* temp = paises->inicio;
        paises->inicio = paises->inicio->sigt;
        if (paises->inicio != NULL) {
        paises->inicio->ante = NULL;
    }
        free(temp); //se libera la memoria del nodo
    }// el while eliminara todos los nodos que hayan al inicio que contengan el valor buscado

    struct Dnodo* actual = paises->inicio;
    while (actual != NULL && actual->sigt != NULL) {
        if (actual->sigt->aspecto1 == 3 && actual->sigt->aspecto2 == 3) {
            struct Dnodo* temp = actual->sigt;
            if (actual->sigt->sigt!=NULL){
                actual->sigt->sigt->ante=actual;
            }
            actual->sigt = actual->sigt->sigt;
            printf("\n %s, ha sido bombardeada por iran, f en el chat\n", temp->pais);
            free(temp);
            // Libera la memoria del nodo eliminado y conecta el nodo actual con el siguiente
        } else {
            actual = actual->sigt;
            // Avanza al siguiente nodo si no se elimina ninguno
        }
    }

    return 0;
}
int expandir_problematicas(struct Dlista* paises){
    
    for(int i = 0; i < 3; i++){
        struct Dnodo* actual = paises -> inicio; 
        int indice_aleatorio1 = rand() % 9; 
        for(int i = 0; i < indice_aleatorio1; i++){
            if (actual -> sigt == NULL) break; 
            actual = actual -> sigt;
        }
        int num_aspecto = rand() % 2;
        //si num_aspecto=0, aspecto guarda la direccion de memoria de aspecto1, en los demas casos
        //guarda la de aspecto2
        int* aspecto = (num_aspecto == 0) ? &actual->aspecto1 : &actual->aspecto2;

        if (*aspecto < 3) {
            (*aspecto)++;
        } else {
            // Si ya está en 3, se propaga a los vecinos 
            if (actual->ante != NULL) {
                int* vecino_izq = (num_aspecto == 0) ? &actual->ante->aspecto1 : &actual->ante->aspecto2;
                if (*vecino_izq < 3) {
                    (*vecino_izq)++;
                }
                
            }
            if (actual->sigt != NULL) {
                int* vecino_der = (num_aspecto == 0) ? &actual->sigt->aspecto1 : &actual->sigt->aspecto2;
                if (*vecino_der < 3) {
                    (*vecino_der)++;
            }
            }
        }
    }
    return 0;
}




int imprementar_proyecto(){
    return 0;
}

//Creo que es void o lo es por el momento xd (ok hijito)
void turno_jugador(struct jugadores** jugador, struct Dlista* paises){
    int accion;
    int turnosRestantes = 4;

    printf("\nTurno del jugador %s\n", (*jugador) -> nombre);
    while(turnosRestantes){
        printf("\nAcciones disponibles (Acciones restantes %d)\n", turnosRestantes);
        printf("1. Desplazarse de un país a otro (Cuesta 1 acción)\n");
        printf("2. Implementar Proyectos (Cuesta 1 acción)\n");
        printf("3. Ver país en el que se encuentra (No cuesta acción)\n");
        printf("4. Ver Estado del Tablero/Países (No cuesta acción)\n");

        printf("\n¿Qué acción deseas hacer? \n");
        scanf("%d", &accion);

        if(accion == 4){
            imprimir_paises(paises);
        }else if(accion == 3){
            printf("\nUsted se encuentra en el país: %s\n", (*jugador) -> paisActual -> pais);
        }else if(accion == 2){
            turnosRestantes--;
        }else if(accion == 1){
            int exito = desplazarse_pais(jugador, paises);
            if (exito == 1) {
                turnosRestantes--;
            }
        }else{
            printf("\nNever gonna give you up\n");
        }
        //printf("Acción seleccionada: %d\n", accion);
    } 
    expandir_problematicas(paises);
    return;
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
    srand(time(NULL));
    printf("--- Bienvenido a Pandemic ---\n");


    //hacer talvez un menu random, tipo jugar- instrucciones- datos de los creadores o algo asi, 
    //no me haga caso, es solo talvez    
    //int menu;
    //scanf("%d", &menu);

    //Insertar paises:
    struct Dlista* juego = crear_lista();
    const char* archivo_paises = "../Documentos/Países de América Latina.txt";
    crear_tablero(juego, archivo_paises);

    //Crear jugadores:
    struct jugadores* jugador1;
    struct jugadores* jugador2;
    inicializar_jugadores(&jugador1, &jugador2, juego);

    creacion_problematicas(juego);

    //Jugadores y su posicion en el mapa:
    printf("\n--- JUGADORES ---\n");
    printf("\nEl jugador %s ha aparecido en el pais: %s\nEl jugador %s ha aparecido en el pais: %s\n", jugador1 -> nombre, jugador1 -> paisActual -> pais, jugador2 -> nombre, jugador2 -> paisActual -> pais);

    int turnoJugador = 0;
    int jugadas = 5;
    while(jugadas--){
        if(turnoJugador == 0){
            turno_jugador(&jugador1, juego);
            turnoJugador = 1;
        }else{
            turno_jugador(&jugador2, juego);
            turnoJugador = 0;
        }
        eliminar_paises(juego);
    }
    
    printf("\nFin del juego xd\n");
    //Liberación de memoria:
    liberar_lista(juego);
    liberar_jugador(jugador1);
    liberar_jugador(jugador2);
    return 0;
}
