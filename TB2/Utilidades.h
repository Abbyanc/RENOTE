#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include "Matrices.h"
using namespace System;
using namespace std;

#define LIMPIAR limpiar()

void pausarPantalla()
{
    cout << "\nPresiona ENTER para continuar...";
    cin.get();
}

inline bool validarFecha(string fecha) {
    if (fecha.length() != 10) return false;
    if (fecha[2] != '/' || fecha[5] != '/') return false;
    for (int i = 0; i < 10; i++) {
        if (i != 2 && i != 5) {
            if (fecha[i] < '0' || fecha[i] > '9') return false;
        }
    }
    return true;
}

void ventana() {
    Console::SetWindowSize(100, 50);
}

void asignarColor(int color) {
    switch (color)
    {
    case 0: Console::ForegroundColor = ConsoleColor::Black; break;
    case 1: Console::ForegroundColor = ConsoleColor::Blue; break;
    case 2: Console::ForegroundColor = ConsoleColor::Green; break;
    case 3: Console::ForegroundColor = ConsoleColor::Cyan; break;
    case 4: Console::ForegroundColor = ConsoleColor::Red; break;
    case 5: Console::ForegroundColor = ConsoleColor::Magenta; break;
    case 6: Console::ForegroundColor = ConsoleColor::Yellow; break;
    case 7: Console::ForegroundColor = ConsoleColor::White; break;
    case 8: Console::ForegroundColor = ConsoleColor::Gray; break;
    case 9: Console::ForegroundColor = ConsoleColor::DarkBlue; break;
    case 10: Console::ForegroundColor = ConsoleColor::DarkGreen; break;
    case 11: Console::ForegroundColor = ConsoleColor::DarkCyan; break;
    case 12: Console::ForegroundColor = ConsoleColor::DarkRed; break;
    case 13: Console::ForegroundColor = ConsoleColor::DarkMagenta; break;
    case 14: Console::ForegroundColor = ConsoleColor::DarkYellow; break;
    case 15: Console::ForegroundColor = ConsoleColor::DarkGray; break;
    default: Console::ForegroundColor = ConsoleColor::Black; break;
    }
}

void ubicar(int x, int y) {
    Console::SetCursorPosition(x, y);
}

void limpiar() { Console::Clear(); }

void limpiarLinea(int linea, int ancho = 100) {
    ubicar(0, linea);
    for (int i = 0; i < ancho; ++i) {
        cout << ' ';
    }
}

void escribir(int x, int y, string texto, int color = 7) {
    asignarColor(color);
    ubicar(x, y);
    cout << texto;
}

template<size_t FILAS, size_t COLUMNAS>
void dibujar(int(&matriz)[FILAS][COLUMNAS], int offsetX = 0, int offsetY = 0) {
    for (size_t fila = 0; fila < FILAS; ++fila) {
        for (size_t columna = 0; columna < COLUMNAS; ++columna) {
            if (matriz[fila][columna] != 0 && matriz[fila][columna] != -1) {
                ubicar(columna + offsetX, fila + offsetY);
                asignarColor(matriz[fila][columna]);
                cout << char(219);
            }
        }
    }
}

extern bool logoMostrado;

void logoInicio() {
    if (!logoMostrado) {
        limpiar();
        dibujar(logo, 30, 5);
        logoMostrado = true;
        Sleep(1000);
        limpiar();
        dibujar(faz1, 0, 0);

        asignarColor(7);
    }
}

void mostrarFondo() {
    dibujar(faz1, 0, 0);
}


inline void limpiarRastro(int y) {
    ubicar(8, y);
    std::cout << "                                                                            ";
}

inline std::string leerTextoRobusto(int x, int y, std::string mensaje) {
    std::string input;
    while (true) {
        limpiarRastro(y);
        ubicar(x, y);
        std::cout << mensaje;
        std::getline(std::cin, input);

        if (!input.empty() && input.find_first_not_of(' ') != std::string::npos) {
            return input;
        }
    }
}

inline int leerNumeroRobusto(int x, int y, std::string mensaje, int maxVal = 0) {
    std::string input;
    int numero;
    while (true) {
        limpiarRastro(y);
        ubicar(x, y);
        std::cout << mensaje;
        std::getline(std::cin, input);

        if (input.empty()) continue;

        try {
            numero = std::stoi(input);
            if (numero >= 0 && (maxVal == 0 || numero <= maxVal)) {
                return numero;
            }
        }
        catch (...) {}

        ubicar(x, y + 1); asignarColor(4);
        std::cout << ">> Error: Entrada invalida.";
        asignarColor(7);
        Sleep(800);
        limpiarRastro(y + 1);
    }
}

inline bool leerOpcionSN(int x, int y, std::string mensaje) {
    std::string resp;
    while (true) {
        limpiarRastro(y);
        ubicar(x, y);
        std::cout << mensaje;
        std::getline(std::cin, resp);

        if (resp.empty()) continue;

        if (resp == "s" || resp == "S" || resp == "si" || resp == "SI") return true;
        if (resp == "n" || resp == "N" || resp == "no" || resp == "NO") return false;

        ubicar(x, y + 1); asignarColor(4);
        std::cout << ">> Error: Responde si o no.";
        asignarColor(7);
        Sleep(800);
        limpiarRastro(y + 1);
    }
}

inline string toLower(string s) {
    string res = s;
    for (char& c : res) c = tolower(c);
    return res;
}
inline string toUpper(string s) {
    string res = s;
    for (char& c : res) c = toupper(c);
    return res;
}