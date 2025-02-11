#include "grafo.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cstdlib> // Para srand() e rand()
#include <ctime>

using namespace std;

int calcularCusto(Grafo& g, vector<int>& caminho) {
    int custo = 0;
    for (size_t i = 0; i < caminho.size() - 1; i++) {
        if (!g.buscarAresta(caminho[i], caminho[i + 1])) {
            cout << "Caminho invalido: " << caminho[i] + 1 << " -> " << caminho[i + 1] + 1 << endl;
            return numeric_limits<int>::max(); // Caminho inválido
        }
        custo += g.pesoAresta(caminho[i], caminho[i + 1]);
    }
    if (!g.buscarAresta(caminho.back(), caminho[0])) {
        cout << "Caminho invalido: " << caminho.back() + 1 << " -> " << caminho[0] + 1 << endl;
        return numeric_limits<int>::max(); // Retorno inválido
    }
    custo += g.pesoAresta(caminho.back(), caminho[0]);
    return custo;
}

bool grafoConectado(Grafo& g, int numVertices) {
    for (int i = 0; i < numVertices; i++) {
        bool conectado = false;
        for (int j = 0; j < numVertices; j++) {
            if (i != j && g.buscarAresta(i, j)) {
                conectado = true;
                break;
            }
        }
        if (!conectado) {
            cout << "Erro: O vertice " << i + 1 << " estah isolado! O algoritmo nao pode rodar." << endl;
            return false;
        }
    }
    return true;
}


void caixeiroViajanteForcaBruta(Grafo& g, int numCidades) {
    vector<int> cidades(numCidades);
    for (int i = 0; i < numCidades; i++) {
        cidades[i] = i;
    }

    int menorCusto = numeric_limits<int>::max();
    vector<int> melhorCaminho;

    auto inicio = chrono::high_resolution_clock::now();
    int contador = 0;

    do {
        // Verifica se todas as arestas do caminho existem 
        bool caminhoValido = true;
        for (size_t i = 0; i < cidades.size() - 1; i++) {
            if (!g.buscarAresta(cidades[i], cidades[i + 1])) {
                caminhoValido = false;
                break;  // Sai do loop, pois o caminho já está inválido
            }
        }
        if (!g.buscarAresta(cidades.back(), cidades[0])) {
            caminhoValido = false;  
        }

        if (!caminhoValido) {
            continue;  // Pula para a próxima sem calcular o custo
        }

        // Calcula o custo do caminho válido
        int custo = calcularCusto(g, cidades);
        if (custo < menorCusto) {
            menorCusto = custo;
            melhorCaminho = cidades;
        }

        contador++;
        if (contador % 100000 == 0) {
            auto tempoAtual = chrono::high_resolution_clock::now();
            chrono::duration<double> tempoPassado = tempoAtual - inicio;
            cout << "Rodadas: " << contador << " | Tempo: " << tempoPassado.count() << " segundos" << endl;
        }
    } while (next_permutation(cidades.begin() + 1, cidades.end()));

    auto fim = chrono::high_resolution_clock::now();
    chrono::duration<double> tempoExecucao = fim - inicio;

    if (melhorCaminho.empty()) {
        cout << "Nenhum caminho valido foi encontrado!" << endl;
    } else {
        cout << "Melhor caminho encontrado: ";
        for (int cidade : melhorCaminho) {
            cout << cidade + 1 << " -> ";
        }
        cout << melhorCaminho[0] + 1 << endl;
    }
    cout << "Custo: " << menorCusto << endl;
    cout << "Tempo de execucao: " << tempoExecucao.count() << " segundos" << endl;
}


// Função para carregar as arestas do arquivo e gerar pesos aleatórios
void carregarGrafoComPesosAleatorios(Grafo& g, const string& nomeArquivo, int maxPeso) {
    srand(time(NULL)); // Inicializa gerador de números aleatórios
    ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        cerr << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        return;
    }

    int numArestas;
    arquivo >> numArestas;

    vector<bool> temAresta(g.getNumVertices(), false); // Vrificar se todas as cidades têm conexões

    int u, v;
    while (arquivo >> u >> v) {
        int peso = rand() % maxPeso + 1; 
        g.inserirAresta(u - 1, v - 1, peso);  // Certifique-se de que os índices começam de 0
        cout << "Aresta carregada: " << u << " - " << v << " | Peso: " << peso << endl;

        temAresta[u - 1] = true;  // Marca que o vértice `u` tem conexão
        temAresta[v - 1] = true;  // Marca que o vértice `v` tem conexão
    }
    
    arquivo.close();

    // Verificar se todas as cidades têm pelo menos uma conexão
    cout << "\nVerificacao de conectividade:" << endl;
    bool grafoConexo = true;
    for (int i = 0; i < g.getNumVertices(); i++) {
        if (!temAresta[i]) {
            cout << "Erro: Cidade " << i + 1 << " estah isolada!" << endl;
            grafoConexo = false;
        }
    }

    if (!grafoConexo) {
        cout << "O grafo estah desconexo! O algoritmo **NAO** encontrarah uma solucao valida!" << endl;
    } else {
        cout << "O grafo estah conectado, pode rodar o algoritmo!" << endl;
    }
}

int main() {
    srand(time(NULL)); 

    string nomeArquivo = "../Instancias Força Bruta/instancia_n12_45.txt"; 
    int numVertices = 13;
    int maxPeso = 100; 

    // Criando o grafo
    Grafo g = *Grafo::criar(numVertices, MATRIZ_ADJ, NAO_DIRECIONADO);

    // Carregar o grafo e gerar pesos aleatórios
    carregarGrafoComPesosAleatorios(g, nomeArquivo, maxPeso);

    // Verifica a conectividade do grafo
    cout << "\nVerificando conexoes:" << endl;
    for (int i = 0; i < g.getNumVertices(); i++) {
        for (int j = 0; j < g.getNumVertices(); j++) {
            if (i != j && g.buscarAresta(i, j)) {
                cout << "Aresta valida: " << i + 1 << " - " << j + 1 << " | Peso: " << g.pesoAresta(i, j) << endl;
            }
        }
    }

    // Verifica se o grafo está conectado antes de rodar o algoritmo
    if (!grafoConectado(g, numVertices)) {
        return 1; // Sai do programa se o grafo não for válido
    }

    caixeiroViajanteForcaBruta(g, numVertices);

    return 0;
}
