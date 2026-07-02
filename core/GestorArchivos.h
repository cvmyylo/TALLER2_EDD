#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <string>
#include "../data_structures/ListaDobleEnlazada.h"
#include "../classes/Cancion.h"

class GestorArchivos {
public:
    static void cargarFuenteMusica(const std::string& nombreArchivo, ListaDobleEnlazada<Cancion>& listaCanciones);
    static void guardarFuenteMusica(const std::string& nombreArchivo, ListaDobleEnlazada<Cancion>& listaCanciones);
    
    static void cargarRankings(const std::string& nombreArchivo, ListaDobleEnlazada<Cancion>& listaCanciones);
    static void guardarRankings(const std::string& nombreArchivo, ListaDobleEnlazada<Cancion>& listaCanciones);
};
#endif