#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Boveda.h"
#include "Nota.h"
#include "NotaSecreta.h"
#include "Tarea.h"
#include "Cifrador.h" 

using namespace std;
typedef unsigned int uint;

class GestorArchivos {
private:
    string nombreArchivo;

    void reconstruirNota(string tipoStr, stringstream& ss, Boveda& miVault, uint& maxIdNota) {
        try {
            string idStr, titulo, contenido, fecha;

            // extrac base
            getline(ss, idStr, '|');
            getline(ss, titulo, '|');
            getline(ss, contenido, '|');
            getline(ss, fecha, '|');

            uint idN = stoi(idStr);
            if (idN > maxIdNota) maxIdNota = idN;

            Nota* nuevaNota = nullptr;

            if (tipoStr == "NOTA") {
                nuevaNota = new Nota(idN, titulo, contenido, fecha);
            }
            else if (tipoStr == "NOTA SECRETA") {
                string contrasenaCifrada, strIntentos, strTiempo;
                getline(ss, contrasenaCifrada, '|');
                getline(ss, strIntentos, '|');
                getline(ss, strTiempo, '|');

                Cifrador cif;
                string contenidoReal = cif.desencriptar(contenido);
                string contrasenaReal = cif.desencriptar(contrasenaCifrada);

                int intentosGuardados = (!strIntentos.empty()) ? stoi(strIntentos) : 0;
                time_t tiempoGuardado = (!strTiempo.empty()) ? stoll(strTiempo) : 0;

                nuevaNota = new NotaSecreta(idN, titulo, contenidoReal, fecha, contrasenaReal, intentosGuardados, tiempoGuardado);
            }
            else if (tipoStr == "TAREA") {
                string strFechaLimite, strChecklist, strEtiquetas;
                getline(ss, strFechaLimite, '|');
                getline(ss, strChecklist, '|');
                
                getline(ss, strEtiquetas, '|');
                
                if (!strEtiquetas.empty() && strEtiquetas.back() == '\r') {
                    strEtiquetas.pop_back();
                }

                nuevaNota = new Tarea(idN, titulo, contenido, fecha, strFechaLimite, strChecklist, strEtiquetas);
                
                if (nuevaNota != nullptr) {
                    miVault.agregarNotaGlobal(nuevaNota);
                }
                return;
            }

            if (nuevaNota != nullptr) {
                string strEtiquetas = "";
                if (getline(ss, strEtiquetas, '|')) {
                    if (!strEtiquetas.empty() && strEtiquetas.back() == '\r') {
                        strEtiquetas.pop_back();
                    }

                    if (!strEtiquetas.empty()) {
                        stringstream ssTags(strEtiquetas);
                        string tagToken;
                        while (getline(ssTags, tagToken, ';')) {
                            if (tagToken.empty()) continue;
                            size_t posIgual = tagToken.find('=');
                            if (posIgual != string::npos) {
                                string nomEti = tagToken.substr(0, posIgual);
                                string colEti = tagToken.substr(posIgual + 1);
                                nuevaNota->agregarEtiqueta(nomEti, colEti);
                            }
                        }
                    }
                }
                miVault.agregarNotaGlobal(nuevaNota);
            }
        }
        catch (...) {}
    }

public:
    GestorArchivos() : nombreArchivo("default_datos.txt") {}
    void setNombreArchivo(string nuevoNombre) {
        nombreArchivo = nuevoNombre;
    }

    void guardarBoveda(Boveda& miVault) {
        ofstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            return;
        }
        miVault.getNotasGlobales().ejecutarSi(
            [](Nota* n) { return true; },
            [&archivo](Nota* n) {
                if (n->getTipo() == "TAREA") {
                    archivo << n->serializar() << "\n";
                } else {
                    archivo << n->serializar() << "|" << n->serializarEtiquetas() << "\n";
                }
            }
        );

        archivo.close();
    }
    void cargarBoveda(Boveda& miVault, uint& contadorIdsMaestro) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) return;

        string linea;
        uint maxIdNota = 0;

        while (getline(archivo, linea)) {
            if (linea.empty()) continue;

            stringstream ss(linea);
            string tipoToken;
            getline(ss, tipoToken, '|');

            reconstruirNota(tipoToken, ss, miVault, maxIdNota);
        }

        archivo.close();
        if (maxIdNota > 0) contadorIdsMaestro = maxIdNota + 1;
    }
};