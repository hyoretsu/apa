#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#include "./funcs/split.cpp"

using namespace std;

typedef struct Edge {
    int source;
    int destination;
    int weight;
} Edge;

typedef struct InputInfo {
    int manufacturingLines;
    int productN;
    int* times;
    int** switchTimes;
} InputInfo;

InputInfo parseInput(ifstream* input) {
    string line;
    vector<string> lines;

    while (getline(*input, line)) {
        lines.push_back(line);
    }

    const int manufacturingLines = stoi(lines[0]);
    const int productN = stoi(lines[1]);

    int* times = new int[productN];
    vector<string> timesTmp = split(lines[3], " ");
    for (int i = 0; i < productN;i++) {
        times[i] = stoi(timesTmp[i]);
    }

    int** switchTimes = new int* [productN];
    for (int i = 0; i < productN; i++) {
        vector<string> switchTmp = split(lines[5 + i], " ");
        // Alocando memória a cada array de switchTimes
        switchTimes[i] = new int[productN];

        for (int j = 0; j < productN; j++) {
            switchTimes[i][j] = stoi(switchTmp[j]);
        }
    }

    return {
        manufacturingLines,
        productN,
        times,
        switchTimes,
    };
}

int main(int argc, char* argv[]) {
    auto beginningTimestamp = chrono::high_resolution_clock::now();

    ifstream inputFile(argv[1]);
    InputInfo info = parseInput(&inputFile);
    inputFile.close();

    auto fileReadTimestamp = chrono::high_resolution_clock::now();

    vector<Edge> manufacturingLineGraph[info.productN + 1];
    // Começamos sem nenhum produto
    for (int i = 0; i < info.productN + 1; i++) {
        // E para nenhum produto e para cada, podemos ir para todo outro produto
        for (int j = 0; j < info.productN; j++) {
            int selfWeight = ((i - 1) == j) ? 0 : info.times[j];
            int cleaningWeight = ((i == 0) ? 0 : info.switchTimes[i - 1][j]);

            Edge edge;
            edge.source = i;
            edge.destination = j + 1;
            // Caso os produtos sejam iguais, o peso é 0 (inválido)
            edge.weight = selfWeight + cleaningWeight;
            manufacturingLineGraph[i].push_back(edge);
            // cout << edge.source << "->" << edge.destination << "." << selfWeight << "+" << cleaningWeight << ", ";
        }
        // cout << endl;
    }

    auto graphConstructionTimestamp = chrono::high_resolution_clock::now();

    // Liberando memória
    delete[] info.times;
    for (int i = 0; i < info.productN; i++) {
        delete[] info.switchTimes[i];
    }
    delete[] info.switchTimes;

    auto executionTimestamp = chrono::high_resolution_clock::now();

    // Calculando estatísticas
    int fileReadTime = chrono::duration_cast<chrono::microseconds>(
        fileReadTimestamp - beginningTimestamp
    ).count();
    int graphConstructionTime = chrono::duration_cast<chrono::microseconds>(
        graphConstructionTimestamp - fileReadTimestamp
    ).count();
    int executionTime = chrono::duration_cast<chrono::microseconds>(
        executionTimestamp - beginningTimestamp
    ).count();

    cout << "Tempo de leitura do arquivo: " << fileReadTime << "µs" << endl;
    cout << "Tempo de construção do grafo: " << graphConstructionTime << "µs" << endl;
    cout << "Tempo de execução do programa: " << executionTime << "µs" << endl;

    return 0;
}
