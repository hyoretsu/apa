#ifndef Heap_H
#define Heap_H

#include <string>
#include <vector>

template<typename T = int>
class Heap {
private:
    std::vector<T> arr;
    std::string type;
public:
    explicit Heap(const std::string& type);
    T& operator[](int index);

    typename std::vector<T>::iterator begin();
    bool empty();
    typename std::vector<T>::iterator end();
    void erase();
    std::vector<T>* expose();
    T front();
    void insert(T value);
    void pop();
    void print();
    long size();
};

#include "implementations/Heap.cpp"

#endif
