#ifndef MAXHEAPARTISTAS_H
#define MAXHEAPARTISTAS_H

#include "../classes/Cancion.h"
#include "ListaDobleEnlazada.h"
#include <string>

// Estructura auxiliar para agrupar los datos del artista
struct ArtistaStats {
    std::string nombre;
    int totalReproducciones;
    ListaDobleEnlazada<Cancion*> cancionesDelArtista; 

    ArtistaStats(std::string nom);
};

class MaxHeapArtistas {
private:
    ArtistaStats** arreglo;
    int capacidad;
    int tamanoActual;

    int padre(int i);
    int hijoIzquierdo(int i);
    int hijoDerecho(int i);

    bool esMayorPrioridad(ArtistaStats* a1, ArtistaStats* a2);
    void intercambiar(int i, int j);
    void flotarArriba(int indice);
    void hundirAbajo(int indice);

public:
    MaxHeapArtistas(int maxCapacidad);
    ~MaxHeapArtistas();

    void insertar(ArtistaStats* a);
    ArtistaStats* extraerMaximo();
    int getTamano() const;
};

#endif