#pragma once
#include <iostream>
#include <string>
#include "Lista.h"
#include "Nota.h"
#include "Boveda.h"
#include "ArbolAVL.h"
#include "Utilidades.h"
#include "NotaSecreta.h"
#include "Render.h"

extern UI motorUI;

class PanelNotas {
private:
    void quicksortNotas(Nota** Abi, int inicio, int fin, bool ascendente) {
        if (inicio >= fin) {
            return;
        }
        int pivote = Abi[fin]->contarLetras();
        int i = inicio - 1;
        
        for (int j = inicio; j < fin; j++) {
            bool condicion = false;
            
            if (ascendente == true) {
                if (Abi[j]->contarLetras() < pivote) {
                    condicion = true;
                }
            } else {
                if (Abi[j]->contarLetras() > pivote) {
                    condicion = true;
                }
            }
            
            if (condicion == true) {
                i++;
                Nota* temp = Abi[i];
                Abi[i] = Abi[j];
                Abi[j] = temp;
            }
        }
        Nota* temp = Abi[i + 1];
        Abi[i + 1] = Abi[fin];
        Abi[fin] = temp;
        
        int posPivote = i + 1;
        quicksortNotas(Abi, inicio, posPivote - 1, ascendente);
        quicksortNotas(Abi, posPivote + 1, fin, ascendente);
    }

    void verDetalleNota(Nota* n, Boveda& miVault) {
        bool leyendo = true;

        while (leyendo == true) {
            n->renderVisual();

            int actTecla = _getch();

            if (actTecla == '0' || actTecla == ESC) {
                leyendo = false;
            }
            else if (actTecla == 'e' || actTecla == 'E') {
                bool seBorro = miVault.eliminarSeguro(n->getId());

                if (seBorro == true) {
                    motorUI.limpiarPantalla();
                    asignarColor(11);
                    cout << "\n\n\n\n\n\n\n          Nota eliminada del sistema correctamente.\n";
                    asignarColor(7);
                    Sleep(1500);
                    leyendo = false; 
                }
            }
        }
    }

public:
    void mostrarPanel(Boveda& miVault) {
        bool enPanel = true;
        int paginaActual = 0;
        int modoOrden = 0;
        bool forzarActualizacion = true;

        const int MAX_POR_PAGINA = 6;
        string patron = "";

        Lista<Nota*, nullptr> listaRender;
        Lista<uint, 0> mapNotas; 

        int pLineaY[6] = { 36, 47, 58, 69, 80, 91 };

        motorUI.limpiarPantalla();
        motorUI.iniciarBuffer();
        motorUI.dibujarMatriz(fondoPanelNotas, 0, 0);
        motorUI.presentarBuffer();

        while (enPanel == true) {
            if (forzarActualizacion == true) {
                while (listaRender.longitud() > 0) {
                    listaRender.eliminarPor([](Nota* n) { return true; });
                }
                while (mapNotas.longitud() > 0) {
                    mapNotas.eliminarPor([](uint i) { return true; });
                }
                mapNotas.agregaFinal(0); 

                Lista<Nota*, nullptr> notasBase;
                miVault.getNotasGlobales().ejecutarSi(
                    [](Nota* n) { return n->getTipo() == "NOTA" || n->getTipo() == "NOTA SECRETA"; },
                    [&notasBase](Nota* n) { notasBase.agregaFinal(n); }
                );

                if (patron != "") {
                    ArbolAVL<Nota*> arbol;
                    notasBase.ejecutarSi([](Nota* n) { return true; }, [&arbol](Nota* n) { arbol.insertar(n, n->getTitulo()); });
                    arbol.buscarPorPatron(patron, listaRender);
                }
                else {
                    notasBase.ejecutarSi([](Nota* n) { return true; }, [&listaRender](Nota* n) { listaRender.agregaFinal(n); });
                }

                if (modoOrden != 0) {
                    if (listaRender.longitud() > 1) {
                        int valN = listaRender.longitud();
                        Nota** Abi = new Nota * [valN];
                        int indX = 0;
                        listaRender.ejecutarSi([](Nota* n) { return true; }, [&Abi, &indX](Nota* n) { Abi[indX++] = n; });
                        
                        bool pAscend = false;
                        if (modoOrden == 1) {
                            pAscend = true;
                        }
                        quicksortNotas(Abi, 0, valN - 1, pAscend);
                        
                        while (listaRender.longitud() > 0) {
                            listaRender.eliminarPor([](Nota* n) { return true; });
                        }
                        for (int indAr = 0; indAr < valN; indAr++) {
                            listaRender.agregaFinal(Abi[indAr]);
                        }
                        delete[] Abi;
                    }
                }

                int totalNotas = listaRender.longitud();
                int totalPaginas = (totalNotas + MAX_POR_PAGINA - 1) / MAX_POR_PAGINA;
                
                if (totalPaginas == 0) {
                    totalPaginas = 1;
                }
                if (paginaActual >= totalPaginas) {
                    paginaActual = totalPaginas - 1;
                }
                if (paginaActual < 0) {
                    paginaActual = 0;
                }

                listaRender.ejecutarSi([](Nota* n) { return true; }, [&mapNotas](Nota* n) { mapNotas.agregaFinal(n->getId()); });

                motorUI.iniciarBuffer();

                motorUI.eliminarMatriz(fondoPanelNotas, 75, 400, 0, 35); 
                motorUI.eliminarMatriz(fondoPanelNotas, 9, 300, 45, 21);
                motorUI.eliminarMatriz(fondoPanelNotas, 9, 100, 262, 21);
                motorUI.eliminarMatriz(fondoPanelNotas, 9, 150, 264, 104);
                motorUI.eliminarMatriz(Bbusqueda1, 15, 340, 44, 4); 

                if (patron == "") {
                    motorUI.dibujarMatriz(Bbusqueda1, 44, 4);
                } else {
                    motorUI.dibujarMatriz(Bbusqueda2, 44, 4);
                    renderTextoRapido(patron, 60, 7);
                }

                motorUI.dibujarMatriz(fArribaPanOF, 5, 20);
                motorUI.dibujarMatriz(fAbajoPanOF, 18, 20);
                
                string letTxtOrden = "";
                if (modoOrden == 0) {
                    letTxtOrden = "OFF";
                } else if (modoOrden == 1) {
                    letTxtOrden = "MENOR A MAYOR";
                } else if (modoOrden == 2) {
                    letTxtOrden = "MAYOR A MENOR";
                }
                
                renderTextoRapido("ORDENADO TAMANO: " + letTxtOrden, 45, 21);

                motorUI.dibujarMatriz(fIzquierdaPanOF, 248, 20);
                motorUI.dibujarMatriz(fDerechaPanOF, 366, 20);
                
                string letPag = "PAG: " + to_string(paginaActual + 1) + " de " + to_string(totalPaginas);
                renderTextoRapido(letPag, 262, 21);

                motorUI.dibujarMatriz(EnterPanOF, 12, 100); 
                renderTextoRapido("Abrir nota por numero", 43, 104);

                motorUI.dibujarMatriz(EscPanOF, 233, 100); 
                
                string letSalida = "";
                if (patron == "") {
                    letSalida = "Salir al menu";
                } else {
                    letSalida = "Limpiar Busqueda";
                }
                renderTextoRapido(letSalida, 264, 104);

                if (totalNotas == 0) {
                    renderTextoRapido("No se encontraron notas", 6, 36);
                } else {
                    int numInit = paginaActual * MAX_POR_PAGINA;
                    int numFin = numInit + MAX_POR_PAGINA;
                    if (numFin > totalNotas) {
                        numFin = totalNotas;
                    }
                    
                    int indexLinea = 0;

                    for (int nRend = numInit; nRend < numFin; nRend++) {
                        Nota* notaTemp = listaRender.obtenerPorIndice(nRend);
                        int posTempX = 6;
                        int posTempY = pLineaY[indexLinea];

                        string rPref = "[" + to_string(nRend + 1) + "]  ";
                        renderTextoRapido(rPref, posTempX, posTempY);
                        
                        int textAncho = 0; 
                        for(int carR = 0; carR < rPref.length(); carR++) { 
                            char carT = rPref[carR];
                            if (carT == ' ') {
                                textAncho = textAncho + 3;
                            } else {
                                textAncho = textAncho + 7;
                            }
                        }
                        posTempX = posTempX + textAncho;

                        if (notaTemp->getTipo() == "NOTA SECRETA") {
                            motorUI.dibujarMatriz(sCandado, posTempX, posTempY);
                        } else {
                            motorUI.dibujarMatriz(sBolita, posTempX, posTempY);
                        }
                        posTempX = posTempX + 13; 
                        
                        string txtCompleto = string(notaTemp->getTitulo()) + " [" + to_string(notaTemp->contarLetras()) + " letras]";
                        renderTextoRapido(txtCompleto, posTempX, posTempY);

                        indexLinea = indexLinea + 1;
                    }
                }

                motorUI.presentarBuffer();
                forzarActualizacion = false;
            }

            int actTecla = _getch();

            if (actTecla == ESC) {
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(EscPanOn, 233, 100);
                motorUI.presentarBuffer();
                Sleep(80);

                if (patron != "") { 
                    patron = ""; 
                    forzarActualizacion = true; 
                } else { 
                    enPanel = false; 
                }
            }
            else if (actTecla == BACKSPACE) {
                if (patron != "") { 
                    int letUltimo = patron.back();
                    int valorRest = 0;
                    int limBorrado = 0;
                    
                    if (letUltimo == ' ') {
                        valorRest = 3;
                        limBorrado = 3;
                    } else {
                        valorRest = 7;
                        limBorrado = 9;
                    }
                    
                    int valorGrosor = 0; 
                    for(int cPos = 0; cPos < patron.length(); cPos++) { 
                        char letC = patron[cPos];
                        if (letC == ' ') {
                            valorGrosor = valorGrosor + 3;
                        } else {
                            valorGrosor = valorGrosor + 7;
                        }
                    }
                    int posiLetX = 60 + valorGrosor - valorRest;

                    motorUI.iniciarBuffer();
                    motorUI.eliminarMatriz(Bbusqueda2, 9, limBorrado, posiLetX, 7); 
                    patron.pop_back();

                    if (patron != "") {
                        int ultimoLet = patron.back();
                        if (ultimoLet != '\n' && ultimoLet != ' ') {
                            int tamPrev = 0;
                            if (ultimoLet == ' ') {
                                tamPrev = 3;
                            } else {
                                tamPrev = 7;
                            }
                            int posBack = posiLetX - tamPrev;
                            _renderLetra(ultimoLet, posBack, 7, motorUI);
                        }
                    }
                    motorUI.presentarBuffer();
                    
                    if (patron == "") {
                        forzarActualizacion = true; 
                    }
                }
            }
            else if (actTecla == ENTER) {
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(EnterPanOn, 12, 100);
                motorUI.eliminarMatriz(fondoPanelNotas, 9, 200, 43, 104);
                renderTextoRapido("Abrir nota # : ", 43, 104);
                motorUI.presentarBuffer();

                string varNumInp = motorUI.escribirLetraLinea(fondoPanelNotas, 150, 104, 3);

                if (varNumInp != "") {
                    try {
                        uint cNum = stoi(varNumInp);
                        if (cNum > 0 && cNum <= mapNotas.longitud() - 1) {
                            uint identReal = mapNotas.obtenerPorIndice(cNum);
                            listaRender.ejecutarSi([identReal](Nota* n) { return n->getId() == identReal; }, [&](Nota* n) {

                                bool accesoPermitido = true;
                                if (n->getTipo() == "NOTA SECRETA") {
                                    NotaSecreta* ns = (NotaSecreta*)n;
                                    motorUI.limpiarPantalla();

                                    if (ns->estaBloqueada() == true) {
                                        asignarColor(4);
                                        int segFalt = ns->getSegundosParaDesbloqueo();
                                        cout << "\n\n\n\n\n[!] ACCESO BLOQUEADO. Faltan " << segFalt << " segundos para intentar de nuevo.\n";
                                        asignarColor(7);
                                        Sleep(2000);
                                        accesoPermitido = false;
                                    }
                                    else {
                                        asignarColor(13);
                                        cout << "\n\n\n\n\n======================================================\n";
                                        cout << "              NOTA SECRETA             \n";
                                        cout << "======================================================\n";
                                        asignarColor(7);
                                        cout << ">> Ingrese la contrasena para descifrarla: ";

                                        string passInput = "";
                                        cin >> passInput;

                                        if (ns->intentarDesbloqueo(passInput, 300) == true) {
                                            accesoPermitido = true;
                                        }
                                        else {
                                            motorUI.limpiarPantalla();
                                            asignarColor(4);
                                            if (ns->estaBloqueada() == true) {
                                                cout << "\n\n\n\n\n[!] DEMASIADOS INTENTOS FALLIDOS. Nota bloqueada temporalmente.\n";
                                            }
                                            else {
                                                int intRest = ns->getIntentosRestantes();
                                                cout << "\n\n\n\n\n[ERROR] Contrasena incorrecta. Te quedan " << intRest << " intentos.\n";
                                            }
                                            asignarColor(7);
                                            Sleep(2000);
                                            accesoPermitido = false;
                                        }
                                    }
                                }
                                if (accesoPermitido == true) {
                                    bool fueSecreta = (n->getTipo() == "NOTA SECRETA");
                                    verDetalleNota(n, miVault);

                                    if (fueSecreta) {
                                        ((NotaSecreta*)n)->bloquear();
                                    }
                                }

                                });
                        }
                    }
                    catch (...) {}
                }

                motorUI.limpiarPantalla();
                motorUI.iniciarBuffer();
                motorUI.dibujarMatriz(fondoPanelNotas, 0, 0);
                motorUI.presentarBuffer();
                forzarActualizacion = true;
            }
            else if (actTecla == ESPECIAL) {
                int tecEspe = _getch();
                if (tecEspe == IZQUIERDA) { 
                    motorUI.iniciarBuffer(); 
                    motorUI.dibujarMatriz(fIzquierdaPanON, 248, 20); 
                    motorUI.presentarBuffer(); 
                    Sleep(80);
                    if (paginaActual > 0) {
                        paginaActual = paginaActual - 1; 
                    }
                    forzarActualizacion = true; 
                }
                else if (tecEspe == DERECHA) { 
                    motorUI.iniciarBuffer(); 
                    motorUI.dibujarMatriz(fDerechaPanON, 366, 20); 
                    motorUI.presentarBuffer(); 
                    Sleep(80);
                    paginaActual = paginaActual + 1; 
                    forzarActualizacion = true; 
                }
                else if (tecEspe == ARRIBA) { 
                    motorUI.iniciarBuffer(); 
                    motorUI.dibujarMatriz(fArribaPanON, 5, 20); 
                    motorUI.presentarBuffer(); 
                    Sleep(80);
                    if (modoOrden == 2) {
                        modoOrden = 0;
                    } else {
                        modoOrden = 2;
                    }
                    paginaActual = 0; 
                    forzarActualizacion = true; 
                }
                else if (tecEspe == ABAJO) { 
                    motorUI.iniciarBuffer(); 
                    motorUI.dibujarMatriz(fAbajoPanON, 18, 20); 
                    motorUI.presentarBuffer(); 
                    Sleep(80);
                    if (modoOrden == 1) {
                        modoOrden = 0;
                    } else {
                        modoOrden = 1;
                    }
                    paginaActual = 0; 
                    forzarActualizacion = true; 
                }
            }
            else if (actTecla >= 32 && actTecla <= 126) {
                if (patron.length() < 24) { 
                    int valorGrosorT = 0; 
                    for(int cP1 = 0; cP1 < patron.length(); cP1++) { 
                        char letC1 = patron[cP1];
                        if (letC1 == ' ') {
                            valorGrosorT = valorGrosorT + 3;
                        } else {
                            valorGrosorT = valorGrosorT + 7;
                        }
                    }
                    int ubcX = 60 + valorGrosorT;
                    
                    motorUI.iniciarBuffer();
                    if(patron == "") {
                        motorUI.dibujarMatriz(Bbusqueda2, 44, 4); 
                    }
                    if (actTecla != ' ') {
                        _renderLetra((char)actTecla, ubcX, 7, motorUI);
                    }
                    motorUI.presentarBuffer();

                    patron = patron + (char)actTecla;
                    paginaActual = 0; 
                    forzarActualizacion = true; 
                }
            }
        }
    }
};