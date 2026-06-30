#ifndef NODO_CPP
#define NODO_CPP
#include "Nodo.h"

template <class T>
Nodo<T>::Nodo(T dato) : dato(dato), siguiente(nullptr), anterior(nullptr) {}

#endif