#ifndef Graph_H
#define Graph_H

#include <map>
#include <vector>

using Edge = std::map<int, float>;

class Graph {
private:
    std::vector<Edge> edges;
public:
    explicit Graph(const int verticesN);
    Graph(const Graph& other);
    Graph& operator=(const Graph& other);

    void addEdge(int source, int destination, float weight);
    float calculateCost(const std::vector<int>& vertices, bool initialVertex);
    std::vector<Edge> filterEdges(const std::vector<int>& edges);
    long getVerticesCount();
    Edge& getEdges(int vertex);
    void printEdges();
};

#include "implementations/Graph.cpp"

#endif
