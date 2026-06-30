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
    std::cout << "N - Agregar nueva cancion\n"; // <--- AGREGAR ESTO
    std::cout << "D - Eliminar cancion\n";      // <--- AGREGAR ESTO
    std::cout << "X - Salir\n";
    std::cout << "\nIngrese Opcion: ";
}

void manejarMenuLista(ControladorReproductor& reproductor) {
    bool enSubmenu = true;
    std::string entrada;

    while (enSubmenu) {
        limpiarPantalla();
        reproductor.mostrarMenuListaActual(); 
        
        std::cout << "\nOpciones:\n";
        std::cout << "S<num> - Saltar a la cancion seleccionada\n";
        std::cout << "V - Volver al menu principal\n";
        std::cout << "\nIngrese Opcion: ";
        
        std::cin >> entrada;
        char comando = toupper(entrada[0]);
        if (comando == 'V') {
            enSubmenu = false; 
        } else if (comando == 'S' && entrada.length() > 1) {
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

void manejarMenuListadoGlobal(ControladorReproductor& reproductor) {
    bool enSubmenu = true;
    std::string entrada;

        while (enSubmenu) {
        limpiarPantalla();
        reproductor.mostrarListadoGlobal();
        
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
                if (comando == 'R') {
                    reproductor.reproducirDesdeGlobal(indice);
                enSubmenu = false; 
                } else if (comando == 'A') {
                    reproductor.encolarDesdeGlobal(indice);
                }
            } catch (...) {
                std::cout << "Error: Debe ingresar un numero valido despues de la letra.\n";
                std::cout << "Presione Enter para intentar nuevamente...";               
                
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cin.get();
            }
        }
    }
}

void manejarAgregarCancion(ControladorReproductor& reproductor) {
    limpiarPantalla();
    std::string titulo, artista, album, añoStr, duracionStr, ruta;
    
    std::cout << "--- Agregar Nueva Cancion ---\n";
    std::cout << "Titulo: "; std::getline(std::cin, titulo);
    std::cout << "Artista: "; std::getline(std::cin, artista);
    std::cout << "Album: "; std::getline(std::cin, album);
    std::cout << "Año: "; std::getline(std::cin, añoStr);
    std::cout << "Duracion (segundos): "; std::getline(std::cin, duracionStr);
    std::cout << "Ruta del archivo: "; std::getline(std::cin, ruta);

    try {
        int año = std::stoi(añoStr);
        int duracion = std::stoi(duracionStr);
        
        int nuevoId = reproductor.getRegistroGlobal()->getTamaño() + 1;
        Cancion nueva(nuevoId, titulo, artista, album, año, duracion, ruta);
        
        reproductor.agregarCancionGlobal(nueva);
        std::cout << "\nCancion agregada y guardada exitosamente.\n";
    } catch (...) {
        std::cout << "\nError: Datos numericos invalidos. No se guardo la cancion.\n";
    }
    std::cout << "Presione Enter para continuar...";
    std::getline(std::cin, titulo); 
}

void manejarEliminarCancion(ControladorReproductor& reproductor) {
    limpiarPantalla();
    std::string idStr;
    std::cout << "--- Eliminar Cancion ---\n";
    std::cout << "Ingrese el ID de la cancion a eliminar: ";
    std::getline(std::cin, idStr);

    try {
        int id = std::stoi(idStr);
        reproductor.eliminarCancionGlobal(id);
        std::cout << "\nOperacion completada.\n";
    } catch (...) {
        std::cout << "\nError: ID invalido.\n";
    }
    std::cout << "Presione Enter para continuar...";
    std::string pausa; std::getline(std::cin, pausa);
}

int main() {
    ListaDobleEnlazada<Cancion> registroCanciones;
    GestorArchivos::cargarFuenteMusica("music_source.txt", registroCanciones);
    ControladorReproductor reproductor(&registroCanciones);
    reproductor.cargarEstado();

    bool ejecutando = true;
    std::string entrada;

while (ejecutando) {
        imprimirMenuPrincipal(reproductor);
        
        std::getline(std::cin, entrada); 
        if (entrada.empty()) continue;
        
        char comando = toupper(entrada[0]);

        switch (comando) {
            case 'W': reproductor.alternarReproduccion(); break;
            case 'S': reproductor.alternarAleatorio(); break;
            case 'R': reproductor.ciclarModoRepeticion(); break;
            case 'Q': reproductor.reproducirAnterior(); break;
            case 'E': reproductor.reproducirSiguiente(); break;
            case 'A': manejarMenuLista(reproductor); break;
            case 'L': manejarMenuListadoGlobal(reproductor); break;
            case 'N': manejarAgregarCancion(reproductor); break; 
            case 'D': manejarEliminarCancion(reproductor); break; 
            case 'X': 
                reproductor.guardarEstado();
                ejecutando = false; 
                break;
            default: break;
        }
    }

    limpiarPantalla();
    std::cout << "Saliendo del reproductor. ¡Hasta luego!\n";
    return 0;
}