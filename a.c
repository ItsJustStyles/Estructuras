#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- CONSTANTES ---
#define TAMANO_INICIAL_PROYECTOS 17 // Tamaño inicial primo para buen rendimiento
#define FACTOR_CARGA_MAXIMO 0.75    // Umbral de carga para redimensionar (75%)
#define MAX_NOMBRE_PROYECTO 50
#define MAX_DESCRIPCION 256
#define MAX_BIBLIOGRAFIA 100
#define MAX_PAISES_APLICADOS 100

// ==========================================================
// ESTRUCTURAS BÁSICAS (NODOS Y LISTAS PARA EL ENCADENAMIENTO)
// ==========================================================

// 1. Nodo de Proyecto (Contiene la clave y el valor)
struct Pnodo {
    char nombre[MAX_NOMBRE_PROYECTO];        // CLAVE: Nombre del proyecto
    char descripcion[MAX_DESCRIPCION];
    char bibliografia[MAX_BIBLIOGRAFIA];
    char paises_aplicados[MAX_PAISES_APLICADOS]; // Países donde se ha aplicado
    
    struct Pnodo* sigt; // Puntero para el encadenamiento (colisiones)
};

// 2. Lista de Proyectos (Un bucket en la tabla hash)
struct Plista {
    struct Pnodo* inicio;
};

// 3. Estructura de la Tabla Hash (Diccionario)
struct ProyectosHash {
    struct Plista** buckets; 
    int tamano;              
    int cantidad_elementos;  // Contador para el Factor de Carga
};

// ==========================================================
// FUNCIONES AUXILIARES DE LISTA
// ==========================================================

struct Plista* crear_lista_proyecto() {
    struct Plista* nueva_lista = calloc(1, sizeof(struct Plista));
    if(nueva_lista == NULL) return NULL;
    nueva_lista->inicio = NULL;
    return nueva_lista;
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

// Función auxiliar para desconectar un nodo de su lista vieja y conectarlo al inicio de una nueva.
void transferir_nodo_proyecto(struct Plista* lista_vieja, struct Pnodo* nodo, struct Plista* lista_nueva) {
    // 1. Desconectar el nodo de la lista vieja (Simple: solo puntero 'sigt')
    if (nodo == lista_vieja->inicio) {
        lista_vieja->inicio = nodo->sigt;
    } else {
        struct Pnodo* anterior = lista_vieja->inicio;
        while (anterior != NULL && anterior->sigt != nodo) {
            anterior = anterior->sigt;
        }
        if (anterior != NULL) {
            anterior->sigt = nodo->sigt;
        }
    }

    // 2. Conectar el nodo al inicio de la lista nueva
    nodo->sigt = lista_nueva->inicio;
    lista_nueva->inicio = nodo;
}

// ==========================================================
// FUNCIONES DEL DICCIONARIO HASH
// ==========================================================

// 1. Función Hash (Convierte el nombre del proyecto en un índice)
unsigned int funcion_hash_proyecto(const char* clave, int tamano) {
    unsigned int hash = 0;
    for (int i = 0; clave[i] != '\0'; i++) {
        hash = hash * 31 + clave[i];
    }
    return hash % tamano;
}

// 2. Cálculo del Factor de Carga
double calcular_factor_carga_proyecto(const struct ProyectosHash* tabla) {
    if (tabla == NULL || tabla->tamano == 0) {
        return 0.0;
    }
    return (double)tabla->cantidad_elementos / tabla->tamano;
}

// 3. Creación e Inicialización del Diccionario Hash
struct ProyectosHash* crear_tabla_hash_proyectos() {
    struct ProyectosHash* tabla = calloc(1, sizeof(struct ProyectosHash));
    if (tabla == NULL) return NULL;
    
    tabla->buckets = calloc(TAMANO_INICIAL_PROYECTOS, sizeof(struct Plista*));
    if (tabla->buckets == NULL) {
        free(tabla);
        return NULL;
    }

    tabla->tamano = TAMANO_INICIAL_PROYECTOS;
    tabla->cantidad_elementos = 0;

    for (int i = 0; i < tabla->tamano; i++) {
        tabla->buckets[i] = crear_lista_proyecto();
        if (tabla->buckets[i] == NULL) {
            // Manejo de error parcial
            return tabla; 
        }
    }
    return tabla;
}

// 4. Implementación del Rehashing (Redimensionamiento)
int redimensionar_proyectos(struct ProyectosHash* tabla) {
    int nuevo_tamano = tabla->tamano * 2; 

    // Crear la nueva tabla (sin liberar la estructura temporal al final)
    struct ProyectosHash* nueva_tabla = crear_tabla_hash_proyectos();
    if (nueva_tabla == NULL) return -1;
    
    // Asignar el nuevo tamaño a la estructura temporal
    free(nueva_tabla->buckets); // Libera el array de tamaño inicial
    nueva_tabla->buckets = calloc(nuevo_tamano, sizeof(struct Plista*));
    if (nueva_tabla->buckets == NULL) {
        free(nueva_tabla);
        return -1;
    }
    nueva_tabla->tamano = nuevo_tamano;
    for (int i = 0; i < nuevo_tamano; i++) {
        nueva_tabla->buckets[i] = crear_lista_proyecto();
    }
    
    // Transferir todos los nodos (REHASHING)
    for (int i = 0; i < tabla->tamano; i++) {
        struct Plista* lista_vieja = tabla->buckets[i];
        struct Pnodo* actual = lista_vieja->inicio;
        struct Pnodo* siguiente;

        while (actual != NULL) {
            siguiente = actual->sigt;
            
            // Recalcular el hash con el nuevo tamaño
            unsigned int nuevo_indice = funcion_hash_proyecto(actual->nombre, nueva_tabla->tamano);
            
            // Transferir el nodo (lo desconecta del viejo y lo conecta al nuevo)
            transferir_nodo_proyecto(lista_vieja, actual, nueva_tabla->buckets[nuevo_indice]);
            
            actual = siguiente;
        }
    }
    
    // Intercambiar las partes dinámicas
    
    // Liberar los 'struct Plista' vacíos de la tabla vieja
    for (int i = 0; i < tabla->tamano; i++) {
        free(tabla->buckets[i]);
    }
    free(tabla->buckets); // Liberar el array de punteros viejo

    // Asignar los nuevos componentes
    tabla->buckets = nueva_tabla->buckets;
    tabla->tamano = nueva_tabla->tamano;
    
    // Liberar la estructura TablaHash temporal (pero no su contenido)
    free(nueva_tabla); 

    printf("\n[INFO HASH] Diccionario de Proyectos redimensionado. Nuevo tamaño: %d\n", tabla->tamano);
    return 0;
}


// 5. Inserción de un Nuevo Proyecto
struct Pnodo* crear_nodo_proyecto(const char* nombre, const char* desc, const char* biblio, const char* paises) {
    struct Pnodo* nuevo_nodo = calloc(1, sizeof(struct Pnodo));
    if(nuevo_nodo == NULL) return NULL;

    strncpy(nuevo_nodo->nombre, nombre, MAX_NOMBRE_PROYECTO - 1);
    strncpy(nuevo_nodo->descripcion, desc, MAX_DESCRIPCION - 1);
    strncpy(nuevo_nodo->bibliografia, biblio, MAX_BIBLIOGRAFIA - 1);
    strncpy(nuevo_nodo->paises_aplicados, paises, MAX_PAISES_APLICADOS - 1);
    
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
        redimensionar_proyectos(tabla);
    }

    return 0;
}

// 6. Búsqueda de un Proyecto
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

// 7. Liberación de Memoria
void liberar_tabla_hash_proyectos(struct ProyectosHash* tabla) {
    if (tabla == NULL) return;
    
    for (int i = 0; i < tabla->tamano; i++) {
        liberar_lista_proyecto(tabla->buckets[i]); 
    }
    
    free(tabla->buckets);
    free(tabla);
}


// ==========================================================
// EJEMPLO DE USO (MAIN)
// ==========================================================
/*
int main() {
    printf("--- INICIANDO DICCIONARIO DE PROYECTOS ---\n");

    // 1. Creación de la tabla
    struct ProyectosHash* proyectos = crear_tabla_hash_proyectos();
    if (proyectos == NULL) return 1;

    // 2. Creación e Inserción de nodos de proyecto
    struct Pnodo* p1 = crear_nodo_proyecto("Campaña de Concienciacion", "Informar sobre el Aspecto 1", "OMS 2023", "Peru, Chile");
    struct Pnodo* p2 = crear_nodo_proyecto("Sistemas de Alerta Temprana", "Mejorar la respuesta a Aspecto 2", "UNICEF 2024", "Mexico");
    struct Pnodo* p3 = crear_nodo_proyecto("Reduccion de la Vulnerabilidad", "Proyectos de infraestructura", "FAO 2022", "Argentina");
    
    insertar_proyecto_hash(proyectos, p1);
    insertar_proyecto_hash(proyectos, p2);
    insertar_proyecto_hash(proyectos, p3);

    // 3. Demostración de Búsqueda
    const char* nombre_buscado = "Campaña de Concienciacion";
    struct Pnodo* encontrado = buscar_proyecto_hash(proyectos, nombre_buscado);

    if (encontrado != NULL) {
        printf("\nProyecto Encontrado: %s\n", encontrado->nombre);
        printf("  - Descripcion: %s\n", encontrado->descripcion);
        printf("  - Aplicado en: %s\n", encontrado->paises_aplicados);
    } else {
        printf("\nProyecto '%s' no encontrado.\n", nombre_buscado);
    }

    // 4. Liberación de memoria
    liberar_tabla_hash_proyectos(proyectos);
    printf("\n--- MEMORIA LIBERADA ---\n");

    return 0;
}
*/