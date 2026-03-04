#! /bin/bash
#SBATCH --job-name=pranav-benchmark
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=00:15:00
#SBATCH --output=output_%j.txt
#SBATCH --cpus-per-task=8

make clean
make
./client "$1" "$2"