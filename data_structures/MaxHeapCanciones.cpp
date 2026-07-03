#include "MaxHeapCanciones.h"

MaxHeapCanciones::MaxHeapCanciones(int maxCapacidad) {
    capacidad = maxCapacidad;
    tamanoActual = 0;
    arreglo = new Cancion*[capacidad];
}

MaxHeapCanciones::~MaxHeapCanciones() {
    delete[] arreglo; 
}

int MaxHeapCanciones::padre(int i) { return (i - 1) / 2; }
int MaxHeapCanciones::hijoIzquierdo(int i) { return (2 * i) + 1; }
int MaxHeapCanciones::hijoDerecho(int i) { return (2 * i) + 2; }

// Regla del enunciado: 1° Reproducciones, 2° Alfabetico Cancion, 3° Alfabetico Artista
bool MaxHeapCanciones::esMayorPrioridad(Cancion* c1, Cancion* c2) {
    if (c1->getCantidadReproducciones() != c2->getCantidadReproducciones()) {
        return c1->getCantidadReproducciones() > c2->getCantidadReproducciones();
    }
    if (c1->getTitulo() != c2->getTitulo()) {
        return c1->getTitulo() < c2->getTitulo(); 
    }
    return c1->getArtista() < c2->getArtista();
}

void MaxHeapCanciones::intercambiar(int i, int j) {
    Cancion* temp = arreglo[i];
    arreglo[i] = arreglo[j];
    arreglo[j] = temp;
}

void MaxHeapCanciones::flotarArriba(int indice) {
    while (indice != 0 && esMayorPrioridad(arreglo[indice], arreglo[padre(indice)])) {
        intercambiar(indice, padre(indice));
        indice = padre(indice);
    }
}

void MaxHeapCanciones::hundirAbajo(int indice) {
    int prioridadMaxima = indice;
    int izq = hijoIzquierdo(indice);
    int der = hijoDerecho(indice);

    if (izq < tamanoActual && esMayorPrioridad(arreglo[izq], arreglo[prioridadMaxima])) {
        prioridadMaxima = izq;
    }
    if (der < tamanoActual && esMayorPrioridad(arreglo[der], arreglo[prioridadMaxima])) {
        prioridadMaxima = der;
    }

    if (prioridadMaxima != indice) {
        intercambiar(indice, prioridadMaxima);
        hundirAbajo(prioridadMaxima);
    }
}

void MaxHeapCanciones::insertar(Cancion* c) {
    if (tamanoActual == capacidad) return;
    arreglo[tamanoActual] = c;
    flotarArriba(tamanoActual);
    tamanoActual++;
}

Cancion* MaxHeapCanciones::extraerMaximo() {
    if (tamanoActual <= 0) return nullptr;
    if (tamanoActual == 1) {
        tamanoActual--;
        return arreglo[0];
    }

    Cancion* raiz = arreglo[0];
    arreglo[0] = arreglo[tamanoActual - 1];
    tamanoActual--;
    hundirAbajo(0);

    return raiz;
}

int MaxHeapCanciones::getTamano() const {
    return tamanoActual;
}