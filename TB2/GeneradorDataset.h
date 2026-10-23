#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Boveda.h"
#include "Nota.h"
#include "Tarea.h"
#include "NotaSecreta.h"
#include "Utilidades.h" 

using namespace std;

class GeneradorDataset {
private:
    string nombresTags[6] = { "UNIVERSIDAD", "PROYECTO", "PERSONAL", "URGENTE", "TRABAJO", "IDEAS" };
    string coloresTags[6] = { "AZUL", "VERDE", "GRIS", "ROJO", "AMARILLO", "MORADO" };

    string generarFechaAleatoria() {
        int dia = rand() % 28 + 1;
        int mes = rand() % 12 + 1;
        int anio = 2026;

        string strDia = (dia < 10) ? "0" + to_string(dia) : to_string(dia);
        string strMes = (mes < 10) ? "0" + to_string(mes) : to_string(mes);

        return strDia + "/" + strMes + "/" + to_string(anio);
    }

public:
    void generarMasivo(Boveda& miVault, uint& contadorIds, int cantidad = 800) {
        srand(time(0));

        for (int i = 0; i < cantidad; i++) {
            int tipoDato = rand() % 10;

            string tituloBase = "Registro Generado #" + to_string(contadorIds);
            string contenidoBase = "Este es un texto autogenerado para probar el rendimiento del Arbol AVL y la carga masiva.\nId interno: " + to_string(contadorIds);

            Nota* nuevoRegistro = nullptr;

            if (tipoDato <= 5) {
                string fechaLim = generarFechaAleatoria();
                Tarea* t = new Tarea(contadorIds, "Tarea: " + tituloBase, contenidoBase, fechaLim);

                int cantSubs = rand() % 3 + 1;
                for (int j = 0; j < cantSubs; j++) {
                    bool estado = (rand() % 2 == 0);
                    t->agregarSubtarea("Subtarea autogenerada " + to_string(j + 1), estado);
                }
                nuevoRegistro = t;
            }
            else if (tipoDato <= 8) {
                nuevoRegistro = new Nota(contadorIds, "Nota: " + tituloBase, contenidoBase);
            }
            else {
                nuevoRegistro = new NotaSecreta(contadorIds, "Secreto: " + tituloBase, contenidoBase, "1234");
            }

            int numTags = rand() % 2 + 1;
            for (int k = 0; k < numTags; k++) {
                int indiceTag = rand() % 6;
                nuevoRegistro->agregarEtiqueta(nombresTags[indiceTag], coloresTags[indiceTag]);
            }

            miVault.agregarNotaGlobal(nuevoRegistro);
            contadorIds++;
        }
    }
};