#pragma once
#include <functional>

using namespace std;
typedef unsigned int uint;

template<typename T, T NADA = 0>
class Lista {
private:
    struct Nodo;
    Nodo* ini;
    Nodo* fin;
    uint lon;

public:
    Lista();
    ~Lista();

    void agregaFinal(T elem);
    uint longitud();
    void mostrarTodos();
    void mostrarSi(function<bool(T)> condicion);
    void ejecutarSi(function<bool(T)> condicion, function<void(T)> accion);
    T buscarDato(function<bool(T)> condicion);
    T eliminarPor(function<bool(T)> condicion);

    T obtenerPorIndice(uint indice);
    void intercambiar(uint pos1, uint pos2);
};

template<typename T, T NADA>
struct Lista<T, NADA>::Nodo {
    T elem;
    Nodo* sig;
    Nodo* ant;
    Nodo(T elem = NADA, Nodo* sig = nullptr, Nodo* ant = nullptr) : elem(elem), sig(sig), ant(ant) {}
};

template<typename T, T NADA>
Lista<T, NADA>::Lista() : ini(nullptr), fin(nullptr), lon(0) {}

template<typename T, T NADA> 
Lista<T, NADA>::~Lista() {
    Nodo* aux = ini;
    while (aux != nullptr) {
        Nodo* aBorrar = aux;
        aux = aux->sig;
        delete aBorrar;
    }
    ini = nullptr;
    fin = nullptr;
    lon = 0;
}

template<typename T, T NADA>
void Lista<T, NADA>::agregaFinal(T elem) {
    Nodo* nuevo = new Nodo(elem);
    if (ini == nullptr) {
        ini = nuevo;
        fin = nuevo;
    }
    else {
        fin->sig = nuevo;
        nuevo->ant = fin;
        fin = nuevo;
    }
    lon++;
}

template<typename T, T NADA>
uint Lista<T, NADA>::longitud() { return lon; }

template<typename T, T NADA>
T Lista<T, NADA>::obtenerPorIndice(uint indice) {
    if (indice >= lon) return NADA;

    Nodo* aux = ini;
    for (uint i = 0; i < indice; i++) {
        aux = aux->sig;
    }
    return aux->elem;
}

template<typename T, T NADA>
void Lista<T, NADA>::intercambiar(uint pos1, uint pos2) {
    if (pos1 >= lon || pos2 >= lon || pos1 == pos2) return;

    Nodo* nodo1 = ini;
    for (uint i = 0; i < pos1; i++) nodo1 = nodo1->sig;

    Nodo* nodo2 = ini;
    for (uint i = 0; i < pos2; i++) nodo2 = nodo2->sig;

    T temp = nodo1->elem;
    nodo1->elem = nodo2->elem;
    nodo2->elem = temp;
}


template<typename T, T NADA>
void Lista<T, NADA>::mostrarTodos() {
    Nodo* aux = ini;
    while (aux != nullptr) {
        aux->elem->show();
        aux = aux->sig;
    }
}

template<typename T, T NADA>
void Lista<T, NADA>::mostrarSi(function<bool(T)> condicion) {
    Nodo* aux = ini;
    while (aux != nullptr) {
        if (condicion(aux->elem)) {
            aux->elem->show();
        }
        aux = aux->sig;
    }
}

template<typename T, T NADA>
void Lista<T, NADA>::ejecutarSi(function<bool(T)> condicion, function<void(T)> accion) {
    Nodo* aux = ini;
    while (aux != nullptr) {
        if (condicion(aux->elem)) {
            accion(aux->elem);
        }
        aux = aux->sig;
    }
}

template<typename T, T NADA>
T Lista<T, NADA>::buscarDato(function<bool(T)> condicion) { 
    Nodo* aux = ini;
    while (aux != nullptr) {
        if (condicion(aux->elem)) {
            return aux->elem;
        }
        aux = aux->sig;
    }
    return NADA;
}

template<typename T, T NADA>
T Lista<T, NADA>::eliminarPor(function<bool(T)> condicion) {
    Nodo* aux = ini;

    while (aux != nullptr) {
        if (condicion(aux->elem)) {
            T elementoEliminado = aux->elem;

            if (aux->ant != nullptr) {
                aux->ant->sig = aux->sig;
            }
            else  ini = aux->sig;

            if (aux->sig != nullptr) {
                aux->sig->ant = aux->ant;
            }
            else fin = aux->ant;

            delete aux;
            lon--;
            return elementoEliminado;
        }
        aux = aux->sig;
    }

    return NADA;
}