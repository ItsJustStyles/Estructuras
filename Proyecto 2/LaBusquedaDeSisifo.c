//Zona de imports necesarios para el programa:
#include <stdio.h>
#include <stdlib.h>

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

struct Heap {
    int *array;
    int size;
    int capacity;
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

// Crear heap
struct Heap* crear_heap(int capacidad) {
    struct Heap* heap = calloc(1, sizeof(struct Heap));
    if(heap == NULL) {
        return NULL;
    }

    heap->array = calloc(capacidad, sizeof(int));
    if(heap->array == NULL) {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = capacidad;

    return heap;
}

// Función para intercambiar valores
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Mantiene la propiedad del heap máximo
void heapify(struct Heap* heap, int i) {
    int largest = i;
    int left  = 2 * i + 1;
    int right = 2 * i + 2;

    if(left < heap->size && heap->array[left] > heap->array[largest]) {
        largest = left;
    }

    if(right < heap->size && heap->array[right] > heap->array[largest]) {
        largest = right;
    }

    if(largest != i) {
        swap(&heap->array[i], &heap->array[largest]);
        heapify(heap, largest);
    }
}

// Insertar un valor en el heap
int insertar_heap(struct Heap* heap, int valor) {
    if(heap == NULL) return -1;

    if(heap->size == heap->capacity) {
        return -1; // Overflow
    }

    int i = heap->size;
    heap->array[i] = valor;
    heap->size++;

    // Subir para mantener el heap máximo
    while(i != 0 && heap->array[(i - 1) / 2] < heap->array[i]) {
        swap(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    return 0;
}

// Extraer el valor máximo (raíz del heap)
int extraer_max(struct Heap* heap) {
    if(heap == NULL || heap->size <= 0) {
        return -999999; // Error según tu estilo
    }

    if(heap->size == 1) {
        heap->size--;
        return heap->array[0];
    }

    int root = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;

    heapify(heap, 0);

    return root;
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


void liberar_heap(struct Heap* heap) {
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

void imprimir_heap(struct Heap* heap) {
    if(heap == NULL) return;

    for(int i = 0; i < heap->size; i++) {
        printf("%d ", heap->array[i]);
    }
    printf("\n");
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