#include "../proyecto_poo/src/include/Util.h"
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "../include/ParejaPalabraImagen.h"

// Implementación de mezclarArreglo y seleccionarPalabrasEImagenesAleatorias

void mezclarArreglo(int *arreglo, int tamaño) {
    for (int i = tamaño - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arreglo[i];
        arreglo[i] = arreglo[j];
        arreglo[j] = temp;
    }
}

void seleccionarPalabrasEImagenesAleatorias(ParejaPalabraImagen *arregloFuente, int tamañoFuente, ParejaPalabraImagen *arregloObjetivo, int tamañoObjetivo) {
    int *indices = (int *)malloc(tamañoFuente * sizeof(int));
    for (int i = 0; i < tamañoFuente; i++) indices[i] = i;
    mezclarArreglo(indices, tamañoFuente);
    for (int i = 0; i < tamañoObjetivo; i++) arregloObjetivo[i] = arregloFuente[indices[i]];
    free(indices);
}
