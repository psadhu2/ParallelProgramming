#! /bin/bash
#SBATCH --job-name=pranav-merge-sort
#SBATCH --partition=Centaurus
#SBATCH --mem=32G
#SBATCH --time=00:05:00

SOURCE_FILE="main.cpp"

MERGE_SORT_FUNCTIONS="mergeSort.cpp"

EXECUTABLE="./a.exe"

g++ -O3 "$SOURCE_FILE" "$MERGE_SORT_FUNCTIONS" -o "$EXECUTABLE"