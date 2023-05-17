#include <iostream>
#include <map>
#include <vector>

using Edge = std::map<int, float>;

class Graph {
private:
    Edge* edges;
    int verticesN;
public:
    // Constructor
    explicit Graph(const int verticesN) {
        this->edges = new Edge[verticesN];
        this->verticesN = verticesN;
    }

    // Copy constructor
    Graph(const Graph& other) {
        this->edges = new Edge[other.verticesN];
        for (int i = 0; i < other.verticesN; i++) {
            this->edges[i] = other.edges[i];
        }

        this->verticesN = other.verticesN;
    };

    // Copy assignment
    Graph& operator=(const Graph& other) {
        // I'm not sure if this is the right way to do copy assignment
        this->edges = other.edges;
        this->verticesN = other.verticesN;

        return *this;
    }

    void addEdge(int source, int destination, float weight) {
        edges[source][destination] = weight;
    }

    Edge* filterEdges(std::vector<int> edges) {
        Edge* filteredEdges = new Edge[this->verticesN];
        int i = 0;
        while (i < verticesN) {
            for (int elem : edges) {
                if (i == elem) {
                    filteredEdges[i] = this->edges[i];
                }
            }

            i += 1;
        }

        return filteredEdges;
    }

    int getVerticesCount() {
        return verticesN;
    }

    Edge* getEdges(int vertex) {
        return &this->edges[vertex];
    }

    void printEdges() {
        for (int i = 0; i < this->verticesN; i++) {
            int adjacentVerticesN = this->edges->size();
            for (int j = 0; j < adjacentVerticesN; j++) {
                std::cout << i << "->" << j + 1 << "." << this->edges[i][j] << ((j != adjacentVerticesN - 1) ? ", " : "");
            }

            std::cout << std::endl;
        }
    }
};
