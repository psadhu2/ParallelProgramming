## Merge Sort Project

This project demonstrates the implementation and benchmarking of the Merge Sort algorithm in C++.

---

### Running Merge Sort

To compile and run the merge sort implementation with custom input:

```sh
g++ main.cpp mergeSort.cpp -o a
./a
```

Or, you can use the provided script:

```sh
# Only required the first time
chmod +x mergeSort.sh

# Run the script
./mergeSort.sh

# Then run the executable
./a.exe

# Enter the size of the array you want to sort when prompted
```

---

### Running Benchmarks

To run the benchmarks, use the `benchmark.sh` script:

```sh
# Only required the first time
chmod +x benchmark.sh

# Run the benchmark script
./benchmark.sh
```

---

### Running on Centaurus

To run the scripts on Centaurus:

```sh
sbatch ./benchmark
sbatch ./mergeSort.sh
```

---

### Files
- `main.cpp`, `mergeSort.cpp`, `merge_sort_functions.h`: Source code for merge sort
- `mergeSort.sh`: Script to compile and run merge sort
- `benchMark.cpp`, `benchmark.sh`: Benchmarking code and script
- `plot.py`: Python script for plotting results
- `results.csv`: Benchmark results

---
