#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "./aux.cpp"
#include "./data_structures/Graph.hpp"

int main(int argc, char* argv[]) {
#ifdef __WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    if (argc == 0) {
        std::cout << "Por favor, passe o caminho de um arquivo" << std::endl;
    } else if (argc > 2) {
        std::cout << "Argumentos inválidos, apenas 1 arquivo é suportado" << std::endl;

        return 1;
    }

    TimeTracker timeTracker = TimeTracker();

    ifstream inputFile(argv[1]);
    InputInfo info = parseInput(inputFile);
    inputFile.close();

    timeTracker.lap("leitura do arquivo");

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

    // graph.printEdges();

    timeTracker.lap("construção do grafo");

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
        // std::cout << lineProductN[i] << ", ";
    }
    // std::cout << std::endl;

    DijkstraCopycat dijkstraCopycat = DijkstraCopycat(graph);

    std::vector<AlgorithmReturn> greedyResult;
    for (int i = 0; i < info.manufacturingLines; i++) {
        std::vector<int> filteredEdges = std::vector<int>();

        if (i != 0) {
            for (int j = i - 1; j >= 0; j--) {
                AlgorithmReturn lastResult = greedyResult[j];

                filteredEdges.insert(filteredEdges.end(), lastResult.sequence.begin(), lastResult.sequence.end());
            }
        }

        greedyResult.push_back(dijkstraCopycat.findShortestPath(0, filteredEdges, lineProductN[i] + 1));
    }

    timeTracker.lap("execução do algoritmo guloso");

    VariableNeighborhoodDescent<int> vnd = VariableNeighborhoodDescent<int>(graph);
    std::vector<AlgorithmReturn> vndResult = vnd.execute(greedyResult);

    timeTracker.lap("execução do VND");

    IteratedLocalSearch<int> ils = IteratedLocalSearch<int>(vnd);
    std::vector<AlgorithmReturn> ilsResult = ils.execute(vndResult);

    timeTracker.lap("execução do ILS");

    // Troque para false para visualizar a sequência e o custo de cada linha de produção para cada algoritmo
    std::vector<float> maxCosts = printDetails(greedyResult, vndResult, ilsResult, true);

    // Liberando memória
    delete[] info.times;
    for (int i = 0; i < info.productN; i++) {
        delete[] info.switchTimes[i];
    }
    delete[] info.switchTimes;


    std::cout << "---------- Resumo ----------" << std::endl;
    std::cout << "Solução do guloso: " << maxCosts[0] << std::endl;
    std::cout << "Solução do VND: " << maxCosts[1] << std::endl;
    std::cout << "Solução do ILS: " << maxCosts[2] << std::endl;

    std::cout << "------- Estatísticas -------" << std::endl;
    timeTracker.print();

    return 0;
}
