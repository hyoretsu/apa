#include <algorithm>
#include <cmath>

#include "./dijkstra.cpp"
#include "../data_structures/Graph.hpp"
#include "../utils/random.hpp"
#include "../utils/swap.hpp"

typedef struct {
    std::vector<int> sequence;
    float cost;
} VNDReturn;

template<typename T = int>
class VariableNeighborhoodDescent {
private:
    Graph& graph;
    int lineMovementsN = 2;
    int columnMovementsN = 1;

    // Return - swapped lines indexes
    int* lineSwap(std::vector<std::vector<T>>& arr) {
        int x, y;
        if (arr.size() == 2) {
            x = 0;
            y = 1;
        } else {
            x = random(0, arr.size() - 1);
            y = random(0, arr.size() - 1);

            while (x == y) {
                x = random(0, arr.size() - 1);
                y = random(0, arr.size() - 1);
            }
        }

        // Gerando dois índices aleatórios para trocar
        int i = random(0, arr[x].size() - 1);
        int j = random(0, arr[y].size() - 1);

        while (i == j) {
            i = random(0, arr.size() - 1);
            j = random(0, arr.size() - 1);
        }

        auto tmp = arr[x][i];
        arr[x][i] = arr[y][j];
        arr[y][j] = tmp;

        int* swappedIndexes = new int[2];
        swappedIndexes[0] = x;
        swappedIndexes[1] = y;

        return swappedIndexes;
    }

    void reverseSubsequence(std::vector<T>& arr) {
        int startIndex = 0;
        int endIndex = arr.size() - 1;

        while (startIndex < endIndex) {
            utils::swap(arr, startIndex, endIndex);

            startIndex += 1;
            endIndex -= 1;
        }
    }

    void scramble(std::vector<T>& arr) {
        for (int i = 0; i < arr.size(); i++) {
            int j = random(0, arr.size() - 1);

            utils::swap(arr, i, j);
        }
    }

public:
    explicit VariableNeighborhoodDescent(Graph& graph) : graph(graph) {}

    std::vector<VNDReturn> execute(const std::vector<DijkstraReturn>& initialResult) {
        int solutionSize = initialResult.size();

        std::vector<std::vector<T>> bestSolution(solutionSize);
        std::vector<float> bestCost(solutionSize);
        for (int i = 0; i < solutionSize; i++) {
            bestSolution[i] = initialResult[i].sequence;
            bestCost[i] = initialResult[i].cost;
        }

        int currentLineMovement = 0, timeout = 20, unsuccessfulTries = 0;
        while (unsuccessfulTries < timeout) {
            std::vector<std::vector<T>> newSolution = bestSolution;
            std::vector<float> newCost = bestCost;

            for (int i = 0; i < solutionSize; i++) {
                std::vector<T> newLineSolution = newSolution[i];
                float newLineCost = newCost[i];

                // Applying in-line movements
                int currentMovement = 0;
                while (currentMovement < lineMovementsN) {
                    switch (currentMovement) {
                    case 0:
                        this->reverseSubsequence(newLineSolution);
                        break;
                    case 1:
                        this->scramble(newLineSolution);
                        break;
                    }

                    newLineCost = graph.calculateCost(newLineSolution, true);

                    if (newLineCost < newCost[i]) {
                        newSolution[i] = newLineSolution;
                        newCost[i] = newLineCost;

                        currentMovement = 0;
                    } else {
                        currentMovement += 1;
                    }
                }
            }

            for (int i = 0; i < solutionSize; i++) {
                bestSolution[i] = newSolution[i];
                bestCost[i] = newCost[i];
            }

            int* swappedIndexes = new int[2];
            switch (currentLineMovement) {
            case 0:
                swappedIndexes = this->lineSwap(newSolution);

                newCost[swappedIndexes[0]] = graph.calculateCost(newSolution[swappedIndexes[0]], true);
                newCost[swappedIndexes[1]] = graph.calculateCost(newSolution[swappedIndexes[1]], true);

                break;
            }

            float maxNewCost = *std::max_element(newCost.begin(), newCost.end());
            float maxBestCost = *std::max_element(bestCost.begin(), bestCost.end());

            // Se o maior custo das modificações atuais for menor que
            // o maior custo atual, atualize a nova solução
            if (maxNewCost < maxBestCost) {
                // É mais feio porém menos custoso atualizar apenas o que foi modificado
                bestSolution[swappedIndexes[0]] = newSolution[swappedIndexes[0]];
                bestCost[swappedIndexes[0]] = newCost[swappedIndexes[0]];

                bestSolution[swappedIndexes[1]] = newSolution[swappedIndexes[1]];
                bestCost[swappedIndexes[1]] = newCost[swappedIndexes[1]];

                unsuccessfulTries = 0;
            } else {
                unsuccessfulTries += 1;
            }
        }

        std::vector<VNDReturn> result(solutionSize);
        for (int i = 0; i < solutionSize; i++) {
            VNDReturn solutionCost = { bestSolution[i], bestCost[i] };
            result.push_back(solutionCost);
        };

        return result;
    }
};
