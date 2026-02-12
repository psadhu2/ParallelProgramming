# N-Body Simulation

This folder contains a C++ N-body simulator (`main.cpp`), a plotting script (`plot.py`), and Slurm scripts for benchmarking and plotting on Centaurus.

## What The Program Does

`nbody` simulates gravitational interactions and writes simulation state to a TSV file.

Each output line contains:
- number of particles
- for each particle: `mass x y z vx vy vz fx fy fz`

This matches the format expected by `plot.py`.

## CLI Format

```
nbody <init> <dt> <steps> <dumpEvery> [output.tsv]
```

- `<init>`:
  - integer `N` -> random initialization with `N` particles
  - `sem` -> predefined Sun-Earth-Moon setup
  - file path (for example `solar.tsv`) -> load initial state from file
- `<dt>`: time step size
- `<steps>`: number of iterations
- `<dumpEvery>`: write one state every `dumpEvery` steps
- `[output.tsv]` (optional): output file path

---

## Run Locally (Windows PowerShell)

From `n-body_sim`:

```powershell
g++ -std=c++17 -O2 -Wall -Wextra -pedantic .\main.cpp -o .\nbody.exe
.\nbody.exe sem 3600 8760 24 output.tsv
python .\plot.py .\output.tsv .\output.pdf 0.0
```

Random example:

```powershell
.\nbody.exe 100 1 10000 100 output.tsv
python .\plot.py .\output.tsv .\output.pdf 0.0
```

---

## Run Locally (Linux/macOS shell)

From `n-body_sim`:

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o nbody
./nbody sem 3600 8760 24 output.tsv
python3 plot.py output.tsv output.pdf 0.0
```

---

## Python Dependency (matplotlib)

`plot.py` requires `matplotlib`.

Recommended install (virtualenv):

```bash
python3 -m venv .venv
source .venv/bin/activate
python -m pip install --upgrade pip matplotlib
```

Then run plotting with that Python:

```bash
python plot.py output.tsv output.pdf 0.0
```

---

## Run Benchmark On Slurm (Centaurus)

From `n-body_sim`:

```bash
sbatch n-body-bench.sh
```

This script:
- builds `nbody`
- runs 3 benchmark cases
- writes:
  - `bench-solar.tsv`
  - `bench-100.tsv`
  - `bench-1000.tsv`
  - `benchmark-summary.txt`
  - `nbody-bench-<jobid>.out`

Check status/output:

```bash
squeue -u $USER
cat nbody-bench-<jobid>.out
cat benchmark-summary.txt
```

---

## Plot Benchmark Outputs On Slurm

From `n-body_sim`:

```bash
sbatch n-body-plot-bench.sh
```

Optional arguments:

```bash
sbatch n-body-plot-bench.sh 0.0
sbatch n-body-plot-bench.sh 0.0 bench-100.tsv bench-1000.tsv
```

Outputs:
- `bench-solar.pdf`
- `bench-100.pdf`
- `bench-1000.pdf`
- `nbody-plot-<jobid>.out`

---

## Notes

- If you only see 2 states in TSV, your `dumpEvery` is too large (often equal to `steps`).
- Number of output lines is: `1 + floor(steps / dumpEvery)`.
- For cleaner plots, use arrow scale `0.0` in `plot.py`.
