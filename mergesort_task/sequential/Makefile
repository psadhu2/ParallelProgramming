CFLAGS=-O3 -std=c11 -fPIC -g
CXXFLAGS=-O3 -std=c++17 -fPIC -g
LD=g++
 
 
all: mergesort_par
 
 
mergesort_par: mergesort_seq_nocopy.o
	$(LD) $(LDFLAGS) mergesort_seq_nocopy.o $(ARCHIVES) -fopenmp -o mergesort_par
 
mergesort_seq_nocopy.o: mergesort_seq_nocopy.cpp
	g++ $(CXXFLAGS) -fopenmp -c mergesort_seq_nocopy.cpp -o mergesort_seq_nocopy.o
 
 
clean:
	-rm *.o
	-rm mergesort_par.exe
 
.PHONY: all clean