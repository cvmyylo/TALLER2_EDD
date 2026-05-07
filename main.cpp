#include <iostream>
#include "classes/Cancion.h"
#include "data_structures/ListaDobleEnlazada.h"
#include "core/GestorArchivos.h"

int main() {

    ListaDobleEnlazada<Cancion> registroCanciones;

    std::cout << "Iniciando reproductos... \n";
    GestorArchivos::cargarFuenteMusica("music_source.txt", registroCanciones);


    std::cout << "Se han cargado " << registroCanciones.getTamaño() << " canciones en el registro.\n";
    std::cout << "------------------------------------------\n";

    Nodo<Cancion>* current = registroCanciones.getCabeza();
    while (current != nullptr)
    {
        std::cout << current->dato.getId() << ". "
                  << current->dato.getStringMostrar() << "\n";
    }   current = current->siguiente;
    
    return 0;
}
