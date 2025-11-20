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