#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

#include "./algorithms/dijkstra.cpp"
#include "./funcs/split.cpp"

using namespace std;

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
    int i = 0;
    for (string time : timesTmp) {
        // Filter unwanted whitespaces
        if (!regex_match(time, regex("\\d+"))) continue;

        times[i] = stoi(time);
        i += 1;
    }

    int** switchTimes = new int* [productN];
    for (i = 0; i < productN; i++) {
        vector<string> switchTmp = split(lines[5 + i], " ");
        // Alocando memória a cada array de switchTimes
        switchTimes[i] = new int[productN];

        int j = 0;
        for (string switchTime : switchTmp) {
            // Filter unwanted whitespaces
            if (!regex_match(switchTime, regex("\\d+"))) continue;

            switchTimes[i][j] = stoi(switchTime);
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

int main(int argc, char* argv[]) {
    auto beginningTimestamp = chrono::high_resolution_clock::now();

    ifstream inputFile(argv[1]);
    InputInfo info = parseInput(&inputFile);
    inputFile.close();

    auto fileReadTimestamp = chrono::high_resolution_clock::now();

    Graph graph = Graph(info.productN + 1);

    // Começamos sem nenhum produto
    for (int i = 0; i < info.productN + 1; i++) {
        // E para nenhum produto e para cada, podemos ir para todo outro produto
        for (int j = 0; j < info.productN; j++) {
            // Não há repetição de produtos
            if (i == j + 1) continue;

            graph.addEdge(
                i,
                j,
                info.times[j] + ((i == 0) ? 0 : info.switchTimes[i - 1][j])
            );
        }
    }

    // graph->printEdges();

    auto graphConstructionTimestamp = chrono::high_resolution_clock::now();

    // Descobrir quantos produtos teremos em cada linha de produção se divididos igualmente
    int lineProductN[info.manufacturingLines];
    const int lineTmp[2] = {
         info.productN / info.manufacturingLines,
         info.productN % info.manufacturingLines,
    };
    for (int i = 0; i < info.manufacturingLines; i++) {
        bool overhead = false;

        if (i < lineTmp[1]) {
            overhead = true;
        }

        lineProductN[i] = lineTmp[0] + (overhead ? 1 : 0);
        // cout << lineProductN[i] << ", ";
    }
    // cout << endl;

    vector<DijkstraReturn> firstRunResult;
    for (int i = 0; i < info.manufacturingLines; i++) {
        std::vector<int> filteredEdges(lineProductN[0]);

        if (i != 0) {
            for (int j = i - 1; j >= 0; j--) {
                DijkstraReturn lastResult = firstRunResult[j];

                std::copy(lastResult.sequence.begin(), lastResult.sequence.end(), filteredEdges.begin());
            }
        }

        firstRunResult.push_back(dijkstraCopycat(&graph, 0, &filteredEdges, lineProductN[i] + 1));
    }

    for (DijkstraReturn result : firstRunResult) {
        cout << "Sequência: ";
        for (int elem : result.sequence) {
            cout << elem << ", ";
        }
        cout << endl;
        cout << "Custo: " << result.cost << endl;
    }

    auto greedyAlgorithmTimestamp = chrono::high_resolution_clock::now();

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
    int greedyAlgorithmTime = chrono::duration_cast<chrono::microseconds>(
        greedyAlgorithmTimestamp - graphConstructionTimestamp
    ).count();
    int executionTime = chrono::duration_cast<chrono::microseconds>(
        executionTimestamp - beginningTimestamp
    ).count();

    cout << "Tempo de leitura do arquivo: " << fileReadTime << "µs" << endl;
    cout << "Tempo de construção do grafo: " << graphConstructionTime << "µs" << endl;
    cout << "Tempo de execução do algoritmo guloso: " << greedyAlgorithmTime << "µs" << endl;
    cout << "Tempo de execução do programa: " << executionTime << "µs" << endl;

    return 0;
}
