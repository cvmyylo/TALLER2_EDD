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
