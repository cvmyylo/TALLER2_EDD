#include "ArbolTrie.h"
NodoTrie::NodoTrie(char c){
    caracter=c;
    finPalabra=false;
    for(int i=0;i<128;i++) hijos[i]=nullptr;
}
NodoTrie::~NodoTrie(){
    for(int i=0;i<128;i++)
        if(hijos[i]!=nullptr)
            delete hijos[i];
}
ArbolTrie::ArbolTrie(){
    raiz=new NodoTrie('\0');
}
ArbolTrie::~ArbolTrie(){
    delete raiz;
}
void ArbolTrie::insertarCadena(const std::string& texto,Cancion* cancion){
    NodoTrie* actual=raiz;
    for(unsigned int i=0;i<texto.length();i++){
        unsigned char indice=(unsigned char)texto[i];
        if(actual->hijos[indice]==nullptr)
            actual->hijos[indice]=new NodoTrie(texto[i]);
        actual=actual->hijos[indice];
        actual->canciones.agregarAlFinal(cancion);
    }
    actual->finPalabra=true;
}
void ArbolTrie::insertarSufijos(const std::string& texto,Cancion* cancion){
    for(unsigned int i=0;i<texto.length();i++)
        insertarCadena(texto.substr(i),cancion);
}
void ArbolTrie::insertarCancion(Cancion* cancion){
    insertarSufijos(cancion->getTitulo(),cancion);
    insertarSufijos(cancion->getArtista(),cancion);
}
