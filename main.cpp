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

    Nodo<Cancion>* actual = registroCanciones.getCabeza();
    while (actual != nullptr)
    {
        std::cout << actual->dato.getId() << ". "
                  << actual->dato.getStringMostrar() << "\n";
       actual = actual->siguiente;
    
    }
    
    return 0;
}
