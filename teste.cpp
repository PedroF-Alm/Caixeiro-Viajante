#include "grafo.hpp"
#include <iostream>

using namespace std;

int main() {
    Grafo g = *Grafo::criar(5, MATRIZ_ADJ, NAO_DIRECIONADO);
    g.inserirAresta(0, 1, 3);
    g.inserirAresta(1, 2, 3);
    g.inserirAresta(4, 1, 3);
    g.inserirAresta(4, 2, 3);
    g.inserirAresta(4, 3, 3);
    g.inserirAresta(4, 0, 3);
    
    cout << g.pesoAresta(0,1) << endl;
    cout << g.buscarAresta(1,2) << endl;
    cout << g.cicloEuleriano() << endl;

    g.imprimir();

    g.buscaEmProfundidade();
    g.buscaEmLargura();
    g.imprimirCaminho(1, 4);
}