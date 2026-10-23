#pragma once
#include "Render.h"


inline int convertirFechaAEntero(const string& fecha) {
    if (fecha.length() < 10) return 0;

    try {
        int dia = stoi(fecha.substr(0, 2));
        int mes = stoi(fecha.substr(3, 2));
        int anio = stoi(fecha.substr(6, 4));

        return (anio * 10000) + (mes * 100) + dia;
    }
    catch (...) {  return 0;  }
}

extern UI motorUI;

inline void dibujar(const vvi& mat, int offsetX = 0, int offsetY = 0) {
    motorUI.dibujarMatriz(mat, offsetX, offsetY);
}

inline void eliminarMatriz(const vvi& fondo, int filas, int cols, int posX, int posY) {
    motorUI.eliminarMatriz(fondo, filas, cols, posX, posY);
}

inline void imprimirTexto(const string& txt, int x, int y) {
    motorUI.imprimirTexto(txt, x, y);
}

inline void limpiarTexto(const string& txt, int x, int y, const vvi& fondo) {
    motorUI.limpiarTexto(txt, x, y, fondo);
}

inline void limpiarPantalla() {
    motorUI.limpiarPantalla();
}

inline void posicionarCursor(int x, int y) {
    motorUI.posicionarCursor(x, y);
}

inline void establecerColorTexto(int color) {
    motorUI.establecerColorTexto(color);
}


inline void iniciarBuffer() {
    motorUI.iniciarBuffer();
}

inline void presentarBuffer() {
    motorUI.presentarBuffer();
}


inline int obtenerAnchoMatriz(const vvi& mat) {
    if (mat.empty()) return 0;
    return mat[0].size();
}

inline int obtenerAltoMatriz(const vvi& mat) {
    return mat.size();
}


inline bool estaBloqMayusOn() {
    return estaBloqMayusActivado();
}

inline string toLowerLocal(const string& s) {
    string res = s;
    for (char& c : res) c = tolower(c);
    return res;
}

inline bool leerRPTASN(int xLabel, int yLabel, const vvi& fondo) {
    while (true) {
        iniciarBuffer();
        imprimirTexto(">Respuesta: ", xLabel, yLabel);
        presentarBuffer();

        string resp = motorUI.escribirLetraLinea(fondo, 101, yLabel, 5);
        if (resp.empty()) continue;

        string r = toLowerLocal(resp);
        if (r == "s" || r == "si") {
            iniciarBuffer();
            limpiarTexto(">Respuesta: ", xLabel, yLabel, fondo);
            presentarBuffer();
            return true;
        }
        if (r == "n" || r == "no") {
            iniciarBuffer();
            limpiarTexto(">Respuesta: ", xLabel, yLabel, fondo);
            presentarBuffer();
            return false;
        }

        iniciarBuffer();
        imprimirTexto(">> Error: Responde si o no.", 19, 70);
        presentarBuffer();
        Sleep(800);
        iniciarBuffer();
        limpiarTexto(">> Error: Responde si o no.", 19, 70, fondo);
        limpiarTexto(">Respuesta: ", xLabel, yLabel, fondo);
        presentarBuffer();
    }
}

