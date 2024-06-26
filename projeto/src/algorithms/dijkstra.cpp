#include <algorithm>
#include <limits>
#include <vector>

#include "../data_structures/Graph.hpp"
#include "../data_structures/Heap.hpp"

class VertexCost {
public:
    float cost;
    int vertex;

    VertexCost() {}
    VertexCost(int vertex, float cost) : cost(cost), vertex(vertex) {}

    friend bool operator==(const VertexCost& a, const VertexCost& b) {
        return a.cost == b.cost && a.vertex == b.vertex;
    }

    friend bool operator<(const VertexCost& a, const VertexCost& b) {
        int firstValue = a.cost;
        int secondValue = b.cost;

        // If both values are the same
        if (firstValue == secondValue) {
            int firstKey = a.vertex;
            int secondKey = b.vertex;

            // Order ascendingly by their keys
            return firstKey > secondKey;
        }

        return firstValue < secondValue;
    }

    friend bool operator>(const VertexCost& a, const VertexCost& b) {
        int firstValue = a.cost;
        int secondValue = b.cost;

        // If both values are the same
        if (firstValue == secondValue) {
            int firstKey = a.vertex;
            int secondKey = b.vertex;

            // Order ascendingly by their keys
            return firstKey > secondKey;
        }

        return firstValue > secondValue;
    }

    friend std::ostream& operator<<(std::ostream& os, const VertexCost& elem) {
        os << elem.vertex << "->" << elem.cost;

        return os;
    };
};

typedef struct {
    std::vector<int> sequence;
    float cost;
} AlgorithmReturn;

class DijkstraCopycat {
private:
    Graph& graph;
    Heap<VertexCost> priorityQueue;

    void initPriorityQueue(int initialVertex) {
        this->priorityQueue.erase();

        for (int i = 0; i < this->graph.getVerticesCount(); i++) {
            int cost = std::numeric_limits<int>::max();
            if (i == initialVertex) cost = 0;

            this->priorityQueue.insert(VertexCost(i, cost));
        }
    }

public:
    explicit DijkstraCopycat(Graph& graph) : graph(graph), priorityQueue(Heap<VertexCost>("min")) {
    }

    AlgorithmReturn findShortestPath(
        int initialVertex,
        const std::vector<int>& filteredEdges = std::vector<int>(),
        int maxSteps = 0
    ) {
        this->initPriorityQueue(initialVertex);

        // std::cout << "fila de prioridade: ";
        // this->priorityQueue.print();

        std::vector<int> sequence;
        float totalCost = 0;
        int steps = 0;

        while ((maxSteps && steps < maxSteps) || (!maxSteps && !this->priorityQueue.empty())) {
            int currentVertex = this->priorityQueue.front().vertex;
            float currentCost = this->priorityQueue.front().cost;
            this->priorityQueue.pop();

            sequence.push_back(currentVertex);
            // totalCost += currentCost;
            // REMOVE
            totalCost = currentCost;

            // std::cout << currentVertex << "-" << currentCost << "=" << totalCost << std::endl;

            Edge edges = this->graph.getEdges(currentVertex);

            int pqSize = this->priorityQueue.size();
            std::vector<VertexCost> newPqElems;

            for (int i = 0; i < pqSize; i++) {
                int vertex = this->priorityQueue.front().vertex;
                float cost = this->priorityQueue.front().cost;
                this->priorityQueue.pop();

                // If current vertex is in the array
                if (
                    !filteredEdges.empty() &&
                    (std::find(filteredEdges.begin(), filteredEdges.end(), vertex) != filteredEdges.end())
                    ) {
                    continue;
                }

                // REMOVE -1
                float edgeWeight = edges[vertex - 1];
                if (edgeWeight) {
                    float newCost = currentCost + edgeWeight;

                    // REMOVE
                    // if (newCost < cost) {
                    newPqElems.push_back(VertexCost(vertex, newCost));
                    continue;
                    // }
                }

                newPqElems.push_back(VertexCost(vertex, cost));
            }

            for (VertexCost elem : newPqElems) {
                // std::cout << elem << ", ";
                this->priorityQueue.insert(elem);
            }
            // std::cout << std::endl;

            steps += 1;

            // std::cout << "nova fila: ";
            // this->priorityQueue.print();
        }

        // REMOVE
        // Clean resulting sequence
        sequence.erase(sequence.begin());

        AlgorithmReturn result = { sequence, totalCost };

        return result;
    }
};
