#pragma once
#include <iostream>
#include <string>

using namespace std;

class Subtarea {
private:
    string nombre;
    bool completada;

public:
    Subtarea(string miNombre, bool estado = false) {
        nombre = miNombre;
        completada = estado;
    }


    void alternarEstado() {
        completada = !completada;
    }

    void renombrar(string nuevoNombre) {
        if (!nuevoNombre.empty()) {
            nombre = nuevoNombre;
        }
    }

    string obtenerFormatoVisual() {
        string casilla = completada ? "[X] " : "[ ] ";
        return casilla + nombre;
    }

    string getNombre() { return nombre; }
    bool estaCompletada() { return completada; }
};