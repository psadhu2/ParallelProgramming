#include <iostream>
#include <vector>
#include <random>
#include <chrono>

std::vector<int> mergeSort(std::vector<int> array);
std::vector<int> merge(std::vector<int> arrayA, std::vector<int> arrayB);
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

    auto start = high_resolution_clock::now();

    std::vector<int> sorted = mergeSort(array);

    auto end = high_resolution_clock::now();

    cout << "Sorted array: " << toString(sorted) << '\n';

    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "It took " << duration << " milliseconds to sort the array" << '\n';

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


std::vector<int> mergeSort(std::vector<int> array) {
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

std::vector<int> merge(std::vector<int> leftArray, std::vector<int> rightArray) {
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