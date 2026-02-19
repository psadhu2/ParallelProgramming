#!/bin/bash
#SBATCH --job-name=pranav-graph-crawler
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=00:05:00

g++ bfs.cpp -I. -o bfs.exe -lcurl
./bfs.exe "Tom Holland" 3