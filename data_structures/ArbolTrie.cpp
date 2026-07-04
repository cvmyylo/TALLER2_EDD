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