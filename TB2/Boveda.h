#pragma once
#include "Lista.h"
#include "Nota.h"
#include "NotaSecreta.h" 
#include "Grafo.h" 
#include <sstream>

using namespace std;
typedef unsigned int uint;

class Boveda {
private:
    Lista<Nota*> notasGlobales;
    CGrafo<string> redDeConocimiento;;

public:
    Boveda() {}

    ~Boveda() {
        notasGlobales.ejecutarSi([](Nota* n) { return true; }, [](Nota* n) { delete n; });
    }

    CGrafo<string>& getGrafo() { return redDeConocimiento; } 

    void agregarNotaGlobal(Nota* n) { 
        notasGlobales.agregaFinal(n); 
        
        string serial = n->serializarEtiquetas();
        if (!serial.empty()) {
            string idNotaStr = "N_" + to_string(n->getId());
            redDeConocimiento.adicionarVertice(idNotaStr);

            stringstream ss(serial);
            string token;

            while (getline(ss, token, ';')) {
                if (token.empty()) continue;
                size_t posIgual = token.find('=');
                if (posIgual != string::npos) {
                    string nomEti = token.substr(0, posIgual);
                    string idEtiStr = "E_" + nomEti;
                    
                    redDeConocimiento.adicionarVertice(idEtiStr);
                    redDeConocimiento.adicionarArcoBidireccional(idNotaStr, idEtiStr); 
                }
            }
        }
    }

    Lista<Nota*>& getNotasGlobales() { return notasGlobales; }

    bool estaVacia() { return notasGlobales.longitud() == 0; }

    bool eliminarSeguro(uint idReal) {
        Nota* target = notasGlobales.eliminarPor([idReal](Nota* n) { return n->getId() == idReal; });

        if (target != nullptr) {
            if (!target->puedeSerEliminada()) {
                notasGlobales.agregaFinal(target);
                return false;
            }
            delete target;
            return true;
        }
        return false;
    }

    void extraerEtiquetasUnicas(Lista<Etiqueta*, nullptr>& unicas) {
        notasGlobales.ejecutarSi(
            [](Nota* n) { return true; },
            [&unicas](Nota* n) {
                string serial = n->serializarEtiquetas();
                if (serial.empty()) return;

                stringstream ss(serial);
                string token;

                while (getline(ss, token, ';')) {
                    if (token.empty()) continue;

                    size_t posIgual = token.find('=');
                    if (posIgual != string::npos) {
                        string nom = token.substr(0, posIgual);
                        string col = token.substr(posIgual + 1);
                        bool existe = false;
                        unicas.ejecutarSi([&nom](Etiqueta* e) { return e->getNombre() == nom; },
                            [&existe](Etiqueta* e) { existe = true; } 
                        );

                        if (!existe) unicas.agregaFinal(new Etiqueta(nom, col));
                    }
                }
            }
        );
    }
};