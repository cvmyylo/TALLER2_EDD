#include "ControladorReproductor.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

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