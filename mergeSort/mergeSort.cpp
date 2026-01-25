#include <iostream>
#include "merge_sort_functions.h"

void mergeSort(std::vector<int> &array, int leftIdx, int rightIdx) {
    if (leftIdx >= rightIdx) {
        return;
    }
    int midIdx = (leftIdx + rightIdx) / 2;
    mergeSort(array, leftIdx, midIdx);
    mergeSort(array, midIdx + 1, rightIdx);
    merge(array, leftIdx, midIdx, rightIdx);
}

void merge(std::vector<int> &array, int leftIdx, int midIdx, int rightIdx) {
    std::vector<int> leftArray(midIdx - leftIdx + 1), rightArray(rightIdx - midIdx);

    for (int i = 0; i < leftArray.size(); i++) {
        leftArray[i] = array[leftIdx + i];
    }
    
    for (int i = 0; i < rightArray.size(); i++) {
        rightArray[i] = array[midIdx + 1 + i];
    }

    int i = 0, j = 0;
    int k = leftIdx;

    while (i < leftArray.size() && j < rightArray.size()) {
        if (leftArray[i] < rightArray[j]) {
            array[k] = leftArray[i];
            i++;
        } else {
            array[k] = rightArray[j];
            j++;
        }
        k++;
    }

    while (i < leftArray.size()) {
        array[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < rightArray.size()) {
        array[k] = rightArray[j];
        j++;
        k++;
    }
}

//Issues: The function copies all the values of the array every time is called causing insanely high memory overload

// std::vector<int> mergeSort(std::vector<int> &array) {
//     int length = array.size();

//     if (length <= 1) {
//         return array;
//     }

//     int mid = (length / 2);

//     std::vector<int> arrayLeft(array.begin(), array.begin() + mid);
//     std::vector<int> arrayRight(array.begin() + mid, array.end());


//     std::vector<int> left = mergeSort(arrayLeft);
//     std::vector<int> right = mergeSort(arrayRight);

//     return merge(left, right);
// }

// std::vector<int> merge(std::vector<int> &leftArray, std::vector<int> &rightArray) {
//     std::vector<int> array;

//     int i = 0; // ptr to traverse first array
//     int j = 0; // ptr to traverse second array

//     while (i < leftArray.size() && j < rightArray.size()) {
//         if (leftArray[i] < rightArray[j]) {
//             array.push_back(leftArray[i]);
//             i++;
//         } else {
//             array.push_back(rightArray[j]);
//             j++;
//         }
//     }

//     while (i < leftArray.size()) {
//         array.push_back(leftArray[i]);
//         i++;
//     }
//     while (j < rightArray.size()) {
//         array.push_back(rightArray[j]);
//         j++;
//     }

//     return array;
// }