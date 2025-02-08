#include "grafo.hpp"
#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

Grafo* Grafo::criar(int numVertices, int tipo, bool direcionado)
{
    if (tipo != LISTA_ADJ && tipo != MATRIZ_ADJ)
        return nullptr;
    return new Grafo(numVertices, tipo, direcionado);
} 

Grafo::Grafo(int numVertices, int tipo, bool direcionado) 
{
    this->numVertices = numVertices;
    this->tipo = tipo;
    this->direcionado = direcionado;
    this->antecessor = new int[numVertices] {-1};
    if (tipo == LISTA_ADJ)
    {
        lista_adj = (vector<pair<int, int>>*) malloc(sizeof(vector<pair<int, int>>) * numVertices);
    }
    else {
        matriz_adj = new int*[numVertices];
        for (int i = 0; i < numVertices; i++)
            matriz_adj[i] = new int[numVertices];
    }
}

Grafo::~Grafo() 
{
    delete [] antecessor;
    if (tipo == LISTA_ADJ)
        free(lista_adj);
    else
    {
        for (int i = 0; i < numVertices; i++)
            delete [] matriz_adj[i];
        delete [] matriz_adj;
    }
}

bool Grafo::avaliarVertice(int vertice)
{
    return vertice >= 0 && vertice < numVertices; 
}

bool Grafo::cicloEuleriano()
{
   if (tipo == LISTA_ADJ) 
   {
        for (int i = 0; i < numVertices; i++)
        {
            int grau = 0;
            for (int j = 0; j < numVertices; j++)
                if (i != j)
                    grau += buscarAresta(i, j) + buscarAresta(j, i);
            if (grau % 2 != 0 || grau == 0)
                return false;
        }
   }
   return true;
}

void Grafo::visitaDfs(int u, cor *cores, int *antecessor)
{
    cores[u] = cinza; 

    if (tipo == LISTA_ADJ)
    {
        for (auto adj: lista_adj[u])
        {
            int v = adj.first;
            if (cores[v] == branco)
            {
                antecessor[v] = u;
                visitaDfs(v, cores, antecessor);
            }
        }
    }
    else
    {
        for (int v = 0; v < numVertices; v++)
            if (matriz_adj[u][v] && cores[v] == branco)
                antecessor[v] = u, visitaDfs(v, cores, antecessor);
    }

    cores[u] = preto; 
}

void Grafo::buscaEmProfundidade() 
{
    cor cores[numVertices];
    
    for (int i = 0; i < numVertices; i++)
    {
        cores[i] = branco;
        antecessor[i] = -1;
    }

    for (int i = 0; i < numVertices; i++)
        if (cores[i] == branco)
            visitaDfs(i, cores, antecessor);

    cout << "Busca em profundidade\n";
    for (int i = 0; i < numVertices; i++)
        cout << antecessor[i] << " ";
    cout << endl;
}

void Grafo::buscaEmProfundidade(int origem) 
{
    cor cores[numVertices];

    for (int i = 0; i < numVertices; i++)
    {
        cores[i] = branco;
        antecessor[i] = -1;
    }

    visitaDfs(origem, cores, antecessor);

    cout << "Busca em profundidade\n";
    for (int i = 0; i < numVertices; i++)
        cout << antecessor[i] << " ";
    cout << endl;
}

void Grafo::visitaBfs(int u, cor* cores, int *antecessor)
{
    cores[u] = cinza;
    queue<int> fila;
    fila.push(u);
    while (!fila.empty())
    {
        u = fila.front();
        fila.pop();
        if (tipo == LISTA_ADJ)
        {
            for (auto adj: lista_adj[u])
            {
                int v = adj.first;
                if (cores[v] == branco)
                {
                    cores[v] = cinza;
                    fila.push(v);
                    antecessor[v] = u;
                }
            }
        }
        else
        {
            for (int v = 0; v < numVertices; v++)
            {
                if (matriz_adj[u][v] && cores[v] == branco)
                {
                    cores[v] = cinza;
                    fila.push(v);
                    antecessor[v] = u;
                }
            }
        }
        cores[u] = preto;
    }
}

void Grafo::buscaEmLargura()
{
    cor cores[numVertices];

    for (int i = 0; i < numVertices; i++)
    {
        cores[i] = branco;
        antecessor[i] = -1;
    }

    for (int i = 0; i < numVertices; i++)
        if (cores[i] == branco)
            visitaBfs(i, cores, antecessor);    

    cout << "Busca em largura\n";
    for (int i = 0; i < numVertices; i++)
        cout << antecessor[i] << " ";
    cout << endl;
}

void Grafo::buscaEmLargura(int origem)
{
    cor cores[numVertices];
    int antecessor[numVertices];

    for (int i = 0; i < numVertices; i++)
    {
        cores[i] = branco;
        antecessor[i] = -1;
    }

    visitaBfs(origem, cores, antecessor);    

    cout << "Busca em largura\n";
    for (int i = 0; i < numVertices; i++)
        cout << antecessor[i] << " ";
    cout << endl;
}

void Grafo::imprimirCaminho(int u, int v)
{
    if (u == v)
        cout << u << " ";
    else if (antecessor[v] == -1)
        cout << "Não existe caminho de " << u << " até " << v << endl;
    else
    {
        imprimirCaminho(u, antecessor[v]);
        cout << v << " ";
    }
}

bool Grafo::buscarAresta(int u, int v)
{
    return arestas.find({u, v}) != arestas.end();
}

int Grafo::pesoAresta(int u, int v) 
{
    if (!avaliarVertice(u) || !avaliarVertice(v)) 
    {
        cout << "A aresta inválida.\n";
        return 0;
    }

    if (!buscarAresta(u, v))
    {
        cout << "A aresta (" << u << ", " << v << ") não existe!\n";
        return 0;
    }

    if (tipo == LISTA_ADJ)
    {
        auto index = find_if(lista_adj[u].begin(), lista_adj[u].end(), [v](const auto& p) {return p.first == v;});
        return index->second;
    }
    else 
    {
        return matriz_adj[u][v];
    }

    return 0;
}

void Grafo::inserirAresta(int u, int v)
{
    if (!avaliarVertice(u) || !avaliarVertice(v)) 
    {
        cout << "Aresta inválida.\n";
        return;
    }

    if (buscarAresta(u, v))
    {
        cout << "A aresta (" << u << ", " << v << ") já existe!\n";
        return;
    }

    arestas.insert({u, v});
    if (!direcionado)
        arestas.insert({v, u});

    if (tipo == LISTA_ADJ) 
    {
        lista_adj[u].push_back({v, 1});
        if (!direcionado)
            lista_adj[v].push_back({u, 1});
    }
    else
    {
        matriz_adj[u][v] = 1;
        if (!direcionado)
            matriz_adj[v][u] = 1;
    }
}

void Grafo::inserirAresta(int u, int v, int peso)
{
    if (!avaliarVertice(u) || !avaliarVertice(v)) 
    {
        cout << "Aresta inválida.\n";
        return;
    }

    if (buscarAresta(u, v))
    {
        cout << "A aresta (" << u << ", " << v << ") já existe!\n";
        return;
    }

    arestas.insert({u, v});
    if (!direcionado)
        arestas.insert({v, u});

    if (tipo == LISTA_ADJ) 
    {
        lista_adj[u].push_back({v, peso});
        if (!direcionado)
            lista_adj[v].push_back({u, peso});
    }
    else
    {
        matriz_adj[u][v] = peso;
        if (!direcionado)
            matriz_adj[v][u] = peso;
    }
}

void Grafo::removerAresta(int u, int v)
{
    if (!avaliarVertice(u) || !avaliarVertice(v)) 
    {
        cout << "Aresta inválida.\n";
        return;
    }

    if (!buscarAresta(u, v))
    {
        cout << "A aresta (" << u << ", " << v << ") já não existe!\n";
        return;
    }

    arestas.erase({u, v});
    if (!direcionado)
        arestas.erase({v, u});
    
    if (tipo == LISTA_ADJ)
    {
        auto index = find_if(lista_adj[u].begin(), lista_adj[u].end(), [v](const auto& p) {return p.first == v;});
        lista_adj[u].erase(index);
        if (!direcionado)
        {
            index = find_if(lista_adj[v].begin(), lista_adj[v].end(), [u](const auto& p) {return p.first == u;});
            lista_adj[v].erase(index);
        }
    }
    else 
    {
        matriz_adj[u][v] = 0;
        if (!direcionado)
            matriz_adj[v][u] = 0;
    }
}

void Grafo::imprimir()
{
    if (tipo == LISTA_ADJ)
    {
        cout << "- Lista de Adjacências -\n";
        for (int i = 0; i < numVertices; i++)
        {
            cout << i << " -> ";
            int size = lista_adj[i].size();
            for (int j = 0; j < size; j++)
                cout << "(" << lista_adj[i][j].first << ", " << lista_adj[i][j].second << ")" << (j < size - 1 ? ", " : "");
            cout << "\n";
        }
    }
    else 
    {
        cout << "- Matriz de Adjacências -\n   ";
        for (int i = 0; i < numVertices; i++)
            cout << i << " ";
        cout << "\n";
        for (int i = 0; i < numVertices; i++)
        {
            cout << i << "| ";
            for (int j = 0; j < numVertices; j++)
                cout << matriz_adj[i][j] << " ";
            cout << "\n";
        }
    }
}