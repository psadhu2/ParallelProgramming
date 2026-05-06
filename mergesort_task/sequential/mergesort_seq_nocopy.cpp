#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <vector>
#include "omp_tasking.hpp"

#define DEBUG 0
#define THRESHOLD 1000

void generateMergeSortData (std::vector<int>& arr, size_t n) {
  for (size_t  i=0; i< n; ++i) {
    arr[i] = rand();
  }
}
  
void checkMergeSortResult (std::vector<int>& arr, size_t n) {
  bool ok = true;
  for (size_t  i=1; i<n; ++i)
    if (arr[i]< arr[i-1])
      ok = false;
  if(!ok)
    std::cerr<<"notok"<<std::endl;
}


void merge(int* arr, size_t l, size_t mid, size_t r) {
 
#if DEBUG
  std::cout << l << " " << mid << " " << r << std::endl;
#endif
 
  // short circuits
  if (l == r) return;
  if (r - l == 1) {
    if (arr[l] > arr[r]) {
      int t = arr[l];
      arr[l] = arr[r];
      arr[r] = t;
    }
    return;
  }
 
  size_t i, j, k;
  size_t n = mid - l;
 
  // local temp buffer — each call owns its own, no race condition
  std::vector<int> temp(n);
 
  for (i=0; i<n; ++i)
    temp[i] = arr[l+i];
 
  i = 0;    // index into temp (left half)
  j = mid;  // index into right half in arr
  k = l;    // write position in arr
 
  while (i < n && j <= r) {
    if (temp[i] <= arr[j]) {
      arr[k++] = temp[i++];
    } else {
      arr[k++] = arr[j++];
    }
  }
 
  while (i < n) {
    arr[k++] = temp[i++];
  }
}

void mergesort(int* arr, size_t l, size_t r) {
  if (l < r) {
    size_t mid = (l + r) / 2;
    mergesort(arr, l, mid);
    mergesort(arr, mid + 1, r);
    merge(arr, l, mid + 1, r);
  }
}

void mergesort_par(int* arr, size_t l, size_t r) {
  if (l >= r) return;
 
  size_t mid = (l + r) / 2;
 
  if (r - l > THRESHOLD) {
    tasking::taskstart([arr, l, mid]() {
      mergesort_par(arr, l, mid);
    });
    tasking::taskstart([arr, mid, r]() {
      mergesort_par(arr, mid + 1, r);
    });
    tasking::taskwait();
  } else {
    mergesort(arr, l, mid);
    mergesort(arr, mid + 1, r);
  }
 
  merge(arr, l, mid + 1, r);
}


int main (int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nthreads>"<<std::endl;
    return -1;
  }
  
  // command line parameter
  size_t n = atol(argv[1]);
  int nthreads = atoi(argv[2]);

  // get arr data
  std::vector<int> arr (n);
  generateMergeSortData (arr, n);

#if DEBUG
  for (size_t i=0; i<n; ++i) 
    std::cout<<arr[i]<<" ";
  std::cout<<std::endl;
#endif

  int* arr_ptr = &arr[0];
  size_t sz = n;

  // begin timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  tasking::doinparallel([arr_ptr, sz]() {
    tasking::taskstart([arr_ptr, sz]() {
      mergesort_par(arr_ptr, 0, sz - 1);
    });
    tasking::taskwait();
  }, nthreads);

  // end timing
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;

  // display time to cerr
  std::cerr<<elpased_seconds.count()<<std::endl;
  checkMergeSortResult (arr, n);

#if DEBUG
  for (size_t i=0; i<n; ++i)  
    std::cout<<arr[i]<<" ";
  std::cout<<std::endl;
#endif

  return 0;
}
