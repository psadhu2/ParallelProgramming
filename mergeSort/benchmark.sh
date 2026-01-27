#! /bin/bash
#SBATCH --job-name=pranav-benchmark
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=00:010:00

SOURCE_FILE="benchMark.cpp"

MERGE_SORT_FUNCTIONS="mergeSort.cpp"

EXECUTABLE="./merge.exe"

OUT="results.csv"

g++ -O3 "$SOURCE_FILE" "$MERGE_SORT_FUNCTIONS" -o "$EXECUTABLE"


echo "input size,time" > "$OUT"

for N in 10 100 1000 10000 100000 1000000 10000000 100000000 1000000000; do
    result=$("$EXECUTABLE" "$N")
    time_value="${result#*,}"
    echo "Input = $N, Time = $time_value"
    echo "$result" >> "$OUT"
done

echo "Saved results to $OUT"
