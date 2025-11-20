//Zona de imports necesarios para el programa:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// - Definiciones de  estructuras de datos:

// Lista doblemente enlazada:

// Nodo de la lista doblemente enlazada:
struct Dnodo{
    int num;

    struct Dnodo* sigt;
    struct Dnodo* ant;
};

struct Dlista{
    struct Dnodo* inicio;
};

struct HeapArticulo {

    struct Articulo *array; 
    int size;               
    int capacity;           
   
    int (*comparador)(const struct Articulo*, const struct Articulo*);
};

struct Articulo {
    char nombre_autor[21];   //Se puede hacer mejor xd, ahora si de verdad, luego vemos 
    char apellidos_autor[41]; 
    char titulo[71];          
    char ruta[51];            
    char anio[5];             
    char resumen[81];         
};
//Funciones para el manejo de la lista enlazada:

struct Dnodo* crear_Dnodo(int n){
    struct Dnodo* nn = calloc(1, sizeof(struct Dnodo));
    if(nn == NULL){
        return NULL;
    }

    nn -> num = n;
    nn -> sigt = NULL;
    nn -> ant = NULL;
    return nn;
}

struct Dlista* crear_Dlista(){
    struct Dlista* lista = calloc(1, sizeof(struct Dlista));
    if(lista == NULL){
        return NULL;
    }
    lista -> inicio = NULL;
    return lista;
}

int insertar_inicio_Dlista(struct Dlista* lista, int n){
    if(lista == NULL){
        return -1; // ! Si la lista no existe porque no se creo o por otro error la función termina
    }

    struct Dnodo* nn = crear_Dnodo(n);
    if(nn == NULL){
        return -1; // ! Si el nodo no se crea la función termina
    }

    nn -> sigt = lista -> inicio;

    if(lista -> inicio != NULL){
        lista -> inicio -> ant = nn;
    }

    lista -> inicio = nn;
    return 0;
}

//Funciones para el manejo del Heap:
//*********************************************************************** */

struct HeapArticulo* crear_heap_articulos(int capacidad,
                                          int (*comparador)(const struct Articulo*, const struct Articulo*)) {
    if(capacidad <= 0 || comparador == NULL) return NULL;

    struct HeapArticulo* h = calloc(1, sizeof(struct HeapArticulo));
    if(h == NULL) return NULL;

    h->array = calloc(capacidad, sizeof(struct Articulo));
    if(h->array == NULL) {
        free(h);
        return NULL;
    }

    h->size = 0;
    h->capacity = capacidad;
    h->comparador = comparador;
    return h;
}

// intercambia dos posiciones del array
void swap_articulo(struct Articulo *a, struct Articulo *b) {
    struct Articulo tmp;
    memcpy(&tmp, a, sizeof(struct Articulo));
    memcpy(a, b, sizeof(struct Articulo));
    memcpy(b, &tmp, sizeof(struct Articulo));
}

//mantener propiedad de montículo máximo según comparador
void heapify_articulos(struct HeapArticulo* heap, int i) {
    if(heap == NULL) return;
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if(left < heap->size && heap->comparador(&heap->array[left], &heap->array[largest]) > 0)
        largest = left;

    if(right < heap->size && heap->comparador(&heap->array[right], &heap->array[largest]) > 0)
        largest = right;

    if(largest != i) {
        swap_articulo(&heap->array[i], &heap->array[largest]);
        heapify_articulos(heap, largest);
    }
}


int insertar_heap_articulo(struct HeapArticulo* heap, const struct Articulo* art) {
    if(heap == NULL || art == NULL) return -1;
    if(heap->size == heap->capacity) return -1; // overflow

    int i = heap->size;
    
    memcpy(&heap->array[i], art, sizeof(struct Articulo));
    heap->size++;


    while(i != 0 && heap->comparador(&heap->array[i], &heap->array[(i - 1) / 2]) > 0) {
        swap_articulo(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
    return 0;
}

// extraer máximo 
int extraer_max_articulo(struct HeapArticulo* heap, struct Articulo* out) {
    if(heap == NULL || out == NULL) return -1;
    if(heap->size <= 0) return -1;

    
    memcpy(out, &heap->array[0], sizeof(struct Articulo));

    
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    heapify_articulos(heap, 0);
    return 0;
}
// - Funciones para manejo de memoria:

int liberar_Dlista(struct Dlista* lista){
    if(lista == NULL){
        return 0;
    }
    struct Dnodo* actual = lista -> inicio;
    struct Dnodo* siguiente = NULL; // ! Este nodo evita que el nodo actual se vuelva invalido cuando se es liberado

    while(actual != NULL){
        siguiente = actual -> sigt;
        free(actual);
        actual = siguiente;
    }

    free(lista);
    return 0;
}

void liberar_heap_articulos(struct HeapArticulo* heap) {
    if(heap == NULL) return;
    free(heap->array);
    free(heap);
}

// - Funciones para realizar debugging:

int imprimir_Dlista(struct Dlista* lista){
    if(lista == NULL){
        return 0;
    }

    struct Dnodo* actual = lista -> inicio;
    while(actual != NULL){
        printf("%d\n", actual -> num);
        actual = actual -> sigt;
    }
    return 0;

}

void imprimir_articulo(const struct Articulo* articulo) {
    if(articulo == NULL) return;
    printf("Autor: %s %s\n", articulo->nombre_autor, articulo->apellidos_autor);
    printf("Titulo: %s\n", articulo->titulo);
    printf("Ruta: %s\n", articulo->ruta);
    printf("Anio: %s\n", articulo->anio);
    printf("Resumen: %s\n", articulo->resumen);
    printf("---------------\n");
}





int main(){
    // * Iniciar las estructuras:
    struct Dlista* listaDoblementeEnlazada = crear_Dlista();
    insertar_inicio_Dlista(listaDoblementeEnlazada, 1);
    insertar_inicio_Dlista(listaDoblementeEnlazada, 2);
    insertar_inicio_Dlista(listaDoblementeEnlazada, 3);
    insertar_inicio_Dlista(listaDoblementeEnlazada, 4);
    insertar_inicio_Dlista(listaDoblementeEnlazada, 5);

    // * Prueba:
    imprimir_Dlista(listaDoblementeEnlazada); // / Deberia imprimir: 5, 4, 3, 2, 1 separados por un salto de linea xd

}