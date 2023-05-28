#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <locale>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

// #include "./algorithms/dijkstra.cpp"
#include "./funcs/split.cpp"
#include "./algorithms/VariableNeighborhoodDescent.cpp"

using namespace std;

typedef struct InputInfo {
    int manufacturingLines;
    int productN;
    int* times;
    int** switchTimes;
} InputInfo;

class TimeTracker {
private:
    vector<tuple<string, chrono::system_clock::time_point>> times;
public:
    TimeTracker() {
        this->times = vector<tuple<string, chrono::system_clock::time_point>>();

        auto beginningTimestamp = chrono::system_clock::now();
        this->times.push_back(make_tuple("beginning", beginningTimestamp));
    }

    void lap(const char* key) {
        auto timestamp = chrono::system_clock::now();

        this->times.push_back(make_tuple(key, timestamp));
    }

    void print() {
        auto executionTimestamp = chrono::system_clock::now();

        int loopN = 0;
        for (auto timeInfo : this->times) {
            if (loopN == 0) {
                loopN += 1;
                continue;
            }

            int time = chrono::duration_cast<chrono::microseconds>(
                get<1>(timeInfo) - get<1>(this->times[loopN - 1])
            ).count();

            cout << "Tempo de " << get<0>(timeInfo) << ": " << time << "µs" << endl;
            loopN += 1;
        }

        int time = chrono::duration_cast<chrono::microseconds>(
            executionTimestamp - get<1>(this->times[0])
        ).count();

        cout << "Tempo de execução do programa: " << time << "µs" << endl;
    }
};

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

int main(int argc, char* argv[]) {
#ifdef __WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    TimeTracker timeTracker = TimeTracker();

    ifstream inputFile(argv[1]);
    InputInfo info = parseInput(&inputFile);
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
        // cout << lineProductN[i] << ", ";
    }
    // cout << endl;

    DijkstraCopycat dijkstraCopycat = DijkstraCopycat(&graph);

    vector<DijkstraReturn> firstRunResult;
    for (int i = 0; i < info.manufacturingLines; i++) {
        std::vector<int> filteredEdges = std::vector<int>();

        if (i != 0) {
            for (int j = i - 1; j >= 0; j--) {
                DijkstraReturn lastResult = firstRunResult[j];

                filteredEdges.insert(filteredEdges.end(), lastResult.sequence.begin(), lastResult.sequence.end());
            }
        }

        firstRunResult.push_back(dijkstraCopycat.findShortestPath(0, filteredEdges, lineProductN[i] + 1));
    }
    
    timeTracker.lap("execução do algoritmo guloso");

    cout << "+-------Dijkstra-------+" << endl;

    for (DijkstraReturn result : firstRunResult) {
        cout << "Sequência: ";
        for (int elem : result.sequence) {
            cout << elem << " > ";
        }
        cout << endl;
        cout << "Custo: " << result.cost << endl;
    }
    
    cout << "+---------------------+" << endl;

    cout << endl;

    vector<vector<DijkstraReturn>> vndResult;
    
    int i = 0;

    for (DijkstraReturn result : firstRunResult) {
        VariableNeighborhoodDescent<int> vnd = VariableNeighborhoodDescent<int>(graph, result.sequence);

        vndResult.resize(firstRunResult.size());
        
        for (int j = 0; j < 10; j++) {
            vndResult[i].push_back(vnd.execute(result.sequence));
        }
        
        i++;
    }

    
    cout << "+-------VariableNeighborhoodDescent-------+" << endl;

    for (vector<DijkstraReturn> resultArr : vndResult) {
        for (int i = 0; i < resultArr.size(); i++) {
            cout << "--------------" << endl;
            cout << "Sequência: ";
            for (int elem : resultArr[i].sequence) {
                cout << elem << " > ";
            }
            cout << endl;
            cout << "Custo: " << resultArr[i].cost << endl;
            cout << "--------------" << endl;
        }
    }

    cout << "+---------------------+" << endl;

        
    // Liberando memória
    delete[] info.times;
    for (int i = 0; i < info.productN; i++) {
        delete[] info.switchTimes[i];
    }
    delete[] info.switchTimes;

    timeTracker.print();

    return 0;
}