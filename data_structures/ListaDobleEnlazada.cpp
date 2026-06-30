#ifndef LISTADOBLEENLAZADA_CPP
#define LISTADOBLEENLAZADA_CPP
#include "ListaDobleEnlazada.h"

template <class T>
ListaDobleEnlazada<T>::ListaDobleEnlazada() : cabeza(nullptr), cola(nullptr), tamaño(0) {}

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
    tamaño++;
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
    tamaño = 0;
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
            tamaño--;
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}

template <class T>
int ListaDobleEnlazada<T>::getTamaño() const { return tamaño; }

template <class T>
bool ListaDobleEnlazada<T>::estaVacia() const { return tamaño == 0; }

template <class T>
Nodo<T>* ListaDobleEnlazada<T>::getCabeza() const { return cabeza; }

#endif