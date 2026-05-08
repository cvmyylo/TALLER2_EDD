#ifndef CONTROLADORREPRODUCTOR_H
#define CONTROLADORREPRODUCTOR_H

#include "../data_structures/ListaDobleEnlazada.h"
#include "../classes/Cancion.h"

enum ModoRepeticion {
    REPETICION_APAGADA, 
    REPETICION_UNA,     //(r1)
    REPETICION_TODAS    //(rA)
};

class ControladorReproductor {
private:
    ListaDobleEnlazada<Cancion>* registroGlobal; 
    ListaDobleEnlazada<Cancion> listaActual; 
    Nodo<Cancion>* nodoCancionActual;            
    
    bool estaReproduciendo;
    bool aleatorioActivado;
    ModoRepeticion modoRepeticion;


public:
    ControladorReproductor(ListaDobleEnlazada<Cancion>* registro);

    bool getEstaReproduciendo() const;
    bool getAleatorioActivado() const;
    ModoRepeticion getModoRepeticion() const;
    Cancion getCancionActual() const;
    bool tieneCancionActual() const;

    void alternarReproduccion();            
    void reproducirAnterior();               
    void reproducirSiguiente();                   
    void alternarAleatorio();              
    void ciclarModoRepeticion();            

    ListaDobleEnlazada<Cancion>& getListaActual();
    void mostrarMenuListaActual() const;
    void saltarACancion(int indiceDesplazamiento);
    void mostrarListadoGlobal() const;
    void reproducirDesdeGlobal(int indice);
    void encolarDesdeGlobal(int indice);
    void poblarListaAleatoria();
    void guardarEstado() const;
    void cargarEstado();

};

#endif