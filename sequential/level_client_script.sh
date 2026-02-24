#! /bin/bash
#SBATCH --job-name=pranav-benchmark
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=00:15:00
#SBATCH --output=output_%j.txt
#SBATCH --cpus-per-task=16

make clean
make
./level_client "$1" "$2"