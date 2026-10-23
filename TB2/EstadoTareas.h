#pragma once
#include <iostream>
#include <string>
#include "Utilidades.h"
#include "Lista.h"
#include "Tarea.h"
#include "Render.h"
#include "utilidades2.h"

using namespace std;
typedef unsigned int uint;

extern UI motorUI;

inline void printTXTKB(const string& txt, int x, int y) {
    int posX = x;
    int posY = y;
    for (int incL = 0; incL < txt.length(); incL++) {
        char valTlT = txt[incL];
        if (valTlT == ' ') {
            posX = posX + 3;
        }
        else {
            extern void _renderLetra(char a, int posX, int posY, UI & motor);
            _renderLetra(valTlT, posX, posY, motorUI);
            posX = posX + 7;
        }
    }
}

inline int evalAnchoKB(string textoInp) {
    int anAcum = 0;
    for (int posK = 0; posK < textoInp.length(); posK++) {
        char lTrA = textoInp[posK];
        if (lTrA == ' ') {
            anAcum = anAcum + 3;
        }
        else {
            anAcum = anAcum + 7;
        }
    }
    return anAcum;
}

class EstadoTareas {
private:

    void heapify(Tarea** arr, int n, int i) {
        int masGrande = i;
        int izq = 2 * i + 1;
        int der = 2 * i + 2;

        if (izq < n && arr[izq]->calcularDiasRestantes() > arr[masGrande]->calcularDiasRestantes())
            masGrande = izq;

        if (der < n && arr[der]->calcularDiasRestantes() > arr[masGrande]->calcularDiasRestantes())
            masGrande = der;

        if (masGrande != i) {
            Tarea* temp = arr[i];
            arr[i] = arr[masGrande];
            arr[masGrande] = temp;
            heapify(arr, n, masGrande);
        }
    }

    void heapSort(Tarea** arr, int n) {
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);

        for (int i = n - 1; i > 0; i--) {
            Tarea* temp = arr[0];
            arr[0] = arr[i];
            arr[i] = temp;
            heapify(arr, i, 0);
        }
    }

    int determinarEstado(Tarea* t) { 
        Lista<Subtarea*, nullptr>& chk = t->getChecklist();
        int total = chk.longitud();

        if (total == 0) return 0;

        int completadas = 0;
        chk.ejecutarSi([](Subtarea* s) { return true; }, [&completadas](Subtarea* s) {
            if (s->estaCompletada()) completadas++;
            });

        if (completadas == 0) return 0;
        if (completadas == total) return 2;
        return 1;
    }

public:
    void mostrarTableroKanban(Lista<Tarea*, nullptr>& todasLasTareas) {

        Lista<Tarea*, nullptr> lstToDo;
        Lista<Tarea*, nullptr> lstDoing;
        Lista<Tarea*, nullptr> lstDone;

        todasLasTareas.ejecutarSi([](Tarea* t) { return true; }, [&](Tarea* t) {
            int estado = determinarEstado(t);
            if (estado == 0) lstToDo.agregaFinal(t);
            else if (estado == 1) lstDoing.agregaFinal(t);
            else lstDone.agregaFinal(t);
            });

        int totToDo = lstToDo.longitud();
        int totDoing = lstDoing.longitud();
        int totDone = lstDone.longitud();

        Tarea** arrToDo = new Tarea * [totToDo > 0 ? totToDo : 1];
        Tarea** arrDoing = new Tarea * [totDoing > 0 ? totDoing : 1];
        Tarea** arrDone = new Tarea * [totDone > 0 ? totDone : 1];

        int idx = 0; lstToDo.ejecutarSi([](Tarea* t) { return true; }, [&](Tarea* t) { arrToDo[idx++] = t; });
        idx = 0; lstDoing.ejecutarSi([](Tarea* t) { return true; }, [&](Tarea* t) { arrDoing[idx++] = t; });
        idx = 0; lstDone.ejecutarSi([](Tarea* t) { return true; }, [&](Tarea* t) { arrDone[idx++] = t; });
        
        if (totToDo > 1) heapSort(arrToDo, totToDo);
        if (totDoing > 1) heapSort(arrDoing, totDoing);
        if (totDone > 1) heapSort(arrDone, totDone);

        bool enKanban = true; 
        int colActiva = 0; 

        int scrollY[3] = { 0, 0, 0 };
        int totales[3] = { totToDo, totDoing, totDone };
        Tarea** columnas[3] = { arrToDo, arrDoing, arrDone };

        const int MAX_VISIBLES = 4; 
        
        int baseX[3] = { 4, 136, 268 };
        int baseYTitle[4] = { 32, 54, 76, 98 };
        int baseYDetal[4] = { 42, 64, 86, 108 };

        while (enKanban == true) {

            motorUI.limpiarPantalla();
            motorUI.iniciarBuffer();
            motorUI.dibujarMatriz(fondoKanva, 0, 0);

            if (colActiva == 0) { motorUI.dibujarMatriz(porhacerON, 4, 19); } 
            else { motorUI.dibujarMatriz(porhacerOF, 4, 19); }
            printTXTKB(to_string(totToDo), 104, 19);

            if (colActiva == 1) { motorUI.dibujarMatriz(enprocesoON, 136, 19); } 
            else { motorUI.dibujarMatriz(enprocesoOF, 136, 19); }
            printTXTKB(to_string(totDoing), 237, 19);

            if (colActiva == 2) { motorUI.dibujarMatriz(terminadoON, 268, 19); } 
            else { motorUI.dibujarMatriz(terminadoOF, 268, 19); }
            printTXTKB(to_string(totDone), 369, 19);

            for (int col = 0; col < 3; col++) {
                int inicio = scrollY[col];
                int fin = inicio + MAX_VISIBLES;
                if (fin > totales[col]) { fin = totales[col]; }

                int idxRen = 0;
                for (int i = inicio; i < fin; i++) {
                    Tarea* tGbl = columnas[col][i];
                    int tPosX = baseX[col];
                    int tPosYTit = baseYTitle[idxRen];
                    int tPosYDet = baseYDetal[idxRen];

                    string trTit = tGbl->getTitulo();
                    if (trTit.length() > 14) {
                        trTit = trTit.substr(0, 11) + "..."; 
                    }
                    printTXTKB(trTit, tPosX, tPosYTit);

                    string datTXT = "v: " + tGbl->getFechaLimite();
                    printTXTKB(datTXT, tPosX, tPosYDet);
                    int dtWn = evalAnchoKB(datTXT);

                    string valPrd = tGbl->getPrioridad();
                    
                    int pSimbX = tPosX + dtWn + 4; 
                    if (valPrd == "vencido") { motorUI.dibujarMatriz(simVencido, pSimbX, tPosYDet); }
                    else if (valPrd == "urgente!!!!") { motorUI.dibujarMatriz(simUrgente, pSimbX, tPosYDet); }
                    else if (valPrd == "alta") { motorUI.dibujarMatriz(simAlta, pSimbX, tPosYDet); }
                    else if (valPrd == "media") { motorUI.dibujarMatriz(simMedio, pSimbX, tPosYDet); }
                    else if (valPrd == "chill") { motorUI.dibujarMatriz(simChill, pSimbX, tPosYDet); }
                    else { motorUI.dibujarMatriz(simChill, pSimbX, tPosYDet); }

                    idxRen++;
                }
            }

            motorUI.presentarBuffer();

            int tecla = _getch(); 

            if (tecla == ESC) {
                enKanban = false;
            }
            else if (tecla == ESPECIAL) {
                int tecla2 = _getch();

                if (tecla2 == IZQUIERDA) {
                    if (colActiva > 0) colActiva--;
                }
                else if (tecla2 == DERECHA) {
                    if (colActiva < 2) colActiva++;
                }
                else if (tecla2 == ABAJO) {
                    if (scrollY[colActiva] + MAX_VISIBLES < totales[colActiva]) {
                        scrollY[colActiva]++;
                    }
                }
                else if (tecla2 == ARRIBA) {
                    if (scrollY[colActiva] > 0) {
                        scrollY[colActiva]--;
                    }
                }
            }
        }
        delete[] arrToDo;
        delete[] arrDoing;
        delete[] arrDone;
    }
};