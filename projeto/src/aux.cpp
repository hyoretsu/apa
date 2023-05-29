#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

// #include "./algorithms/dijkstra.cpp"
// #include "./algorithms/VariableNeighborhoodDescent.cpp"
#include "./algorithms/IteratedLocalSearch.cpp"
#include "./funcs/split.cpp"

typedef struct InputInfo {
    int manufacturingLines;
    int productN;
    int* times;
    int** switchTimes;
} InputInfo;

class TimeTracker {
private:
    std::vector<std::tuple<std::string, std::chrono::system_clock::time_point>> times;
public:
    TimeTracker() {
        this->times = std::vector<std::tuple<std::string, std::chrono::system_clock::time_point>>();

        auto beginningTimestamp = std::chrono::system_clock::now();
        this->times.push_back(std::make_tuple("beginning", beginningTimestamp));
    }

    void lap(const char* key) {
        auto timestamp = std::chrono::system_clock::now();

        this->times.push_back(std::make_tuple(key, timestamp));
    }

    void print() {
        auto executionTimestamp = std::chrono::system_clock::now();

        int loopN = 0;
        for (auto timeInfo : this->times) {
            if (loopN == 0) {
                loopN += 1;
                continue;
            }

            int time = std::chrono::duration_cast<std::chrono::microseconds>(
                std::get<1>(timeInfo) - std::get<1>(this->times[loopN - 1])
            ).count();

            std::cout << "Tempo de " << std::get<0>(timeInfo) << ": " << time << "µs" << std::endl;
            loopN += 1;
        }

        int time = std::chrono::duration_cast<std::chrono::microseconds>(
            executionTimestamp - std::get<1>(this->times[0])
        ).count();

        std::cout << "Tempo de execução do programa: " << time << "µs" << std::endl;
    }
};

InputInfo parseInput(std::ifstream& input) {
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(input, line)) {
        lines.push_back(line);
    }

    const int manufacturingLines = stoi(lines[0]);
    const int productN = stoi(lines[1]);

    int* times = new int[productN];
    std::vector<std::string> timesTmp = split(lines[3], " ");
    int i = 0;
    for (std::string time : timesTmp) {
        // Filter unwanted whitespaces
        if (!regex_match(time, regex("\\d+"))) continue;

        times[i] = stoi(time);
        i += 1;
    }

    int** switchTimes = new int* [productN];
    for (i = 0; i < productN; i++) {
        std::vector<std::string> switchTmp = split(lines[5 + i], " ");
        // Alocando memória a cada array de switchTimes
        switchTimes[i] = new int[productN];

        int j = 0;
        for (std::string switchTime : switchTmp) {
            // Filter unwanted whitespaces
            if (!regex_match(switchTime, regex("\\d+"))) continue;

            switchTimes[i][j] = stoi(switchTime);
            // Filter unwanted whitespaces
            j += 1;
        }
    }

    return {
        manufacturingLines,
        productN,
        times,
        switchTimes,
    };
}

std::vector<float> printDetails(
    const std::vector<AlgorithmReturn>& greedyResult,
    const std::vector<AlgorithmReturn>& vndResult,
    const std::vector<AlgorithmReturn>& ilsResult,
    bool calculateOnly = false
) {
    std::vector<float> maxCosts = { 0, 0, 0 };

    if (!calculateOnly) {
        std::cout << "----- Algoritmo guloso -----" << std::endl;
    }

    for (AlgorithmReturn result : greedyResult) {
        if (result.cost > maxCosts[0]) {
            maxCosts[0] = result.cost;
        }

        if (!calculateOnly) {
            std::cout << "Sequência: ";
            for (int elem : result.sequence) {
                std::cout << elem << ", ";
            }
            std::cout << std::endl;
            std::cout << "Custo: " << result.cost << std::endl;
        }
    }

    if (!calculateOnly) {
        std::cout << "------- Busca local -------" << std::endl;
    }

    for (AlgorithmReturn result : vndResult) {
        if (result.cost > maxCosts[1]) {
            maxCosts[1] = result.cost;
        }

        if (!calculateOnly) {
            std::cout << "Sequência: ";
            for (int elem : result.sequence) {
                std::cout << elem << ", ";
            }
            std::cout << std::endl;
            std::cout << "Custo: " << result.cost << std::endl;
        }
    }

    if (!calculateOnly) {
        std::cout << "----- Meta-heurística -----" << std::endl;
    }

    for (AlgorithmReturn result : ilsResult) {
        if (result.cost > maxCosts[2]) {
            maxCosts[2] = result.cost;
        }

        if (!calculateOnly) {
            std::cout << "Sequência: ";
            for (int elem : result.sequence) {
                std::cout << elem << ", ";
            }
            std::cout << std::endl;
            std::cout << "Custo: " << result.cost << std::endl;
        }
    }

    return maxCosts;
}
