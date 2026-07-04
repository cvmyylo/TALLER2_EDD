#ifndef ARBOLTRIE_H
#define ARBOLTRIE_H
#include "../classes/Cancion.h"
#include "ListaDobleEnlazada.h"
class NodoTrie {
public:
    char caracter;
    bool finPalabra;
    ListaDobleEnlazada<Cancion*> canciones;
    NodoTrie* hijos[128];
    NodoTrie(char c);
    ~NodoTrie();
};
#endif