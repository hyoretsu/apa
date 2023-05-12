#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

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
    ifstream inputFile(argv[1]);
    InputInfo info = parseInput(&inputFile);
    inputFile.close();

    // Liberando memória
    delete[] info.times;
    for (int i = 0; i < info.productN; i++) {
        delete[] info.switchTimes[i];
    }
    delete[] info.switchTimes;

    return 0;
}
