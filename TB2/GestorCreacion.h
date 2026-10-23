#pragma once
#include <iostream>
#include <string>
#include "Utilidades.h"
#include "Boveda.h"
#include "Nota.h"
#include "NotaSecreta.h" 
#include "Tarea.h"
#include "Render.h"
#include "utilidades2.h"
using namespace std;
typedef unsigned int uint;


class GestorCreacion {
public:
    static void flujoCrearNota(Boveda& miVault, uint& contadorIdsNotas) {

        motorUI.limpiarPantalla();
        motorUI.iniciarBuffer();
        motorUI.dibujarMatriz(secuenciaNota, 0, 0);
        renderTextoRapido("Ingresa el titulo de tu nota:", 80, 6);
        motorUI.presentarBuffer();

        string tituloNota = motorUI.escribirLetraLinea(secuenciaNota, 80, 18, 40);

        motorUI.iniciarBuffer();
        renderTextoRapido("Proteger con contrasena? (SI/NO):", 80, 35);
        renderTextoRapido(">> Respuesta: ", 80, 47);
        motorUI.presentarBuffer();

        string respSecreta = "";
        bool esSecreta = false;

        string textoRespX = ">> Respuesta: ";
        int posEscribirX = 80 + calcularAnchoPixels(textoRespX);

        while (true) {
            respSecreta = motorUI.escribirLetraLinea(secuenciaNota, posEscribirX, 47, 5);

            string respMinus = "";
            for (int indG = 0; indG < respSecreta.length(); indG++) {
                respMinus = respMinus + (char)tolower(respSecreta[indG]);
            }

            if (respMinus == "si" || respMinus == "s") {
                esSecreta = true;
                break;
            }
            else if (respMinus == "no" || respMinus == "n") {
                esSecreta = false;
                break;
            }
            else {
                motorUI.iniciarBuffer();
                motorUI.eliminarMatriz(secuenciaNota, 9, 300, posEscribirX, 47);
                motorUI.presentarBuffer();
            }
        }

        string claveNota = "";

        if (esSecreta == true) {
            while (true) {
                motorUI.iniciarBuffer();
                renderTextoRapido("Ingresa tu contrasena:", 80, 64);
                motorUI.presentarBuffer();

                string ingresoClave1 = motorUI.escribirLetraLinea(secuenciaNota, 80, 76, 20);

                motorUI.iniciarBuffer();
                renderTextoRapido("Confirma tu contrasena:", 80, 93);
                motorUI.presentarBuffer();

                string ingresoClave2 = motorUI.escribirLetraLinea(secuenciaNota, 80, 105, 20);

                if (ingresoClave1 == ingresoClave2) {
                    claveNota = ingresoClave1;
                    break;
                }
                else {
                    motorUI.iniciarBuffer();
                    motorUI.eliminarMatriz(secuenciaNota, 9, 300, 80, 93);
                    motorUI.eliminarMatriz(secuenciaNota, 9, 300, 80, 105);
                    renderTextoRapido("No coinciden, intentalo de nuevo", 80, 105);
                    motorUI.presentarBuffer();

                    Sleep(1500);

                    motorUI.iniciarBuffer();
                    motorUI.eliminarMatriz(secuenciaNota, 9, 300, 80, 76);
                    motorUI.eliminarMatriz(secuenciaNota, 9, 300, 80, 105);
                    motorUI.presentarBuffer();
                }
            }
        }

        motorUI.limpiarPantalla();
        motorUI.iniciarBuffer();
        motorUI.dibujarMatriz(fondoEscribirNota, 0, 0);
        renderTextoRapido(tituloNota, 105, 2);
        renderTextoRapido("Escribir fin en la ultima linea para guardar", 5, 17);
        motorUI.presentarBuffer();

        int coordFilaX[8] = { 80, 80, 80, 80, 80, 80, 80, 80 };
        int coordFilaY[8] = { 34, 44, 54, 64, 74, 84, 94, 104 };

        int filaActualN = 0;
        int cursXAct = coordFilaX[filaActualN];
        int cursYAct = coordFilaY[filaActualN];

        string textNoteTotal = "";
        string textoEnLn = "";

        while (true) {
            int botPulsado = _getch();

            if (botPulsado == ENTER) {
                if (textoEnLn == "fin") {
                    break;
                }
                textNoteTotal = textNoteTotal + textoEnLn + "\n";
                textoEnLn = "";

                filaActualN = filaActualN + 1;
                if (filaActualN > 7) {
                    break;
                }
                cursXAct = coordFilaX[filaActualN];
                cursYAct = coordFilaY[filaActualN];
            }
            else if (botPulsado == BACKSPACE) {
                if (textoEnLn != "") {
                    int ultimCar = textoEnLn.back();
                    int valorDescuento = 0;
                    int mtrizLimpiar = 0;

                    if (ultimCar == ' ') {
                        valorDescuento = 3;
                        mtrizLimpiar = 3;
                    }
                    else {
                        valorDescuento = 7;
                        mtrizLimpiar = 9;
                    }

                    cursXAct = cursXAct - valorDescuento;

                    motorUI.iniciarBuffer();
                    motorUI.eliminarMatriz(fondoEscribirNota, 9, mtrizLimpiar, cursXAct, cursYAct);
                    motorUI.presentarBuffer();

                    textoEnLn.pop_back();
                }
            }
            else if (botPulsado >= 32 && botPulsado <= 126) {
                int anLetraTmp = 0;
                if (botPulsado == ' ') {
                    anLetraTmp = 3;
                }
                else {
                    anLetraTmp = 7;
                }

                if (cursXAct + anLetraTmp <= 342) {
                    textoEnLn = textoEnLn + (char)botPulsado;

                    motorUI.iniciarBuffer();
                    if (botPulsado != ' ') {
                        extern void _renderLetra(char a, int posX, int posY, UI & motor);
                        _renderLetra((char)botPulsado, cursXAct, cursYAct, motorUI);
                    }
                    motorUI.presentarBuffer();

                    cursXAct = cursXAct + anLetraTmp;
                }
                else {
                    textNoteTotal = textNoteTotal + textoEnLn + "\n";
                    textoEnLn = "";
                    textoEnLn = textoEnLn + (char)botPulsado;

                    filaActualN = filaActualN + 1;
                    if (filaActualN > 7) {
                        break;
                    }
                    cursXAct = coordFilaX[filaActualN];
                    cursYAct = coordFilaY[filaActualN];

                    motorUI.iniciarBuffer();
                    if (botPulsado != ' ') {
                        extern void _renderLetra(char a, int posX, int posY, UI & motor);
                        _renderLetra((char)botPulsado, cursXAct, cursYAct, motorUI);
                    }
                    motorUI.presentarBuffer();

                    cursXAct = cursXAct + anLetraTmp;
                }
            }
        }

        if (textoEnLn != "fin" && textoEnLn != "") {
            textNoteTotal = textNoteTotal + textoEnLn;
        }

        Nota* nuevaNotaLista = nullptr;
        if (esSecreta == true) {
            nuevaNotaLista = new NotaSecreta(contadorIdsNotas, tituloNota, textNoteTotal, claveNota);
        }
        else {
            nuevaNotaLista = new Nota(contadorIdsNotas, tituloNota, textNoteTotal);
        }
        contadorIdsNotas = contadorIdsNotas + 1;

        bool seguirEtiquetas = true;

        while (seguirEtiquetas == true) {
            motorUI.limpiarPantalla();
            motorUI.iniciarBuffer();
            motorUI.dibujarMatriz(secuenciaNota, 0, 0);
            renderTextoRapido("Agregar etiqueta? (SI/NO):", 80, 6);
            motorUI.presentarBuffer();

            string resAgEtiq = motorUI.escribirLetraLinea(secuenciaNota, 80, 18, 5);
            string resEtiqMin = "";
            for (int kE = 0; kE < resAgEtiq.length(); kE++) {
                resEtiqMin = resEtiqMin + (char)tolower(resAgEtiq[kE]);
            }

            if (resEtiqMin == "si" || resEtiqMin == "s") {
                Lista<Etiqueta*, nullptr> listaEtiquetasVal;
                miVault.extraerEtiquetasUnicas(listaEtiquetasVal);

                bool loopPaginas = true;
                int pagEtiqAct = 0;

                while (loopPaginas == true) {
                    motorUI.iniciarBuffer();
                    motorUI.eliminarMatriz(secuenciaNota, 80, 300, 80, 35);

                    renderTextoRapido("ETIQUETAS DISPONIBLES (flechas <> para ver mas):", 80, 35);

                    int cantTotalEtiq = listaEtiquetasVal.longitud();
                    int limitePagsEtq = (cantTotalEtiq + 5) / 6;

                    if (limitePagsEtq == 0) { limitePagsEtq = 1; }
                    if (pagEtiqAct >= limitePagsEtq) { pagEtiqAct = limitePagsEtq - 1; }
                    if (pagEtiqAct < 0) { pagEtiqAct = 0; }

                    if (cantTotalEtiq == 0) {
                        renderTextoRapido("Aun no hay etiquetas disponibles", 85, 51);
                    }
                    else {
                        int indexInic = pagEtiqAct * 6;
                        int indexFinal = indexInic + 6;
                        if (indexFinal > cantTotalEtiq) { indexFinal = cantTotalEtiq; }

                        int vxEId[] = { 85, 85, 85, 212, 212, 212 };
                        int vyEId[] = { 51, 70, 89, 51, 70, 89 };
                        int vxMtx[] = { 104, 104, 104, 231, 231, 231 };
                        int vyMtx[] = { 48, 67, 86, 48, 67, 86 };
                        int vxTtx[] = { 107, 107, 107, 234, 234, 234 };
                        int vyTtx[] = { 51, 70, 89, 51, 70, 89 };

                        int itemPosActual = 0;
                        for (int iterEnL = indexInic; iterEnL < indexFinal; iterEnL++) {
                            Etiqueta* puntEtiq = listaEtiquetasVal.obtenerPorIndice(iterEnL);

                            string digIndICE = to_string(iterEnL + 1);
                            renderTextoRapido(digIndICE, vxEId[itemPosActual], vyEId[itemPosActual]);

                            string nmC = puntEtiq->getColor();
                            if (nmC == "AZUL") { motorUI.dibujarMatriz(etAzul, vxMtx[itemPosActual], vyMtx[itemPosActual]); }
                            else if (nmC == "VERDE") { motorUI.dibujarMatriz(etVerde, vxMtx[itemPosActual], vyMtx[itemPosActual]); }
                            else if (nmC == "GRIS") { motorUI.dibujarMatriz(etGris, vxMtx[itemPosActual], vyMtx[itemPosActual]); }
                            else if (nmC == "ROJO") { motorUI.dibujarMatriz(etRojo, vxMtx[itemPosActual], vyMtx[itemPosActual]); }
                            else if (nmC == "AMARILLO") { motorUI.dibujarMatriz(etAmarillo, vxMtx[itemPosActual], vyMtx[itemPosActual]); }
                            else if (nmC == "MORADO") { motorUI.dibujarMatriz(etMorado, vxMtx[itemPosActual], vyMtx[itemPosActual]); }
                            else { motorUI.dibujarMatriz(etGris, vxMtx[itemPosActual], vyMtx[itemPosActual]); }

                            string nmE = puntEtiq->getNombre();
                            if (nmE.length() > 12) {
                                nmE = nmE.substr(0, 12);
                            }
                            renderTextoRapido(nmE, vxTtx[itemPosActual], vyTtx[itemPosActual]);

                            itemPosActual = itemPosActual + 1;
                        }
                    }

                    renderTextoRapido("Elige un numero (0 para crear): ", 79, 107);
                    motorUI.presentarBuffer();

                    int numEliGido = -1;
                    string numIngreTx = "";

                    string textoBaseP = "Elige un numero (0 para crear): ";
                    int anchBasTex = 79 + calcularAnchoPixels(textoBaseP);

                    while (true) {
                        int tclOprimida = _getch();

                        if (tclOprimida == ESPECIAL) {
                            int tclDir = _getch();
                            if (tclDir == IZQUIERDA) {
                                if (pagEtiqAct > 0) {
                                    pagEtiqAct = pagEtiqAct - 1;
                                }
                                break;
                            }
                            else if (tclDir == DERECHA) {
                                pagEtiqAct = pagEtiqAct + 1;
                                break;
                            }
                        }
                        else if (tclOprimida == ENTER) {
                            try {
                                numEliGido = stoi(numIngreTx);
                            }
                            catch (...) {
                                numEliGido = -1;
                            }

                            if (numEliGido > 0 && numEliGido <= cantTotalEtiq) {
                                Etiqueta* flSel = listaEtiquetasVal.obtenerPorIndice(numEliGido - 1);
                                nuevaNotaLista->agregarEtiqueta(flSel->getNombre(), flSel->getColor());
                                loopPaginas = false;
                                break;
                            }
                            else if (numEliGido == 0 && numIngreTx != "") {
                                motorUI.limpiarPantalla();
                                motorUI.iniciarBuffer();
                                motorUI.dibujarMatriz(secuenciaNota, 0, 0);
                                renderTextoRapido("Nombre de la etiqueta:", 80, 6);
                                motorUI.presentarBuffer();

                                string inpNomCr = motorUI.escribirLetraLinea(secuenciaNota, 80, 18, 12);
                                string cpNmFq = "";
                                for (int bUq = 0; bUq < inpNomCr.length(); bUq++) {
                                    cpNmFq = cpNmFq + (char)toupper(inpNomCr[bUq]);
                                }

                                bool loopColorVerif = true;
                                string endColorMay = "";

                                while (loopColorVerif == true) {
                                    motorUI.iniciarBuffer();
                                    motorUI.eliminarMatriz(secuenciaNota, 30, 300, 80, 35);
                                    renderTextoRapido("Azul, Verde, Gris, Rojo, Amarillo, Morado", 80, 35);
                                    renderTextoRapido("Escribe el color: ", 80, 47);
                                    motorUI.presentarBuffer();

                                    string msgEsCol = "Escribe el color: ";
                                    int aXeCT = 80 + calcularAnchoPixels(msgEsCol);

                                    string txtColorIn = motorUI.escribirLetraLinea(secuenciaNota, aXeCT, 47, 10);
                                    string colMinTest = "";
                                    for (int eF = 0; eF < txtColorIn.length(); eF++) {
                                        colMinTest = colMinTest + (char)tolower(txtColorIn[eF]);
                                    }

                                    if (colMinTest == "azul" || colMinTest == "verde" || colMinTest == "gris" || colMinTest == "rojo" || colMinTest == "amarillo" || colMinTest == "morado") {
                                        for (int rQ = 0; rQ < colMinTest.length(); rQ++) {
                                            endColorMay = endColorMay + (char)toupper(colMinTest[rQ]);
                                        }
                                        nuevaNotaLista->agregarEtiqueta(cpNmFq, endColorMay);
                                        loopColorVerif = false;
                                    }
                                    else {
                                        motorUI.iniciarBuffer();
                                        motorUI.eliminarMatriz(secuenciaNota, 9, 300, 80, 47);
                                        renderTextoRapido("color no entontrado", 80, 47);
                                        motorUI.presentarBuffer();
                                        Sleep(1000);
                                    }
                                }
                                loopPaginas = false;
                                break;
                            }
                            else {
                                motorUI.iniciarBuffer();
                                motorUI.eliminarMatriz(secuenciaNota, 9, 300, 79, 107);
                                renderTextoRapido("opcion invalida", 79, 107);
                                motorUI.presentarBuffer();
                                Sleep(1200);

                                motorUI.iniciarBuffer();
                                motorUI.eliminarMatriz(secuenciaNota, 9, 300, 79, 107);
                                renderTextoRapido("Elige un numero (0 para crear): ", 79, 107);
                                motorUI.presentarBuffer();
                                numIngreTx = "";
                            }
                        }
                        else if (tclOprimida == BACKSPACE) {
                            if (numIngreTx != "") {
                                int qUlT = numIngreTx.back();
                                int restAncM = 0;
                                int grBorrM = 0;

                                if (qUlT == ' ') {
                                    restAncM = 3;
                                    grBorrM = 3;
                                }
                                else {
                                    restAncM = 7;
                                    grBorrM = 9;
                                }

                                int limBorX = anchBasTex + calcularAnchoPixels(numIngreTx) - restAncM;

                                motorUI.iniciarBuffer();
                                motorUI.eliminarMatriz(secuenciaNota, 9, grBorrM, limBorX, 107);
                                numIngreTx.pop_back();

                                if (numIngreTx != "") {
                                    int uLtxH = numIngreTx.back();
                                    if (uLtxH != ' ' && uLtxH != '\n') {
                                        int atResLx = 0;
                                        if (uLtxH == ' ') {
                                            atResLx = 3;
                                        }
                                        else {
                                            atResLx = 7;
                                        }
                                        extern void _renderLetra(char a, int posX, int posY, UI & motor);
                                        _renderLetra((char)uLtxH, limBorX - atResLx, 107, motorUI);
                                    }
                                }
                                motorUI.presentarBuffer();
                            }
                        }
                        else if (tclOprimida >= 32 && tclOprimida <= 126) {
                            int mXAvR = anchBasTex + calcularAnchoPixels(numIngreTx);
                            motorUI.iniciarBuffer();
                            if (tclOprimida != ' ') {
                                extern void _renderLetra(char a, int posX, int posY, UI & motor);
                                _renderLetra((char)tclOprimida, mXAvR, 107, motorUI);
                            }
                            motorUI.presentarBuffer();
                            numIngreTx = numIngreTx + (char)tclOprimida;
                        }
                    }
                }

                listaEtiquetasVal.ejecutarSi([](Etiqueta* e) { return true; }, [](Etiqueta* e) { delete e; });
            }
            else if (resEtiqMin == "no" || resEtiqMin == "n") {
                seguirEtiquetas = false;
            }
            else {
                motorUI.iniciarBuffer();
                motorUI.eliminarMatriz(secuenciaNota, 9, 300, 80, 18);
                renderTextoRapido("Respuesta invalida", 80, 18);
                motorUI.presentarBuffer();
                Sleep(800);

                motorUI.iniciarBuffer();
                motorUI.eliminarMatriz(secuenciaNota, 9, 300, 80, 18);
                motorUI.presentarBuffer();
            }
        }

        miVault.agregarNotaGlobal(nuevaNotaLista);
    }

    static void flujoCrearTarea(Boveda& miVault, uint& contadorIdsNotas) {

        motorUI.limpiarPantalla();
        motorUI.iniciarBuffer();
        motorUI.dibujarMatriz(secuenciaTarea, 0, 0);
        renderTextoRapido("Titulo de la tarea:", 80, 6);
        motorUI.presentarBuffer();

        string tituloTarInp = motorUI.escribirLetraLinea(secuenciaTarea, 80, 18, 40);

        string fechaTarInp = "";
        while (true) {
            motorUI.iniciarBuffer();
            renderTextoRapido("Escribe la fecha de tarea (DD/MM/YYYY):", 80, 35);
            motorUI.presentarBuffer();

            fechaTarInp = motorUI.escribirLetraLinea(secuenciaTarea, 80, 47, 10);

            bool checkFecha = validarFecha(fechaTarInp);
            if (checkFecha == true) {
                break;
            }
            else {
                motorUI.iniciarBuffer();
                motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 80, 47);
                renderTextoRapido("fecha invalida", 80, 47);
                motorUI.presentarBuffer();
                Sleep(1000);

                motorUI.iniciarBuffer();
                motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 80, 47);
                motorUI.presentarBuffer();
            }
        }

        motorUI.limpiarPantalla();
        motorUI.iniciarBuffer();
        motorUI.dibujarMatriz(fondoEscribirTarea, 0, 0);
        renderTextoRapido(tituloTarInp, 105, 2);
        renderTextoRapido(fechaTarInp, 105, 16);
        motorUI.presentarBuffer();

        int fPosX[8] = { 80, 80, 80, 80, 80, 80, 80, 80 };
        int fPosY[8] = { 34, 44, 54, 64, 74, 84, 94, 104 };

        int contFilaT = 0;
        int ptScX = fPosX[contFilaT];
        int ptScY = fPosY[contFilaT];

        string fullTextTar = "";
        string textoLnTar = "";

        while (true) {
            int tclOprimida = _getch();

            if (tclOprimida == ENTER) {
                if (textoLnTar == "fin") {
                    break;
                }
                fullTextTar = fullTextTar + textoLnTar + "\n";
                textoLnTar = "";

                contFilaT = contFilaT + 1;
                if (contFilaT > 7) {
                    break;
                }
                ptScX = fPosX[contFilaT];
                ptScY = fPosY[contFilaT];
            }
            else if (tclOprimida == BACKSPACE) {
                if (textoLnTar != "") {
                    int ulcTb = textoLnTar.back();
                    int decTb = 0;
                    int mtBt = 0;

                    if (ulcTb == ' ') {
                        decTb = 3;
                        mtBt = 3;
                    }
                    else {
                        decTb = 7;
                        mtBt = 9;
                    }

                    ptScX = ptScX - decTb;

                    motorUI.iniciarBuffer();
                    motorUI.eliminarMatriz(fondoEscribirTarea, 9, mtBt, ptScX, ptScY);
                    motorUI.presentarBuffer();

                    textoLnTar.pop_back();
                }
            }
            else if (tclOprimida >= 32 && tclOprimida <= 126) {
                int awLetTmp = 0;
                if (tclOprimida == ' ') {
                    awLetTmp = 3;
                }
                else {
                    awLetTmp = 7;
                }

                if (ptScX + awLetTmp <= 342) {
                    textoLnTar = textoLnTar + (char)tclOprimida;

                    motorUI.iniciarBuffer();
                    if (tclOprimida != ' ') {
                        extern void _renderLetra(char a, int posX, int posY, UI & motor);
                        _renderLetra((char)tclOprimida, ptScX, ptScY, motorUI);
                    }
                    motorUI.presentarBuffer();

                    ptScX = ptScX + awLetTmp;
                }
                else {
                    fullTextTar = fullTextTar + textoLnTar + "\n";
                    textoLnTar = "";
                    textoLnTar = textoLnTar + (char)tclOprimida;

                    contFilaT = contFilaT + 1;
                    if (contFilaT > 7) {
                        break;
                    }
                    ptScX = fPosX[contFilaT];
                    ptScY = fPosY[contFilaT];

                    motorUI.iniciarBuffer();
                    if (tclOprimida != ' ') {
                        extern void _renderLetra(char a, int posX, int posY, UI & motor);
                        _renderLetra((char)tclOprimida, ptScX, ptScY, motorUI);
                    }
                    motorUI.presentarBuffer();

                    ptScX = ptScX + awLetTmp;
                }
            }
        }

        if (textoLnTar != "fin" && textoLnTar != "") {
            fullTextTar = fullTextTar + textoLnTar;
        }

        Tarea* nuevaTareaLista = new Tarea(contadorIdsNotas, tituloTarInp, fullTextTar, fechaTarInp);
        contadorIdsNotas = contadorIdsNotas + 1;

        bool flowChecklist = true;

        while (flowChecklist == true) {
            motorUI.limpiarPantalla();
            motorUI.iniciarBuffer();
            motorUI.dibujarMatriz(secuenciaTarea, 0, 0);
            renderTextoRapido("Agregar subtareas (Checklist)? (SI/NO):", 80, 6);
            motorUI.presentarBuffer();

            string rptaCheck = motorUI.escribirLetraLinea(secuenciaTarea, 80, 18, 5);
            string rptaCheckMin = "";
            for (int k = 0; k < rptaCheck.length(); k++) {
                rptaCheckMin = rptaCheckMin + (char)tolower(rptaCheck[k]);
            }

            if (rptaCheckMin == "si" || rptaCheckMin == "s") {
                motorUI.iniciarBuffer();
                renderTextoRapido("Describe tu subtarea: ", 80, 35);
                motorUI.presentarBuffer();

                string textoSubT = motorUI.escribirLetraLinea(secuenciaTarea, 80, 46, 30);
                nuevaTareaLista->agregarSubtarea(textoSubT, false);
            }
            else if (rptaCheckMin == "no" || rptaCheckMin == "n") {
                flowChecklist = false;
            }
            else {
                motorUI.iniciarBuffer();
                motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 80, 18);
                renderTextoRapido("respuesta invalida", 80, 18);
                motorUI.presentarBuffer();
                Sleep(800);

                motorUI.iniciarBuffer();
                motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 80, 18);
                motorUI.presentarBuffer();
            }
        }

        bool seguirEtiquetasT = true;

        while (seguirEtiquetasT == true) {
            motorUI.limpiarPantalla();
            motorUI.iniciarBuffer();
            motorUI.dibujarMatriz(secuenciaTarea, 0, 0);
            renderTextoRapido("Agregar etiqueta? (SI/NO):", 80, 6);
            motorUI.presentarBuffer();

            string resAgEtiqT = motorUI.escribirLetraLinea(secuenciaTarea, 80, 18, 5);
            string resEtiqMinT = "";
            for (int kE = 0; kE < resAgEtiqT.length(); kE++) {
                resEtiqMinT = resEtiqMinT + (char)tolower(resAgEtiqT[kE]);
            }

            if (resEtiqMinT == "si" || resEtiqMinT == "s") {
                Lista<Etiqueta*, nullptr> listaEtiquetasValT;
                miVault.extraerEtiquetasUnicas(listaEtiquetasValT);

                bool loopPagsT = true;
                int pagEtiqActT = 0;

                while (loopPagsT == true) {
                    motorUI.iniciarBuffer();
                    motorUI.eliminarMatriz(secuenciaTarea, 80, 300, 80, 35);

                    renderTextoRapido("ETIQUETAS DISPONIBLES (flechas < o > para ver mas):", 80, 35);

                    int cantTotalEtiqT = listaEtiquetasValT.longitud();
                    int limitePagsEtqT = (cantTotalEtiqT + 5) / 6;

                    if (limitePagsEtqT == 0) { limitePagsEtqT = 1; }
                    if (pagEtiqActT >= limitePagsEtqT) { pagEtiqActT = limitePagsEtqT - 1; }
                    if (pagEtiqActT < 0) { pagEtiqActT = 0; }

                    if (cantTotalEtiqT == 0) {
                        renderTextoRapido("Aun no hay etiquetas disponibles", 85, 51);
                    }
                    else {
                        int indexInicT = pagEtiqActT * 6;
                        int indexFinalT = indexInicT + 6;
                        if (indexFinalT > cantTotalEtiqT) { indexFinalT = cantTotalEtiqT; }

                        int vxEId[] = { 85, 85, 85, 212, 212, 212 };
                        int vyEId[] = { 51, 70, 89, 51, 70, 89 };
                        int vxMtx[] = { 104, 104, 104, 231, 231, 231 };
                        int vyMtx[] = { 48, 67, 86, 48, 67, 86 };
                        int vxTtx[] = { 107, 107, 107, 234, 234, 234 };
                        int vyTtx[] = { 51, 70, 89, 51, 70, 89 };

                        int itemPosActT = 0;
                        for (int iterEnL = indexInicT; iterEnL < indexFinalT; iterEnL++) {
                            Etiqueta* puntEtiqT = listaEtiquetasValT.obtenerPorIndice(iterEnL);

                            string digIndICE_T = to_string(iterEnL + 1);
                            renderTextoRapido(digIndICE_T, vxEId[itemPosActT], vyEId[itemPosActT]);

                            string nmC_T = puntEtiqT->getColor();
                            if (nmC_T == "AZUL") { motorUI.dibujarMatriz(etAzul, vxMtx[itemPosActT], vyMtx[itemPosActT]); }
                            else if (nmC_T == "VERDE") { motorUI.dibujarMatriz(etVerde, vxMtx[itemPosActT], vyMtx[itemPosActT]); }
                            else if (nmC_T == "GRIS") { motorUI.dibujarMatriz(etGris, vxMtx[itemPosActT], vyMtx[itemPosActT]); }
                            else if (nmC_T == "ROJO") { motorUI.dibujarMatriz(etRojo, vxMtx[itemPosActT], vyMtx[itemPosActT]); }
                            else if (nmC_T == "AMARILLO") { motorUI.dibujarMatriz(etAmarillo, vxMtx[itemPosActT], vyMtx[itemPosActT]); }
                            else if (nmC_T == "MORADO") { motorUI.dibujarMatriz(etMorado, vxMtx[itemPosActT], vyMtx[itemPosActT]); }
                            else { motorUI.dibujarMatriz(etGris, vxMtx[itemPosActT], vyMtx[itemPosActT]); }

                            string nmE_T = puntEtiqT->getNombre();
                            if (nmE_T.length() > 12) {
                                nmE_T = nmE_T.substr(0, 12);
                            }
                            renderTextoRapido(nmE_T, vxTtx[itemPosActT], vyTtx[itemPosActT]);

                            itemPosActT = itemPosActT + 1;
                        }
                    }

                    renderTextoRapido("Elige un numero (0 para crear): ", 79, 107);
                    motorUI.presentarBuffer();

                    int numEliGidoT = -1;
                    string numInpT = "";

                    string textBaseT = "Elige un numero (0 para crear): ";
                    int anchBasTexT = 79 + calcularAnchoPixels(textBaseT);

                    while (true) {
                        int tclOprimida = _getch();

                        if (tclOprimida == ESPECIAL) {
                            int tclDirT = _getch();
                            if (tclDirT == IZQUIERDA) {
                                if (pagEtiqActT > 0) {
                                    pagEtiqActT = pagEtiqActT - 1;
                                }
                                break;
                            }
                            else if (tclDirT == DERECHA) {
                                pagEtiqActT = pagEtiqActT + 1;
                                break;
                            }
                        }
                        else if (tclOprimida == ENTER) {
                            try {
                                numEliGidoT = stoi(numInpT);
                            }
                            catch (...) {
                                numEliGidoT = -1;
                            }

                            if (numEliGidoT > 0 && numEliGidoT <= cantTotalEtiqT) {
                                Etiqueta* flSelT = listaEtiquetasValT.obtenerPorIndice(numEliGidoT - 1);
                                nuevaTareaLista->agregarEtiqueta(flSelT->getNombre(), flSelT->getColor());
                                loopPagsT = false;
                                break;
                            }
                            else if (numEliGidoT == 0 && numInpT != "") {
                                motorUI.limpiarPantalla();
                                motorUI.iniciarBuffer();
                                motorUI.dibujarMatriz(secuenciaTarea, 0, 0);
                                renderTextoRapido("Nombre de la etiqueta:", 80, 6);
                                motorUI.presentarBuffer();

                                string inpNomCrT = motorUI.escribirLetraLinea(secuenciaTarea, 80, 18, 12);
                                string cpNmFT = "";
                                for (int bUq = 0; bUq < inpNomCrT.length(); bUq++) {
                                    cpNmFT = cpNmFT + (char)toupper(inpNomCrT[bUq]);
                                }

                                bool loopColorT = true;
                                string endColMayT = "";

                                while (loopColorT == true) {
                                    motorUI.iniciarBuffer();
                                    motorUI.eliminarMatriz(secuenciaTarea, 30, 300, 80, 35);
                                    renderTextoRapido("Azul, Verde, Gris, Rojo, Amarillo, Morado", 80, 35);
                                    renderTextoRapido("Escribe el color: ", 80, 47);
                                    motorUI.presentarBuffer();

                                    string msgEsColT = "Escribe el color: ";
                                    int aXeC_T = 80 + calcularAnchoPixels(msgEsColT);

                                    string txtColorInT = motorUI.escribirLetraLinea(secuenciaTarea, aXeC_T, 47, 10);
                                    string colMinTestT = "";
                                    for (int eF = 0; eF < txtColorInT.length(); eF++) {
                                        colMinTestT = colMinTestT + (char)tolower(txtColorInT[eF]);
                                    }

                                    if (colMinTestT == "azul" || colMinTestT == "verde" || colMinTestT == "gris" || colMinTestT == "rojo" || colMinTestT == "amarillo" || colMinTestT == "morado") {
                                        for (int rQ = 0; rQ < colMinTestT.length(); rQ++) {
                                            endColMayT = endColMayT + (char)toupper(colMinTestT[rQ]);
                                        }
                                        nuevaTareaLista->agregarEtiqueta(cpNmFT, endColMayT);
                                        loopColorT = false;
                                    }
                                    else {
                                        motorUI.iniciarBuffer();
                                        motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 80, 47);
                                        renderTextoRapido("color no entontrado", 80, 47);
                                        motorUI.presentarBuffer();
                                        Sleep(1000);
                                    }
                                }
                                loopPagsT = false;
                                break;
                            }
                            else {
                                motorUI.iniciarBuffer();
                                motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 79, 107);
                                renderTextoRapido("opcion invalida", 79, 107);
                                motorUI.presentarBuffer();
                                Sleep(1200);

                                motorUI.iniciarBuffer();
                                motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 79, 107);
                                renderTextoRapido("Elige un numero (0 para crear): ", 79, 107);
                                motorUI.presentarBuffer();
                                numInpT = "";
                            }
                        }
                        else if (tclOprimida == BACKSPACE) {
                            if (numInpT != "") {
                                int qUlT = numInpT.back();
                                int restAnc_t = 0;
                                int grBor_t = 0;

                                if (qUlT == ' ') {
                                    restAnc_t = 3;
                                    grBor_t = 3;
                                }
                                else {
                                    restAnc_t = 7;
                                    grBor_t = 9;
                                }

                                int limBxT = anchBasTexT + calcularAnchoPixels(numInpT) - restAnc_t;

                                motorUI.iniciarBuffer();
                                motorUI.eliminarMatriz(secuenciaTarea, 9, grBor_t, limBxT, 107);
                                numInpT.pop_back();

                                if (numInpT != "") {
                                    int uLtxH = numInpT.back();
                                    if (uLtxH != ' ' && uLtxH != '\n') {
                                        int atResLxT = 0;
                                        if (uLtxH == ' ') {
                                            atResLxT = 3;
                                        }
                                        else {
                                            atResLxT = 7;
                                        }
                                        extern void _renderLetra(char a, int posX, int posY, UI & motor);
                                        _renderLetra((char)uLtxH, limBxT - atResLxT, 107, motorUI);
                                    }
                                }
                                motorUI.presentarBuffer();
                            }
                        }
                        else if (tclOprimida >= 32 && tclOprimida <= 126) {
                            int mXAvR_T = anchBasTexT + calcularAnchoPixels(numInpT);
                            motorUI.iniciarBuffer();
                            if (tclOprimida != ' ') {
                                extern void _renderLetra(char a, int posX, int posY, UI & motor);
                                _renderLetra((char)tclOprimida, mXAvR_T, 107, motorUI);
                            }
                            motorUI.presentarBuffer();
                            numInpT = numInpT + (char)tclOprimida;
                        }
                    }
                }

                listaEtiquetasValT.ejecutarSi([](Etiqueta* e) { return true; }, [](Etiqueta* e) { delete e; });
            }
            else if (resEtiqMinT == "no" || resEtiqMinT == "n") {
                seguirEtiquetasT = false;
            }
            else {
                motorUI.iniciarBuffer();
                motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 80, 18);
                renderTextoRapido("Respuesta invalida", 80, 18);
                motorUI.presentarBuffer();
                Sleep(800);

                motorUI.iniciarBuffer();
                motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 80, 18);
                motorUI.presentarBuffer();
            }
        }
        miVault.agregarNotaGlobal(nuevaTareaLista);
    }
};