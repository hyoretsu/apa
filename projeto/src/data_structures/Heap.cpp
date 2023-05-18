#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

template<typename T = int>
class Heap {
private:
    std::vector<T> arr;
    std::string type;
public:
    // Constructor
    explicit Heap(const std::string& type) {
        if (type != "min" && type != "max") {
            throw std::invalid_argument("There are only min and max heaps");
        }

        this->type = type;
    }

    T& operator[](int index) {
        return this->arr[index];
    }

    auto begin() {
        return this->arr.begin();
    }

    bool empty() {
        return this->arr.empty();
    }

    auto end() {
        return this->arr.end();
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
            T temp = this->arr[fatherIndex];
            this->arr[fatherIndex] = this->arr[index];
            this->arr[index] = temp;

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

            T temp = this->arr[index];
            this->arr[index] = this->arr[biggestChildIndex];
            this->arr[biggestChildIndex] = temp;

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