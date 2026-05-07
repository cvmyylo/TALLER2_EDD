#ifndef GESTORARCHIVOS_H
#define GESTORARCHIVOS_H

#include <string>
#include "../data_structures/ListaDobleEnlazada.h"
#include "../classes/Cancion.h"

class GestorArchivos {
public:
    static void cargarFuenteMusica(const std::string& nombreArchivo, ListaDobleEnlazada<Cancion>& listaCanciones);
};

#endif