#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include "Nota.h"
#include "Lista.h"
#include "Subtarea.h"
#include "Render.h" 

using namespace std;
typedef unsigned int uint;

class Tarea : public Nota {
private:
    string fechaLimite;
    Lista<Subtarea*, nullptr> checklist;

    string obtenerPrioridad();
    string serializarChecklist();

public:
    Tarea();
    Lista<Subtarea*, nullptr>& getChecklist() { return checklist; }
    Tarea(uint _id, string _titulo, string _contenido, string _fechaLimite);

    Tarea(uint _id, string _titulo, string _contenido, string _fechaCreacion, string _fechaLimite, string _checklistStr, string _etiquetasStr);

    ~Tarea();

    int calcularDiasRestantes();
    string getFechaLimite();
    string getPrioridad();
    string getTipo() override;

    void renderVisual() override;

    void agregarSubtarea(string nombreSubtarea, bool completada = false);
    string serializar() override;
    bool puedeSerEliminada() override { return true; }
    bool marcarSubtarea(int numV);
};

Tarea::Tarea() : Nota(), fechaLimite("") {}

Tarea::Tarea(uint _id, string _titulo, string _contenido, string _fechaLimite)
    : Nota(_id, _titulo, _contenido), fechaLimite(_fechaLimite) {
}

Tarea::Tarea(uint _id, string _titulo, string _contenido, string _fecha, string _fechaLimite, string _checklistStr, string _etiquetasStr)
    : Nota(_id, _titulo, _contenido, _fecha), fechaLimite(_fechaLimite)
{
    string rItemTemp = "";
    for (int pwE = 0; pwE < _checklistStr.length(); pwE++) {
        char letCkJ = _checklistStr[pwE];
        if (letCkJ == '~') {
            if (rItemTemp != "") {
                size_t pFindE = rItemTemp.find('^');
                if (pFindE != string::npos) {
                    string nvNomT = rItemTemp.substr(0, pFindE);
                    bool estB = (rItemTemp.substr(pFindE + 1) == "1");
                    agregarSubtarea(nvNomT, estB);
                }
            }
            rItemTemp = "";
        }
        else {
            rItemTemp = rItemTemp + letCkJ;
        }
    }

    string txtGrpEtiq = "";
    for (int pmT = 0; pmT < _etiquetasStr.length(); pmT++) {
        char chLtE = _etiquetasStr[pmT];
        if (chLtE == ';') {
            if (txtGrpEtiq != "") {
                size_t pSepV = txtGrpEtiq.find('=');
                if (pSepV != string::npos) {
                    string nEtqNm = txtGrpEtiq.substr(0, pSepV);
                    string cEtqCol = txtGrpEtiq.substr(pSepV + 1);
                    agregarEtiqueta(nEtqNm, cEtqCol);
                }
            }
            txtGrpEtiq = "";
        }
        else {
            txtGrpEtiq = txtGrpEtiq + chLtE;
        }
    }
}

Tarea::~Tarea() {
    checklist.ejecutarSi(
        [](Subtarea* s) { return true; },
        [](Subtarea* s) { delete s; }
    );
}

void Tarea::agregarSubtarea(string nombreSubtarea, bool completada) {
    Subtarea* nueva = new Subtarea(nombreSubtarea, completada);
    checklist.agregaFinal(nueva);
}

bool Tarea::marcarSubtarea(int numV) {
    if (numV <= 0 || numV > checklist.longitud()) return false;

    Subtarea* sub = checklist.obtenerPorIndice(numV - 1);
    if (sub != nullptr) {
        sub->alternarEstado();
        return true;
    }
    return false;
}

string Tarea::serializarChecklist() {
    string resultado = "";
    checklist.ejecutarSi(
        [](Subtarea* s) { return true; },
        [&resultado](Subtarea* s) {

            resultado += s->getNombre() + "^" + (s->estaCompletada() ? "1" : "0") + "~";
        }
    );
    return resultado;
}

string Tarea::serializar() {
    return getTipo() + "|" + to_string(id) + "|" + titulo + "|" +
        hideSaltos(contenido) + "|" + fechaCreacion + "|" + fechaLimite + "|" + serializarChecklist() + "|" + serializarEtiquetas();
}

int Tarea::calcularDiasRestantes() {
    time_t tiempoActual = time(0);
    int diaLimite = 0, mesLimite = 0, yearLimite = 0;
    int numFecha = 0, indFecha = 0;

    for (int i = 0; i < fechaLimite.length(); i++) {
        char caracter = fechaLimite[i];
        if (caracter >= '0' && caracter <= '9') {
            numFecha = numFecha * 10 + (caracter - '0');
        }
        else if (caracter == '/') {
            if (indFecha == 0) diaLimite = numFecha;
            else if (indFecha == 1) mesLimite = numFecha;
            numFecha = 0;
            indFecha++;
        }
    }
    yearLimite = numFecha;

    tm* fechaHoy = localtime(&tiempoActual);
    int diaHoy = fechaHoy->tm_mday;
    int mesHoy = fechaHoy->tm_mon + 1;
    int yearHoy = fechaHoy->tm_year + 1900;

    int diasRestantes = 0;
    diasRestantes += (yearLimite - yearHoy) * 365;
    diasRestantes += (mesLimite - mesHoy) * 30;
    diasRestantes += (diaLimite - diaHoy);

    return diasRestantes;
}

string Tarea::obtenerPrioridad() {
    int diasRestantes = calcularDiasRestantes();
    if (diasRestantes < 0) return "vencido";
    else if (diasRestantes <= 3) return "urgente!!!!";
    else if (diasRestantes <= 7) return "alta";
    else if (diasRestantes <= 14) return "media";
    else return "chill";
}

string Tarea::getFechaLimite() { return fechaLimite; }
string Tarea::getPrioridad() { return obtenerPrioridad(); }
string Tarea::getTipo() { return "TAREA"; }

void Tarea::renderVisual() {
    Nota::renderVisual();

    extern UI motorUI;

    extern void renderTextoRapido(const string & txt, int x, int y);

    renderTextoRapido("VENCE: " + fechaLimite, 290, 30);

    string prio = getPrioridad();
    if (prio == "urgente!!!!" || prio == "vencido") {
        renderTextoRapido("PRIORIDAD: URGENTE", 3, 76);
    }
    else {   renderTextoRapido("PRIORIDAD: " + prio, 3, 76);  }

    if (checklist.longitud() > 0) {
        int checkY = 120; 
        int checkX = 80;  

        renderTextoRapido("--- CHECKLIST ---", checkX, checkY);
        checkY += 10;

        int indice = 1;
        checklist.ejecutarSi(
            [](Subtarea* s) { return true; },
            [&](Subtarea* s) {
                string lineaCheck = "[" + to_string(indice) + "] ";
                lineaCheck += (s->estaCompletada() ? "[X] " : "[ ] ");
                lineaCheck += s->getNombre();

                renderTextoRapido(lineaCheck, checkX, checkY);

                checkY += 10;
                indice++;
            }
        );
    }

    motorUI.presentarBuffer();
}