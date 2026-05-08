#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include "classes/Cancion.h"
#include "data_structures/ListaDobleEnlazada.h"
#include "core/GestorArchivos.h"
#include "core/ControladorReproductor.h"

void limpiarPantalla() {
    system("clear"); //"clear" ya que uso MacOS
}

    std::string modificadores = "";
    bool tieneS = reproductor.getAleatorioActivado();
    ModoRepeticion modoR = reproductor.getModoRepeticion();
    
    if (tieneS || modoR != REPETICION_APAGADA) {
        modificadores += "(";
        if (tieneS) modificadores += "S";
        if (tieneS && modoR != REPETICION_APAGADA) modificadores += "-";
        if (modoR == REPETICION_UNA) modificadores += "R1";
        if (modoR == REPETICION_TODAS) modificadores += "RA";
        modificadores += ") ";
    }



   if (reproductor.tieneCancionActual()) {
        Cancion actual = reproductor.getCancionActual();
        std::cout << textoEstado << " " << modificadores << ": " << actual.getTitulo() << "\n";
        std::cout << "Artista: " << actual.getArtista() << "\n";
        std::cout << "Album: " << actual.getAlbum() << " [" << actual.getAnio() << "]\n";
    } else {
        std::cout << textoEstado << "\n"; 
    }

    std::cout << "\nOpciones:\n";
    std::cout << "W - Reproducir/Pausar\n";
    std::cout << "Q - Pista Anterior\n";
    std::cout << "E - Pista Siguiente\n";
    std::cout << "S - Activar/Desactivar modo aleatorio\n";
    std::cout << "R - Repeticion (Desactivado/Repetir una/Repetir todas)\n";
    std::cout << "A - Ver lista de reproduccion actual\n";
    std::cout << "L - Listado de canciones\n";
    std::cout << "X - Salir\n";
    std::cout << "\nIngrese Opcion: ";
}

int main() {
    ListaDobleEnlazada<Cancion> registroCanciones;
    GestorArchivos::cargarFuenteMusica("music_source.txt", registroCanciones);
    
    ControladorReproductor reproductor(&registroCanciones);

    bool ejecutando = true;
    std::string entrada;

    while (ejecutando) {
        imprimirMenuPrincipal(reproductor);
        std::cin >> entrada;
        
        char comando = toupper(entrada[0]);

   switch (command) {
            case 'W':
                reproductor.alternarReproduccion();
                break;
            case 'S':
                reproductor.alternarAleatorio();
                break;
            case 'R':
                reproductor.ciclarModoRepeticion();
                break;
            case 'Q':
                reproductor.reproducirAnterior();
                break;
            case 'E':
                reproductor.reproducirSiguiente();
                break;
            case 'A':
                // TODO: Submenu Lista Reproduccion Actual
                break;
            case 'L':
                // TODO: Submenu Listado de Canciones
                break;
            case 'X':
                ejecutando = false;
                // TODO: Faltaria guardar el estado en status.cfg antes de salir
                break;
            default:
                // Si presiona otra tecla, el bucle simplemente limpia la pantalla y vuelve a mostrar el menu
                break;
        }
    }

    limpiarPantalla();
    std::cout << "Saliendo del reproductor. ¡Hasta luego!\n";
    return 0;
}