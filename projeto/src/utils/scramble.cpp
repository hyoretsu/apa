#include <vector>
#include <random>

template <typename T>
void scramble(std::vector<T>& arr) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, arr.size() - 1);

    for (int i = 0; i < arr.size(); i++) {
        int j = dist(gen);
        swap(arr, i, j);
    }
}
