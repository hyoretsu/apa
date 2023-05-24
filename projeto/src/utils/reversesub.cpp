#include <vector>

template <typename T>
void reverseSubsequence(std::vector<T>& arr, int start, int end) {
    while (start < end) {
        swap(arr, start, end);
        start++;
        end--;
    }
}
