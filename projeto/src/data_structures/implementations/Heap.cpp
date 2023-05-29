#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../utils/swap.hpp"
#include "../Heap.hpp"

template<typename T>
Heap<T>::Heap(const std::string& type) : type(type) {
    if (type != "min" && type != "max") {
        throw std::invalid_argument("There are only min and max heaps");
    }
}

template<typename T>
T& Heap<T>::operator[](int index) {
    return this->arr[index];
}

template<typename T>
typename std::vector<T>::iterator Heap<T>::begin() {
    return this->arr.begin();
}

template<typename T>
bool Heap<T>::empty() {
    return this->arr.empty();
}

template<typename T>
typename std::vector<T>::iterator Heap<T>::end() {
    return this->arr.end();
}

template<typename T>
void Heap<T>::erase() {
    this->arr.clear();
}

template<typename T>
std::vector<T>& Heap<T>::expose() {
    return this->arr;
}

template<typename T>
T Heap<T>::front() {
    return this->arr.front();
}

template<typename T>
void Heap<T>::insert(T value) {
    this->arr.push_back(value);

    int index = this->arr.size() - 1;
    int fatherIndex = floor((index - 1) / 2);

    // Gambiarra - .cost
    while (this->type == "min" ? this->arr[fatherIndex].cost > value.cost : this->arr[fatherIndex].cost < value.cost) {
        utils::swap<T>(this->arr, index, fatherIndex);

        index = fatherIndex;
        fatherIndex = floor((index - 1) / 2);
    }
}

template<typename T>
void Heap<T>::pop() {
    int index = 0;

    if (arr.size() <= 1) {
        this->arr.pop_back();
        return;
    }

    this->arr[index] = this->arr[this->arr.size() - 1];
    this->arr.pop_back();

    T biggestChild;

    do {
        int leftChildIndex = 2 * index + 1;
        int rightChildIndex = 2 * index + 2;

        if (std::max(leftChildIndex, rightChildIndex) > this->arr.size() - 1) {
            break;
        }

        biggestChild = std::max(this->arr[leftChildIndex], this->arr[rightChildIndex]);
        int biggestChildIndex = (biggestChild == this->arr[leftChildIndex]) ? leftChildIndex : rightChildIndex;

        utils::swap<T>(this->arr, index, biggestChildIndex);

        index = biggestChildIndex;
    } while (this->type == "min" ? this->arr[index] > biggestChild : this->arr[index] < biggestChild);
}

template<typename T>
void Heap<T>::print() {
    for (T elem : this->arr) {
        std::cout << elem << ", ";
    }
    std::cout << std::endl;
}

template<typename T>
long Heap<T>::size() {
    return this->arr.size();
}
