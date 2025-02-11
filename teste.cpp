#include "ga.hpp"
#include <iostream>
#include <chrono>

using namespace std;

int main() {
    int n;
    cin >> n;
    Grafo cidades = *Grafo::criar(n, MATRIZ_ADJ, DIRECIONADO);

    srand(time(NULL));

    std::string uline;
    std::string vline;
    int u, v;
    while (std::getline(std::cin, uline, ' ') && std::getline(std::cin, vline, ' '))
    {
        u = atoi(uline.c_str());
        v = atoi(vline.c_str());
        cidades.inserirAresta(u - 1, v - 1, 1);
    }

    GA ga = GA(&cidades, 50, 50, 3, 0.5, 0.2);

    auto inicio = chrono::high_resolution_clock::now();
    auto solucao = ga.executar();
    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> tempoExecucao = fim - inicio;
    printf("Solução subótima encontrada com GA para N=%d cidades:\n", cidades.getNumVertices());
    printf("\tCaminho: ");
    for (auto cidade: solucao.first)
        printf("%d, ", cidade);
    printf("\n");
    int distancia = solucao.second;
    printf("\tDistancia: %d\n", distancia);
    printf("Tempo de execução: %.4fs\n", tempoExecucao.count());
}