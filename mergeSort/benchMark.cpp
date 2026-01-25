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
    
    std::vector<int> array(size);

    std::random_device r;
    std::mt19937 gen(r());
    std::uniform_int_distribution<int> randNums(1, 1000);

    for (int i = 0; i < size; i++) {
        array[i] = randNums(r);
    }

    auto start = std::chrono::steady_clock::now();

    // std::vector<int> sorted = mergeSort(array);
    mergeSort(array, 0, array.size() - 1);

    auto end = std::chrono::steady_clock::now();

    double duration = std::chrono::duration<double>(end - start).count();

    cout << size << "," << duration << '\n';

    return 0;
}