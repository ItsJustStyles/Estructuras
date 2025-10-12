#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>
#include <stdbool.h>

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
    struct Dnodo** vecinos;
    int cantidad_vecinos;
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

#define TAMANO_INICIAL_PROYECTOS 13 //Primo para mejor rendimiento xd
#define FACTOR_CARGA_MAXIMO 0.75 //El valor de carga para si se pasa redimencionar el diccionario xd
#define MAX_NOMBRE_PROYECTO 100
#define MAX_DESCRIPCION 256
#define MAX_BIBLIOGRAFIA 100
#define MAX_PAISES_APLICADOS 100

struct Pnodo{
    char nombre[MAX_NOMBRE_PROYECTO];
    char descripcion[MAX_DESCRIPCION];
    char bibliografia[MAX_BIBLIOGRAFIA];

    struct Pnodo* sigt;
};

struct Plista{
    struct Pnodo* inicio;
};

struct ProyectosHash{
    struct Plista** buckets;
    int tamano;
    int cantidad_elementos;
    double factorDecarga;
};

struct Plista* crear_lista_proyecto(){
    struct Plista* nueva_lista = calloc(1, sizeof(struct Plista));
    if(nueva_lista == NULL){
        return NULL;
    }
    nueva_lista -> inicio = NULL;
    return nueva_lista;
}

unsigned int funcion_hash_proyecto(const char* clave, int tamano){
    unsigned int hash = 0;
    for(int i = 0; clave[i] != '\0'; i++){
        hash = hash * 31 + clave[i];
    }
    return hash % tamano;
}

double calcular_factor_carga_proyecto(struct ProyectosHash* tabla){
    if (tabla == NULL || tabla->tamano == 0) {
        tabla -> factorDecarga  = 0.0;
        return 0.0;
    }
    tabla -> factorDecarga = (double)tabla->cantidad_elementos / tabla->tamano;;
    return tabla -> factorDecarga;
}

struct ProyectosHash* crear_tabla_hash_proyectos(){
    struct ProyectosHash* tabla = calloc(1, sizeof(struct ProyectosHash));
    if(tabla == NULL) return NULL;

    tabla -> buckets = calloc(TAMANO_INICIAL_PROYECTOS, sizeof(struct Plista*));
    if(tabla->buckets == NULL){
        free(tabla);
        return NULL;
    }

    tabla -> tamano = TAMANO_INICIAL_PROYECTOS;
    tabla -> cantidad_elementos = 0;
    tabla -> factorDecarga = 0.0;

    for (int i = 0; i < tabla->tamano; i++) {
        tabla->buckets[i] = crear_lista_proyecto();
        if (tabla->buckets[i] == NULL) {
            return tabla; 
        }
    }

    return tabla;
}

struct Pnodo* crear_nodo_proyecto(const char* nombre, const char* desc, const char* biblio) {
    struct Pnodo* nuevo_nodo = calloc(1, sizeof(struct Pnodo));
    if(nuevo_nodo == NULL) return NULL;

    strncpy(nuevo_nodo->nombre, nombre, MAX_NOMBRE_PROYECTO - 1);
    strncpy(nuevo_nodo->descripcion, desc, MAX_DESCRIPCION - 1);
    strncpy(nuevo_nodo->bibliografia, biblio, MAX_BIBLIOGRAFIA - 1);
    
    nuevo_nodo->sigt = NULL;
    return nuevo_nodo;
}

int insertar_proyecto_hash(struct ProyectosHash* tabla, struct Pnodo* nuevo_nodo) {
    if (tabla == NULL || nuevo_nodo == NULL) return -1;

    // 1. Obtener el índice
    unsigned int indice = funcion_hash_proyecto(nuevo_nodo->nombre, tabla->tamano);
    
    // 2. Insertar al inicio del bucket (Simple Chaining)
    nuevo_nodo->sigt = tabla->buckets[indice]->inicio;
    tabla->buckets[indice]->inicio = nuevo_nodo;
    
    // 3. Incrementar el contador
    tabla->cantidad_elementos++; 

    // 4. Revisar Factor de Carga y redimensionar
    if (calcular_factor_carga_proyecto(tabla) > FACTOR_CARGA_MAXIMO) {
        return -1;
    }

    return 0;
}

struct Pnodo* buscar_proyecto_hash(struct ProyectosHash* tabla, const char* nombre_proyecto) {
    if (tabla == NULL || nombre_proyecto == NULL) return NULL;

    // 1. Calcular el índice
    unsigned int indice = funcion_hash_proyecto(nombre_proyecto, tabla->tamano);
    
    // 2. Recorrer la lista (manejar colisiones)
    struct Pnodo* actual = tabla->buckets[indice]->inicio;

    while (actual != NULL) {
        if (strcmp(actual->nombre, nombre_proyecto) == 0) {
            return actual; // ¡Encontrado!
        }
        actual = actual->sigt;
    }
    
    return NULL; // Proyecto no encontrado
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
int desplazarse_pais(struct jugadores** jugador) {
    struct Dnodo* actual = (*jugador)->paisActual;

    if (actual->cantidad_vecinos == 0) {
        printf("bro, que paso, no hay vecinos, error\n");
        return 0;
    }

    // 1. Array temporal para mapear la opción del menú al puntero del vecino real
    struct Dnodo** vecinos_disponibles = malloc(actual->cantidad_vecinos * sizeof(struct Dnodo*));
    if (vecinos_disponibles == NULL) {
        printf("Error: Fallo de asignación de memoria.\n");
        return 0;
    }

    int contadorOpciones = 0;
    
    printf("\n--- Los amigos del tilin (Vecinos) ---\n\n");
    
    // 2. Llenar el array temporal con solo los punteros NO NULL
    for (int i = 0; i < actual->cantidad_vecinos; i++) {
        struct Dnodo* vecino = actual->vecinos[i];
        if (vecino != NULL) {
            // Imprimir la opción (1, 2, 3...) y guardar el puntero en el array temporal
            printf("%d. %s (Aspecto 1: %d, Aspecto 2: %d)\n",
                   contadorOpciones + 1, vecino->pais, vecino->aspecto1, vecino->aspecto2);
            vecinos_disponibles[contadorOpciones] = vecino;
            contadorOpciones++;
        }
    }

    // 3. Validación de no tener vecinos disponibles
    if (contadorOpciones == 0){
        printf("Este pais ya no tiene vecinos\n");
        free(vecinos_disponibles);
        return -1;
    }

    int opcion;
    printf("\nElige el destino, evita peru \n");

    // 4. Lectura de la opción
    if (scanf("%d", &opcion) != 1) {
        printf("\nSea serio\n");
        while (getchar() != '\n'); 
        free(vecinos_disponibles);
        return 0;
    }
    while (getchar() != '\n');
    
    // 5. Validación CRÍTICA: La opción debe estar en el rango de vecinos REALES
    if (opcion < 1 || opcion > contadorOpciones) {
        printf("\nelije bien bro. Opcion invalida. \n");
        free(vecinos_disponibles);
        return 0;
    }
    
    // 6. Asignar el nuevo país usando el puntero del array temporal
    (*jugador)->paisActual = vecinos_disponibles[opcion - 1];
    
    printf("\nFelicidades, evitaste bolivia, fuiste a: %s\n", (*jugador)->paisActual->pais);
    
    free(vecinos_disponibles); // CRÍTICO: Liberar la memoria temporal
    return 1;
}



int escoger_proyecto(struct ProyectosHash* proyectos, const char* nombreProyecto){
    char implementacion[5];
    struct Pnodo* encontrado = buscar_proyecto_hash(proyectos, nombreProyecto);

    if (encontrado != NULL) {
        printf("\nProyecto Encontrado: %s\n", encontrado->nombre);
        printf("  - Descripcion: %s\n", encontrado->descripcion);
        printf("  - Actores: %s\n", encontrado->bibliografia);
    } else {
        printf("\nProyecto '%s' no encontrado.\n", nombreProyecto);
        return -1;
    }

    printf("\n¿Desea implementar el proyecto? (Si o no) ");
    if (scanf("%4s", implementacion) != 1) { 
        return -1; 
    }
    if(strcasecmp(implementacion, "si") == 0){
        return 1;
    }
    return -1;
}

int imprementar_proyecto(struct ProyectosHash* proyectos){
    int numProyecto;
    printf("\n --- PROYECTOS DISPONIBLES ---\n\n");
    printf("1. Programa de intervención focalizada \"Grupos de Riesgo\" (Focused Dete-rrence)\n");
    printf("2. Control de contenedores y trazabilidad agrícola inteligente\n");

    printf("\n¿Qué proyecto desea implementar: ");
    scanf("%d", &numProyecto);
    if(numProyecto == 1){
        if(escoger_proyecto(proyectos, "Programa de intervención focalizada \"Grupos de Riesgo\" (Focused Dete-rrence)") == 1){
            printf("\nImprementando proyecto\n");
        }else{
            return -1;
        }
    }else if(numProyecto == 2){
            if(escoger_proyecto(proyectos, "Control de contenedores y trazabilidad agrícola inteligente") == 1){
                printf("\nImprementando proyecto\n");
            }
    }else{
        printf("\nBomboclat\n");
        return -1;
    }
    return 0;
}

void limpiar_vecindad(struct Dlista* paises, struct Dnodo* eliminado) {
    if (paises == NULL || paises->inicio == NULL || eliminado == NULL) {
        return;
    }

    struct Dnodo* actual = paises->inicio;
    
    // Iterar sobre TODOS los países que quedan en el mapa
    while (actual != NULL) {
        // Un país no puede ser su propio vecino, pero verificamos si es el que se está eliminando
        if (actual != eliminado) {
            
            // Iterar sobre el array de vecinos de 'actual'
            for (int i = 0; i < actual->cantidad_vecinos; i++) {
                
                // Si encontramos una referencia al país que se va a eliminar
                if (actual->vecinos[i] == eliminado) {
                    
                    // Reemplazar el puntero por NULL para evitar un puntero colgante (dangling pointer)
                    actual->vecinos[i] = NULL; 
                    
                }
            }
        }
        actual = actual->sigt;
    }
}

int eliminar_paises(struct Dlista* paises){
    if (paises == NULL || paises->inicio == NULL) 
        return -1;

    // eliminar nodos al inicio que coincidan
    while (paises->inicio != NULL && (paises->inicio-> aspecto1 == 3 && paises->inicio-> aspecto2 == 3)) {
        struct Dnodo* temp = paises->inicio;
        limpiar_vecindad(paises, temp);
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
            limpiar_vecindad(paises, temp); 
            if (actual->sigt->sigt!=NULL){
                actual->sigt->sigt->ante=actual;
            }
            actual->sigt = actual->sigt->sigt;
            printf("\n %s, ha sido bombardeada por Iran, F en el chat\n", temp->pais);
            free(temp);
            // Libera la memoria del nodo eliminado y conecta el nodo actual con el siguiente
        } else {
            actual = actual->sigt;
            // Avanza al siguiente nodo si no se elimina ninguno
        }
    }

    return 0;
}

#define MAX_CAMBIOS 20
struct cambio{
    char pais[25];
    char aspecto[15];
};

//ya editada, se supone que lo hace con los vecinos del array, pero no lo he probado del todo, tal vez con unos prints luego
int expandir_problematicas(struct Dlista* paises) {
    if (paises == NULL || paises->inicio == NULL)
        return -1;

    struct cambio cambios[MAX_CAMBIOS];
    int contador_cambios = 0;
    
    for (int i = 0; i < 3; i++) {
        struct Dnodo* actual = paises->inicio;
        int indice_aleatorio1 = rand() % 9;

        
        for (int j = 0; j < indice_aleatorio1; j++) {
            if (actual->sigt == NULL) break;
            actual = actual->sigt;
        }

        int num_aspecto = rand() % 2;
        int* aspecto = (num_aspecto == 0) ? &actual->aspecto1 : &actual->aspecto2;
        const char* nombre_aspecto = (num_aspecto == 0) ? "Aspecto 1" : "Aspecto 2";

        if (*aspecto < 3) {
            (*aspecto)++;

            if(contador_cambios < MAX_CAMBIOS){
                strncpy(cambios[contador_cambios].pais, actual-> pais, 24);
                cambios[contador_cambios].pais[24] = '\0'; 
                strncpy(cambios[contador_cambios].aspecto, nombre_aspecto, 14);
                cambios[contador_cambios].aspecto[14] = '\0'; 
                contador_cambios++;
            }

        } else {
            
            for (int v = 0; v < actual->cantidad_vecinos; v++) {
                struct Dnodo* vecino = actual->vecinos[v];
                if (vecino == NULL) continue;

                int* aspecto_vecino = (num_aspecto == 0) ? &vecino->aspecto1 : &vecino->aspecto2;
                if (*aspecto_vecino < 3) {
                    (*aspecto_vecino)++;

                    if (contador_cambios < MAX_CAMBIOS) {
                        strncpy(cambios[contador_cambios].pais, actual-> pais, 24);
                        cambios[contador_cambios].pais[24] = '\0'; 
                        strncpy(cambios[contador_cambios].aspecto, nombre_aspecto, 14);
                        cambios[contador_cambios].aspecto[14] = '\0'; 
                        contador_cambios++;
                }
            }
        }
    }
    }
    printf("\n=======================================================\n");
    printf("        --- REPORTE DE EXPANSIÓN DE PROBLEMAS ---\n");
    printf("=======================================================\n");
    
    if (contador_cambios == 0) {
        printf("No hubo cambios en este turno.\n");
    } else {
        for (int k = 0; k < contador_cambios; k++) {
            printf("⚠️ [%s] aumentó el %s\n", 
                   cambios[k].pais, 
                   cambios[k].aspecto);
        }
    }
    printf("-------------------------------------------------------\n");
    return 0;
}


int Generacion_vecinos(struct Dlista* paises){
    if (paises == NULL || paises->inicio == NULL) 
        return -1;

    struct Dnodo* actual = paises->inicio;

    while (actual != NULL){
        int cant_vecinos = rand() % 3 + 1; // entre 1 y 3 vecinos
        actual->cantidad_vecinos = cant_vecinos;
        actual->vecinos = calloc(cant_vecinos, sizeof(struct Dnodo*)); // reserva memoria
        actual = actual->sigt;
    }
    return 0; 
}

// se que esta en ingles, no se me ocurria nada xd, a lo diego god
// Eso fue chat no mienta
struct Dnodo* random_country(struct Dlista* paises, struct Dnodo* excluido){
    if (paises == NULL || paises->inicio == NULL)
        return NULL;

    int total = 0;
    struct Dnodo* actual = paises->inicio;
    while (actual != NULL) {
        total++;
        actual = actual->sigt;
    }

    struct Dnodo* elegido = NULL;
    do {
        int indice = rand() % total;
        actual = paises->inicio;
        for (int i = 0; i < indice; i++) {
            actual = actual->sigt;
        }
        elegido = actual;
    } while (elegido == excluido); 

    return elegido;
}

int asignar_vecinos(struct Dlista* paises){
    if (paises == NULL || paises->inicio == NULL) 
        return -1;

    struct Dnodo* actual = paises->inicio;

    while (actual != NULL){
        for (int i = 0; i < actual->cantidad_vecinos; i++){
            if (actual->vecinos[i] != NULL){
                continue; 
            } 
                

            struct Dnodo* candidato = random_country(paises, actual);
            if (candidato == NULL) {
                continue;
            }

            int repetido = 0;
            for (int j = 0; j < actual->cantidad_vecinos; j++){
                if (actual->vecinos[j] == candidato){
                    repetido = 1;
                    break;
                }
            }
            if (repetido) {
                continue;
            }

            actual->vecinos[i] = candidato;

            
            int bidireccion = 0;
            for (int k = 0; k < candidato->cantidad_vecinos; k++){
                if (candidato->vecinos[k] == actual){
                    bidireccion = 1;
                    break;
                }
            }

            if (!bidireccion){
                for (int k = 0; k < candidato->cantidad_vecinos; k++){
                    if (candidato->vecinos[k] == NULL){
                        candidato->vecinos[k] = actual;
                        break;
                    }
                }
            }
        }

        actual = actual->sigt;
    }

    return 0;
}

void mostrar_vecinos_pais(struct Dnodo* inicio_lista, const char* nombre_pais_central) {
    if (inicio_lista == NULL || nombre_pais_central == NULL) {
        printf("Error: La lista de países no ha sido inicializada.\n");
        return;
    }

    // 1. Buscar el país central recorriendo la lista (O(N) - Búsqueda Lenta)
    struct Dnodo* pais_central = NULL;
    struct Dnodo* actual = inicio_lista;

    while (actual != NULL) {
        // Usamos strcmp para comparar cadenas de caracteres (pais)
        if (strcmp(actual->pais, nombre_pais_central) == 0) {
            pais_central = actual; 
            break;
        }
        // Avanzamos al siguiente nodo en la lista
        actual = actual->sigt; 
    }

    if (pais_central == NULL) {
        printf("Error: El país '%s' no se encuentra en el tablero.\n", nombre_pais_central);
        return;
    }

    // 2. Imprimir encabezado

    // 3. Iterar sobre el array de vecinos del nodo encontrado
    int vecinos_encontrados = 0;
    int paisIndice = 1;
    
    // El bucle recorre la capacidad máxima del array de vecinos
    for (int i = 0; i < pais_central->cantidad_vecinos; i++) {
        struct Dnodo* vecino = pais_central->vecinos[i];
        
        // Solo mostramos si el puntero no es NULL (es una conexión activa)
        if (vecino != NULL) {
            printf("  %d.  %s (Aspecto 1: %d, Aspecto 2: %d)\n", 
                   paisIndice,vecino->pais, vecino->aspecto1, vecino->aspecto2);
            vecinos_encontrados++;
            paisIndice++;
        }
    }
    
    if (vecinos_encontrados == 0) {
        printf("Este país no tiene vecinos.\n");
    }
    //printf("------------------------------------------------\n");
}

int verificar_ganar_o_perder(const struct Dlista* juego){
    //Verificar si se gano:
    bool aspecto_1 = true;
    bool aspecto_2 = true;
    int contadorPaises = 1;
    struct Dnodo* actual = juego -> inicio;
    while(actual -> sigt != NULL){
        if(actual -> aspecto1 > 0){
            aspecto_1 = false;
        }
        if(actual -> aspecto2){
            aspecto_2 = false;
        }
        actual = actual -> sigt;
        contadorPaises++;
    }
    if(aspecto_1 || aspecto_2){
        return 1;
    }
    if(contadorPaises <= 3){
        return -1;
    }

    return 0;
}

//Creo que es void o lo es por el momento xd (ok hijito)
void turno_jugador(struct jugadores** jugador, struct Dlista* paises, struct ProyectosHash* proyectos){
    int accion;
    int turnosRestantes = 4;

    printf("\nTurno del jugador %s\n", (*jugador) -> nombre);
    while(turnosRestantes){
        printf("\nAcciones disponibles (Acciones restantes %d)\n", turnosRestantes);
        printf("1. Desplazarse de un país a otro (Cuesta 1 acción)\n");
        printf("2. Implementar Proyectos (Cuesta 1 acción)\n");
        printf("3. Ver país en el que se encuentra (No cuesta acción)\n");
        printf("4. Ver Estado del Tablero/Países (No cuesta acción)\n");

        printf("\n¿Qué acción deseas hacer? ");
        scanf("%d", &accion);

        if(accion == 4){
            imprimir_paises(paises);
        }else if(accion == 3){
            printf("\nUsted se encuentra en el país: %s\n", (*jugador) -> paisActual -> pais);
        }else if(accion == 2){
            if(imprementar_proyecto(proyectos) == 0){
                //Falta que se baje el aspecto del pais en el que se encuentra
                turnosRestantes--;
            }
        }else if(accion == 1){
    int exito = desplazarse_pais(jugador);
    if (exito == 1) {
        turnosRestantes--;
    }
        }else{
            printf("\nNever gonna give you up\n");
        }
        //printf("Acción seleccionada: %d\n", accion);
    } 
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

void liberar_lista_proyecto(struct Plista* lista) {
    if (lista == NULL) return;
    struct Pnodo* actual = lista->inicio;
    struct Pnodo* temp;
    while (actual != NULL) {
        temp = actual;
        actual = actual->sigt;
        free(temp);
    }
    free(lista);
}

void liberar_tabla_hash_proyectos(struct ProyectosHash* tabla) {
    if (tabla == NULL) return;
    
    for (int i = 0; i < tabla->tamano; i++) {
        liberar_lista_proyecto(tabla->buckets[i]); 
    }
    
    free(tabla->buckets);
    free(tabla);
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

    //Crear proyectos por problematicas (Tabla de dispersión):

    //Paso 1: Creación de la tabla
    struct ProyectosHash* proyectos = crear_tabla_hash_proyectos();

    //Paso 2: Creación e inserción de nodos de proyecto:
    //La estructura es: nombre del proyecto, descripcion, bibliografia
    struct Pnodo* p1 = crear_nodo_proyecto("Programa de intervención focalizada \"Grupos de Riesgo\" (Focused Dete-rrence)", "Identificar bandas o zonas con alta incidencia de homicidios y ofrecer mediación social, oportunidades educativas y laborales, \n   junto con aplicación rigurosa de la ley a infractores reincidentes", "Policía, fiscalía, municipalidades, ONGs sociales");
    struct Pnodo* p2 = crear_nodo_proyecto("Control de contenedores y trazabilidad agrícola inteligente", "Implementar sistemas de escaneo no invasivo en puertos y aeropuertos, junto con plataformas de trazabilidad basadas en blockchain para productos agrícolas de exportación", "Aduanas, ministerios de seguridad y agricultura, cooperación internacional (UNODC)");


    //Se inserta
    insertar_proyecto_hash(proyectos, p1);
    insertar_proyecto_hash(proyectos, p2);

    //Crear jugadores:
    struct jugadores* jugador1;
    struct jugadores* jugador2;
    inicializar_jugadores(&jugador1, &jugador2, juego);

    creacion_problematicas(juego);
    Generacion_vecinos(juego);
    asignar_vecinos(juego);
    //Jugadores y su posicion en el mapa:
    printf("\n--- JUGADORES ---\n");
    printf("\nEl jugador %s ha aparecido en el pais: %s\nEl jugador %s ha aparecido en el pais: %s\n", jugador1 -> nombre, jugador1 -> paisActual -> pais, jugador2 -> nombre, jugador2 -> paisActual -> pais);
    int turnoJugador = 0;
    int jugadas = verificar_ganar_o_perder(juego);
    while(jugadas == 0){
        if(turnoJugador == 0){
            turno_jugador(&jugador1, juego, proyectos);
            turnoJugador = 1;
            //mostrar_vecinos(juego);
        }else{
            turno_jugador(&jugador2, juego, proyectos);
            turnoJugador = 0;
            //mostrar_vecinos(juego);
        }
        expandir_problematicas(juego);
        eliminar_paises(juego);
        jugadas = verificar_ganar_o_perder(juego);

    }

    if(jugadas == -1){
        printf("\n--- Has perdido, muy noob bro ---\n");
    }else{
        printf("\n--- Has ganado, que crack ---\n");
    }

    printf("\nFin del juego xd\n");
    //Liberación de memoria:
    liberar_tabla_hash_proyectos(proyectos);
    liberar_lista(juego);
    liberar_jugador(jugador1);
    liberar_jugador(jugador2);
    return 0;
}