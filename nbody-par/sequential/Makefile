CXXFLAGS=-O3 -fopenmp

nbody: nbody.cpp
	g++ -O3 -fopenmp nbody.cpp -o nbody

solar.out: nbody
	echo %TIME%
	nbody planet 200 5000000 10000 4 > solar.out
	echo %TIME%

solar.pdf: solar.out
	python3 plot.py solar.out solar.pdf 1000

random.out: nbody
	echo %TIME%
	nbody 1000 1 10000 100 4 > random.out
	echo %TIME%
