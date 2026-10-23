#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Cifrador.h"
#include "TablaHash.h"
#include "Render.h"
#include "utilidades2.h"

using namespace std;

class SistemaLogin {
private:
    TablaHash<string, string> tablaUsuarios;
    string archivoMaster;
    Cifrador cif;

    static const int p_iniciar = 2;
    static const int p_registrar = 1;
    static const int p_salir = 0;

    bool esNombreValidoWindows(string nombre) {
        if (nombre.empty()) return false;
        string invalidos = "\\/:*?\"<>|";
        for (char c : nombre) {
            if (invalidos.find(c) != string::npos) return false;
        }
        return true;
    }

    void cargarMasterUsuarios() {
        ifstream archivo(archivoMaster);
        if (!archivo.is_open()) return;

        string linea;
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;
            if (linea.back() == '\r') linea.pop_back();

            size_t posPipe = linea.find('|');
            if (posPipe != string::npos) {
                string user = linea.substr(0, posPipe);
                string passCifrada = linea.substr(posPipe + 1);
                string passReal = cif.desencriptar(passCifrada);
                tablaUsuarios.insertar(user, passReal);
            }
        }
        archivo.close();
    }

    void registrarEnMaster(string user, string pass) {
        ofstream archivo(archivoMaster, ios::app);
        if (archivo.is_open()) {
            archivo << user << "|" << cif.encriptar(pass) << "\n";
            archivo.close();
            tablaUsuarios.insertar(user, pass);
        }
    }

    void dibujarMenuLogin(int pisoActual) {
        iniciarBuffer();
        dibujar(fondoLogin, 0, 0);

        int btnX = 266;
        int btnY[3] = { 52, 69, 86 };

        dibujar(blg_iniciar, btnX, btnY[0]);
        dibujar(blg_registrar, btnX, btnY[1]);
        dibujar(blg_salir, btnX, btnY[2]);

        if (pisoActual == p_iniciar) {
            dibujar(blg_iniciarON, btnX, btnY[0]);
        }
        else if (pisoActual == p_registrar) {
            dibujar(blg_registrarON, btnX, btnY[1]);
        }
        else if (pisoActual == p_salir) {
            dibujar(blg_salirON, btnX, btnY[2]);
        }

        presentarBuffer();
    }

    void cambiarPiso(int pisoAnt, int pisoNuevo) {
        iniciarBuffer();

        int btnX = 266;
        int btnY[3] = { 52, 69, 86 };

        if (pisoAnt == p_iniciar) {
            eliminarMatriz(fondoLogin, obtenerAltoMatriz(blg_iniciarON), obtenerAnchoMatriz(blg_iniciarON), btnX, btnY[0]);
            dibujar(blg_iniciar, btnX, btnY[0]);
        }
        else if (pisoAnt == p_registrar) {
            eliminarMatriz(fondoLogin, obtenerAltoMatriz(blg_registrarON), obtenerAnchoMatriz(blg_registrarON), btnX, btnY[1]);
            dibujar(blg_registrar, btnX, btnY[1]);
        }
        else if (pisoAnt == p_salir) {
            eliminarMatriz(fondoLogin, obtenerAltoMatriz(blg_salirON), obtenerAnchoMatriz(blg_salirON), btnX, btnY[2]);
            dibujar(blg_salir, btnX, btnY[2]);
        }

        if (pisoNuevo == p_iniciar) {
            dibujar(blg_iniciarON, btnX, btnY[0]);
        }
        else if (pisoNuevo == p_registrar) {
            dibujar(blg_registrarON, btnX, btnY[1]);
        }
        else if (pisoNuevo == p_salir) {
            dibujar(blg_salirON, btnX, btnY[2]);
        }

        presentarBuffer();
    }

    string flujoIniciarSesion() {
        iniciarBuffer();
        dibujar(fl_usuario, 0, 0);
        presentarBuffer();

        int posX = 267;
        int posYusr = 73;
        int posYpass = 91;

        string usuario = motorUI.escribirLetraLinea(fl_usuario, posX, posYusr, 30);

        string passGuardada = tablaUsuarios.buscar(usuario, "NO_EXISTE");

        string password = motorUI.escribirLetraLinea(fl_usuario, posX, posYpass, 30);

        if (passGuardada != "NO_EXISTE" && passGuardada == password) {
            iniciarBuffer();
            presentarBuffer();
            return usuario;
        }
        else {
            Sleep(500);
            return "";
        }
    }

    void flujoRegistrarUsuario() {
        iniciarBuffer();
        dibujar(rl_usuario, 0, 0);
        presentarBuffer();

        int posX = 267;
        int posYusr = 73;
        int posYpass = 91;

        string usuario = motorUI.escribirLetraLinea(rl_usuario, posX, posYusr, 30);

        if (!esNombreValidoWindows(usuario)) {
            Sleep(500);
            return;
        }

        if (tablaUsuarios.buscar(usuario, "NO_EXISTE") != "NO_EXISTE") {
            Sleep(500);
            return;
        }

        string password = motorUI.escribirLetraLinea(rl_usuario, posX, posYpass, 30);

        registrarEnMaster(usuario, password);
        Sleep(500);
    }

public:
    SistemaLogin() {
        archivoMaster = "master_usuarios.txt";
        cargarMasterUsuarios();
    }

    string manejarLoginMenu() {
        int pisoActual = p_iniciar;
        bool enMenu = true;

        dibujarMenuLogin(pisoActual);

        while (enMenu) {
            int tecla = _getch();

            if (tecla == ESPECIAL) {
                tecla = _getch();

                if (tecla == ARRIBA) {
                    int pisoAnt = pisoActual;
                    pisoActual++;
                    if (pisoActual > p_iniciar) {
                        pisoActual = p_salir;
                    }
                    cambiarPiso(pisoAnt, pisoActual);
                }
                else if (tecla == ABAJO) {
                    int pisoAnt = pisoActual;
                    pisoActual--;
                    if (pisoActual < p_salir) {
                        pisoActual = p_iniciar;
                    }
                    cambiarPiso(pisoAnt, pisoActual);
                }
            }
            else if (tecla == ENTER) {
                if (pisoActual == p_iniciar) {
                    string usuario = flujoIniciarSesion();
                    if (usuario != "") {
                        return usuario;
                    }
                    dibujarMenuLogin(pisoActual);
                }
                else if (pisoActual == p_registrar) {
                    flujoRegistrarUsuario();
                    dibujarMenuLogin(pisoActual);
                }
                else if (pisoActual == p_salir) {
                    return "";
                }
            }
        }

        return "";
    }
};