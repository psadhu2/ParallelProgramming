#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H
#include <vector>

// Passed arguments as references to increase efficenicy of the program when dealing with very large vectors
std::vector<int> mergeSort(std::vector<int> &array);
std::vector<int> merge(std::vector<int> &arrayA, std::vector<int> &arrayB);

#endif