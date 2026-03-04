# Running `level_client` on Slurm

This project provides a Slurm batch script: `level_client_script.sh`.

The script will:
1. Run `make clean`
2. Build the program with `make`
3. Execute `./level_client "$1" "$2"`

## Prerequisites

1. You are on a Slurm cluster.
2. You are in the `sequential` directory.
3. The cluster has `g++`, `make`, and the required libraries (`curl`, `pthread`).

## Usage

Submit the job with:

```bash
sbatch level_client_script.sh "<name>" <level>
```

Example:

```bash
sbatch level_client_script.sh "Tom Hanks" 3
```

## Check job status

```bash
squeue -u $USER
```

## View output

The script writes logs to:

```bash
output_<jobid>.txt
```

Example:

```bash
cat output_12345.txt
```

## Notes

- The script currently requests:
  - Partition: `Centaurus`
  - Memory: `10G`
  - Time: `00:15:00`
  - CPUs per task: `16`
- If your cluster uses a different partition/account/QoS, update the `#SBATCH` lines in `level_client_script.sh`.
