#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "merge_sort_functions.h"


std::string toString(std::vector<int> array);


// Need to debug mergeSort function


int main () {
    using std::cout;
    using std::cin;
    using namespace std::chrono;

    int size;
    
    cout << "Input the size of an array you want to sort" << '\n';
    cin >> size;
    std::vector<int> array;

    
    // Generating random numbers, then appending each random number 
    // to the array and adding to a string for a nice output
    
    for (int i = 0; i < size; i++) {
        std::random_device r;
        std::uniform_int_distribution<int> randNums(1, 1000);

        int rN = randNums(r);
        array.push_back(rN);
    }
    

    cout << "Unsorted array: " << toString(array) << '\n';

    auto start = std::chrono::steady_clock::now();

    std::vector<int> sorted = mergeSort(array);

    auto end = std::chrono::steady_clock::now();

    cout << "Sorted array: " << toString(sorted) << '\n';

    double duration = std::chrono::duration<double>(end - start).count();

    cout << "It took " << duration << " seconds to sort the array" << '\n';

    return 0;
}


std::string toString(std::vector<int> array) {
    std::string strRandList = "[";
    for (int i = 0; i < array.size(); i++) {
        strRandList += std::to_string(array[i]);
        if (i != (array.size() - 1)) {
            strRandList += ", ";
        }
    }
    strRandList += "]";
    return strRandList;
}
