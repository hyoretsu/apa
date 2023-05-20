#include <algorithm>
#include <limits>
#include <vector>

#include "../data_structures/Graph.cpp"
#include "../data_structures/Heap.cpp"

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
} DijkstraReturn;

DijkstraReturn dijkstraCopycat(Graph* graph, int initialVertex, std::vector<int>* filteredEdges, int maxSteps = 0) {
    Heap<VertexCost> priorityQueue = Heap<VertexCost>("min");

    for (int i = 0; i < graph->getVerticesCount(); i++) {
        int cost = std::numeric_limits<int>::max();
        if (i == initialVertex) cost = 0;

        priorityQueue.insert(VertexCost(i, cost));
    }

    // std::cout << "fila de prioridade: ";
    // priorityQueue.print();

    std::vector<int> sequence;
    float totalCost = 0;
    int steps = 0;

    while ((maxSteps && steps < maxSteps) || (!maxSteps && !priorityQueue.empty())) {
        int currentVertex = priorityQueue.front().vertex;
        float currentCost = priorityQueue.front().cost;
        priorityQueue.pop();

        sequence.push_back(currentVertex);
        // totalCost += currentCost;
        // REMOVE
        totalCost = currentCost;

        // std::cout << currentVertex << "-" << currentCost << "=" << totalCost << std::endl;

        Edge* edges = graph->getEdges(currentVertex);

        int pqSize = priorityQueue.size();
        std::vector<VertexCost> newPqElems;

        for (int i = 0; i < pqSize; i++) {
            int vertex = priorityQueue.front().vertex;
            float cost = priorityQueue.front().cost;
            priorityQueue.pop();

            // If current vertex is in the array
            if (std::find(filteredEdges->begin(), filteredEdges->end(), vertex) != filteredEdges->end()) {
                continue;
            }

            // REMOVE -1
            float edgeWeight = (*edges)[vertex - 1];
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
            // std::cout << elem << std::endl;
            priorityQueue.insert(elem);
        }

        steps += 1;

        // std::cout << "nova fila: ";
        // priorityQueue.print();
    }

    // REMOVE
    // Clean resulting sequence
    sequence.erase(sequence.begin());

    DijkstraReturn result = { sequence, totalCost };

    return result;
}
