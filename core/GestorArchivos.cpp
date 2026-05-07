#include "GestorArchivos.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string limpiarEspacios(const std::string& str) {
    size_t primero = str.find_first_not_of(' ');
    if (std::string::npos == primero) return str;
    size_t ultimo = str.find_last_not_of(' ');
    return str.substr(primero, (ultimo - primero + 1));
}

void GestorArchivos::cargarFuenteMusica(const std::string& nombreArchivo, ListaDobleEnlazada<Cancion>& listaCanciones) {
    std::ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        std::cerr << "Advertencia: No se encontro " << nombreArchivo << ". El listado iniciara vacio.\n";
        return;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        if (linea.empty()) continue;

        std::stringstream ss(linea);
        std::string idStr, titulo, artista, album, añoStr, durStr, ruta;

        std::getline(ss, idStr, ',');
        std::getline(ss, titulo, ',');
        std::getline(ss, artista, ',');
        std::getline(ss, album, ',');
        std::getline(ss, añoStr, ',');
        std::getline(ss, durStr, ',');
        std::getline(ss, ruta, ',');

        try {
            int id = std::stoi(limpiarEspacios(idStr));
            int año = std::stoi(limpiarEspacios(añoStr));
            int duracion = std::stoi(limpiarEspacios(durStr));

            Cancion nuevaCancion(id, limpiarEspacios(titulo), limpiarEspacios(artista), limpiarEspacios(album), año, duracion, limpiarEspacios(ruta));
            listaCanciones.agregarAlFinal(nuevaCancion);
        } catch (const std::exception& e) {
            std::cerr << "Error al procesar la linea: " << linea << "\n";
        }
    }
    archivo.close();
}