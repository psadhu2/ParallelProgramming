#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H
#include <vector>

// std::vector<int> mergeSort(std::vector<int> &array);
// std::vector<int> merge(std::vector<int> &arrayA, std::vector<int> &arrayB);
void mergeSort(std::vector<int> &array, int leftIdx, int rightIdx);
void merge(std::vector<int> &array, int leftIdx, int midIdx, int rightIdx);

#endif