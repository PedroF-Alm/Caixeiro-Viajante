#include <vector>
#include <set>
#include <utility>

#define LISTA_ADJ 1
#define MATRIZ_ADJ 2
#define DIRECIONADO true
#define NAO_DIRECIONADO false

class Grafo {
    private:
        enum cor {
            branco = 3,
            cinza = 4,
            preto = 5
        };

        std::set<std::pair<int, int>> arestas;
        std::vector<std::pair<int, int>> *lista_adj;
        int **matriz_adj;
        int *antecessor;
        int numVertices;
        int tipo;
        bool direcionado;

        Grafo(int numVertices, int tipo, bool direcionado);
        bool avaliarVertice(int vertice);
        void visitaDfs(int u, cor *cores, int *antecessor);
        void visitaBfs(int u, cor *cores, int *antecessor);
    public:
        static Grafo* criar(int numVertices, int tipo, bool direcionado);
        ~Grafo();
        bool cicloEuleriano();
        void buscaEmProfundidade();
        void buscaEmProfundidade(int origem);
        void buscaEmLargura();
        void buscaEmLargura(int origem);
        void imprimirCaminho(int u, int v);
        bool buscarAresta(int u, int v);
        int pesoAresta(int u, int v);
        void inserirAresta(int u, int v);
        void inserirAresta(int u, int v, int peso);
        void removerAresta(int u, int v);
        void imprimir();
        int getNumVertices() {
            return numVertices;
        }
        int eVizinho(int u, int v) {
            return matriz_adj[u][v];
        }
        std::pair<std::vector<int>, int> caixeiroViajanteSbOtm();
};