#include <vector>
#include "grafo.hpp"

class GA 
{
    private:
        int min_val;
        int max_val;
        int pop_size;
        int max_variables;
        int num_generations;
        int tournament_size;
        double crossover_rate;
        double mutation_rate;
        Grafo *g;
        std::vector<std::vector<int>> iniciarPop();
        std::vector<int> avaliarPop(std::vector<std::vector<int>> pop);
        int avaliarCaminho(std::vector<int> caminho);
        void encontrarMelhor(std::vector<std::vector<int>> pop, std::vector<int> fitness, std::vector<std::vector<int>> *melhoresPopulacoes, std::vector<int> *melhoresFitness);
        std::vector<std::vector<int>> selecionarPais(std::vector<std::vector<int>> pop, std::vector<int> fitness);
        std::vector<std::vector<int>> crossover(std::vector<std::vector<int>> pop);
        std::vector<std::vector<int>> mutate(std::vector<std::vector<int>> pop); 
    public:
        GA(Grafo *g, int pop_size, int num_generations, int tournament_size, double crossover_rate, double mutation_rate);
        std::pair<std::vector<int>, int> executar();
};