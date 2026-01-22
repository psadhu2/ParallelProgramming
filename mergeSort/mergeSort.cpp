#include <iostream>
#include "merge_sort_functions.h"

//Issues: The function copies all the values of the array every time is called causing insanely high memory overload

std::vector<int> mergeSort(std::vector<int> &array) {
    int length = array.size();

    if (length <= 1) {
        return array;
    }

    int mid = (length / 2);

    std::vector<int> arrayLeft(array.begin(), array.begin() + mid);
    std::vector<int> arrayRight(array.begin() + mid, array.end());


    std::vector<int> left = mergeSort(arrayLeft);
    std::vector<int> right = mergeSort(arrayRight);

    return merge(left, right);
}

std::vector<int> merge(std::vector<int> &leftArray, std::vector<int> &rightArray) {
    std::vector<int> array;

    int i = 0; // ptr to traverse first array
    int j = 0; // ptr to traverse second array

    while (i < leftArray.size() && j < rightArray.size()) {
        if (leftArray[i] < rightArray[j]) {
            array.push_back(leftArray[i]);
            i++;
        } else {
            array.push_back(rightArray[j]);
            j++;
        }
    }

    while (i < leftArray.size()) {
        array.push_back(leftArray[i]);
        i++;
    }
    while (j < rightArray.size()) {
        array.push_back(rightArray[j]);
        j++;
    }

    return array;
}