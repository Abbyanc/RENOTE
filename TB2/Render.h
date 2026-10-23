#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <sstream>
#include <algorithm>
#include <winuser.h>
#include "Matrices.h"


#pragma comment(lib, "user32.lib") //bloq mayus

#define ENTER 13
#define BACKSPACE 8
#define ESC 27
#define ESPECIAL 224
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77

using std::cout;
using std::string;
using std::vector;
using std::cin;

typedef vector<vector<int>> vvi;

const string MAPA_COLORES[57] = {
    "\x1b[38;2;0;0;0m"
    "\x1b[38;2;0;0;0m",           // 0
    "\x1b[38;2;255;255;255m",     // 1
    "\x1b[38;2;255;0;0m",         // 2
    "\x1b[38;2;165;0;33m",        // 3
    "\x1b[38;2;242;121;30m",      // 4
    "\x1b[38;2;231;198;79m",      // 5
    "\x1b[38;2;255;255;1m",       // 6
    "\x1b[38;2;143;243;75m",      // 7
    "\x1b[38;2;5;255;65m",        // 8
    "\x1b[38;2;29;255;153m",      // 9
    "\x1b[38;2;8;252;246m",       // 10
    "\x1b[38;2;41;128;255m",      // 11
    "\x1b[38;2;17;5;255m",        // 12
    "\x1b[38;2;149;21;255m",      // 13
    "\x1b[38;2;255;29;239m",      // 14
    "\x1b[38;2;255;51;158m",      // 15
    "\x1b[38;2;151;152;183m",     // 16
    "\x1b[38;2;109;89;167m",      // 17
    "\x1b[38;2;1;2;120m",         // 18
    "\x1b[38;2;178;178;178m",     // 19
    "\x1b[38;2;62;3;119m",        // 20
    "\x1b[38;2;0;11;53m",         // 21
    "\x1b[38;2;148;169;216m",     // 22
    "\x1b[38;2;211;225;251m",     // 23
    "\x1b[38;2;251;244;207m",     // 24

    "\x1b[38;2;167;141;46m",      // 25
    "\x1b[38;2;208;142;107m",     // 26
    "\x1b[38;2;255;225;209m",     // 27
    "\x1b[38;2;132;92;71m",       // 28

    "\x1b[38;2;33;66;97m",        // 29
    "\x1b[38;2;25;70;127m",       // 30
    "\x1b[38;2;36;50;76m",        // 31
    "\x1b[38;2;17;37;62m",        // 32

    "\x1b[38;2;145;197;211m",     // 33
    "\x1b[38;2;224;240;213m",     // 34
    "\x1b[38;2;14;47;88m",        // 35
    "\x1b[38;2;2;2;26m",          // 36
    "\x1b[38;2;48;72;96m",        // 37
    "\x1b[38;2;92;92;92m",        // 38
    "\x1b[38;2;143;131;81m",      // 39

    "\x1b[38;2;4;114;248m",      // 40
    "\x1b[38;2;0;158;180m",      // 41
    "\x1b[38;2;0;133;100m",      // 42
    "\x1b[38;2;24;25;26m",       // 43
    "\x1b[38;2;28;43;66m",       // 44
    "\x1b[38;2;102;157;241m",    // 45
    "\x1b[38;2;206;207;210m",    // 46
    "\x1b[38;2;232;232;232m",    // 47

    "\x1b[38;2;31;31;33m",       // 48
    "\x1b[38;2;70;71;75m",       // 49
    "\x1b[38;2;20;34;56m",       // 50
    "\x1b[38;2;23;46;82m",       // 51
    "\x1b[38;2;36;37;40m",       // 52

    "\x1b[38;2;33;215;42m",       // 53
    "\x1b[38;2;81;163;85m",       // 54
    "\x1b[38;2;44;114;47m",       // 55
    "\x1b[38;2;255;79;79m",       // 56
};

#define RESET_COLOR "\x1b[0m"


static bool estaBloqMayusActivado() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

class UI {
private:
    static const int ANCHO_MAX = 400;
    static const int ALTO_MAX = 120;
    static const int ANCHO_LETRA = 7;
    static const int ALTO_LETRA = 9;
    static const int ESPACIO_LINEA = 9;
    static const int ANCHO_ESPACIO = 3;

    std::stringstream bufferSalida;

    int colorActualBuffer;
    bool usarBuffer;

    void _posicionar(int x, int y);
    void _aplicarColor(int color);

public:
    UI(bool conDobleBuffer = true);

    void habilitarANSI();
    void inicializarConsola(int ancho = 400, int alto = 120);

    void dibujarMatriz(const vvi& mat, int posX = 0, int posY = 0);
    void eliminarMatriz(const vvi& fondo, int filas, int cols, int posX, int posY);

    void imprimirTexto(const string& txt, int x, int y);
    void limpiarTexto(const string& txt, int x, int y, const vvi& fondo);
    string escribirLetraLinea(const vvi& fondo, int posXini, int posYini, int maxChars = 50);
    string escribirLetraMultilinea(const vvi& fondo, int posXini, int posYini, int maxChars = 500);

    int calcularAnchoTexto(const string& txt);
    void limpiarPantalla();
    void establecerColorTexto(int color);
    void posicionarCursor(int x, int y);
    void pausar();

    void iniciarBuffer();
    void presentarBuffer();
    void limpiarBuffer();
};



UI::UI(bool conDobleBuffer)
    : usarBuffer(conDobleBuffer), colorActualBuffer(-1) {
}

void UI::habilitarANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

void UI::inicializarConsola(int ancho, int alto) {
    habilitarANSI();
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { (SHORT)ancho, (SHORT)alto };
    SetConsoleScreenBufferSize(hOut, coord);
    SMALL_RECT rect = { 0, 0, (SHORT)(ancho - 1), (SHORT)(alto - 1) };
    SetConsoleWindowInfo(hOut, TRUE, &rect);
}

void UI::_posicionar(int x, int y) {
    if (usarBuffer) {
        bufferSalida << "\x1b[" << y << ";" << x << "H";
    }
    else {
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
}

void UI::_aplicarColor(int color) {
    if (color < 0 || color >= 57) return;

    if (usarBuffer) {
        if (color != colorActualBuffer) {
            bufferSalida << MAPA_COLORES[color];
            colorActualBuffer = color;
        }
    }
    else {
        cout << MAPA_COLORES[color];
    }
}

void UI::dibujarMatriz(const vvi& mat, int posX, int posY) {
    int colorAnt = -2;

    for (int i = 0; i < mat.size(); i++) {
        bool cursorListo = false;

        for (int j = 0; j < mat[i].size(); j++) {
            int color = mat[i][j];

            if (color == -1) {
                cursorListo = false;
                continue;
            }

            if (!cursorListo) {
                _posicionar(j + posX, i + posY);
                cursorListo = true;
            }

            if (color != colorAnt) {
                _aplicarColor(color);
                colorAnt = color;
            }

            if (usarBuffer) {
                bufferSalida << char(219);
            }
            else {
                cout << char(219);
            }
        }
    }

    if (!usarBuffer) {
        cout << RESET_COLOR;
    }
}

void UI::eliminarMatriz(const vvi& fondo, int filas, int cols, int posX, int posY) {
    int cAnt = -2;

    int fM = fondo.size();
    int cM = fM > 0 ? fondo[0].size() : 0;

    for (int i = 0; i < filas; i++) {
        int fN = posY + i;
        if (fN >= fM) break;

        _posicionar(posX, fN);

        for (int j = 0; j < cols; j++) {
            int cN = posX + j;
            if (cN >= cM) break;

            int cA = fondo[fN][cN];

            if (cA != cAnt) {
                _aplicarColor(cA);
                cAnt = cA;
            }

            if (usarBuffer) {
                bufferSalida << char(219);
            }
            else {
                cout << char(219);
            }
        }
    }

    if (!usarBuffer) {
        cout << RESET_COLOR;
    }
}

void _renderLetra(char a, int posX, int posY, UI& motor);

void UI::imprimirTexto(const string& txt, int x, int y) {
    int posX = x;
    int posY = y;

    for (int i = 0; i < txt.size(); i++) {
        char temp = txt[i];

        if (temp == ' ') {
            posX += ANCHO_ESPACIO;
        }
        else {
            _renderLetra(temp, posX, posY, *this);
            posX += ANCHO_LETRA;
        }
        Sleep(5);
    }
}

int UI::calcularAnchoTexto(const string& txt) {
    int ancho = 0;
    for (int i = 0; i < txt.size(); i++) {
        ancho += (txt[i] == ' ') ? ANCHO_ESPACIO : ANCHO_LETRA;
    }
    return ancho;
}

void UI::limpiarTexto(const string& txt, int x, int y, const vvi& fondo) {
    int ancho = calcularAnchoTexto(txt);
    eliminarMatriz(fondo, ALTO_LETRA, ancho, x, y);
}

string UI::escribirLetraLinea(const vvi& fondo, int posXini, int posYini, int maxChars) {
    int tecla;
    vector<int> letras;
    int posX = posXini;
    int posY = posYini;
    string resultado = "";

    while (true) {
        tecla = _getch();

        if (tecla == ENTER) {
            for (int i = 0; i < letras.size(); i++) {
                resultado += char(letras[i]);
            }
            break;
        }
        else if (tecla == BACKSPACE) {
            if (letras.size() > 0) {
                int ulChar = letras.back();
                int retroceso = (ulChar == ' ') ? ANCHO_ESPACIO : ANCHO_LETRA;
                
                posX -= retroceso;
                if (posX >= 0) {
                    int borCols = (ulChar == ' ') ? ANCHO_ESPACIO : 9;
                    eliminarMatriz(fondo, ALTO_LETRA, borCols, posX, posY);
                    
                    letras.pop_back();

                    if (letras.size() > 0) {
                        int lastC = letras.back();
                        if (lastC != ' ') {
                            int prevPos = posX - ((lastC == ' ') ? ANCHO_ESPACIO : ANCHO_LETRA);
                            _renderLetra(lastC, prevPos, posY, *this);
                        }
                    }
                    
                    presentarBuffer();
                } else {
                    letras.pop_back();
                }
            }
        }
        else {
            if (letras.size() < maxChars) {
                int avance = (tecla == ' ') ? ANCHO_ESPACIO : ANCHO_LETRA;
                if (tecla != ' ') {
                    _renderLetra(tecla, posX, posY, *this);
                    presentarBuffer();
                }
                posX += avance;
                letras.push_back(tecla);
            }
        }
    }

    return resultado;
}

string UI::escribirLetraMultilinea(const vvi& fondo, int posXini, int posYini, int maxChars) {
    int tecla;
    vector<int> letras;
    int posX = posXini;
    int posY = posYini;
    bool seguir = true;
    string resultado = "";

    while (seguir) {
        tecla = _getch();

        if (tecla == ESC) {
            seguir = false;
            for (int i = 0; i < letras.size(); i++) {
                resultado += char(letras[i]);
            }
        }
        else if (tecla == ENTER) {
            posY += ESPACIO_LINEA;
            posX = posXini;
            letras.push_back('\n');
        }
        else if (tecla == BACKSPACE) {
            if (letras.size() > 0) {
                int ulChar = letras.back();
                if (ulChar != '\n') {
                    int retroceso = (ulChar == ' ') ? ANCHO_ESPACIO : ANCHO_LETRA;
                    
                    posX -= retroceso;
                    if (posX >= 0) {
                        int borCols = (ulChar == ' ') ? ANCHO_ESPACIO : 9;
                        eliminarMatriz(fondo, ALTO_LETRA, borCols, posX, posY);
                        
                        letras.pop_back();

                        if (letras.size() > 0) {
                            int lastC = letras.back();
                            if (lastC != '\n' && lastC != ' ') {
                                int prevPos = posX - ((lastC == ' ') ? ANCHO_ESPACIO : ANCHO_LETRA);
                                _renderLetra(lastC, prevPos, posY, *this);
                            }
                        }
                        
                        presentarBuffer();
                    } else {
                        letras.pop_back();
                    }
                } else {
                    letras.pop_back();
                }
            }
        }
        else {
            if (letras.size() < maxChars) {
                int avance = (tecla == ' ') ? ANCHO_ESPACIO : ANCHO_LETRA;
                if (posX + avance > ANCHO_MAX) {
                    posY += ESPACIO_LINEA;
                    posX = posXini;
                    letras.push_back('\n');
                }
                else if (tecla != ' ') {
                    _renderLetra(tecla, posX, posY, *this);
                    presentarBuffer();
                }
                posX += avance;
                letras.push_back(tecla);

                if (letras.size() >= 4) {
                    int size = letras.size();
                    if (letras[size - 4] == '.' && letras[size - 3] == 'F' &&
                        letras[size - 2] == 'I' && letras[size - 1] == 'N') {
                        seguir = false;
                        resultado = "";
                        for (int i = 0; i < size - 4; i++) {
                            resultado += char(letras[i]);
                        }
                    }
                }
            }
        }
    }

    return resultado;
}

void UI::limpiarPantalla() {
    system("cls");
    colorActualBuffer = -1;
}

void UI::establecerColorTexto(int color) {
    if (!usarBuffer) {
        cout << MAPA_COLORES[color];
    }
}

void UI::posicionarCursor(int x, int y) {
    if (!usarBuffer) {
        _posicionar(x, y);
    }
}

void UI::pausar() {
    cout << "\nPresiona ENTER para continuar...";
    cin.ignore();
    cin.get();
}

void UI::iniciarBuffer() {
    bufferSalida.str("");
    bufferSalida.clear();
    colorActualBuffer = -1;
}

void UI::presentarBuffer() {
    if (usarBuffer && bufferSalida.tellp() > 0) {
        cout << bufferSalida.str();
        cout.flush();
        iniciarBuffer();
    }
}

void UI::limpiarBuffer() {
    bufferSalida.str("");
    bufferSalida.clear();
    colorActualBuffer = -1;
}


void _renderLetra(char a, int posX, int posY, UI& motor) {

    bool esMinuscula = (a >= 'a' && a <= 'z');
    char c = toupper(a);

    if (esMinuscula) {
        switch (c) {
        case 'A': motor.dibujarMatriz(mA, posX, posY); break;
        case 'B': motor.dibujarMatriz(mB, posX, posY); break;
        case 'C': motor.dibujarMatriz(mC, posX, posY); break;
        case 'D': motor.dibujarMatriz(mD, posX, posY); break;
        case 'E': motor.dibujarMatriz(mE, posX, posY); break;
        case 'F': motor.dibujarMatriz(mF, posX, posY); break;
        case 'G': motor.dibujarMatriz(mG, posX, posY); break;
        case 'H': motor.dibujarMatriz(mH, posX, posY); break;
        case 'I': motor.dibujarMatriz(mI, posX, posY); break;
        case 'J': motor.dibujarMatriz(mJ, posX, posY); break;
        case 'K': motor.dibujarMatriz(mK, posX, posY); break;
        case 'L': motor.dibujarMatriz(mL, posX, posY); break;
        case 'M': motor.dibujarMatriz(mM, posX, posY); break;
        case 'N': motor.dibujarMatriz(mN, posX, posY); break;
        case 'O': motor.dibujarMatriz(mO, posX, posY); break;
        case 'P': motor.dibujarMatriz(mP, posX, posY); break;
        case 'Q': motor.dibujarMatriz(mQ, posX, posY); break;
        case 'R': motor.dibujarMatriz(mR, posX, posY); break;
        case 'S': motor.dibujarMatriz(mS, posX, posY); break;
        case 'T': motor.dibujarMatriz(mT, posX, posY); break;
        case 'U': motor.dibujarMatriz(mU, posX, posY); break;
        case 'V': motor.dibujarMatriz(mV, posX, posY); break;
        case 'W': motor.dibujarMatriz(mW, posX, posY); break;
        case 'X': motor.dibujarMatriz(mX, posX, posY); break;
        case 'Y': motor.dibujarMatriz(mY, posX, posY); break;
        case 'Z': motor.dibujarMatriz(mZ, posX, posY); break;
        case '1': motor.dibujarMatriz(uno, posX, posY); break;
        case '2': motor.dibujarMatriz(dos, posX, posY); break;
        case '3': motor.dibujarMatriz(tres, posX, posY); break;
        case '4': motor.dibujarMatriz(cuatro, posX, posY); break;
        case '5': motor.dibujarMatriz(cinco, posX, posY); break;
        case '6': motor.dibujarMatriz(seis, posX, posY); break;
        case '7': motor.dibujarMatriz(siete, posX, posY); break;
        case '8': motor.dibujarMatriz(ocho, posX, posY); break;
        case '9': motor.dibujarMatriz(nueve, posX, posY); break;
        case '0': motor.dibujarMatriz(cero, posX, posY); break;
        case '.': motor.dibujarMatriz(punto, posX, posY); break;
        case '¿': motor.dibujarMatriz(preguntaA, posX, posY); break;
        case '?': motor.dibujarMatriz(preguntaB, posX, posY); break;
        case ',': motor.dibujarMatriz(coma, posX, posY); break;
        case ';': motor.dibujarMatriz(puntoComa, posX, posY); break;
        case '-': motor.dibujarMatriz(guion, posX, posY); break;
        case '_': motor.dibujarMatriz(guionBajo, posX, posY); break;
        case '=': motor.dibujarMatriz(igual, posX, posY); break;
        case '/': motor.dibujarMatriz(slash, posX, posY); break;
        case '(': motor.dibujarMatriz(parentesisA, posX, posY); break;
        case ')': motor.dibujarMatriz(parentesisB, posX, posY); break;
        case '|': motor.dibujarMatriz(tab, posX, posY); break;
        case ':': motor.dibujarMatriz(dospuntos, posX, posY); break;
        case '>': motor.dibujarMatriz(mayor, posX, posY); break;
        case '<': motor.dibujarMatriz(menor, posX, posY); break;
        case '$': motor.dibujarMatriz(dolar, posX, posY); break;
        case '#': motor.dibujarMatriz(michi, posX, posY); break;
        case '\'': motor.dibujarMatriz(apost, posX, posY); break;
        case '\!': motor.dibujarMatriz(exclamacion, posX, posY); break;
        case '"': motor.dibujarMatriz(comillas, posX, posY); break;
        case '%': motor.dibujarMatriz(porcent, posX, posY); break;
        case '&': motor.dibujarMatriz(amper, posX, posY); break;
        case '@': motor.dibujarMatriz(arroba, posX, posY); break;
        case '[': motor.dibujarMatriz(corcheteA, posX, posY); break;
        case ']': motor.dibujarMatriz(corcheteB, posX, posY); break;
        }
    }
    else {
        switch (c) {
        case 'A': motor.dibujarMatriz(A, posX, posY); break;
        case 'B': motor.dibujarMatriz(B, posX, posY); break;
        case 'C': motor.dibujarMatriz(C, posX, posY); break;
        case 'D': motor.dibujarMatriz(D, posX, posY); break;
        case 'E': motor.dibujarMatriz(E, posX, posY); break;
        case 'F': motor.dibujarMatriz(F, posX, posY); break;
        case 'G': motor.dibujarMatriz(G, posX, posY); break;
        case 'H': motor.dibujarMatriz(H, posX, posY); break;
        case 'I': motor.dibujarMatriz(I, posX, posY); break;
        case 'J': motor.dibujarMatriz(J, posX, posY); break;
        case 'K': motor.dibujarMatriz(K, posX, posY); break;
        case 'L': motor.dibujarMatriz(L, posX, posY); break;
        case 'M': motor.dibujarMatriz(M, posX, posY); break;
        case 'N': motor.dibujarMatriz(N, posX, posY); break;
        case 'O': motor.dibujarMatriz(O, posX, posY); break;
        case 'P': motor.dibujarMatriz(P, posX, posY); break;
        case 'Q': motor.dibujarMatriz(Q, posX, posY); break;
        case 'R': motor.dibujarMatriz(R, posX, posY); break;
        case 'S': motor.dibujarMatriz(S, posX, posY); break;
        case 'T': motor.dibujarMatriz(T, posX, posY); break;
        case 'U': motor.dibujarMatriz(U, posX, posY); break;
        case 'V': motor.dibujarMatriz(V, posX, posY); break;
        case 'W': motor.dibujarMatriz(W, posX, posY); break;
        case 'X': motor.dibujarMatriz(X, posX, posY); break;
        case 'Y': motor.dibujarMatriz(Y, posX, posY); break;
        case 'Z': motor.dibujarMatriz(Z, posX, posY); break;
        case '1': motor.dibujarMatriz(uno, posX, posY); break;
        case '2': motor.dibujarMatriz(dos, posX, posY); break;
        case '3': motor.dibujarMatriz(tres, posX, posY); break;
        case '4': motor.dibujarMatriz(cuatro, posX, posY); break;
        case '5': motor.dibujarMatriz(cinco, posX, posY); break;
        case '6': motor.dibujarMatriz(seis, posX, posY); break;
        case '7': motor.dibujarMatriz(siete, posX, posY); break;
        case '8': motor.dibujarMatriz(ocho, posX, posY); break;
        case '9': motor.dibujarMatriz(nueve, posX, posY); break;
        case '0': motor.dibujarMatriz(cero, posX, posY); break;
        case '.': motor.dibujarMatriz(punto, posX, posY); break;
        case '¿': motor.dibujarMatriz(preguntaA, posX, posY); break;
        case '?': motor.dibujarMatriz(preguntaB, posX, posY); break;
        case ',': motor.dibujarMatriz(coma, posX, posY); break;
        case ';': motor.dibujarMatriz(puntoComa, posX, posY); break;
        case '-': motor.dibujarMatriz(guion, posX, posY); break;
        case '_': motor.dibujarMatriz(guionBajo, posX, posY); break;
        case '=': motor.dibujarMatriz(igual, posX, posY); break;
        case '/': motor.dibujarMatriz(slash, posX, posY); break;
        case '(': motor.dibujarMatriz(parentesisA, posX, posY); break;
        case ')': motor.dibujarMatriz(parentesisB, posX, posY); break;
        case '|': motor.dibujarMatriz(tab, posX, posY); break;
        case ':': motor.dibujarMatriz(dospuntos, posX, posY); break;
        case '>': motor.dibujarMatriz(mayor, posX, posY); break;
        case '<': motor.dibujarMatriz(menor, posX, posY); break;
        case '$': motor.dibujarMatriz(dolar, posX, posY); break;
        case '#': motor.dibujarMatriz(michi, posX, posY); break;
        case '\'': motor.dibujarMatriz(apost, posX, posY); break;
        case '\!': motor.dibujarMatriz(exclamacion, posX, posY); break;
        case '"': motor.dibujarMatriz(comillas, posX, posY); break;
        case '%': motor.dibujarMatriz(porcent, posX, posY); break;
        case '&': motor.dibujarMatriz(amper, posX, posY); break;
        case '@': motor.dibujarMatriz(arroba, posX, posY); break;
        case '[': motor.dibujarMatriz(corcheteA, posX, posY); break;
        case ']': motor.dibujarMatriz(corcheteB, posX, posY); break;
        }
    }
}

extern UI motorUI;

inline int calcularAnchoPixels(const string& txt) {
    int anchoTotal = 0;
    for (char c : txt) {
        if (c == ' ') anchoTotal += 3;
        else anchoTotal += 7;
    }
    return anchoTotal;
}

inline void renderTextoRapido(const string& txt, int x, int y) {
    int posX = x;
    int posY = y;

    for (int ind = 0; ind < txt.length(); ind++) {
        char tempCar = txt[ind];

        if (tempCar == ' ') {
            posX = posX + 3;
        }
        else {
            extern void _renderLetra(char a, int posX, int posY, UI & motor);
            _renderLetra(tempCar, posX, posY, motorUI);
            posX = posX + 7;
        }
    }
}