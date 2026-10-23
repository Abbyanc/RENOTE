#pragma once
#include <string>
#include "Lista.h"
#include "Utilidades.h"

using namespace std;

template<typename T>
class NodoAVL {
public:
    T dato;
    string clave;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;

    NodoAVL(T d, string clave) {
        dato = d;
        this->clave = toUpper(clave);
        izq = nullptr;
        der = nullptr;
        altura = 1;
    }
};

template<typename T>
class ArbolAVL {
private:
    NodoAVL<T>* raiz;

    int altura(NodoAVL<T>* n) {
        return (n == nullptr) ? 0 : n->altura;
    }

    int factorBalance(NodoAVL<T>* n) {
        return (n == nullptr) ? 0 : altura(n->izq) - altura(n->der);
    }

    void actualizarAltura(NodoAVL<T>* n) {
        int hi = altura(n->izq);
        int hd = altura(n->der);
        n->altura = 1 + (hi > hd ? hi : hd);
    }

    NodoAVL<T>* rotarDerecha(NodoAVL<T>* y) {
        NodoAVL<T>* x = y->izq;
        NodoAVL<T>* T2 = x->der;

        x->der = y;
        y->izq = T2;

        actualizarAltura(y);
        actualizarAltura(x);
        return x;
    }

    NodoAVL<T>* rotarIzquierda(NodoAVL<T>* x) {
        NodoAVL<T>* y = x->der;
        NodoAVL<T>* T2 = y->izq;

        y->izq = x;
        x->der = T2;

        actualizarAltura(x);
        actualizarAltura(y);
        return y;
    }

    NodoAVL<T>* balancear(NodoAVL<T>* nodo) {
        actualizarAltura(nodo);
        int fb = factorBalance(nodo);

        if (fb > 1) {
            if (factorBalance(nodo->izq) < 0) {
                nodo->izq = rotarIzquierda(nodo->izq);
            }
            return rotarDerecha(nodo);
        }

        if (fb < -1) {
            if (factorBalance(nodo->der) > 0) {
                nodo->der = rotarDerecha(nodo->der);
            }
            return rotarIzquierda(nodo);
        }

        return nodo;
    }

    NodoAVL<T>* insertarRec(NodoAVL<T>* nodo, T dato, string clave) {
        if (nodo == nullptr) {
            return new NodoAVL<T>(dato, clave);
        }

        string claveUpper = toUpper(clave);

        if (claveUpper < nodo->clave) {
            nodo->izq = insertarRec(nodo->izq, dato, clave);
        }
        else {
            nodo->der = insertarRec(nodo->der, dato, clave);
        }

        return balancear(nodo);
    }

    void liberarRec(NodoAVL<T>* nodo) {
        if (nodo == nullptr) return;
        liberarRec(nodo->izq);
        liberarRec(nodo->der);
        delete nodo;
    }

    void buscarPatronRec(NodoAVL<T>* nodo, const string& patron, Lista<T, nullptr>& resultados) {
        if (nodo == nullptr) return;

        buscarPatronRec(nodo->izq, patron, resultados);

        if (nodo->clave.find(patron) != string::npos) {
            resultados.agregaFinal(nodo->dato);
        }

        buscarPatronRec(nodo->der, patron, resultados);
    }

public:
    ArbolAVL() : raiz(nullptr) {}

    ~ArbolAVL() {
        liberarRec(raiz);
    }

    void insertar(T dato, string clave) {
        if (dato == nullptr) return;
        raiz = insertarRec(raiz, dato, clave);
    }

    int alturaArbol() {
        return altura(raiz);
    }

    void buscarPorPatron(string patron, Lista<T, nullptr>& resultados) {
        string patronUpper = toUpper(patron);
        buscarPatronRec(raiz, patronUpper, resultados);
    }
};