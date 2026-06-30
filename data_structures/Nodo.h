#ifndef NODO_H
#define NODO_H

template <class T>
class Nodo {
public:
    T dato;
    Nodo<T>* siguiente;
    Nodo<T>* anterior;

    Nodo(T dato);
};

#include "Nodo.cpp"
#endif