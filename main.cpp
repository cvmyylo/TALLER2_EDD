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

void imprimirMenuPrincipal(ControladorReproductor& reproductor) {
    limpiarPantalla();
    
    std::string textoEstado = "Reproduccion Detenida";
    if (reproductor.tieneCancionActual()) {
        textoEstado = reproductor.getEstaReproduciendo() ? "Reproduciendo" : "En pausa";
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
        std::cout << "Album: " << actual.getAlbum() << " [" << actual.getAño() << "]\n";
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

void manejarMenuListadoGlobal(ControladorReproductor& reproductor) {
    bool enSubmenu = true;
    std::string entrada;

        while (enSubmenu) {
        limpiarPantalla();
        reproductor.mostrarMenuListaActual();
        
        std::cout << "\nOpciones:\n";
        std::cout << "R<num> - Reproducir cancion inmediatamente\n";
        std::cout << "A<num> - Agregar cancion a la cola de reproduccion\n";
        std::cout << "V - Volver al menu principal\n";
        std::cout << "\nIngrese Opcion: ";
        
        std::cin >> entrada;
        char comando = toupper(entrada[0]);

            if (comando == 'V') {
            enSubmenu = false; 
        } else if ((comando == 'R' || comando == 'A') && entrada.length() > 1) {
            try {
                std::string parteNumero = entrada.substr(1);
                int indice = std::stoi(parteNumero); 
                reproductor.saltarACancion(indice);
                enSubmenu = false; 
            } catch (...) {
            }
        }
    }
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

   switch (comando) {
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
                reproductor.mostrarMenuListaActual();
                break;
            case 'L':
                // Submenu Listado de Canciones
                break;
            case 'X':
                ejecutando = false;
                // Faltaria guardar el estado en status.cfg antes de salir
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