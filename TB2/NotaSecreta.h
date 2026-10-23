#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include "Nota.h"
#include "Cifrador.h"
#include "Utilidades.h"

using namespace std;
typedef unsigned int uint;

class NotaSecreta : public Nota {
private:
    string contrasena;
    int intentosFallidos;
    time_t tiempoDesbloqueo;

public:
    NotaSecreta(uint _id, string _titulo, string _contenido, string _contrasena)
        : Nota(_id, _titulo, _contenido), contrasena(_contrasena), intentosFallidos(0), tiempoDesbloqueo(0) {
    }

    NotaSecreta(uint _id, string _titulo, string _contenido, string _fecha, string _contrasena, int _intentos, time_t _tiempo)
        : Nota(_id, _titulo, _contenido, _fecha), contrasena(_contrasena), intentosFallidos(_intentos), tiempoDesbloqueo(_tiempo) {
    }

    string getTipo() override { return "NOTA SECRETA"; }

    string getContrasena() { return contrasena; }

    int getSegundosParaDesbloqueo() {
        if (tiempoDesbloqueo == 0) return 0;
        time_t ahora = time(0);
        if (ahora < tiempoDesbloqueo) return (int)(tiempoDesbloqueo - ahora);
        return 0;
    }

    bool estaBloqueada() { return getSegundosParaDesbloqueo() > 0; }

    int getIntentosRestantes() { return 5 - intentosFallidos; }

    bool intentarDesbloqueo(string intento, int castigoSegundos = 120) {
        if (estaBloqueada()) return false;

        if (intento == contrasena) {
            intentosFallidos = 0;
            tiempoDesbloqueo = 0;
            return true;
        }

        intentosFallidos++;
        if (intentosFallidos >= 5) {
            tiempoDesbloqueo = time(0) + castigoSegundos;
            intentosFallidos = 0;
        }
        return false;
    }

    void bloquear() { }

    string serializar() override {
        Cifrador cif;
        return getTipo() + "|" + to_string(id) + "|" + titulo + "|" +
            cif.encriptar(hideSaltos(contenido)) + "|" + fechaCreacion + "|" +
            cif.encriptar(contrasena) + "|" + to_string(intentosFallidos) + "|" + to_string(tiempoDesbloqueo);
    }
};