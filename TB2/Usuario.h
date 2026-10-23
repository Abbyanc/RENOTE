#pragma once
#include <string>

using namespace std;

class Usuario {
private:
    string nombreUsuario;
    string claveCifrada;

public:
    Usuario(string _nombre, string _clave) : nombreUsuario(_nombre), claveCifrada(_clave) {}

    string getNombre() { return nombreUsuario; }
    string getClaveCifrada() { return claveCifrada; }
};