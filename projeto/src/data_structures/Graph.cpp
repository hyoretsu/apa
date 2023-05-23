#include <iostream>
#include <map>
#include <vector>

using Edge = std::map<int, float>;

class Graph {
private:
    std::vector<Edge> edges;
public:
    // Constructor
    explicit Graph(const int verticesN) : edges(verticesN) {}

    // Copy constructor
    Graph(const Graph& other) : edges(other.edges) {};

    // Copy assignment
    Graph& operator=(const Graph& other) {
        if (this != &other) {
            this->edges = other.edges;
        }

        return *this;
    }

    void addEdge(int source, int destination, float weight) {
        edges[source][destination] = weight;
    }

    float calculateCost(std::vector<int>* vertices, bool initialVertex) {
        float totalCost = 0;

        int verticesCount = vertices->size();
        for (int i = 0; i < verticesCount - 1; i++) {
            if (i == 0 && initialVertex) {
                totalCost += this->edges[0][(*vertices)[i]];
            }

            totalCost += this->edges[(*vertices)[i]][(*vertices)[i + 1]];

            i += 1;
        }

        return totalCost;
    }

    std::vector<Edge> filterEdges(const std::vector<int>& edges) {
        std::vector<Edge> filteredEdges(edges.size());
        for (int i : edges) {
            filteredEdges.push_back(this->edges[i]);
        }


        return filteredEdges;
    }

    long getVerticesCount() {
        return this->edges.size();
    }

    Edge& getEdges(int vertex) {
        return this->edges[vertex];
    }

    void printEdges() {
        int verticesN = this->edges.size();

        for (int i = 0; i < verticesN; i++) {
            int adjacentVerticesN = this->edges.size();
            for (int j = 0; j < adjacentVerticesN; j++) {
                std::cout << i << "->" << j + 1 << "." << this->edges[i][j] << ((j != adjacentVerticesN - 1) ? ", " : "");
            }

            std::cout << std::endl;
        }
    }
};
