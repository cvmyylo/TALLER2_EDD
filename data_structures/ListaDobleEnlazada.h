#ifndef LISTADOBLEENLAZADA_H
#define LISTADOBLEENLAZADA_H

#include "Nodo.h"

template <class T>
class ListaDobleEnlazada {
private:
    Nodo<T>* cabeza;
    Nodo<T>* cola;
    int tamaño;

public:
    ListaDobleEnlazada() : cabeza(nullptr), cola(nullptr), tamaño(0) {}

    ~ListaDobleEnlazada() { limpiar(); }

    void agregarAlFinal(T dato) {
        Nodo<T>* nuevoNodo = new Nodo<T>(dato);
        if (cabeza == nullptr) {
            cabeza = cola = nuevoNodo;
        } else {
            cola->siguiente = nuevoNodo;
            nuevoNodo->anterior = cola;
            cola = nuevoNodo;
        }
        tamaño++;
    }

    void limpiar() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* nodoSiguiente = actual->siguiente;
            delete actual;
            actual = nodoSiguiente;
        }
        cabeza = nullptr;
        cola = nullptr;
        tamaño = 0;
    }

    int getTamaño() const { return tamaño; }
    bool estaVacia() const { return tamaño == 0; }
    Nodo<T>* getCabeza() const { return cabeza; }
};

#endif