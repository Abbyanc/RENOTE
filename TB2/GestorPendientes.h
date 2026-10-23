#pragma once
#include <iostream>
#include <string>
#include <cctype>
#include "Utilidades.h" 
#include "Boveda.h"
#include "EstadoTareas.h"
#include "ArbolAVL.h"
#include "Render.h"
#include "ArbolBST.h"
#include "Utilidades2.h"

using namespace std;
typedef unsigned int uint;

extern UI motorUI;

class GestorPendientes {
private:
    void quicksortTareas(Tarea** Abi, int inicio, int fin) {
        if (inicio >= fin) return;
        int pivote = Abi[fin]->contarLetras();
        int i = inicio - 1;

        for (int j = inicio; j < fin; ++j) {
            if (Abi[j]->contarLetras() < pivote) {
                ++i;
                Tarea* temp = Abi[i]; Abi[i] = Abi[j]; Abi[j] = temp;
            }
        }
        Tarea* temp = Abi[i + 1]; Abi[i + 1] = Abi[fin]; Abi[fin] = temp;
        int posPivote = i + 1;

        quicksortTareas(Abi, inicio, posPivote - 1);
        quicksortTareas(Abi, posPivote + 1, fin);
    }

    void merge(Tarea** arr, int inicio, int medio, int fin) {
        int n1 = medio - inicio + 1;
        int n2 = fin - medio;

        Tarea** L = new Tarea * [n1];
        Tarea** R = new Tarea * [n2];

        for (int i = 0; i < n1; ++i) L[i] = arr[inicio + i];
        for (int j = 0; j < n2; ++j) R[j] = arr[medio + 1 + j];

        int i = 0, j = 0, k = inicio;
        while (i < n1 && j < n2) {
            if (L[i]->getTitulo() <= R[j]->getTitulo()) {
                arr[k++] = L[i++];
            }
            else  arr[k++] = R[j++];
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
        delete[] L;
        delete[] R;
    }

    void mergeSort(Tarea** arr, int inicio, int fin) {
        if (inicio >= fin) return;
        int medio = inicio + (fin - inicio) / 2;
        mergeSort(arr, inicio, medio);
        mergeSort(arr, medio + 1, fin);
        merge(arr, inicio, medio, fin);
    }

    void recopilarTareasPlano(Boveda& miVault, Lista<Tarea*, nullptr>& listaDestino) {
        miVault.getNotasGlobales().ejecutarSi(
            [](Nota* n) { return n->getTipo() == "TAREA"; },
            [&listaDestino](Nota* n) {
                Tarea* t = (Tarea*)n;
                listaDestino.agregaFinal(t);
            }
        );
    }

    void dibujarContenidoTarea(Tarea* t) {
        motorUI.limpiarPantalla();
        motorUI.iniciarBuffer();
        motorUI.dibujarMatriz(fondoVisualizarTarea, 0, 0);

        string titTarG = t->getTitulo();
        renderTextoRapido(titTarG, 105, 2);

        string fechTarG = t->getFechaLimite();
        renderTextoRapido(fechTarG, 105, 16);
        
        string valPrioA = t->getPrioridad();
        renderTextoRapido(valPrioA, 3, 61);

        int cTLetr = t->contarLetras();
        string stringLetrT = to_string(cTLetr);
        renderTextoRapido(stringLetrT, 3, 96);

        string txtSrlEtiG = t->serializarEtiquetas();
        string nmEti1T = "";
        string clEti1T = "";
        string nmEti2T = "";
        string clEti2T = "";
        int valNumEqT = 0;
        string boxTempNT = "";
        string boxTempCT = "";
        bool chkEsColT = false;
        
        for (int pXq = 0; pXq < txtSrlEtiG.length(); pXq++) {
            char letEqQ = txtSrlEtiG[pXq];
            if (letEqQ == '=') {
                chkEsColT = true;
            } else if (letEqQ == ';') {
                if (valNumEqT == 0) {
                    nmEti1T = boxTempNT;
                    clEti1T = boxTempCT;
                } else if (valNumEqT == 1) {
                    nmEti2T = boxTempNT;
                    clEti2T = boxTempCT;
                }
                valNumEqT = valNumEqT + 1;
                boxTempNT = "";
                boxTempCT = "";
                chkEsColT = false;
            } else {
                if (chkEsColT == false) {
                    boxTempNT = boxTempNT + letEqQ;
                } else {
                    boxTempCT = boxTempCT + letEqQ;
                }
            }
        }
        
        if (valNumEqT > 0) {
            string txtFNom1 = nmEti1T;
            if (txtFNom1.length() > 12) {
                txtFNom1 = txtFNom1.substr(0, 12);
            }
            
            if (clEti1T == "AZUL") { motorUI.dibujarMatriz(etAzul, 2, 13); }
            else if (clEti1T == "VERDE") { motorUI.dibujarMatriz(etVerde, 2, 13); }
            else if (clEti1T == "GRIS") { motorUI.dibujarMatriz(etGris, 2, 13); }
            else if (clEti1T == "ROJO") { motorUI.dibujarMatriz(etRojo, 2, 13); }
            else if (clEti1T == "AMARILLO") { motorUI.dibujarMatriz(etAmarillo, 2, 13); }
            else if (clEti1T == "MORADO") { motorUI.dibujarMatriz(etMorado, 2, 13); }
            else { motorUI.dibujarMatriz(etGris, 2, 13); }
            
            renderTextoRapido(txtFNom1, 5, 16);
        }
        
        if (valNumEqT > 1) {
            string txtFNom2 = nmEti2T;
            if (txtFNom2.length() > 12) {
                txtFNom2 = txtFNom2.substr(0, 12);
            }
            
            if (clEti2T == "AZUL") { motorUI.dibujarMatriz(etAzul, 118, 13); }
            else if (clEti2T == "VERDE") { motorUI.dibujarMatriz(etVerde, 118, 13); }
            else if (clEti2T == "GRIS") { motorUI.dibujarMatriz(etGris, 118, 13); }
            else if (clEti2T == "ROJO") { motorUI.dibujarMatriz(etRojo, 118, 13); }
            else if (clEti2T == "AMARILLO") { motorUI.dibujarMatriz(etAmarillo, 118, 13); }
            else if (clEti2T == "MORADO") { motorUI.dibujarMatriz(etMorado, 118, 13); }
            else { motorUI.dibujarMatriz(etGris, 118, 13); }
            
            renderTextoRapido(txtFNom2, 121, 16);
        }

        string fullBdyT = t->getContenido();
        int filaCrT = 1;
        int cursrXT = 80;
        int cursrYT = 34;

        for (int indBdy = 0; indBdy < fullBdyT.length(); indBdy++) {
            char letBdyA = fullBdyT[indBdy];

            if (letBdyA == '\n') {
                filaCrT = filaCrT + 1;
                cursrXT = 80;
                
                if (filaCrT == 2) { cursrYT = 44; }
                else if (filaCrT == 3) { cursrYT = 54; }
                else if (filaCrT == 4) { cursrYT = 64; }
                else if (filaCrT == 5) { cursrYT = 74; }
                else if (filaCrT == 6) { cursrYT = 84; }
                else if (filaCrT == 7) { cursrYT = 94; }
                else if (filaCrT == 8) { cursrYT = 104; }
                
                if (filaCrT > 8) {
                    break;
                }
            } 
            else {
                int aVncLt = 0;
                if (letBdyA == ' ') {
                    aVncLt = 3;
                } else {
                    aVncLt = 7;
                }

                if (cursrXT + aVncLt > 342) {
                    filaCrT = filaCrT + 1;
                    cursrXT = 80;
                    
                    if (filaCrT == 2) { cursrYT = 44; }
                    else if (filaCrT == 3) { cursrYT = 54; }
                    else if (filaCrT == 4) { cursrYT = 64; }
                    else if (filaCrT == 5) { cursrYT = 74; }
                    else if (filaCrT == 6) { cursrYT = 84; }
                    else if (filaCrT == 7) { cursrYT = 94; }
                    else if (filaCrT == 8) { cursrYT = 104; }
                    
                    if (filaCrT > 8) {
                        break;
                    }
                }
                
                if (letBdyA != ' ') {
                    extern void _renderLetra(char a, int posX, int posY, UI& motor);
                    _renderLetra(letBdyA, cursrXT, cursrYT, motorUI);
                }
                cursrXT = cursrXT + aVncLt;
            }
        }
        
        motorUI.presentarBuffer();
    }

    void renderizarDetalle(Tarea* t, Boveda& miVault) {
        bool opcTareaVw = true;
        
        while (opcTareaVw == true) {
            dibujarContenidoTarea(t);

            int btnTrSel = _getch();
            
            if (btnTrSel == '0') {
                opcTareaVw = false;
            }
            else if (btnTrSel == 's' || btnTrSel == 'S') {
                motorUI.limpiarPantalla();
                
                bool eliRslT = miVault.eliminarSeguro(t->getId());
                
                if (eliRslT == true) {
                    motorUI.limpiarPantalla();
                    renderTextoRapido("Tarea completada y eliminada", 10, 10);
                    Sleep(1500);
                    opcTareaVw = false;
                }
            }
            else if (btnTrSel == 'c' || btnTrSel == 'C') {
                bool flowCklst = true;
                
                while(flowCklst == true) {
                    motorUI.limpiarPantalla();
                    motorUI.iniciarBuffer();
                    motorUI.dibujarMatriz(secuenciaTarea, 0, 0);
                    
                    renderTextoRapido("---|checklist|---", 80, 6);
                    
                    Lista<Subtarea*, nullptr>& rLstChk = t->getChecklist();
                    int lmSubTs = rLstChk.longitud();
                    if (lmSubTs > 5) {
                        lmSubTs = 5;
                    }
                    
                    int arraySubY[5] = { 18, 30, 42, 54, 66 };
                    
                    for(int idxSbt = 0; idxSbt < lmSubTs; idxSbt++) {
                        Subtarea* sbTargAc = rLstChk.obtenerPorIndice(idxSbt);
                        string mkBxT = "";
                        if (sbTargAc->estaCompletada() == true) {
                            mkBxT = "[x]";
                        } else {
                            mkBxT = "[ ]";
                        }
                        
                        string textSubFnl = to_string(idxSbt + 1) + ". " + mkBxT + " " + sbTargAc->getNombre();
                        renderTextoRapido(textSubFnl, 80, arraySubY[idxSbt]);
                    }
                    
                    renderTextoRapido("escribe el numero de la subtarea para", 79, 83);
                    renderTextoRapido("marcarla [x] o desmarcarla [ ]; 0 salir", 79, 95);
                    
                    motorUI.presentarBuffer();
                    
                    string resNumSubT = motorUI.escribirLetraLinea(secuenciaTarea, 79, 107, 3);
                    
                    if (resNumSubT == "0") {
                        flowCklst = false;
                    } else {
                        int nmCnvrs = 0;
                        bool eValNumV = true;
                        
                        try {
                            nmCnvrs = stoi(resNumSubT);
                        } catch(...) {
                            eValNumV = false;
                        }
                        
                        if (eValNumV == true && nmCnvrs > 0 && nmCnvrs <= rLstChk.longitud()) {
                            t->marcarSubtarea(nmCnvrs);
                        } else {
                            motorUI.iniciarBuffer();
                            motorUI.eliminarMatriz(secuenciaTarea, 9, 300, 79, 107);
                            renderTextoRapido("respuesta invalida", 79, 107);
                            motorUI.presentarBuffer();
                            Sleep(800);
                        }
                    }
                }
            }
        }
    }

public:
    void iniciarPanel(Boveda& miVault) {
        bool enPanelPen = true;
        int modoFiltroSel = 0;
        int pgnActual_t = 0;

        const int MAX_PAGINA_T = 6;

        string searchPtn_t = "";
        bool forzarRedb_t = true;
        Lista<Tarea*, nullptr> lstRenderT;

        int rDaysProx = 9999;
        Tarea* proxyTarM = nullptr;

        int pLnaX[6] = { 6, 6, 6, 6, 6, 6 };
        int pLnaY[6] = { 36, 47, 58, 69, 80, 91 };

        motorUI.limpiarPantalla();
        motorUI.iniciarBuffer();
        motorUI.dibujarMatriz(fondoPanelPendientes1, 0, 0);
        motorUI.presentarBuffer();

        while (enPanelPen == true) {
            if (forzarRedb_t == true) {
                while (lstRenderT.longitud() > 0) {
                    lstRenderT.eliminarPor([](Tarea* t) { return true; });
                }
                proxyTarM = nullptr;
                rDaysProx = 9999;

                Lista<Tarea*, nullptr> activasTs;
                recopilarTareasPlano(miVault, activasTs);

                bool flgModBsq_t = false;
                if (searchPtn_t != "") {
                    flgModBsq_t = true;
                }

                if (flgModBsq_t == true) {
                    ArbolAVL<Tarea*> arbolTBase;
                    activasTs.ejecutarSi([](Tarea* t) { return true; }, [&arbolTBase](Tarea* t) { arbolTBase.insertar(t, t->getTitulo()); });

                    Lista<Tarea*, nullptr> filterFindsT;
                    arbolTBase.buscarPorPatron(searchPtn_t, filterFindsT);

                    int cntTRes = filterFindsT.longitud();
                    if (cntTRes > 0) {
                        Tarea** MtzQ_t = new Tarea * [cntTRes];
                        int gIDx = 0;
                        filterFindsT.ejecutarSi([](Tarea* t) { return true; }, [&MtzQ_t, &gIDx](Tarea* t) { MtzQ_t[gIDx++] = t; });
                        quicksortTareas(MtzQ_t, 0, cntTRes - 1);

                        for (int tK = 0; tK < cntTRes; ++tK) {
                            lstRenderT.agregaFinal(MtzQ_t[tK]);
                        }
                        delete[] MtzQ_t;
                    }
                }
                else if (modoFiltroSel == 1 && activasTs.longitud() > 0) {
                    int cTns_o = activasTs.longitud();
                    Tarea** arrPtx = new Tarea * [cTns_o];
                    int rXN = 0;
                    activasTs.ejecutarSi([](Tarea* t) { return true; }, [&arrPtx, &rXN](Tarea* t) { arrPtx[rXN++] = t; });

                    mergeSort(arrPtx, 0, cTns_o - 1);

                    for (int wC = 0; wC < cTns_o; wC++) {
                        lstRenderT.agregaFinal(arrPtx[wC]);
                    }
                    delete[] arrPtx;
                }
                else if (modoFiltroSel == 2) {
                    string rankPris[] = { "vencido", "urgente!!!!", "alta", "media", "chill" };
                    for (int ptQ = 0; ptQ < 5; ptQ++) {
                        string pCkt = rankPris[ptQ];
                        activasTs.ejecutarSi(
                            [&pCkt](Tarea* t) { return t->getPrioridad() == pCkt; },
                            [&lstRenderT](Tarea* t) { lstRenderT.agregaFinal(t); }
                        );
                    }
                }
                else if (modoFiltroSel == 3) {
                    ArbolBST arbolTime;
                    activasTs.ejecutarSi([](Tarea* t) { return true; }, [&arbolTime](Tarea* t) {
                        int iDfech = convertirFechaAEntero(t->getFechaLimite());
                        arbolTime.insertar(iDfech, t);
                        });
                    arbolTime.extraerCronologico(lstRenderT);
                }
                
                else {
                    activasTs.ejecutarSi([](Tarea* t) { return true; }, [&lstRenderT](Tarea* t) { lstRenderT.agregaFinal(t); });
                }

                lstRenderT.ejecutarSi([](Tarea* t) { return true; }, [&](Tarea* t) {
                    if (t->getPrioridad() == "urgente!!!!" && t->calcularDiasRestantes() < rDaysProx) {
                        rDaysProx = t->calcularDiasRestantes();
                        proxyTarM = t;
                    }
                    });

                int limitTActv = lstRenderT.longitud();
                int pgTotalMax_t = (limitTActv + MAX_PAGINA_T - 1) / MAX_PAGINA_T;

                if (pgTotalMax_t == 0) { pgTotalMax_t = 1; }
                if (pgnActual_t >= pgTotalMax_t) { pgnActual_t = pgTotalMax_t - 1; }
                if (pgnActual_t < 0) { pgnActual_t = 0; }

                motorUI.iniciarBuffer();

                motorUI.eliminarMatriz(fondoPanelPendientes1, 75, 400, 0, 35);
                motorUI.eliminarMatriz(fondoPanelPendientes1, 9, 300, 45, 21);
                motorUI.eliminarMatriz(fondoPanelPendientes1, 9, 100, 262, 21);
                motorUI.eliminarMatriz(fondoPanelPendientes1, 9, 150, 264, 104);
                motorUI.eliminarMatriz(barraBusquedaPT1, 15, 340, 44, 4);

                if (searchPtn_t == "") {
                    motorUI.dibujarMatriz(barraBusquedaPT1, 44, 4);
                }
                else {
                    motorUI.dibujarMatriz(barraBusquedaPT2, 44, 4);
                    renderTextoRapido(searchPtn_t, 60, 7);
                }

                motorUI.dibujarMatriz(ptArribaOFF, 5, 20);
                motorUI.dibujarMatriz(ptAbajoOFF, 18, 20);

                string txtOptFlt = "";
                if (modoFiltroSel == 0) { txtOptFlt = "Defecto"; }
                else if (modoFiltroSel == 1) { txtOptFlt = "Alfabetico (MergeSort)"; }
                else if (modoFiltroSel == 2) { txtOptFlt = "Prioridad"; }
                else if (modoFiltroSel == 3) { txtOptFlt = "Cronologico (BST)"; }

                renderTextoRapido("Filtro: " + txtOptFlt, 45, 21);

                motorUI.dibujarMatriz(ptIzquierdaOFF, 248, 20);
                motorUI.dibujarMatriz(ptDerechaOFF, 366, 20);

                string pgnVis_t = "PAG: " + to_string(pgnActual_t + 1) + " de " + to_string(pgTotalMax_t);
                renderTextoRapido(pgnVis_t, 262, 21);

                motorUI.dibujarMatriz(ptEnterOFF, 12, 100);
                renderTextoRapido("Abrir tarea por numero", 43, 104);

                motorUI.dibujarMatriz(ptEscOFF, 233, 100);
                string lbMExitT = "";
                if (searchPtn_t == "") {
                    lbMExitT = "Salir al menu";
                }
                else {
                    lbMExitT = "Limpiar Busqueda";
                }
                renderTextoRapido(lbMExitT, 264, 104);

                if (limitTActv == 0) {
                    renderTextoRapido("No se encontraron tareas", 6, 36);
                }
                else {
                    int frIn_t = pgnActual_t * MAX_PAGINA_T;
                    int fFn_t = frIn_t + MAX_PAGINA_T;
                    if (fFn_t > limitTActv) { fFn_t = limitTActv; }

                    int idxLiNt = 0;

                    for (int sDnT = frIn_t; sDnT < fFn_t; sDnT++) {
                        Tarea* trGbl = lstRenderT.obtenerPorIndice(sDnT);
                        int tempPosX_t = pLnaX[idxLiNt];
                        int tempPosY_t = pLnaY[idxLiNt];

                        string strTxLId = "[" + to_string(sDnT + 1) + "]  ";
                        renderTextoRapido(strTxLId, tempPosX_t, tempPosY_t); 

                        tempPosX_t = tempPosX_t + calcularAnchoPixels(strTxLId); 

                        string tpRiotk = trGbl->getPrioridad();
                        if (tpRiotk == "vencido") { motorUI.dibujarMatriz(simVencido, tempPosX_t, tempPosY_t); }
                        else if (tpRiotk == "urgente!!!!") { motorUI.dibujarMatriz(simUrgente, tempPosX_t, tempPosY_t); }
                        else if (tpRiotk == "alta") { motorUI.dibujarMatriz(simAlta, tempPosX_t, tempPosY_t); }
                        else if (tpRiotk == "media") { motorUI.dibujarMatriz(simMedio, tempPosX_t, tempPosY_t); }
                        else if (tpRiotk == "chill") { motorUI.dibujarMatriz(simChill, tempPosX_t, tempPosY_t); }
                        else { motorUI.dibujarMatriz(simChill, tempPosX_t, tempPosY_t); }

                        tempPosX_t = tempPosX_t + 13;

                        string ltrGTrR = string(trGbl->getTitulo());
                        renderTextoRapido(ltrGTrR, tempPosX_t, tempPosY_t); 

                        idxLiNt = idxLiNt + 1;
                    }
                }

                motorUI.presentarBuffer();
                forzarRedb_t = false;
            }

            int plTck_t = _getch();

            if (plTck_t == ESC) {
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(ptEscON, 233, 100);
                motorUI.presentarBuffer();
                Sleep(80);

                if (searchPtn_t != "") {
                    searchPtn_t = "";
                    forzarRedb_t = true;
                }
                else {
                    enPanelPen = false;
                }
            }
            else if (plTck_t == BACKSPACE) {
                if (searchPtn_t != "") {
                    int chLTr_t = searchPtn_t.back();
                    int clmRt_t = 0;
                    int fLdMrc_t = 0;

                    if (chLTr_t == ' ') {
                        clmRt_t = 3;
                        fLdMrc_t = 3;
                    }
                    else {
                        clmRt_t = 7;
                        fLdMrc_t = 9;
                    }

                    int grdXBt = 60 + calcularAnchoPixels(searchPtn_t) - clmRt_t;

                    motorUI.iniciarBuffer();
                    motorUI.eliminarMatriz(barraBusquedaPT2, 9, fLdMrc_t, grdXBt, 7);
                    searchPtn_t.pop_back();

                    if (searchPtn_t != "") {
                        int endC_Ts = searchPtn_t.back();
                        if (endC_Ts != '\n' && endC_Ts != ' ') {
                            int rMttBs = 0;
                            if (endC_Ts == ' ') { rMttBs = 3; }
                            else { rMttBs = 7; }

                            extern void _renderLetra(char a, int posX, int posY, UI & motor);
                            _renderLetra(endC_Ts, grdXBt - rMttBs, 7, motorUI);
                        }
                    }
                    motorUI.presentarBuffer();

                    if (searchPtn_t == "") {
                        forzarRedb_t = true;
                    }
                }
            }
            else if (plTck_t == ENTER) {
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(ptEnterON, 12, 100);
                motorUI.eliminarMatriz(fondoPanelPendientes1, 9, 200, 43, 104);
                renderTextoRapido("Abrir tarea # : ", 43, 104);
                motorUI.presentarBuffer();

                string tInpNDB = motorUI.escribirLetraLinea(fondoPanelPendientes1, 150, 104, 3);

                if (tInpNDB != "") {
                    try {
                        uint idQrsW = stoi(tInpNDB);

                        Lista<Tarea*, nullptr> lnTqGbs;
                        recopilarTareasPlano(miVault, lnTqGbs);

                        if (searchPtn_t != "") {
                            ArbolAVL<Tarea*> aTbWq;
                            lnTqGbs.ejecutarSi([](Tarea* t) { return true; }, [&aTbWq](Tarea* t) { aTbWq.insertar(t, t->getTitulo()); });

                            Lista<Tarea*, nullptr> fnXqFrt;
                            aTbWq.buscarPorPatron(searchPtn_t, fnXqFrt);

                            if (idQrsW > 0 && idQrsW <= fnXqFrt.longitud()) {
                                Tarea* nTrFx = fnXqFrt.obtenerPorIndice(idQrsW - 1);
                                renderizarDetalle(nTrFx, miVault);
                            }
                        }
                        else {
                            if (idQrsW > 0 && idQrsW <= lstRenderT.longitud()) {
                                Tarea* nTrFx = lstRenderT.obtenerPorIndice(idQrsW - 1);
                                renderizarDetalle(nTrFx, miVault);
                            }
                        }
                    }
                    catch (...) {}
                }
                motorUI.limpiarPantalla();
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(fondoPanelPendientes1, 0, 0);
                motorUI.presentarBuffer();
                forzarRedb_t = true;
            }
            else if (plTck_t == 9) { // TAB
                Lista<Tarea*, nullptr> todasLTr;
                recopilarTareasPlano(miVault, todasLTr);
                EstadoTareas lzKbn;
                lzKbn.mostrarTableroKanban(todasLTr);

                motorUI.limpiarPantalla();
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(fondoPanelPendientes1, 0, 0);
                motorUI.presentarBuffer();
                forzarRedb_t = true;
            }
            else if (plTck_t == ESPECIAL) {
                int dcTclR = _getch();
                if (dcTclR == IZQUIERDA) {
                    motorUI.iniciarBuffer();
                    motorUI.dibujarMatriz(ptIzquierdaON, 248, 20);
                    motorUI.presentarBuffer();
                    Sleep(80);
                    if (pgnActual_t > 0) {
                        pgnActual_t = pgnActual_t - 1;
                    }
                    forzarRedb_t = true;
                }
                else if (dcTclR == DERECHA) {
                    motorUI.iniciarBuffer();
                    motorUI.dibujarMatriz(ptDerechaON, 366, 20);
                    motorUI.presentarBuffer();
                    Sleep(80);
                    pgnActual_t = pgnActual_t + 1;
                    forzarRedb_t = true;
                }
                else if (searchPtn_t == "") {
                    if (dcTclR == ARRIBA) {
                        motorUI.iniciarBuffer();
                        motorUI.dibujarMatriz(ptArribaON, 5, 20);
                        motorUI.presentarBuffer();
                        Sleep(80);
                        modoFiltroSel = (modoFiltroSel + 3) % 4;
                        pgnActual_t = 0;
                        forzarRedb_t = true;
                    }
                    else if (dcTclR == ABAJO) {
                        motorUI.iniciarBuffer();
                        motorUI.dibujarMatriz(ptAbajoON, 18, 20);
                        motorUI.presentarBuffer();
                        Sleep(80);
                        modoFiltroSel = (modoFiltroSel + 1) % 4;
                        pgnActual_t = 0;
                        forzarRedb_t = true;
                    }
                }
            }
            else if (plTck_t >= 32 && plTck_t <= 126) {
                if (searchPtn_t.length() < 24) {
                    int xXWcI = 60 + calcularAnchoPixels(searchPtn_t);

                    motorUI.iniciarBuffer();
                    if (searchPtn_t == "") {
                        motorUI.dibujarMatriz(barraBusquedaPT2, 44, 4);
                    }
                    if (plTck_t != ' ') {
                        extern void _renderLetra(char a, int posX, int posY, UI & motor);
                        _renderLetra((char)plTck_t, xXWcI, 7, motorUI);
                    }
                    motorUI.presentarBuffer();

                    searchPtn_t = searchPtn_t + (char)plTck_t;
                    pgnActual_t = 0;
                    forzarRedb_t = true;
                }
            }
        }
    }
};