#pragma once
using namespace std;

template <typename T, T NADA = 0>
class Cola {
private:
    struct Nodo {
        T dato;
        Nodo* sig;
        Nodo(T d, Nodo* s = nullptr) : dato(d), sig(s) {}
    };

    Nodo* frente;
    Nodo* finalCola;

public:
    Cola() : frente(nullptr), finalCola(nullptr) {}

    ~Cola() { while (!vacia()) pop(); }

    //FIFO
    void push(T dato) {
        Nodo* nuevo = new Nodo(dato);
        if (vacia()) {
            frente = nuevo;
            finalCola = nuevo;
        }
        else {
            finalCola->sig = nuevo;
            finalCola = nuevo;
        }
    }

    T pop() {
        if (vacia()) return NADA;

        Nodo* temp = frente;
        T dato = temp->dato;
        frente = frente->sig;

        if (frente == nullptr)  finalCola = nullptr;

        delete temp;
        return dato;
    }

    bool vacia() { return frente == nullptr; }
};