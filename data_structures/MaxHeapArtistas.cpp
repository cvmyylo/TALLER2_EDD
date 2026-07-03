#include "MaxHeapArtistas.h"

ArtistaStats::ArtistaStats(std::string nom) {
    nombre = nom;
    totalReproducciones = 0;
}

MaxHeapArtistas::MaxHeapArtistas(int maxCapacidad) {
    capacidad = maxCapacidad;
    tamanoActual = 0;
    arreglo = new ArtistaStats*[capacidad];
}

MaxHeapArtistas::~MaxHeapArtistas() {
    delete[] arreglo; 
}

int MaxHeapArtistas::padre(int i) { return (i - 1) / 2; }
int MaxHeapArtistas::hijoIzquierdo(int i) { return (2 * i) + 1; }
int MaxHeapArtistas::hijoDerecho(int i) { return (2 * i) + 2; }

// Regla del enunciado: 1° Reproducciones Totales, 2° Alfabetico Artista
bool MaxHeapArtistas::esMayorPrioridad(ArtistaStats* a1, ArtistaStats* a2) {
    if (a1->totalReproducciones != a2->totalReproducciones) {
        return a1->totalReproducciones > a2->totalReproducciones;
    }
    return a1->nombre < a2->nombre;
}

void MaxHeapArtistas::intercambiar(int i, int j) {
    ArtistaStats* temp = arreglo[i];
    arreglo[i] = arreglo[j];
    arreglo[j] = temp;
}

void MaxHeapArtistas::flotarArriba(int indice) {
    while (indice != 0 && esMayorPrioridad(arreglo[indice], arreglo[padre(indice)])) {
        intercambiar(indice, padre(indice));
        indice = padre(indice);
    }
}

void MaxHeapArtistas::hundirAbajo(int indice) {
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

void MaxHeapArtistas::insertar(ArtistaStats* a) {
    if (tamanoActual == capacidad) return;
    arreglo[tamanoActual] = a;
    flotarArriba(tamanoActual);
    tamanoActual++;
}

ArtistaStats* MaxHeapArtistas::extraerMaximo() {
    if (tamanoActual <= 0) return nullptr;
    if (tamanoActual == 1) {
        tamanoActual--;
        return arreglo[0];
    }

    ArtistaStats* raiz = arreglo[0];
    arreglo[0] = arreglo[tamanoActual - 1];
    tamanoActual--;
    hundirAbajo(0);

    return raiz;
}

int MaxHeapArtistas::getTamano() const {
    return tamanoActual;
}