#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include "classes/Cancion.h"
#include "data_structures/ListaDobleEnlazada.h"
#include "core/GestorArchivos.h"
#include "core/ControladorReproductor.h"
#include "data_structures/MaxHeapCanciones.h"
#include "data_structures/MaxHeapArtistas.h"
#include "data_structures/ArbolTrie.h"
// Prototipos para que C++ no se queje del orden de las funciones
ArbolTrie arbolBusqueda; // Declaración global del árbol de búsqueda
bool trieCargado = false; // Bandera para indicar si el trie ha sido cargado
struct ArtistaStats; // Declaración adelantada del struct por si acaso
void mostrarTopCanciones(ControladorReproductor& reproductor);
void mostrarTopArtistas(ControladorReproductor& reproductor);
void mostrarCancionesArtista(ControladorReproductor& reproductor, ArtistaStats* artista);

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
        std::cout << "Album: " << actual.getAlbum() << " [" << actual.getAno() << "]\n";
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
    std::cout << "F - Buscar canciones y artistas\n"; 
    std::cout << "T - TOP 10 Artistas y Canciones\n"; 
    std::cout << "L - Listado de canciones\n";
    std::cout << "N - Agregar nueva cancion\n"; 
    std::cout << "D - Eliminar cancion\n";      
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
        
        std::getline(std::cin, entrada);
        if (entrada.empty()) continue;
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
        
        std::getline(std::cin, entrada);
        if (entrada.empty()) continue;
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
    std::string titulo, artista, album, anoStr, duracionStr, ruta;
    
    std::cout << "--- Agregar Nueva Cancion ---\n";
    std::cout << "Titulo: "; std::getline(std::cin, titulo);
    std::cout << "Artista: "; std::getline(std::cin, artista);
    std::cout << "Album: "; std::getline(std::cin, album);
    std::cout << "Ano: "; std::getline(std::cin, anoStr);
    std::cout << "Duracion (segundos): "; std::getline(std::cin, duracionStr);
    std::cout << "Ruta del archivo: "; std::getline(std::cin, ruta);

    try {
        int ano = std::stoi(anoStr);
        int duracion = std::stoi(duracionStr);
        
        int nuevoId = reproductor.getRegistroGlobal()->getTamano() + 1;
        Cancion nueva(nuevoId, titulo, artista, album, ano, duracion, ruta);
        
        reproductor.agregarCancionGlobal(nueva);
        Nodo<Cancion>* ultimo = reproductor.getRegistroGlobal()->getCabeza();
        while (ultimo->siguiente != nullptr){
            ultimo = ultimo->siguiente;
        }
        arbolBusqueda.insertarCancion(&ultimo->dato);
        std::cout << "\nCancion agregada y guardada exitosamente.\n";
    } catch (...) {
        std::cout << "\nError: Datos numericos invalidos. No se guardo la cancion.\n";
    }
    std::cout << "Presione Enter para continuar...";
    std::string pausa; std::getline(std::cin, pausa); 
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

void manejarMenuBusqueda(ControladorReproductor& reproductor) {
    if(!trieCargado) {
        limpiarPantalla();
        std::cout << "Trie no inicializado.\n";
        std::cin.get();
        return;
    }
    bool seguir = true;
    while (seguir) {
        limpiarPantalla();
        std::string texto;
        std::cout << "Busqueda de canciones\n\n";
        std::cout << "Buscar canciones que contengan: ";
        std::getline(std::cin, texto);
        if (texto.empty()) return;
        ListaDobleEnlazada<Cancion*> resultado = arbolBusqueda.buscar(texto);
        while (true) {
            limpiarPantalla();
            std::cout << "Canciones que contienen \"" << texto << "\":\n\n";
            Nodo<Cancion*>* actual = resultado.getCabeza();
            int contador = 1;
            while (actual != nullptr) {
                std::cout << contador << ". " << actual->dato->getTitulo() << " - " << actual->dato->getArtista() << "\n";
                actual = actual->siguiente;
                contador++;
            }
            if(contador==1) {
                std::cout << "No se encontraron canciones.\n";
            }
            std::cout << "\nOpciones:\n";
            std::cout << "R<num> - Reproducir cancion seleccionada\n";
            std::cout << "A<num> - Agregar cancion seleccionada al final de la lista de reproduccion actual\n";
            std::cout << "F - Repetir busqueda con un texto diferente\n";
            std::cout << "V - Volver al menu principal\n";
            std::cout << "\nIngrese Opcion: ";
            std::string opcion;
            std::getline(std::cin, opcion);
            if (opcion.empty()) continue;
            char cmd = toupper(opcion[0]);
            if (cmd == 'V') {
                return; 
            } else if (cmd == 'F') {
                break; 
            } else if ((cmd == 'R' || cmd == 'A') && opcion.length() > 1) {
                try {
                    int indice = std::stoi(opcion.substr(1));
                    if (indice < 1 || indice >= contador) continue;
                    Nodo<Cancion*>* nodo = resultado.getCabeza();
                    for (int i = 1; i < indice; i++) {
                        nodo = nodo->siguiente;
                    }
                    if(nodo == nullptr) continue;
                    int idCancion = nodo->dato->getId();
                    if (cmd == 'R') {
                        reproductor.reproducirDesdeGlobal(idCancion);
                        return;
                    }
                    reproductor.encolarDesdeGlobal(idCancion);
                } catch (...) {}
            }
        }
    }
}



void insertarOrdenadoAlfabetico(ListaDobleEnlazada<Cancion*>& lista, Cancion* nueva) {
    lista.agregarAlFinal(nueva);
}

void mostrarTopCanciones(ControladorReproductor& reproductor) {
    bool enSubmenu = true;
    std::string entrada;

    while (enSubmenu) {
        limpiarPantalla();
        int totalCanciones = reproductor.getRegistroGlobal()->getTamano();
        
        // 1. Construir Max Heap
        MaxHeapCanciones heap(totalCanciones);
        Nodo<Cancion>* cursor = reproductor.getRegistroGlobal()->getCabeza();
        while (cursor != nullptr) {
            Cancion* c = &(cursor->dato);
            heap.insertar(c);
            cursor = cursor->siguiente;
        }

        // 2. Extraer Top 10 (o Top N)
        int mostrar = (totalCanciones < 10) ? totalCanciones : 10;
        Cancion* top10[10]; // Arreglo estático para guardar los punteros extraídos

        std::cout << "Ranking TOP " << mostrar << " Canciones mas escuchadas:\n";
        for (int i = 0; i < mostrar; i++) {
            top10[i] = heap.extraerMaximo();
            int reprod = top10[i]->getCantidadReproducciones();
            std::string padding = (reprod < 10) ? " " : "";             
            std::cout << i + 1 << ". [" << padding << reprod << "] " 
                      << top10[i]->getTitulo() << " - " << top10[i]->getArtista() << "\n";
        }

        std::cout << "\nOpciones:\n";
        std::cout << "R<num> - Reproducir cancion seleccionada\n";
        std::cout << "A<num> - Agregar cancion seleccionada al final de la lista de reproduccion actual\n";
        std::cout << "A - Top 10 artistas mas escuchados\n";
        std::cout << "V - Volver al menu principal\n";
        std::cout << "Ingrese Opcion: ";

        std::getline(std::cin, entrada);
        if (entrada.empty()) continue;
        char cmd = toupper(entrada[0]);

        if (cmd == 'V') {
            enSubmenu = false;
        } else if (cmd == 'A' && entrada.length() == 1) {
            mostrarTopArtistas(reproductor); 
            enSubmenu = false; // Al volver del artista, volvemos al menú principal
        } else if ((cmd == 'R' || cmd == 'A') && entrada.length() > 1) {
            try {
                int num = std::stoi(entrada.substr(1));
                if (num > 0 && num <= mostrar) {
                    int idCancion = top10[num - 1]->getId();
                    if (cmd == 'R') {
                        reproductor.reproducirDesdeGlobal(idCancion);
                    } else {
                        reproductor.encolarDesdeGlobal(idCancion);
                    }
                }
            } catch (...) {}
        }
    }
}

void mostrarTopArtistas(ControladorReproductor& reproductor) {
    bool enSubmenu = true;
    std::string entrada;

    
 //   int totalCanciones = reproductor.getRegistroGlobal()->getTamano();
    ListaDobleEnlazada<ArtistaStats*> listaArtistas;

    Nodo<Cancion>* cursor = reproductor.getRegistroGlobal()->getCabeza();
    while (cursor != nullptr) {
        Cancion* c = &(cursor->dato);
        std::string nombreArt = c->getArtista();
        
       
        bool encontrado = false;
        Nodo<ArtistaStats*>* cursorArt = listaArtistas.getCabeza();
        while (cursorArt != nullptr) {
            if (cursorArt->dato->nombre == nombreArt) {
                cursorArt->dato->totalReproducciones += c->getCantidadReproducciones();
                insertarOrdenadoAlfabetico(cursorArt->dato->cancionesDelArtista, c);
                encontrado = true;
                break;
            }
            cursorArt = cursorArt->siguiente;
        }

        // Si es nuevo, lo creamos
        if (!encontrado) {
            ArtistaStats* nuevoArt = new ArtistaStats(nombreArt);
            nuevoArt->totalReproducciones += c->getCantidadReproducciones();
            insertarOrdenadoAlfabetico(nuevoArt->cancionesDelArtista, c);
            listaArtistas.agregarAlFinal(nuevoArt);
        }
        cursor = cursor->siguiente;
    }

    while (enSubmenu) {
        limpiarPantalla();
        
        // 2. Construir Max Heap
        int cantArtistas = listaArtistas.getTamano();
        MaxHeapArtistas heap(cantArtistas);
        
        Nodo<ArtistaStats*>* rec = listaArtistas.getCabeza();
        while (rec != nullptr) {
            heap.insertar(rec->dato);
            rec = rec->siguiente;
        }

        // 3. Extraer Top 10
        int mostrar = (cantArtistas < 10) ? cantArtistas : 10;
        ArtistaStats* top10[10];

        std::cout << "Ranking TOP " << mostrar << " Artistas mas escuchados:\n";
        for (int i = 0; i < mostrar; i++) {
            top10[i] = heap.extraerMaximo();
            int reprod = top10[i]->totalReproducciones;
            std::string padding = (reprod < 10) ? " " : "";
            std::cout << i + 1 << ". [" << padding << reprod << "] " << top10[i]->nombre << "\n";
        }

        std::cout << "\nOpciones:\n";
        std::cout << "S<num> - Mostrar canciones del artista\n";
        std::cout << "C - Top 10 canciones mas escuchadas\n";
        std::cout << "V - Volver al menu principal\n";
        std::cout << "Ingrese Opcion: ";

        std::getline(std::cin, entrada);
        if (entrada.empty()) continue;
        char cmd = toupper(entrada[0]);

        if (cmd == 'V') {
            enSubmenu = false;
        } else if (cmd == 'C') {
            mostrarTopCanciones(reproductor);
            enSubmenu = false;
        } else if (cmd == 'S' && entrada.length() > 1) {
            try {
                int num = std::stoi(entrada.substr(1));
                if (num > 0 && num <= mostrar) {
                    mostrarCancionesArtista(reproductor, top10[num - 1]);
                }
            } catch (...) {}
        }
    }

   
    Nodo<ArtistaStats*>* curr = listaArtistas.getCabeza();
    while (curr != nullptr) {
        delete curr->dato;
        curr = curr->siguiente;
    }
}

void mostrarCancionesArtista(ControladorReproductor& reproductor, ArtistaStats* artista) {
    bool enSubmenu = true;
    std::string entrada;

    while(enSubmenu) {
        limpiarPantalla();
        int cantCanciones = artista->cancionesDelArtista.getTamano();
        Cancion** arregloCanciones = new Cancion*[cantCanciones]; // Para acceder por índice rápido

        std::cout << "Ranking TOP 10 Artistas mas escuchados:\n";
        std::cout << "Artista: " << artista->nombre << "\n";
        
        Nodo<Cancion*>* cursor = artista->cancionesDelArtista.getCabeza();
        int i = 0;
        while (cursor != nullptr) {
            arregloCanciones[i] = cursor->dato;
            std::cout << i + 1 << ". " << arregloCanciones[i]->getTitulo() << "\n";
            cursor = cursor->siguiente;
            i++;
        }

        std::cout << "\nOpciones:\n";
        std::cout << "R<num> - Reproducir cancion seleccionada\n";
        std::cout << "A<num> - Agregar cancion seleccionada al final de la lista de reproduccion actual\n";
        std::cout << "V - Volver al listado de TOP 10 artistas\n";
        std::cout << "X - Volver al menu principal\n";
        std::cout << "Ingrese Opcion: ";

        std::getline(std::cin, entrada);
        if (entrada.empty()) {
            delete[] arregloCanciones;
            continue;
        }
        
        char cmd = toupper(entrada[0]);

        if (cmd == 'V') {
            enSubmenu = false;
        } else if (cmd == 'X') {
           
            enSubmenu = false;
        } else if ((cmd == 'R' || cmd == 'A') && entrada.length() > 1) {
            try {
                int num = std::stoi(entrada.substr(1));
                if (num > 0 && num <= cantCanciones) {
                    int idCancion = arregloCanciones[num - 1]->getId();
                    if (cmd == 'R') {
                        reproductor.reproducirDesdeGlobal(idCancion);
                        enSubmenu = false; 
                    } else {
                        reproductor.encolarDesdeGlobal(idCancion);
                    }
                }
            } catch (...) {}
        }
        delete[] arregloCanciones;
    }
}

void manejarMenuTop(ControladorReproductor& reproductor) {
    bool enSubmenu = true;
    std::string entrada;

    while (enSubmenu) {
        limpiarPantalla();
        std::cout << "Ranking TOP\n";
        std::cout << "C - Top 10 canciones mas escuchadas\n";
        std::cout << "A - Top 10 artistas mas escuchados\n";
        std::cout << "X - Salir\n";
        std::cout << "Ingrese Opcion: ";

        std::getline(std::cin, entrada);
        if (entrada.empty()) continue;
        char comando = toupper(entrada[0]);

        if (comando == 'X') {
            enSubmenu = false;
        } else if (comando == 'C') {
            mostrarTopCanciones(reproductor);
        } else if (comando == 'A') {
            mostrarTopArtistas(reproductor);
        }
    }
}

int main() {
    ListaDobleEnlazada<Cancion> registroCanciones;
    GestorArchivos::cargarFuenteMusica("music_source.txt", registroCanciones);
    GestorArchivos::cargarRankings("song_ranking.txt", registroCanciones); 
    Nodo<Cancion>* aux = registroCanciones.getCabeza();
    while (aux != nullptr) {
        arbolBusqueda.insertarCancion(&aux->dato);
        aux = aux->siguiente;
    }
    trieCargado = true; // Marcamos que el trie ha sido cargado
    ControladorReproductor reproductor(&registroCanciones);
    Nodo<Cancion>* cursor = registroCanciones.getCabeza();
    while (cursor != nullptr){
        arbolBusqueda.insertarCancion(&cursor->dato);
        cursor = cursor->siguiente;
    }
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
            case 'F': manejarMenuBusqueda(reproductor); break; 
            case 'T': manejarMenuTop(reproductor); break;  
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