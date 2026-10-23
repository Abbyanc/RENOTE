#pragma once
#include <iostream>
#include <string>
#include "Utilidades.h" 
using namespace std;

class Etiqueta {
private:
    string nombre;
    string color;

    int obtenerCodigoColor() {
        string c = toLower(color);
        if (c == "azul") return 1;
        if (c == "verde") return 2;
        if (c == "celeste") return 3;
        if (c == "rojo") return 4;
        if (c == "morado") return 5;
        if (c == "amarillo") return 6;
        if (c == "blanco") return 7;
        if (c == "gris") return 8;
        return 7;
    }

public:
    Etiqueta(string miNombre, string miColor) {
        nombre = toUpper(miNombre);
        color = miColor;
    }
    static bool esColorValido(string colorInput) {
        string c = toLower(colorInput);
        return (c == "azul" || c == "verde" || c == "celeste" || c == "rojo"
            || c == "morado" || c == "amarillo" || c == "blanco" || c == "gris");
    }

    void imprimirEtiqueta() {
        int codigo = obtenerCodigoColor();
        asignarColor(codigo);
        cout << "[" << nombre << "] ";
        asignarColor(7);
    }

    void cambiarNombre(string nuevoNombre) { nombre = toUpper(nuevoNombre); }
    string getNombre() { return nombre; }
    string getColor() { return color; }
};