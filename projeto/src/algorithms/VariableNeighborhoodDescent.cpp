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

    vector<vector<T>> bestSequence; // melhor sequência encontrada

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
        int i = 0;
        int moviments = 0;
        int maxMoviments = 2;

        float currentObjective;
        float bestValues;

        vector<float> currentCost;
        
        for (int i = 0; i < manufacturingLine; i++) {
            bestSequence = arr;
            currentObjective = graph.calculateCost(bestSequence[i], true);
            currentCost.push_back(currentObjective);
        }

        bestValues = currentCost[i];

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, arr[0].size() - 1);
        
        while (moviments < maxMoviments) {
            
            std::vector<T> newSolution = bestSequence[i];
            
            if (moviments == 0) {
                int i = dis(gen);
                int j = dis(gen);

                applySwap(newSolution, i, j);
            } else if (moviments == 1) {
                int i = dis(gen);
                int j = dis(gen);

                applyScramble(newSolution);
            } else if (moviments == 2) {
                applyReverseSubsequence(newSolution);
            }

            // int i = dis(gen);
            // int j = dis(gen);
            // applySwapL(bestSequence, manufacturingLine, i, j);
            
            float finalSolution = graph.calculateCost(newSolution, true);
            
            cout << "Final Souliton: " << finalSolution << endl;
            cout << "New Souliton 0: " << newSolution[0] << endl;
            cout << "New Souliton 1: " << newSolution[1] << endl;
            
            if (finalSolution < currentCost[i]) {
                bestSequence[i] = newSolution;
                currentCost[i] = finalSolution;
                moviments = 0;

            } else {
                moviments++;
                if (i == manufacturingLine - 1) {
                    i = 0;
                } else {
                    i++;
                }
            }
            
        }
        
        DijkstraReturn result = {bestSequence[i], static_cast<float>(currentCost[i])};
        
        return result;
    }

};
