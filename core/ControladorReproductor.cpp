#include "ControladorReproductor.h"
#include "GestorArchivos.h" 
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>

ControladorReproductor::ControladorReproductor(ListaDobleEnlazada<Cancion>* registro) {
    this->registroGlobal = registro;
    this->nodoCancionActual = nullptr;
    this->estaReproduciendo = false;
    this->aleatorioActivado = false;
    this->modoRepeticion = REPETICION_APAGADA;
    std::srand(std::time(0)); 
}

bool ControladorReproductor::getEstaReproduciendo() const { return estaReproduciendo; }
bool ControladorReproductor::getAleatorioActivado() const { return aleatorioActivado; }
ModoRepeticion ControladorReproductor::getModoRepeticion() const { return modoRepeticion; }
bool ControladorReproductor::tieneCancionActual() const { return nodoCancionActual != nullptr; }

Cancion ControladorReproductor::getCancionActual() const { 
    if (nodoCancionActual != nullptr) return nodoCancionActual->dato;
    return Cancion(); 
}

ListaDobleEnlazada<Cancion>& ControladorReproductor::getListaActual() { return listaActual; }

ListaDobleEnlazada<Cancion>* ControladorReproductor::getRegistroGlobal() const {
    return registroGlobal;
}

void ControladorReproductor::agregarCancionGlobal(const Cancion& nuevaCancion) {
    registroGlobal->agregarAlFinal(nuevaCancion);
    GestorArchivos::guardarFuenteMusica("music_source.txt", *registroGlobal);
}

void ControladorReproductor::eliminarCancionGlobal(int id) {
    if (registroGlobal->eliminarPorId(id)) {
        listaActual.eliminarPorId(id);
        if (tieneCancionActual() && nodoCancionActual->dato.getId() == id) {
            nodoCancionActual = nullptr;
            estaReproduciendo = false;
        }
        GestorArchivos::guardarFuenteMusica("music_source.txt", *registroGlobal);
    }
}

void ControladorReproductor::alternarReproduccion() {
    if (!tieneCancionActual()) return; 
    estaReproduciendo = !estaReproduciendo;
}

void ControladorReproductor::alternarAleatorio() {
    if (!tieneCancionActual()) return;
    aleatorioActivado = !aleatorioActivado;
}

void ControladorReproductor::ciclarModoRepeticion() {
    if (!tieneCancionActual()) return;
    if (modoRepeticion == REPETICION_APAGADA) modoRepeticion = REPETICION_UNA;
    else if (modoRepeticion == REPETICION_UNA) modoRepeticion = REPETICION_TODAS;
    else modoRepeticion = REPETICION_APAGADA;
}

void ControladorReproductor::poblarListaAleatoria() {
    if (registroGlobal->estaVacia()) return;
    
    int idActual = tieneCancionActual() ? nodoCancionActual->dato.getId() : -1;
    
    listaActual.limpiar(); 

    int totalCanciones = registroGlobal->getTamaño();
    Cancion* arregloTemporal = new Cancion[totalCanciones];
    
    Nodo<Cancion>* actual = registroGlobal->getCabeza();
    for (int i = 0; i < totalCanciones; ++i) {
        arregloTemporal[i] = actual->dato;
        actual = actual->siguiente;
    }

    for (int i = totalCanciones - 1; i > 0; --i) {
        int j = std::rand() % (i + 1);
        Cancion temp = arregloTemporal[i];
        arregloTemporal[i] = arregloTemporal[j];
        arregloTemporal[j] = temp;
    }

    for (int i = 0; i < totalCanciones; ++i) {
        listaActual.agregarAlFinal(arregloTemporal[i]);
    }
    delete[] arregloTemporal;

    if (idActual != -1) {
        Nodo<Cancion>* temp = listaActual.getCabeza();
        while (temp != nullptr) {
            if (temp->dato.getId() == idActual) {
                nodoCancionActual = temp;
                break;
            }
            temp = temp->siguiente;
        }
    } else {
        nodoCancionActual = listaActual.getCabeza();
    }
}

void ControladorReproductor::reproducirSiguiente() {
    if (registroGlobal->estaVacia()) return;
    if (modoRepeticion == REPETICION_UNA && tieneCancionActual()) {
        estaReproduciendo = true;
        return;
    }

    if (listaActual.estaVacia()) {
        poblarListaAleatoria();
        nodoCancionActual = listaActual.getCabeza();
        estaReproduciendo = true;
        return;
    }
    
    if (nodoCancionActual != nullptr && nodoCancionActual->siguiente != nullptr) {
        nodoCancionActual = nodoCancionActual->siguiente;
        estaReproduciendo = true;
    } else {
        if (modoRepeticion == REPETICION_TODAS) {
            if (aleatorioActivado) poblarListaAleatoria();
            nodoCancionActual = listaActual.getCabeza();
            estaReproduciendo = true;
        } else {
            nodoCancionActual = nullptr;
            estaReproduciendo = false;
        }
    }
}

void ControladorReproductor::reproducirAnterior() {
    if (!tieneCancionActual() || nodoCancionActual->anterior == nullptr) return; 
    if (modoRepeticion == REPETICION_UNA) {
        estaReproduciendo = true;
        return;
    }
    nodoCancionActual = nodoCancionActual->anterior;
    estaReproduciendo = true;
}

void ControladorReproductor::mostrarMenuListaActual() const {
    std::string modificadores = "";
    if (aleatorioActivado || modoRepeticion != REPETICION_APAGADA) {
        modificadores += "(";
        if (aleatorioActivado) modificadores += "S";
        if (aleatorioActivado && modoRepeticion != REPETICION_APAGADA) modificadores += "-";
        if (modoRepeticion == REPETICION_UNA) modificadores += "R1";
        if (modoRepeticion == REPETICION_TODAS) modificadores += "RA";
        modificadores += ") ";
    }
    if (tieneCancionActual()) {
        Cancion actual = getCancionActual();
        std::cout << "Actual " << modificadores << ": " << actual.getTitulo() << " " << actual.getArtista() << "\n";
    } else {
        std::cout << "Actual: Ninguna\n";
    }

    std::cout << "Lista de reproduccion actual:\n";
    if (listaActual.estaVacia() || !tieneCancionActual() || nodoCancionActual->siguiente == nullptr) {
        std::cout << "Vacia\n";
    } else {
        Nodo<Cancion>* temp = nodoCancionActual->siguiente;
        int indice = 1;
        while (temp != nullptr) {
            std::cout << indice << ". " << temp->dato.getTitulo() << " - " << temp->dato.getArtista() << "\n";
            temp = temp->siguiente;
            indice++;
        }
    }
}

void ControladorReproductor::saltarACancion(int indiceDesplazamiento) {
    if (!tieneCancionActual() || nodoCancionActual->siguiente == nullptr) return;

    Nodo<Cancion>* nodoObjetivo = nodoCancionActual->siguiente;
    int contador = 1;
    
    while (nodoObjetivo != nullptr && contador < indiceDesplazamiento) {
        nodoObjetivo = nodoObjetivo->siguiente;
        contador++;
    }

    if (nodoObjetivo != nullptr) {
        nodoCancionActual = nodoObjetivo; 
        estaReproduciendo = true;             
    }
}

void ControladorReproductor::mostrarListadoGlobal() const {
    std::cout << "Listado Completo de Canciones:\n";
    if (registroGlobal->estaVacia()) {
        std::cout << "No hay canciones cargadas en el sistema.\n";
        return;
    }
    
    Nodo<Cancion>* temp = registroGlobal->getCabeza();
    int indice = 1;
    while (temp != nullptr) {
        std::cout << indice << ". " << temp->dato.getTitulo() << " - " << temp->dato.getArtista() << "\n";
        temp = temp->siguiente;
        indice++;
    }
}

void ControladorReproductor::reproducirDesdeGlobal(int indice) {    
    Nodo<Cancion>* temp = registroGlobal->getCabeza();
    int contador = 1;

    while (temp != nullptr && contador < indice) {
        temp = temp->siguiente;
        contador++;
    }
    
    if (temp != nullptr) {
        listaActual.limpiar();
        listaActual.agregarAlFinal(temp->dato);
        nodoCancionActual = listaActual.getCabeza();
        estaReproduciendo = true;
    }
}

void ControladorReproductor::encolarDesdeGlobal(int indice) {
    Nodo<Cancion>* temp = registroGlobal->getCabeza();
    int contador = 1;
    
    while (temp != nullptr && contador < indice) {
        temp = temp->siguiente;
        contador++;
    }
    
    if (temp != nullptr) {
        listaActual.agregarAlFinal(temp->dato);
    }
}

void ControladorReproductor::guardarEstado() const {
    std::ofstream archivo("status.cfg");
    if (!archivo.is_open()) return;

    archivo << aleatorioActivado << "\n";
    archivo << modoRepeticion << "\n";

    if (tieneCancionActual()) {
        archivo << nodoCancionActual->dato.getId() << "\n";
    } else {
        archivo << "-1\n";
    }
    Nodo<Cancion>* temp = listaActual.getCabeza();
    while (temp != nullptr) {
        archivo << temp->dato.getId() << " ";
        temp = temp->siguiente;
    }
    archivo << "\n";
    
    archivo.close();
}

void ControladorReproductor::cargarEstado() {
    std::ifstream archivo("status.cfg");
    if (!archivo.is_open()) return; 

    std::string linea;

    if (std::getline(archivo, linea)) aleatorioActivado = (linea == "1");
    
    if (std::getline(archivo, linea)) modoRepeticion = static_cast<ModoRepeticion>(std::stoi(linea));
    
    int idActual = -1;
    if (std::getline(archivo, linea)) idActual = std::stoi(linea);

    if (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string idStr;
        while (ss >> idStr) {
            int id = std::stoi(idStr);
            Nodo<Cancion>* temp = registroGlobal->getCabeza();
            while (temp != nullptr) {
                if (temp->dato.getId() == id) {
                    listaActual.agregarAlFinal(temp->dato);
                    break;
                }
                temp = temp->siguiente;
            }
        }
    }

    if (idActual != -1) {
        Nodo<Cancion>* temp = listaActual.getCabeza();
        while (temp != nullptr) {
            if (temp->dato.getId() == idActual) {
                nodoCancionActual = temp;
                break;
            }
            temp = temp->siguiente;
        }
    }
    archivo.close();
}

void ControladorReproductor::registrarReproduccionGlobal(int idCancion) {
    Nodo<Cancion>* actual = registroGlobal->getCabeza();
    while (actual != nullptr) {
        if (actual->dato.getId() == idCancion) {
            actual->dato.incrementarReproduccion();
            GestorArchivos::guardarRankings("song_ranking.txt", *registroGlobal);
            
            Nodo<Cancion>* actualLista = listaActual.getCabeza();
            while(actualLista != nullptr) {
                if (actualLista->dato.getId() == idCancion) {
                    actualLista->dato.incrementarReproduccion();
                }
                actualLista = actualLista->siguiente;
            }
            break;
        }
        actual = actual->siguiente;
    }
}