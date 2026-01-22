#include <iostream>
#include <random>
#include <chrono>
#include "merge_sort_functions.h"

int main(int argc, char* argv[]) {
    using std::cout;

    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <size>\n";
        return 1;
    }

    int size = std::stoi(argv[1]);
    
    std::vector<int> array;

    for (int i = 0; i < size; i++) {
        std::random_device r;
        std::uniform_int_distribution<int> randNums(1, 1000);

        int rN = randNums(r);
        array.push_back(rN);
    }

    auto start = std::chrono::steady_clock::now();

    std::vector<int> sorted = mergeSort(array);

    auto end = std::chrono::steady_clock::now();

    double duration = std::chrono::duration<double>(end - start).count();

    cout << size << "," << duration << '\n';

    return 0;
}