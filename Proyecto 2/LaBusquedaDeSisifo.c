//Zona de imports necesarios para el programa:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// - Funciones para manejar el ordenamiento de los articulos:

int contar_palabras(char *texto){

    if(texto == NULL){
        return 0;
    }

    int estado = 0;
    int contadorPalabras = 0;
    int i = 0;

    while(texto[i] != '\0'){

        if(texto[i] == ' ' || texto[i] == '\t' || texto[i] == '\n'){
            estado = 0;
        }

        else if(estado == 0){
            estado = 1;
            contadorPalabras++;
        }

        i++;

    }

    return contadorPalabras;

}

// - Definiciones de  estructuras de datos:

// Lista doblemente enlazada:

//Define la estructura que tendra un articulo
//Esta definición funciona para los 3 heaps a implementar
struct Articulo {
    char *nombre_autor;   
    char *apellidos_autor; 
    char *titulo;          
    char *ruta;            
    int anio;             
    char *resumen;         
};

struct Heap{
    struct Articulo *array;
    int capacidad;
    int tamano;
};


//Funciones para el manejo de la lista enlazada:

struct Heap* crear_heap(int capacidad){

    struct Heap* nuevo_heap = calloc(1, sizeof(struct Heap));
    if(nuevo_heap == NULL){
        return NULL;
    }

    nuevo_heap -> capacidad = capacidad;
    nuevo_heap -> tamano = 0;

    nuevo_heap -> array = (struct Articulo*)calloc(1, capacidad * sizeof(struct Articulo)); // / Se usa el casting para que el puntero generico que devuelve calloc se transforme al valor que espera el array (Articulo en este caso) xd

    if(nuevo_heap -> array == NULL){
        free(nuevo_heap);
        return NULL;
    }

    return nuevo_heap;

}

// / Aqui se insertan los articulos dependiendo del ordenamiento de cada uno en el heap que le corresponda:

int insertar_heap_CantPalabras(struct Heap* heap, struct Articulo articulo){
    if(heap -> tamano == heap -> capacidad){
        return -1;
    }

    heap -> tamano++;
    int i = heap -> tamano - 1;
    heap -> array[i] = articulo;

    //todo: Más adelante se puede imprementar que en el struct Articulo se guarde la cantidad de palabras que tiene el titulo para simplificar esto:
    int palabras_hijo = contar_palabras(heap -> array[i].titulo);

    while(i != 0){
        int indice_padre = (i-1)/2;
        int palabras_padre = contar_palabras(heap -> array[indice_padre].titulo);

        if(palabras_hijo < palabras_padre){
            struct Articulo temp = heap -> array[i];
            heap -> array[i] = heap -> array[indice_padre];
            heap -> array[indice_padre] = temp;

            i = indice_padre;
        }else{
            break;
        }

    }
    return 0;
    
}

struct Articulo extraerArticulo_menor(struct Heap* heap){
    if (heap == NULL || heap->tamano == 0) {

    }
    //todo: Falta poner lo de eliminar y burbujear si es que se implementa con borrar el min (Creo que hay que hacerlo porque es la unica manera de obtener el min)
    return heap -> array[0];
}

// - Funciones para manejo de memoria:

int liberar_heap(struct Heap* heap){
    if(heap == NULL){
        return 0;
    }

    if(heap -> array != NULL){
        for(int i = 0; i < heap->tamano; i++){
            if(heap->array[i].nombre_autor) free(heap->array[i].nombre_autor);
            if(heap->array[i].apellidos_autor) free(heap->array[i].apellidos_autor);
            if(heap->array[i].titulo) free(heap->array[i].titulo);
            if(heap->array[i].ruta) free(heap->array[i].ruta);
            if(heap->array[i].resumen) free(heap->array[i].resumen);
        }
        free(heap -> array);
    }

    free(heap);
    return 0;

}

// - Funciones para el manejo de archivos:

void split(char *linea, char *campos[], int *numCampos, const char *sep){

    char *token = strtok(linea, sep);
    int i = 0;

    while(token != NULL){
        campos[i++] = token;
        token = strtok(NULL, sep);
    }

    *numCampos = i;

}

int cargarIndice(struct Articulo lista[], int MAXARTICULOS, const char *ARCHIVO){

    FILE *f = fopen(ARCHIVO, "r");
    if(!f){
        return -1;
    }

    char linea[2000];
    int cantidad = 0;

    while(fgets(linea, sizeof(linea), f) != NULL && cantidad < MAXARTICULOS){
        linea[strcspn(linea, "\n")] = 0;

        char *campos[10];
        int numCampos = 0;

        split(linea, campos, &numCampos, "|");
        if(numCampos < 0){
            continue;
        }

            lista[cantidad].nombre_autor = strdup(campos[0]);
            lista[cantidad].apellidos_autor = strdup(campos[1]);
            lista[cantidad].titulo = strdup(campos[2]);
            lista[cantidad].ruta = strdup(campos[3]);
            lista[cantidad].anio = atoi(campos[4]); 
            lista[cantidad].resumen = strdup(campos[5]);

            cantidad++;  
    }
    fclose(f);
    return cantidad;

}

// - Funciones para realizar debugging:

void imprimir_heap(struct Heap* heap){
    for(int i = 0; i < heap -> tamano; i++){
        printf("Autor: %s\n", heap->array[i].nombre_autor);
        printf("Apellidos autor: %s\n", heap->array[i].apellidos_autor);
        printf("Titulo: %s\n", heap->array[i].titulo);
        printf("Ruta: %s\n", heap->array[i].ruta);
        printf("Año: %d\n", heap->array[i].anio);
        printf("Resumen: %s\n", heap->array[i].resumen);
        printf("\n");
    }
}

void imprimir_articulo(struct Articulo articulo){
    printf("Autor: %s\n", articulo.nombre_autor);
    printf("Apellidos autor: %s\n", articulo.apellidos_autor);
    printf("Titulo: %s\n", articulo.titulo);
    printf("Ruta: %s\n", articulo.ruta);
    printf("Año: %d\n", articulo.anio);
    printf("Resumen: %s\n", articulo.resumen);
    printf("\n");
}

// - Función principal:

int main(){
    // * Variables ara los archivos:
    int MAXARTICULOS = 10;
    const char *ARCHIVO = "Documentos/Archivos.txt";

    // * Iniciar las estructuras:

    struct Heap* heap = crear_heap(MAXARTICULOS);

    // Se cargan los archivos
    struct Articulo listaArticulos[MAXARTICULOS]; //todo: Más adelante se puede cambiar por una lista simple
    int indicesCargados = cargarIndice(listaArticulos, MAXARTICULOS, ARCHIVO);

    //Añadir al heap:
    if(indicesCargados <= 0){
        printf("XD");
        return -1;
    }

    for(int i = 0; i < indicesCargados; i++){
        insertar_heap_CantPalabras(heap,listaArticulos[i]);
    }

    int opcion;
    do{
        printf("\n========== MENU DE ORDENAMIENTOS ==========\n");
        printf("1. Ordenar por TÍTULO\n");
        printf("2. Ordenar por TAMAÑO DEL TÍTULO\n");
        printf("3. Ordenar por NOMBRE DE ARCHIVO\n");
        printf("4. Ordenar por PALABRA\n");
        printf("5. SALIR\n");
        printf("===========================================\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch(opcion){
            case 2:
                struct Articulo articuloMenor = extraerArticulo_menor(heap);
                printf("\n"); //todo: Añade un espacio en la parte de arriba del primer articulo para que se vea mejor xd
                imprimir_articulo(articuloMenor);
                break;
            case 5:
                printf("Saliendo...\n");
                break;
            default:
                printf("\nOpción no válida. Intente de nuevo.\n");
                continue;
        }

    }while(opcion != 5);

    // * Liberar la memoria usada
    liberar_heap(heap);
}