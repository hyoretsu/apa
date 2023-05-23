#include <vector>

namespace utils {
    template <typename T>
    void swap(std::vector<T>* arr, int i, int j) {
        auto temp = (*arr)[i];
        (*arr)[i] = (*arr)[j];
        (*arr)[j] = temp;
    }
}
