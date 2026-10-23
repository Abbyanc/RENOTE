
#pragma once
#include <string>
#include "Lista.h"
#include "HashEntidad.h"

using namespace std;
typedef unsigned int uint;

template <class K, class V>
class TablaHash {
private:
    Lista<HashEntidad<K, V>*>* tabla;
    int capacidad;
    int cantidadElementos;
    uint miFuncionHash(string claveStr) {
        uint sumaHash = 0;
        int numeroPrimo = 31;

        for (size_t i = 0; i < claveStr.length(); i++) {
            sumaHash += (claveStr[i] * (i + 1) * numeroPrimo);
        }
        return sumaHash % capacidad;
    }

public:
    TablaHash(int _capacidad = 101) {
        capacidad = _capacidad;
        cantidadElementos = 0;
        tabla = new Lista<HashEntidad<K, V>*>[capacidad];
    }

    ~TablaHash() {
        for (int i = 0; i < capacidad; i++) {
            tabla[i].ejecutarSi([](HashEntidad<K, V>* e) { return true; },
                [](HashEntidad<K, V>* e) { delete e; });
        }
        delete[] tabla;
    }
    void insertar(string claveInput, V valor) {
        uint indice = miFuncionHash(claveInput);

        tabla[indice].agregaFinal(new HashEntidad<K, V>(claveInput, valor));
        cantidadElementos++;
    }

    V buscar(string claveInput, V valorPorDefecto) {
        uint indice = miFuncionHash(claveInput);
        V resultado = valorPorDefecto;
        tabla[indice].ejecutarSi(
            [&claveInput](HashEntidad<K, V>* e) { return e->getClave() == claveInput; },
            [&resultado](HashEntidad<K, V>* e) { resultado = e->getValor(); }
        );

        return resultado;
    }
};