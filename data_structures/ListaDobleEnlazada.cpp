#ifndef LISTADOBLEENLAZADA_CPP
#define LISTADOBLEENLAZADA_CPP
#include "ListaDobleEnlazada.h"

template <class T>
ListaDobleEnlazada<T>::ListaDobleEnlazada() : cabeza(nullptr), cola(nullptr), tamano(0) {}

template <class T>
ListaDobleEnlazada<T>::~ListaDobleEnlazada() { limpiar(); }

template <class T>
void ListaDobleEnlazada<T>::agregarAlFinal(T dato) {
    Nodo<T>* nuevoNodo = new Nodo<T>(dato);
    if (cabeza == nullptr) {
        cabeza = cola = nuevoNodo;
    } else {
        cola->siguiente = nuevoNodo;
        nuevoNodo->anterior = cola;
        cola = nuevoNodo;
    }
    tamano++;
}

template <class T>
void ListaDobleEnlazada<T>::limpiar() {
    Nodo<T>* actual = cabeza;
    while (actual != nullptr) {
        Nodo<T>* nodoSiguiente = actual->siguiente;
        delete actual;
        actual = nodoSiguiente;
    }
    cabeza = nullptr;
    cola = nullptr;
    tamano = 0;
}

template <class T>
bool ListaDobleEnlazada<T>::eliminarPorId(int idObjetivo) {
    Nodo<T>* actual = cabeza;
    while (actual != nullptr) {
        if (actual->dato.getId() == idObjetivo) {
            if (actual->anterior != nullptr) actual->anterior->siguiente = actual->siguiente;
            else cabeza = actual->siguiente; 
            
            if (actual->siguiente != nullptr) actual->siguiente->anterior = actual->anterior;
            else cola = actual->anterior; 
            
            delete actual;
            tamano--;
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

template <class T>
int ListaDobleEnlazada<T>::getTamano() const { return tamano; }

template <class T>
bool ListaDobleEnlazada<T>::estaVacia() const { return tamano == 0; }

template <class T>
Nodo<T>* ListaDobleEnlazada<T>::getCabeza() const { return cabeza; }

#endif