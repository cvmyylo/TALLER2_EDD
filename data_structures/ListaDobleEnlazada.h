#ifndef LISTADOBLEENLAZADA_H
#define LISTADOBLEENLAZADA_H

#include "Nodo.h"

template <class T>
class ListaDobleEnlazada {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int tamano;

public:
    ListaDobleEnlazada();
    ~ListaDobleEnlazada();

    void agregarAlFinal(T dato);
    void limpiar();
    bool eliminarPorId(int idObjetivo); 

    int getTamano() const;
    bool estaVacia() const;
    Nodo<T>* getCabeza() const;
};

#include "ListaDobleEnlazada.cpp"
#endif