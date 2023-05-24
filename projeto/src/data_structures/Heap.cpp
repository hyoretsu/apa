#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../utils/swap.cpp"

template<typename T = int>
class Heap {
private:
    std::vector<T> arr;
    std::string type;
public:
    // Constructor
    explicit Heap(const std::string& type) : type(type) {
        if (type != "min" && type != "max") {
            throw std::invalid_argument("There are only min and max heaps");
        }
    }

    T& operator[](int index) {
        return this->arr[index];
    }

    typename std::vector<T>::iterator begin() {
        return this->arr.begin();
    }

    bool empty() {
        return this->arr.empty();
    }

    typename std::vector<T>::iterator end() {
        return this->arr.end();
    }

    void erase() {
        this->arr.clear();
    }

    std::vector<T>* expose() {
        return &this->arr;
    }

    T front() {
        return this->arr.front();
    }

    void insert(T value) {
        this->arr.push_back(value);

        int index = this->arr.size() - 1;
        int fatherIndex = floor((index - 1) / 2);

        // Gambiarra - .cost
        while (this->type == "min" ? this->arr[fatherIndex].cost > value.cost : this->arr[fatherIndex].cost < value.cost) {
            utils::swap<T>(&this->arr, index, fatherIndex);

            index = fatherIndex;
            fatherIndex = floor((index - 1) / 2);
        }
    }

    void pop() {
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

            utils::swap<T>(&this->arr, index, biggestChildIndex);

            index = biggestChildIndex;
        } while (this->type == "min" ? this->arr[index] > biggestChild : this->arr[index] < biggestChild);
    }

    void print() {
        for (T elem : this->arr) {
            std::cout << elem << ", ";
        }
        std::cout << std::endl;
    }

    long size() {
        return this->arr.size();
    }
};
