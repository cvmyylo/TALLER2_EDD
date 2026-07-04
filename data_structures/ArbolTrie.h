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
class ArbolTrie {
private:
    NodoTrie* raiz;
    void insertarCadena(const std::string& texto, Cancion* cancion);
    void insertarSufijos(const std::string& texto, Cancion* cancion);
public:
    ArbolTrie();
    ~ArbolTrie();
    void insertarCancion(Cancion* cancion);
    ListaDobleEnlazada<Cancion*> buscar(const std::string& texto);
};
#endif