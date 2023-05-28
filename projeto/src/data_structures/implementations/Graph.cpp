#include "../Graph.hpp"

#include <iostream>

// Constructor
Graph::Graph(const int verticesN) : edges(verticesN) {}

// Copy constructor
Graph::Graph(const Graph& other) : edges(other.edges) {}

// Copy assignment
Graph& Graph::operator=(const Graph& other) {
    if (this != &other) {
        this->edges = other.edges;
    }

    return *this;
}

void Graph::addEdge(int source, int destination, float weight) {
    this->edges[source][destination] = weight;
}

float Graph::calculateCost(const std::vector<int>& vertices, bool initialVertex = false) {
    float totalCost = 0;

    int verticesCount = vertices.size();
    for (int i = 0; i < verticesCount; i++) {
        if (i == 0 && initialVertex) {
            totalCost += this->edges[0][vertices[i] - 1];
        }

        totalCost += this->edges[vertices[i]][vertices[i + 1] - 1];
    }

    return totalCost;
}

std::vector<Edge> Graph::filterEdges(const std::vector<int>& edges) {
    std::vector<Edge> filteredEdges(edges.size());
    for (int i : edges) {
        filteredEdges.push_back(this->edges[i]);
    }

    return filteredEdges;
}

long Graph::getVerticesCount() {
    return this->edges.size();
}

Edge& Graph::getEdges(int vertex) {
    return this->edges[vertex];
}

void Graph::printEdges() {
    int verticesN = this->edges.size();

    for (int i = 0; i < verticesN; i++) {
        int adjacentVerticesN = this->edges.size();
        for (int j = 0; j < adjacentVerticesN; j++) {
            std::cout << i << "->" << j + 1 << "." << this->edges[i][j] << ((j != adjacentVerticesN - 1) ? ", " : "");
        }

        std::cout << std::endl;
    }
}
