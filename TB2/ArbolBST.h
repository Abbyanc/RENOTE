#pragma once
#include <iostream>
#include "Tarea.h"
#include "Lista.h" 

class NodoBST {
public:
    int llaveFecha;  
    Tarea* data;
    NodoBST* izquierdo;
    NodoBST* derecho;

    NodoBST(int llave, Tarea* tarea)
        : llaveFecha(llave), data(tarea), izquierdo(nullptr), derecho(nullptr) {
    }
};

class ArbolBST {
private:
    NodoBST* raiz;

    NodoBST* insertarRecursivo(NodoBST* nodo, int llave, Tarea* tarea) {
        if (nodo == nullptr) { return new NodoBST(llave, tarea); }

        if (llave < nodo->llaveFecha) {
            nodo->izquierdo = insertarRecursivo(nodo->izquierdo, llave, tarea);
        }
        else {
            nodo->derecho = insertarRecursivo(nodo->derecho, llave, tarea);
        }
        return nodo;
    }
    
    void extraerEnOrdenRecursivo(NodoBST* nodo, Lista<Tarea*, nullptr>& listaDestino) { 
        if (nodo == nullptr) return;

        extraerEnOrdenRecursivo(nodo->izquierdo, listaDestino);

        listaDestino.agregaFinal(nodo->data);

        extraerEnOrdenRecursivo(nodo->derecho, listaDestino);
    }

    void destruirRecursivo(NodoBST* nodo) {
        if (nodo == nullptr) return;
        destruirRecursivo(nodo->izquierdo);
        destruirRecursivo(nodo->derecho);
        delete nodo;
    }

public:
    ArbolBST() : raiz(nullptr) {}

    ~ArbolBST() { destruir(); }

    void insertar(int llave, Tarea* tarea) {
        raiz = insertarRecursivo(raiz, llave, tarea);
    }

    void extraerCronologico(Lista<Tarea*, nullptr>& listaDestino) {
        extraerEnOrdenRecursivo(raiz, listaDestino);
    }

    void destruir() {
        destruirRecursivo(raiz);
        raiz = nullptr;
    }
};