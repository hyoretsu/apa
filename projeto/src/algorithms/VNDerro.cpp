#include <vector>
#include <random>
#include <iostream>
#include <utility> 
#include <algorithm>

#include "../algorithms/dijkstra.cpp"

template <typename T> 

class VariableNeighborhoodDescent {
private:
    // Suponha que costFunction seja a função objetivo que retorna a qualidade da solução atual
    Graph &graph;

    vector<int> bestSequence; // melhor sequência encontrada

    std::vector<std::pair<int, int>> swapHistory; // histórico de trocas

    void applySwap(std::vector<T>& arr, int i, int j) {
        std::swap(arr[i], arr[j]);
        swapHistory.push_back(std::make_pair(i, j)); // registrar a troca no histórico
    }

    void applyReverseSubsequence(std::vector<T>& arr) {
        int startIdx = 0;
        int endIdx = arr.size() - 1;

        while (startIdx < endIdx) {
            std::swap(arr[startIdx], arr[endIdx]);
            startIdx++;
            endIdx--;
        }
    }

    void applyScramble(std::vector<T>& arr) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, arr.size() - 1);

        for (int i = 0; i < arr.size(); i++) {
            int j = dist(gen);
            std::swap(arr[i], arr[j]);
        }
    }

public:

    VariableNeighborhoodDescent(Graph &graph, vector<int> initialSequence) : graph(graph), bestSequence(initialSequence) {}

    DijkstraReturn execute(std::vector<T>& arr) {
        // Lista de operações de vizinhança
    std::vector<std::function<void(std::vector<T>&)>> neighborhoods = {
        [this](std::vector<T>& arr) { 
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(0, arr.size() - 1);
            int i = distr(gen);
            int j = distr(gen);
            this->applySwap(arr, i, j);
        },
        [this](std::vector<T>& arr) { this->applyReverseSubsequence(arr); },
        [this](std::vector<T>& arr) { this->applyScramble(arr); }
    };

        // Valor inicial da função objetivo
        float currentObjective = this -> graph.calculateCost(arr);
        int k = 0;

        while (k < neighborhoods.size()) {
            // Aplica a operação de vizinhança
            neighborhoods[k](arr);

            // Calcula a nova função objetivo
            double newObjective = this -> graph.calculateCost(arr);

            // Se a nova solução é melhor, atualiza a função objetivo e reinicia k
            if (newObjective < currentObjective) {
                currentObjective = newObjective;
                k = 0;
            }
            // Caso contrário, desfaz a última operação e passa para a próxima operação de vizinhança
            else {
                undoLastOperation(arr);
                k++;
            }
        }

        DijkstraReturn result = { arr, currentObjective };
        return result;
    }

    // Desfaz a última operação realizada no vetor
    void undoLastOperation(std::vector<T>& arr) {
        if (swapHistory.size() > 0) {
            std::pair<int, int> lastSwap = swapHistory.back();
            std::swap(arr[lastSwap.first], arr[lastSwap.second]);
            swapHistory.pop_back();
        }
    }

    const std::vector<std::pair<int, int>>& getSwapHistory() const {
        return swapHistory; // retornar o histórico de trocas
    }
};
