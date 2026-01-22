#! /bin/bash

SOURCE_FILE="main.cpp"

MERGE_SORT_FUNCTIONS="mergeSort.cpp"

EXECUTABLE="./a.exe"

g++ "$SOURCE_FILE" "$MERGE_SORT_FUNCTIONS" -o "$EXECUTABLE"