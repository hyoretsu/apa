#ifndef Swap_H
#define Swap_H

#include <vector>

namespace utils {
    template <typename T = int>
    void swap(std::vector<T>& arr, int i, int j);
}

#include "implementations/swap.cpp"

#endif
