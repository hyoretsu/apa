#include <vector>

#include "./VariableNeighborhoodDescent.cpp"

template<typename T>
class IteratedLocalSearch {
private:
    VariableNeighborhoodDescent<T> vnd;
public:
    IteratedLocalSearch(VariableNeighborhoodDescent<T>& vnd) : vnd(vnd) {}

    std::vector<AlgorithmReturn> execute(const std::vector<AlgorithmReturn>& initialResult) {
        std::vector<AlgorithmReturn> prevResult = initialResult;
        int solutionSize = initialResult.size();

        float maxPrevCost = 0;
        for (int i = 0; i < solutionSize; i++) {
            if (initialResult[i].cost > maxPrevCost) {
                maxPrevCost = initialResult[i].cost;
            };
        }

        int timeout = solutionSize * 10, unsuccessfulTries = 0;
        while (unsuccessfulTries < timeout) {
            std::vector<AlgorithmReturn> nextResult, perturbedSolution = prevResult;

            for (int i = 0; i < solutionSize; i++) {
                this->vnd.scramble(perturbedSolution[i].sequence);
                perturbedSolution[i].cost = this->vnd.getGraph().calculateCost(perturbedSolution[i].sequence, true);
            }

            nextResult = this->vnd.execute(perturbedSolution);

            std::vector<float> nextCosts(solutionSize);
            for (int i = 0; i < solutionSize; i++) {
                nextCosts[i] = nextResult[i].cost;
            }

            float maxNextCost = *std::max_element(nextCosts.begin(), nextCosts.end());

            if (maxNextCost < maxPrevCost) {
                prevResult = nextResult;
                maxPrevCost = maxNextCost;

                unsuccessfulTries = 0;
            } else {
                unsuccessfulTries += 1;
            }
        }

        return prevResult;
    }
};
