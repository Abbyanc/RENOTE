#pragma once
#include <iostream>
#include <string>
#include <ctime>

#include "Lista.h"
#include "Etiqueta.h"
#include "Render.h"
#include "utilidades2.h"

using namespace std;
typedef unsigned int uint;

class Nota {
protected:
    uint id;
    string titulo;
    string contenido;
    string fechaCreacion;

    Lista<Etiqueta*, nullptr> misEtiquetas;

    string doFecha() {
        time_t ahora = time(0);
        tm* tiempo = localtime(&ahora);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", tiempo);
        return string(buffer);
    }

    string hideSaltos(string texto) {
        string limpio = "";
        for (char c : texto) {
            if (c == '\n') limpio += "<BR>";
            else limpio += c;
        }
        return limpio;
    }

    string redoSaltos(string texto) {
        string original = "";
        for (size_t i = 0; i < texto.length(); i++) {
            if (texto[i] == '<' && texto.substr(i, 4) == "<BR>") {
                original += "\n";
                i += 3;
            }
            else original += texto[i];
        }
        return original;
    }

public:
    Nota() : id(0), titulo(""), contenido(""), fechaCreacion(doFecha()) {}
    Nota(uint _id, string _titulo, string _contenido) : id(_id), titulo(_titulo), contenido(_contenido), fechaCreacion(doFecha()) {}
    Nota(uint _id, string _titulo, string _contenido, string _fecha) : id(_id), titulo(_titulo), contenido(_contenido), fechaCreacion(_fecha) {}

    virtual ~Nota() {
        misEtiquetas.ejecutarSi([](Etiqueta* e) { return true; }, [](Etiqueta* e) { delete e; });
    }
    uint getId() { return id; }
    string getTitulo() { return titulo; }
    string getContenido() { return redoSaltos(contenido); }
    string getFecha() { return fechaCreacion; }
    virtual string getTipo() { return "NOTA"; }

    void setId(uint nuevoId) { id = nuevoId; }

    int contarLetras() {
        int contador = 0;
        string texto = titulo + contenido;
        for (char c : texto) {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) contador++;
        }
        return contador;
    }
    void agregarEtiqueta(string nombre, string color) {
        misEtiquetas.agregaFinal(new Etiqueta(nombre, color));
    }

    string serializarEtiquetas() {
        string resultado = "";
        misEtiquetas.ejecutarSi([](Etiqueta* e) { return true; },
            [&resultado](Etiqueta* e) { resultado += e->getNombre() + "=" + e->getColor() + ";"; }
        );
        return resultado;
    }

    virtual string serializar() {
        return getTipo() + "|" + to_string(id) + "|" + titulo + "|" + hideSaltos(contenido) + "|" + fechaCreacion;
    }

    virtual bool puedeSerEliminada() { return true; }

    virtual void show() { cout << titulo; }

    virtual void renderVisual() {
        extern UI motorUI;
        extern void renderTextoRapido(const string & txt, int x, int y);
        motorUI.limpiarPantalla();
        motorUI.iniciarBuffer();
        motorUI.dibujarMatriz(fNotasLeer, 0, 0);

        renderTextoRapido(this->titulo, 105, 2);
        renderTextoRapido(this->fechaCreacion, 290, 16);

        string impTipo = (getTipo() == "NOTA SECRETA") ? "SECRETA" : "NOTA";
        renderTextoRapido(impTipo, 3, 61);

        renderTextoRapido(to_string(contarLetras()), 3, 96);

        int numEti = misEtiquetas.longitud();
        if (numEti > 0) {
            Etiqueta* e1 = misEtiquetas.obtenerPorIndice(0);
            string txtNom1 = e1->getNombre();
            if (txtNom1.length() > 12) txtNom1 = txtNom1.substr(0, 12);

            string col1 = e1->getColor();
            if (col1 == "AZUL") { motorUI.dibujarMatriz(etAzul, 2, 13); }
            else if (col1 == "VERDE") { motorUI.dibujarMatriz(etVerde, 2, 13); }
            else if (col1 == "GRIS") { motorUI.dibujarMatriz(etGris, 2, 13); }
            else if (col1 == "ROJO") { motorUI.dibujarMatriz(etRojo, 2, 13); }
            else if (col1 == "AMARILLO") { motorUI.dibujarMatriz(etAmarillo, 2, 13); }
            else if (col1 == "MORADO") { motorUI.dibujarMatriz(etMorado, 2, 13); }
            else { motorUI.dibujarMatriz(etGris, 2, 13); }

            renderTextoRapido(txtNom1, 5, 16);
        }

        if (numEti > 1) {
            Etiqueta* e2 = misEtiquetas.obtenerPorIndice(1);
            string txtNom2 = e2->getNombre();
            if (txtNom2.length() > 12) txtNom2 = txtNom2.substr(0, 12);

            string col2 = e2->getColor();
            if (col2 == "AZUL") { motorUI.dibujarMatriz(etAzul, 118, 13); }
            else if (col2 == "VERDE") { motorUI.dibujarMatriz(etVerde, 118, 13); }
            else if (col2 == "GRIS") { motorUI.dibujarMatriz(etGris, 118, 13); }
            else if (col2 == "ROJO") { motorUI.dibujarMatriz(etRojo, 118, 13); }
            else if (col2 == "AMARILLO") { motorUI.dibujarMatriz(etAmarillo, 118, 13); }
            else if (col2 == "MORADO") { motorUI.dibujarMatriz(etMorado, 118, 13); }
            else { motorUI.dibujarMatriz(etGris, 118, 13); }

            renderTextoRapido(txtNom2, 121, 16);
        }

        string contTotal = getContenido();
        int filaNum = 1;
        int colX = 80;
        int colY = 34;

        for (int rec = 0; rec < contTotal.length(); rec++) {
            char carNow = contTotal[rec];

            if (carNow == '\n') {
                filaNum = filaNum + 1;
                colX = 80;
                if (filaNum == 2) { colY = 44; }
                else if (filaNum == 3) { colY = 54; }
                else if (filaNum == 4) { colY = 64; }
                else if (filaNum == 5) { colY = 74; }
                else if (filaNum == 6) { colY = 84; }
                else if (filaNum == 7) { colY = 94; }
                else if (filaNum == 8) { colY = 104; }
                if (filaNum > 8) break;
            }
            else {
                int avCol = (carNow == ' ') ? 3 : 7;
                if (colX + avCol > 342) {
                    filaNum = filaNum + 1;
                    colX = 80;
                    if (filaNum == 2) { colY = 44; }
                    else if (filaNum == 3) { colY = 54; }
                    else if (filaNum == 4) { colY = 64; }
                    else if (filaNum == 5) { colY = 74; }
                    else if (filaNum == 6) { colY = 84; }
                    else if (filaNum == 7) { colY = 94; }
                    else if (filaNum == 8) { colY = 104; }
                    if (filaNum > 8) break;
                }
                if (carNow != ' ') {
                    extern void _renderLetra(char a, int posX, int posY, UI & motor);
                    _renderLetra(carNow, colX, colY, motorUI);
                }
                colX = colX + avCol;
            }
        }
        motorUI.presentarBuffer();
    }
};