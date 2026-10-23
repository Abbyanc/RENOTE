#pragma once
#include <iostream>
#include <string>
#include "Utilidades.h"
#include "Boveda.h"
#include "Render.h"
#include "Lista.h"
#include "utilidades2.h"

using namespace std;
typedef unsigned int uint;

extern UI motorUI;

inline int evalAnchoG(string textoInp) {
    int anAcum = 0;
    for (int posK = 0; posK < textoInp.length(); posK++) {
        if (textoInp[posK] == ' ') anAcum += 3;
        else anAcum += 7;
    }
    return anAcum;
}

class PanelGrafos {
private:
    Nota* buscarNotaPorIDGrafo(string idGrafo, Boveda& miVault) {
        if (idGrafo.length() > 2 && idGrafo.substr(0, 2) == "N_") {
            try {
                uint idReal = stoi(idGrafo.substr(2));
                Nota* encontrada = nullptr;

                miVault.getNotasGlobales().ejecutarSi(
                    [idReal](Nota* n) { return n->getId() == idReal; },
                    [&encontrada](Nota* n) { encontrada = n; }
                );
                return encontrada;
            }
            catch (...) {} 
        }
        return nullptr;
    }

    void explorarNodo(Etiqueta* e, Boveda& miVault) {
        bool enExploracion = true;
        string nodoRaiz = "E_" + e->getNombre();
        vector<string> resultadosBFSBrutos = miVault.getGrafo().ejecutarBFS(nodoRaiz);

        Lista<Nota*, nullptr> notasTraducidas;
        for (string s : resultadosBFSBrutos) {
            Nota* notaReal = buscarNotaPorIDGrafo(s, miVault);
            if (notaReal != nullptr) {
                notasTraducidas.agregaFinal(notaReal);
            }
        }

        int pagActD = 0;
        const int LMT_PAG_D = 6;
        int pLnaY[6] = { 35, 46, 57, 68, 79, 90 };

        while (enExploracion == true) {
            int tNts = notasTraducidas.longitud();
            int tPgs = (tNts + LMT_PAG_D - 1) / LMT_PAG_D;

            if (tPgs == 0) { tPgs = 1; }
            if (pagActD >= tPgs) { pagActD = tPgs - 1; }
            if (pagActD < 0) { pagActD = 0; }

            int inic = pagActD * LMT_PAG_D;
            int fn = inic + LMT_PAG_D;
            if (fn > tNts) { fn = tNts; }

            motorUI.limpiarPantalla();
            motorUI.iniciarBuffer();
            motorUI.dibujarMatriz(fondoPETIQUETAS, 0, 0);

            string txtCnd = "Conectado a " + to_string(tNts) + " notas";
            renderTextoRapido(txtCnd, 32, 20);

            motorUI.dibujarMatriz(fETizquierdaOF, 248, 20);
            motorUI.dibujarMatriz(fETderechaOF, 366, 20);

            string pgnVis_t = "pag: " + to_string(pagActD + 1) + " de " + to_string(tPgs);
            renderTextoRapido(pgnVis_t, 276, 21);

            int indFila = 0;
            for (int rK = inic; rK < fn; rK++) {
                Nota* nGbl = notasTraducidas.obtenerPorIndice(rK);
                string tpNt = (nGbl->getTipo() == "NOTA SECRETA") ? "[SECRETA]" : "[NOTA]";
                string txtNt = "|- " + tpNt + " " + nGbl->getTitulo() + " (ID I: " + to_string(nGbl->getId()) + ")";
                
                renderTextoRapido(txtNt, 7, pLnaY[indFila]);
                indFila++;
            }

            motorUI.dibujarMatriz(escETOF, 233, 100);
            renderTextoRapido("Volver", 264, 104);

            motorUI.presentarBuffer();

            int tK = _getch();
            if (tK == ESC || tK == '0') {
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(escETON, 233, 100);
                motorUI.presentarBuffer();
                Sleep(80);
                enExploracion = false;
            }
            else if (tK == ESPECIAL) {
                int tK2 = _getch();
                if (tK2 == IZQUIERDA) {
                    motorUI.iniciarBuffer();
                    motorUI.dibujarMatriz(fETizquierdaON, 248, 20);
                    motorUI.presentarBuffer();
                    Sleep(80);
                    if (pagActD > 0) pagActD--;
                }
                else if (tK2 == DERECHA) {
                    motorUI.iniciarBuffer();
                    motorUI.dibujarMatriz(fETderechaON, 366, 20);
                    motorUI.presentarBuffer();
                    Sleep(80);
                    if (pagActD < tPgs - 1) pagActD++;
                }
            }
        }
    }

public:
    void iniciarPanel(Boveda& miVault) {
        bool enPanelE = true;
        int pagAct = 0;
        const int MAX_PAG = 9;

        int pColsX[9] = { 11, 11, 11, 143, 143, 143, 276, 276, 276 };
        int pColsY[9] = { 40, 59, 78, 40, 59, 78, 40, 59, 78 };

        while (enPanelE == true) {
            Lista<Etiqueta*, nullptr> etDisp;
            miVault.extraerEtiquetasUnicas(etDisp);

            int tEtiqs = etDisp.longitud();
            int tPags = (tEtiqs + MAX_PAG - 1) / MAX_PAG;

            if (tPags == 0) tPags = 1;
            if (pagAct >= tPags) pagAct = tPags - 1;
            if (pagAct < 0) pagAct = 0;

            motorUI.limpiarPantalla();
            motorUI.iniciarBuffer();
            motorUI.dibujarMatriz(fondoPETIQUETAS, 0, 0);

            string txtInfo = "Etiquetas disponibles: " + to_string(tEtiqs);
            renderTextoRapido(txtInfo, 39, 20);

            motorUI.dibujarMatriz(fETizquierdaOF, 248, 20);
            motorUI.dibujarMatriz(fETderechaOF, 366, 20);

            string pgnVis_t = "pag: " + to_string(pagAct + 1) + " de " + to_string(tPags);
            renderTextoRapido(pgnVis_t, 276, 21);

            int inic = pagAct * MAX_PAG;
            int fn = inic + MAX_PAG;
            if (fn > tEtiqs) fn = tEtiqs;

            int indRen = 0;
            for (int bB = inic; bB < fn; bB++) {
                Etiqueta* eRf = etDisp.obtenerPorIndice(bB);
                int bsX = pColsX[indRen];
                int bsY = pColsY[indRen];

                string nT = to_string(bB + 1) + " ";
                renderTextoRapido(nT, bsX, bsY + 3);
                
                int wN = evalAnchoG(nT);
                int nBsX = bsX + wN;

                string cl = eRf->getColor();
                if (cl == "AZUL") { motorUI.dibujarMatriz(etAzul, nBsX, bsY); }
                else if (cl == "VERDE") { motorUI.dibujarMatriz(etVerde, nBsX, bsY); }
                else if (cl == "GRIS") { motorUI.dibujarMatriz(etGris, nBsX, bsY); }
                else if (cl == "ROJO") { motorUI.dibujarMatriz(etRojo, nBsX, bsY); }
                else if (cl == "AMARILLO") { motorUI.dibujarMatriz(etAmarillo, nBsX, bsY); }
                else if (cl == "MORADO") { motorUI.dibujarMatriz(etMorado, nBsX, bsY); }
                else { motorUI.dibujarMatriz(etGris, nBsX, bsY); }

                string eNom = eRf->getNombre();
                if (eNom.length() > 12) { eNom = eNom.substr(0, 12); }
                renderTextoRapido(eNom, nBsX + 3, bsY + 3);

                indRen++;
            }

            motorUI.dibujarMatriz(enterETOF, 12, 100);
            renderTextoRapido("Explorar etiqueta #:", 43, 104);

            motorUI.dibujarMatriz(escETOF, 233, 100);
            renderTextoRapido("Volver", 264, 104);

            motorUI.presentarBuffer();

            int plTck_t = _getch();

            if (plTck_t == ESC) {
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(escETON, 233, 100);
                motorUI.presentarBuffer();
                Sleep(80);
                enPanelE = false;
            }
            else if (plTck_t == ENTER) {
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(enterETON, 12, 100);
                motorUI.presentarBuffer();
                Sleep(80);

                if (tEtiqs > 0) {
                    motorUI.iniciarBuffer();
                    motorUI.eliminarMatriz(fondoPETIQUETAS, 9, 200, 43, 104);
                    renderTextoRapido("Explorar etiqueta #: ", 43, 104);
                    motorUI.presentarBuffer();

                    string intpE = motorUI.escribirLetraLinea(fondoPETIQUETAS, 195, 104, 3);
                    if (intpE != "") {
                        try {
                            uint nmEv = stoi(intpE);
                            if (nmEv > 0 && nmEv <= tEtiqs) {
                                Etiqueta* escg = etDisp.obtenerPorIndice(nmEv - 1);
                                explorarNodo(escg, miVault);
                            }
                        }
                        catch (...) {}
                    }
                }
            }
            else if (plTck_t == ESPECIAL) {
                int dcTclR = _getch();
                if (dcTclR == IZQUIERDA) {
                    motorUI.iniciarBuffer();
                    motorUI.dibujarMatriz(fETizquierdaON, 248, 20);
                    motorUI.presentarBuffer();
                    Sleep(80);
                    if (pagAct > 0) pagAct--;
                }
                else if (dcTclR == DERECHA) {
                    motorUI.iniciarBuffer();
                    motorUI.dibujarMatriz(fETderechaON, 366, 20);
                    motorUI.presentarBuffer();
                    Sleep(80);
                    if (pagAct < tPags - 1) pagAct++;
                }
            }

            etDisp.ejecutarSi([](Etiqueta* e) { return true; }, [](Etiqueta* e) { delete e; });
        }
    }
};