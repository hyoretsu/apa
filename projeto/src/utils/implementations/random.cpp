#include <random>

std::random_device rd;
std::mt19937 rng(rd());

int random(int min = 0, int max = 1) {
    std::uniform_int_distribution<std::mt19937::result_type> randomNumber(min, max);

    return randomNumber(rng);
}
