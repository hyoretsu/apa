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

    vector<vector<int>> bestSequence; // melhor sequência encontrada

    std::vector<std::pair<int, int>> swapHistory; // histórico de trocas

    void applySwap(std::vector<T>& arr, int i, int j) {
        std::swap(arr[i], arr[j]);
        swapHistory.push_back(std::make_pair(i, j)); // registrar a troca no histórico
    }

    void applySwapL(std::vector<vector<T>>& arrL, int manufacturingLine, int i, int j) {
        // std::swap(arrL[l][i], arrL[l + 1][j]);
        
        for (int i = 0; i < manufacturingLine; i++) {
            for (int j = 0; j < manufacturingLine; j++) {
                std::swap(arrL[i][j], arrL[i + 1][j]);
            }
        }

        // std::swap(arr[i], arr[j]);
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

    VariableNeighborhoodDescent(Graph &graph, vector<vector<int>> initialSequence) : graph(graph), bestSequence(initialSequence) {}

    DijkstraReturn execute(std::vector<vector<T>>& arr, int manufacturingLine) {
        int k = 0;
        int kMax = 4;
        float currentObjective;

        for (int i = 0; i < manufacturingLine; i++) {
            bestSequence = arr[i];
            currentObjective = graph.calculateCost(arr[i], true);
        }
        
        float bestObjective = currentObjective;

        // Gerando um motor de números aleatórios
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, arr.size() - 1);

        while (k < kMax) {
            std::vector<vector<T>> newSolution = bestSequence;
            if (k == 0) {
                // Gerando dois índices aleatórios para trocar
                int i = dis(gen);
                int j = dis(gen);
                applySwap(newSolution, i, j);
            } else if (k == 1) {
                applyReverseSubsequence(newSolution);
            } else if (k == 2) {
                applyScramble(newSolution);
            } else if (k = 3) {
                int i = dis(gen);
                int j = dis(gen);
                applySwapL(newSolution, manufacturingLine, i, j);
            }

            double newObjective = graph.calculateCost(newSolution, true);

            if (newObjective < bestObjective) {
                bestSequence = newSolution;
                bestObjective = newObjective;
                k = 0;
            } else {
                k++;
            }
        }

        DijkstraReturn result = {bestSequence, static_cast<float>(bestObjective)};
        return result;
    }

};
