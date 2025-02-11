#include "ga.hpp"
#include <random>
#include <algorithm>

GA::GA(Grafo *g, int pop_size, int num_generations, int tournament_size, double crossover_rate, double mutation_rate)
 {
    this->g = g;
    this->min_val = 0;
    this->max_val = g->getNumVertices() - 1;
    this->pop_size = pop_size;
    this->max_variables = g->getNumVertices();
    this->num_generations = num_generations;
    this->tournament_size = tournament_size;
    this->crossover_rate = crossover_rate;
    this->mutation_rate = mutation_rate;
}

std::vector<std::vector<int>> GA::iniciarPop() {
    std::vector<std::vector<int>> pop = {};
    for (int i = 0; i < pop_size; i++)
    {
        pop.push_back({});
        for (int j = 0; j < max_variables; j++)
        {
            int value = rand() % (max_val - min_val + 1);
            if (std::find(pop[i].begin(), pop[i].end(), value) == pop[i].end())
                pop[i].push_back(value);
            else 
                j--;
        }
    }
    return pop;
}

int GA::avaliarCaminho(std::vector<int> caminho) {
    int peso = 0;
    for (int i = 0; i < caminho.size() - 1; i++)
    {
        int u = caminho[i], v = caminho[i+1];
        peso += g->eVizinho(u, v);
        if (!g->eVizinho(u, v))
            return INT32_MAX;
    }

    return peso;
}

std::vector<int> GA::avaliarPop(std::vector<std::vector<int>> pop)
{
    std::vector<int> fitness = {};

    for (auto c: pop)
    {
        int f = avaliarCaminho(c);
        fitness.push_back(f);
    }

    return fitness;
}

void GA::encontrarMelhor(std::vector<std::vector<int>> pop, std::vector<int> fitness, std::vector<std::vector<int>> *melhoresPopulacoes, std::vector<int> *melhoresFitness)
{
    int melhorIdx = 0, melhorFitness = INT32_MAX;
    std::vector<int> melhorPop;

    for (int i = 0; i < pop.size(); i++)
    {
        if (fitness[i] < melhorFitness)
        {
            melhorFitness = fitness[i];
            melhorPop = pop[i];
        }
    }

    melhoresFitness->push_back(melhorFitness);
    melhoresPopulacoes->push_back(melhorPop);
}

std::vector<std::vector<int>> GA::selecionarPais(std::vector<std::vector<int>> pop, std::vector<int> fitness) 
{
    std::vector<std::vector<int>> pais = {};
    
    for (int i = 0; i < pop.size(); i++) 
    {
        std::vector<int> indices = {};
        std::vector<int> fitnessTorneio = {};
        for (int j = 0; j < tournament_size; j++)
        {
            int idx = rand() % max_variables;
            indices.push_back(idx);
            fitnessTorneio.push_back(fitness[idx]);
        }

        int paiSelecionado = 0;
        int melhorFitness = INT32_MAX;
        for (int j = 0; j < tournament_size; j++)
        {
            if (fitnessTorneio[j] < melhorFitness)
            {
                paiSelecionado = j;
                melhorFitness = fitnessTorneio[j];
            }
        }

        pais.push_back(pop[paiSelecionado]);
    }
    return pais;
}

std::vector<std::vector<int>> GA::crossover(std::vector<std::vector<int>> pop)
{
    std::vector<std::vector<int>> filhos = {};
    for (int i = 0; i < pop.size() - 1; i+=2)
    {
        if ((double) rand() / RAND_MAX < crossover_rate)
        {
            int crossover_point = (int) ((double) rand() / RAND_MAX * max_variables);
            filhos.push_back({});
            filhos.push_back({});
            for (int j = 0; j < crossover_point; j++)
                filhos[i].push_back(pop[i][j]);
            for (int j = crossover_point; j < pop.size(); j++)
                filhos[i].push_back(pop[i+1][j]);
            for (int j = 0; j < crossover_point; j++)
                filhos[i+1].push_back(pop[i+1][j]);
            for (int j = crossover_point; j < pop.size(); j++)
                filhos[i].push_back(pop[i][j]);
        }
        else
        {
            filhos.push_back(pop[i]);
            filhos.push_back(pop[i+1]);
        }
    }
    return pop;
}

std::vector<std::vector<int>> GA::mutate(std::vector<std::vector<int>> pop) 
{
    for (int i = 0; i < pop.size(); i++)
    {
        for (int j = 0; j < pop[i].size(); j++)
        {
            double prob = (double) rand() / RAND_MAX;
            if (prob < mutation_rate)
                for (int k = 0; k < max_variables; k++)
                    pop[i][k] = rand() % (max_val - min_val) + 1;
        }
    }
    return pop;
}

std::pair<std::vector<int>, int> GA::executar() 
{
    std::vector<std::vector<int>> pop = iniciarPop();
    std::vector<int> melhoresFitness = {};
    std::vector<std::vector<int>> melhoresPopulacoes = {};
    std::vector<int> fitness = {};
    for (int i = 0; i < num_generations; i++)
    {
        fitness = avaliarPop(pop);
        encontrarMelhor(pop, fitness, &melhoresPopulacoes, &melhoresFitness);
        pop = selecionarPais(pop, fitness);
        pop = crossover(pop);
        pop = mutate(pop);
    }

    int melhorIdx = 0, melhorFitness = INT32_MAX;
    for (int i = 0; i < num_generations; i++)
    {
        if (melhoresFitness[i] < melhorFitness)
            melhorIdx = i, melhorFitness = melhoresFitness[i];
        
    }

    return {melhoresPopulacoes[melhorIdx], melhoresFitness[melhorIdx]};
}