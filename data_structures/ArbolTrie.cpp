#include "ArbolTrie.h"
NodoTrie::NodoTrie(char c){
    caracter=c;
    finPalabra=false;
    for(int i=0;i<128;i++) hijos[i]=nullptr;
}