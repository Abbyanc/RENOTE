#pragma once

template <class K, class V>
class HashEntidad {
private:
    K clave;
    V valor;

public:
    HashEntidad(K _clave, V _valor) {
        clave = _clave;
        valor = _valor;
    }

    K getClave() { return clave; }
    V getValor() { return valor; }
};