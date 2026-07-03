#ifndef MAXHEAPCANCIONES_H
#define MAXHEAPCANCIONES_H

#include "../classes/Cancion.h"

class MaxHeapCanciones {
private:
    Cancion** arreglo; 
    int capacidad;
    int tamanoActual;

    int padre(int i);
    int hijoIzquierdo(int i);
    int hijoDerecho(int i);

    bool esMayorPrioridad(Cancion* c1, Cancion* c2);
    void intercambiar(int i, int j);
    void flotarArriba(int indice);
    void hundirAbajo(int indice);

public:
    MaxHeapCanciones(int maxCapacidad);
    ~MaxHeapCanciones();

    void insertar(Cancion* c);
    Cancion* extraerMaximo();
    int getTamano() const;
};

#endif