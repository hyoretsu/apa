#include <vector>
#include <random>
#include <iostream>
#include <utility> 
#include <algorithm>

template <typename T>
class NeighborhoodMove {
private:
    std::vector<std::pair<int, int>> swapHistory; // histórico de trocas

public:
    void applySwap(std::vector<T>& arr, int i, int j) {
        std::swap(arr[i], arr[j]);
        swapHistory.push_back(std::make_pair(i, j)); // registrar a troca no histórico
    }

    void applyReverseSubsequence(std::vector<T>& arr, int start, int end) {
        while (start < end) {
            std::swap(arr[start], arr[end]);
            start++;
            end--;
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

    const std::vector<std::pair<int, int>>& getSwapHistory() const {
        return swapHistory; // retornar o histórico de trocas
    }
};
