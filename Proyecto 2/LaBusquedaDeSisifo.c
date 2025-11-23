//Zona de imports necesarios para el programa:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// - Funciones para manejar el ordenamiento de los articulos:

char* normalizar_texto(const char* titulo_original){

    if(titulo_original == NULL){
        printf("A");
        return NULL;
    }

    size_t len = strlen(titulo_original);

    char *titulo_normalizado = (char*)calloc(len + 1, sizeof(char));

    if(titulo_normalizado == NULL){
        return NULL;
    }

    int j = 0;
    int i = 0;
    while(titulo_original[i] != '\0'){
        unsigned char c = titulo_original[i];
        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ' || c == ','){

            titulo_normalizado[j++] = tolower(c);
            i++;

        }else{

            if(c == 0xC3){ // ? Corresponde al valor 'identificador' de las letras multibye en UTF - 8

                unsigned char c2 = titulo_original[i + 1];
                bool multiByte = false;

                switch (c2)
                {
                case 0x80: case 0x81: case 0x84: case 0xa1:
                    titulo_normalizado[j++] = 'a';
                    multiByte = true;
                    break;
                case 0x88: case 0x89: case 0x8b: case 0xa9:
                    titulo_normalizado[j++] = 'e';
                    multiByte = true;
                    break;
                case 0x8d: case 0x8f: case 0xad:
                    titulo_normalizado[j++] = 'i';
                    multiByte = true;
                    break;
                case 0x93: case 0x92: case 0xb3: case 0xb2:
                    titulo_normalizado[j++] = 'o';
                    multiByte = true;
                    break;
                case 0x9a: case 0x99: case 0xba: case 0xb9:
                    titulo_normalizado[j++] = 'u';
                    multiByte = true;
                    break;
                case 0x91: case 0xb1:
                    titulo_normalizado[j++] = 'n'; 
                    multiByte = true;
                    break;
                default:
                    multiByte = false;
                    break;
                }

                if(multiByte){
                    i += 2;
                }else{
                    i++;
                }

            }else{
                i++;
            }
        }

    }

    titulo_normalizado[j] = '\0';
    //printf("%s\n", titulo_normalizado);
    return titulo_normalizado;
}

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

int contar_ocurrencias(const char *texto, const char *palabra) {
    if (!texto || !palabra) return 0;

    int count = 0;
    size_t len = strlen(palabra);

    char *texto_normalizado = normalizar_texto(texto);
    const char *p = texto_normalizado;

    while ((p = strstr(p, palabra)) != NULL) {
        count++;
        p += len;
    }

    if(texto_normalizado != NULL){
        free(texto_normalizado);
    }

    return count;
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

// Los heaps
struct Heap{
    struct Articulo *array;
    int capacidad;
    int tamano;
};

struct Heap* crear_heap(int capacidad){

    struct Heap* nuevo_heap = calloc(1, sizeof(struct Heap));
    if(nuevo_heap == NULL){
        return NULL;
    }

    nuevo_heap -> capacidad = capacidad;
    nuevo_heap -> tamano = 0;

    nuevo_heap -> array = (struct Articulo*)calloc(1, capacidad * sizeof(struct Articulo)); // * Se usa el casting para que el puntero generico que devuelve calloc se transforme al valor que espera el array (Articulo en este caso) xd

    if(nuevo_heap -> array == NULL){
        free(nuevo_heap);
        return NULL;
    }

    return nuevo_heap;

}

//* Función auxiliar para intercambiar Articulos en el heap
void intercambiar(struct Articulo *a, struct Articulo *b) {
    struct Articulo temp = *a;
    *a = *b;
    *b = temp;
}

//* Función para imprimir articulos
void imprimir_articulo(struct Articulo articulo){
    printf("Autor: %s\n", articulo.nombre_autor);
    printf("Apellidos autor: %s\n", articulo.apellidos_autor);
    printf("Titulo: %s\n", articulo.titulo);
    printf("Ruta: %s\n", articulo.ruta);
    printf("Año: %d\n", articulo.anio);
    printf("Resumen: %s\n", articulo.resumen);
    printf("\n");
}

// / Aqui se insertan los articulos dependiendo del ordenamiento de cada uno en el heap que le corresponda: Por tamano del titulo

int insertar_heap_tamanoTitulo(struct Heap* heap, struct Articulo articulo, int orden){
    if(heap -> tamano == heap -> capacidad){
        return -1;
    }

    heap -> tamano++;
    int i = heap -> tamano - 1;
    heap -> array[i] = articulo;

    //todo: Más adelante se puede imprementar que en el struct Articulo se guarde la cantidad de palabras que tiene el titulo para simplificar esto:
    int palabras_hijo = contar_palabras(heap -> array[i].titulo);

    if(orden == 1){

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
    }else{

        while(i != 0){
            int indice_padre = (i-1)/2;
            int palabras_padre = contar_palabras(heap -> array[indice_padre].titulo);

            if(palabras_hijo > palabras_padre){
                struct Articulo temp = heap -> array[i];
                heap -> array[i] = heap -> array[indice_padre];
                heap -> array[indice_padre] = temp;

                i = indice_padre;
            }else{
                break;
            }

        }
    }

    return 0;
    
}

void heapify_tamanoTitulo(struct Heap* heap, int i, int orden){
    int menor = i;
    int izq = i * 2 + 1;
    int der = i * 2 + 2;

    bool es_min_heap = (orden == 1);

    if(izq < heap -> tamano){
        int palabras_izq = contar_palabras(heap -> array[izq].titulo);
        int palabras_menor = contar_palabras(heap -> array[menor].titulo);
        if ((es_min_heap && palabras_izq < palabras_menor) || (!es_min_heap && palabras_izq > palabras_menor)) {
            menor = izq;
        }
    }

    if(der < heap -> tamano){
        int palabras_der = contar_palabras(heap -> array[der].titulo);
        int palabras_menor = contar_palabras(heap -> array[menor].titulo);
        if ((es_min_heap && palabras_der < palabras_menor) || (!es_min_heap && palabras_der > palabras_menor)) {
            menor = der;
        }
    }

    if(menor != i){
        intercambiar(&heap->array[i], &heap->array[menor]);
        heapify_tamanoTitulo(heap, menor, orden);
    }

}

struct Articulo extraerHeap_tamanoTitulo(struct Heap* heap, int orden){
    struct Articulo vacio = {0};
    if (heap == NULL || heap->tamano == 0) {
        return vacio;
    }
    
    struct Articulo min_articulo = heap->array[0];

    intercambiar(&heap->array[0], &heap->array[heap->tamano - 1]);

    heap->tamano--;

    if (heap->tamano > 0) {
        heapify_tamanoTitulo(heap, 0, orden); 
    }

    return min_articulo;
}


void heapSortPorTamanoTitulo(struct Articulo lista[], int indicesCargados, struct Heap* heap, int orden, int cantidad){
    heap->tamano = 0;

    for(int i = 0; i < indicesCargados; i++){
        insertar_heap_tamanoTitulo(heap, lista[i], orden);
    }

    int mostrar = (cantidad < indicesCargados) ? cantidad : indicesCargados;
    for(int i = 0; i < mostrar; i++){
        struct Articulo articulo = extraerHeap_tamanoTitulo(heap, orden);
        imprimir_articulo(articulo);
    }
}

// / Por titulo (Alfabeticamente)

int insertar_heap_titulo(struct Heap* heap, struct Articulo articulo, int orden) {
    if (heap->tamano == heap->capacidad) {
        return -1;
    }

    int i = heap->tamano;
    heap->array[i] = articulo;
    heap->tamano++;

    while (i != 0) {
        int padre = (i - 1) / 2;

        char *titulo_hijo = normalizar_texto(heap -> array[i].titulo);
        char *titulo_padre = normalizar_texto(heap -> array[padre].titulo);

        int cmp = 0;
        if(titulo_hijo != NULL && titulo_padre != NULL){
            cmp = strcmp(titulo_hijo, titulo_padre);
        }

        if(titulo_hijo != NULL) free(titulo_hijo);
        if(titulo_padre != NULL) free(titulo_padre);

        if ((orden == 1 && cmp < 0) || (orden == 2 && cmp > 0)) {
            intercambiar(&heap->array[i], &heap->array[padre]);
            i = padre;
        } else {
            break;
        }
    }

    return 0;
}

void heapify_titulo(struct Heap* heap, int i, int orden) {
    int mejor = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;

    if (izq < heap->tamano) {
        char *titulo_izq = normalizar_texto(heap->array[izq].titulo);
        char *titulo_mejor = normalizar_texto(heap->array[mejor].titulo);
        
        int cmp = 0;
        if (titulo_izq != NULL && titulo_mejor != NULL) {
            cmp = strcmp(titulo_izq, titulo_mejor);
        }

        if ((orden == 1 && cmp < 0) || (orden == 2 && cmp > 0)) {
            mejor = izq;
        }

        if (titulo_izq != NULL) free(titulo_izq);
        if (titulo_mejor != NULL) free(titulo_mejor);
    }

    if (der < heap->tamano) {
        char *titulo_der = normalizar_texto(heap->array[der].titulo);
        char *titulo_mejor = normalizar_texto(heap->array[mejor].titulo);

        int cmp = 0;
        if (titulo_der != NULL && titulo_mejor != NULL) {
            cmp = strcmp(titulo_der, titulo_mejor);
        }

        if ((orden == 1 && cmp < 0) || (orden == 2 && cmp > 0)) {
            mejor = der;
        }

        if (titulo_der != NULL) free(titulo_der);
        if (titulo_mejor != NULL) free(titulo_mejor);
    }

    if (mejor != i) {
        intercambiar(&heap->array[i], &heap->array[mejor]);
        heapify_titulo(heap, mejor, orden);
    }
}


struct Articulo extraer_heap_titulo(struct Heap* heap, int orden) {
    if (heap->tamano == 0) {
        struct Articulo vacio = {0};
        return vacio;
    }

    struct Articulo top = heap->array[0];
    heap->array[0] = heap->array[heap->tamano - 1];
    heap->tamano--;

    heapify_titulo(heap, 0, orden);

    return top;
}

void heapSortPorTitulo(struct Articulo lista[], int n, struct Heap* heap, int orden, int cantidad) {
    heap->tamano = 0;

    for (int i = 0; i < n; i++) {
        insertar_heap_titulo(heap, lista[i], orden);
    }

    int mostrar = (cantidad < n) ? cantidad : n;
    for (int i = 0; i < mostrar; i++) {
        struct Articulo art = extraer_heap_titulo(heap, orden);
        imprimir_articulo(art);
    }
}

// / Por nombre de archivo:

int insertar_heap_nombre_archivo(struct Heap* heap, struct Articulo articulo, int orden){
    if(heap -> tamano == heap -> capacidad){
        return -1;
    }

    int i = heap -> tamano;
    heap -> array[i] = articulo;
    heap -> tamano++;

    while(i != 0){
        int padre = (i - 1)/2;

        char *texto_hijo = normalizar_texto(heap -> array[i].ruta);
        char *texto_padre = normalizar_texto(heap -> array[padre].ruta);

        int cmp = 0;
        if(texto_hijo != NULL && texto_padre != NULL){
            cmp = strcmp(texto_hijo, texto_padre);
        }

        if(texto_hijo != NULL) free(texto_hijo);
        if(texto_padre != NULL) free(texto_padre);

        if((orden == 1 && cmp < 0) || (orden == 2 && cmp > 0)){
            intercambiar(&heap -> array[i], &heap -> array[padre]);
            i = padre;
        }else{
            break;
        }
    }

    return 0;

}

void heapify_nombre_archivo(struct Heap* heap, int i, int orden){
    int mejor = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;

    if(izq < heap->tamano){
        char *texto_izq = normalizar_texto(heap -> array[izq].ruta);
        char *texto_mejor = normalizar_texto(heap -> array[mejor].ruta);

        int cmp = 0;
        if(texto_izq != NULL && texto_mejor != NULL){
            cmp = strcmp(texto_izq, texto_mejor);
        }

        if(texto_izq != NULL) free(texto_izq);
        if(texto_mejor != NULL) free(texto_mejor);

        if ((orden == 1 && cmp < 0) || (orden == 2 && cmp > 0)) {
            mejor = izq;
        }
    }

    if(der < heap->tamano){
        char *texto_der = normalizar_texto(heap -> array[der].ruta);
        char *texto_mejor = normalizar_texto(heap -> array[mejor].ruta);

        int cmp = 0;
        if(texto_mejor != NULL && texto_der != NULL){
            cmp = strcmp(texto_der, texto_mejor);
        }

        if(texto_der != NULL) free(texto_der);
        if(texto_mejor != NULL) free(texto_mejor);

        if ((orden == 1 && cmp < 0) || (orden == 2 && cmp > 0)) {
            mejor = der;
        }
    }

    if(mejor != i){
        intercambiar(&heap -> array[i], &heap -> array[mejor]);
        heapify_nombre_archivo(heap, mejor, orden);
    }
}



struct Articulo extraerHeap_nombre_archivo(struct Heap* heap, int orden){
    if(heap -> tamano == 0){
        struct Articulo vacio = {0};
        return vacio;
    }

    struct Articulo top = heap->array[0];
    heap->array[0] = heap->array[heap->tamano - 1];
    heap->tamano--;

    heapify_nombre_archivo(heap, 0, orden);

    return top;
}

void heapSortPorNombreArchivo(struct Articulo lista[], int indicesCargados, struct Heap* heap, int orden, int cantidad){
    heap->tamano = 0;

    for(int i = 0; i < indicesCargados; i++){
        insertar_heap_nombre_archivo(heap, lista[i], orden);
    }

    int mostrar = (cantidad < indicesCargados) ? cantidad : indicesCargados;
    for(int i = 0; i < mostrar; i++){
        struct Articulo articuloExtraido = extraerHeap_nombre_archivo(heap, orden);
        imprimir_articulo(articuloExtraido);
    }
}


// / Por palabra:

int insertar_heap_palabra(struct Heap* heap, struct Articulo articulo, const char *palabra, int orden) {
    if (heap->tamano == heap->capacidad) return -1;

    int i = heap->tamano;
    heap->array[i] = articulo;
    heap->tamano++;

    int occ_hijo = contar_ocurrencias(heap->array[i].resumen, palabra);

    while (i != 0) {
        int padre = (i - 1) / 2;

        int occ_padre = contar_ocurrencias(heap->array[padre].resumen, palabra);

        
        if ((orden == 1 && occ_hijo < occ_padre) ||
            (orden == 2 && occ_hijo > occ_padre)) {

            intercambiar(&heap->array[i], &heap->array[padre]);
            i = padre;

        } else break;
    }

    return 0;
}

void heapify_palabra(struct Heap* heap, int i, const char *palabra, int orden) {

    int mejor = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;

    int occ_mejor = contar_ocurrencias(heap->array[mejor].resumen, palabra);

    if (izq < heap->tamano) {
        int occ_izq = contar_ocurrencias(heap->array[izq].resumen, palabra);
        if ((orden == 1 && occ_izq < occ_mejor) ||
            (orden == 2 && occ_izq > occ_mejor)) {

            mejor = izq;
            occ_mejor = occ_izq;
        }
    }

    if (der < heap->tamano) {
        int occ_der = contar_ocurrencias(heap->array[der].resumen, palabra);
        if ((orden == 1 && occ_der < occ_mejor) ||
            (orden == 2 && occ_der > occ_mejor)) {

            mejor = der;
        }
    }

    if (mejor != i) {
        intercambiar(&heap->array[i], &heap->array[mejor]);
        heapify_palabra(heap, mejor, palabra, orden);
    }
}

struct Articulo extraerHeap_palabra(struct Heap* heap, const char *palabra, int orden) {
    if (heap->tamano == 0) {
        struct Articulo vacio = {0};
        return vacio;
    }

    struct Articulo top = heap->array[0];
    heap->array[0] = heap->array[heap->tamano - 1];
    heap->tamano--;

    heapify_palabra(heap, 0, palabra, orden);

    return top;
}

void heapSortPorPalabra(struct Articulo lista[], int n, struct Heap* heap, const char *palabra, int orden, int cantidad) {
    heap->tamano = 0;
    char *palabra_normalizada = normalizar_texto(palabra);

    for (int i = 0; i < n; i++) {
        insertar_heap_palabra(heap, lista[i], palabra_normalizada, orden);
    }

    printf("\nORDENANDO POR PALABRA: \"%s\"\n\n", palabra);

    int mostrar = (cantidad < n) ? cantidad : n;
    for (int i = 0; i < mostrar; i++) {
        struct Articulo art = extraerHeap_palabra(heap, palabra_normalizada, orden);
        imprimir_articulo(art);
    }

    if(palabra_normalizada != NULL){
        free(palabra_normalizada);
    }
}

//todo: Un ordenamiento por año de los articulos por si es necesario el uso del mismo (Por lo del enredo del grupo xd):

int insertar_heap_year(struct Heap* heap, struct Articulo articulo, int orden){
    if(heap -> tamano == heap -> capacidad){
        return -1;
    }

    int i = heap -> tamano;
    heap -> array[i] = articulo;
    heap -> tamano++;

    int yearHijo = heap -> array[i].anio;
    while(i != 0){
        int padre = (i - 1)/2;
        int yearPadre = heap -> array[padre].anio;

        if((orden == 1 && yearHijo < yearPadre) || (orden == 2 && yearHijo > yearPadre)){
            intercambiar(&heap -> array[i], &heap -> array[padre]);
            i = padre;
        }else{
            break;
        }
    }
    return 0;
}

void heapify_year(struct Heap* heap, int i, int orden){
    int mejor = i;
    int izq = 2 * i + 1;
    int der = 2 * i + 2;

    if(izq < heap -> tamano){
        int yearHijoIzq = heap -> array[izq].anio;
        int yearPadre = heap -> array[mejor].anio;

        if((orden == 1 && yearHijoIzq < yearPadre) || (orden == 2 && yearHijoIzq > yearPadre)){
            mejor = izq;
        }
    }

    if(der < heap -> tamano){
        int yearPadre = heap -> array[mejor].anio;
        int yearHijoDer = heap -> array[der].anio;
        
        if((orden == 1 && yearHijoDer < yearPadre) || (orden == 2 && yearHijoDer > yearPadre)){
            mejor = der;
        }
    }

    if(mejor != i){
        intercambiar(&heap -> array[i], &heap -> array[mejor]);
        heapify_year(heap, mejor, orden);
    }
}


struct Articulo extraer_heap_year(struct Heap* heap, int orden){
    if (heap->tamano == 0) {
        struct Articulo vacio = {0};
        return vacio;
    }

    struct Articulo top = heap -> array[0];

    heap -> array[0] = heap -> array[heap -> tamano - 1];
    heap -> tamano--;
    heapify_year(heap, 0, orden);

    return top;
}

void heapSortPorYear(struct Articulo listaArticulos[],int cantIndices, struct Heap* heap, int orden, int cantidad){
    heap->tamano = 0;

    for(int i = 0; i < cantIndices; i++){
        insertar_heap_year(heap, listaArticulos[i], orden);
    }

    int mostrar = (cantidad < cantIndices) ? cantidad : cantIndices;
    for(int i = 0; i < mostrar; i++){
        struct Articulo art = extraer_heap_year(heap, orden);
        imprimir_articulo(art);
    }
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

// - Función principal:

int main() {
    // * Variables para los archivos:
    int MAXARTICULOS = 70;
    const char *ARCHIVO = "Documentos/Archivos.txt";

    // * Iniciar las estructuras:
    struct Heap* heap = crear_heap(MAXARTICULOS);

    // Se cargan los archivos
    struct Articulo listaArticulos[MAXARTICULOS];
    int indicesCargados = cargarIndice(listaArticulos, MAXARTICULOS, ARCHIVO);

    int opcion, orden, cantidad;
    char palabra[200];

    do {
        printf("\n========== MENU DE ORDENAMIENTOS ==========\n");
        printf("1. Ordenar por TÍTULO\n");
        printf("2. Ordenar por TAMAÑO DEL TÍTULO\n");
        printf("3. Ordenar por NOMBRE DE ARCHIVO\n");
        printf("4. Ordenar por PALABRA\n");
        printf("5. SALIR\n");
        printf("6. Ordenar por AÑO\n");
        printf("===========================================\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        if (opcion == 5) {
            printf("Saliendo...\n");
            break;
        }

        if (opcion < 1 || opcion > 6) {
            printf("\nOpción no válida. Intente de nuevo.\n");
            continue;
        }

        printf("\n");
        printf("1. Ascendente\n");
        printf("2. Descendente\n");
        printf("Seleccione el orden: ");
        scanf("%d", &orden);

        printf("¿Cuántos artículos desea mostrar? ");
        scanf("%d", &cantidad);

        // Validar que la cantidad no sea mayor que los artículos disponibles
        if (cantidad > indicesCargados) {
            cantidad = indicesCargados;
            printf("Mostrando todos los %d artículos disponibles.\n", indicesCargados);
        } else if (cantidad <= 0) {
            cantidad = indicesCargados;
            printf("Cantidad inválida. Mostrando todos los %d artículos.\n", indicesCargados);
        }

        printf("\n");

        switch (opcion) {
            case 1:
                printf("--- ORDENAR POR TÍTULO (%d artículos) ---\n\n", cantidad);
                heapSortPorTitulo(listaArticulos, indicesCargados, heap, orden, cantidad);
                break;

            case 2:
                printf("--- ORDENAR POR TAMAÑO DE TÍTULO (%d artículos) ---\n\n", cantidad);
                heapSortPorTamanoTitulo(listaArticulos, indicesCargados, heap, orden, cantidad);
                break;

            case 3:
                printf("--- ORDENAR POR NOMBRE DE ARCHIVO (%d artículos) ---\n\n", cantidad);
                heapSortPorNombreArchivo(listaArticulos, indicesCargados, heap, orden, cantidad);
                break;

            case 4:
                printf("\n--- ORDENAR POR PALABRA EN RESUMEN ---\n");
                printf("Digite la palabra a buscar: ");
                scanf("%s", palabra);
                printf("Mostrando %d artículos:\n\n", cantidad);
                heapSortPorPalabra(listaArticulos, indicesCargados, heap, palabra, orden, cantidad);
                break;

            case 6:
                printf("--- ORDENAR POR AÑO (%d artículos) ---\n\n", cantidad);
                heapSortPorYear(listaArticulos, indicesCargados, heap, orden, cantidad);
                break;
        }

    } while (opcion != 5);

    // * Liberar la memoria usada
    printf("%d artículos cargados.\n", indicesCargados);
    liberar_heap(heap);

    return 0;
}