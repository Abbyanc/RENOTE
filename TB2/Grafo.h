#ifndef __GRAFO_HPP__
#define __GRAFO_HPP__

#include <vector>
#include "Cola.h"

using namespace std;

template<class T>
class CGrafo {
private:
    class CArco {
    public:
        T info;
        int v; 
        CArco(int vLlegada) { 
            info = T();
            v = vLlegada; 
        }
    };

    class CVertice {
    public:
        T info; 
        vector<CArco*>* ady; 
        CVertice() {
            info = T();
            ady = new vector<CArco*>();
        }
        ~CVertice() {
            for (size_t i = 0; i < ady->size(); i++) delete ady->at(i);
            delete ady;
        }
    };

    vector<CVertice*>* vertices; 

    int buscarVertice(T info) {
        for (size_t i = 0; i < vertices->size(); i++) {
            if (vertices->at(i)->info == info) return i;
        }
        return -1;
    }

public:
    CGrafo() {
        vertices = new vector<CVertice*>(); 
    }

    ~CGrafo() {
        for (size_t i = 0; i < vertices->size(); i++) delete vertices->at(i);
        delete vertices;
    }

    void adicionarVertice(T info) {
        if (buscarVertice(info) == -1) {
            CVertice* vert = new CVertice(); 
            vert->info = info; 
            vertices->push_back(vert); 
        }
    }

    int cantidadVertices() {
        return vertices->size(); 
    }

    void adicionarArcoBidireccional(T origen, T destino) {
        int iOrigen = buscarVertice(origen);
        int iDestino = buscarVertice(destino);

        if (iOrigen != -1 && iDestino != -1) {
            bool existe = false;
            for (size_t i = 0; i < vertices->at(iOrigen)->ady->size(); i++) {
                if (vertices->at(iOrigen)->ady->at(i)->v == iDestino) existe = true;
            }

            if (!existe) {
                CArco* arc1 = new CArco(iDestino);
                vertices->at(iOrigen)->ady->push_back(arc1);

                CArco* arc2 = new CArco(iOrigen);
                vertices->at(iDestino)->ady->push_back(arc2);
            }
        }
    }

    vector<T> ejecutarBFS(T inicio) {
        vector<T> resultado;
        int idxInicio = buscarVertice(inicio);
        if (idxInicio == -1) return resultado;

        int n = vertices->size();
        bool* visitados = new bool[n];
        for (int i = 0; i < n; i++) visitados[i] = false;

        Cola<int, -1> frontera;
        frontera.push(idxInicio);
        visitados[idxInicio] = true;

        while (!frontera.vacia()) {
            int actual = frontera.pop();

            if (actual != idxInicio) {
                resultado.push_back(vertices->at(actual)->info);
            }

            for (size_t i = 0; i < vertices->at(actual)->ady->size(); i++) {
                int vecino = vertices->at(actual)->ady->at(i)->v; 
                if (!visitados[vecino]) {
                    visitados[vecino] = true;
                    frontera.push(vecino);
                }
            }
        }
        delete[] visitados;
        return resultado;
    }
};

#endif // !__GRAFO_HPP__