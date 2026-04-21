import sys
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import math

def parse_line(line):
    """Parse a single TSV line into a list of particle dicts."""
    tokens = line.strip().split('\t')
    idx = 0
    nbpart = int(tokens[idx]); idx += 1
    particles = []
    for _ in range(nbpart):
        p = {}
        p['mass'] = float(tokens[idx]); idx += 1
        p['x']  = float(tokens[idx]); idx += 1
        p['y']  = float(tokens[idx]); idx += 1
        p['z']  = float(tokens[idx]); idx += 1
        p['vx'] = float(tokens[idx]); idx += 1
        p['vy'] = float(tokens[idx]); idx += 1
        p['vz'] = float(tokens[idx]); idx += 1
        p['fx'] = float(tokens[idx]); idx += 1
        p['fy'] = float(tokens[idx]); idx += 1
        p['fz'] = float(tokens[idx]); idx += 1
        particles.append(p)
    return particles

def main():
    if len(sys.argv) < 3:
        print("usage: python3 plot.py <input.tsv> <output.pdf> [max_frames]")
        sys.exit(1)

    input_file  = sys.argv[1]
    output_file = sys.argv[2]
    max_frames  = int(sys.argv[3]) if len(sys.argv) >= 4 else 100

    # Read all lines
    with open(input_file) as f:
        lines = [l for l in f if l.strip()]

    # Sample evenly if there are more lines than max_frames
    total = len(lines)
    if total <= max_frames:
        sampled = lines
    else:
        indices = [int(i * (total - 1) / (max_frames - 1)) for i in range(max_frames)]
        sampled = [lines[i] for i in indices]

    print(f"Rendering {len(sampled)} frames from {total} total snapshots...")

    with PdfPages(output_file) as pdf:
        for frame_idx, line in enumerate(sampled):
            particles = parse_line(line)

            xs = [p['x'] for p in particles]
            ys = [p['y'] for p in particles]
            masses = [p['mass'] for p in particles]

            # Scale dot size by mass (normalised)
            min_m = min(masses)
            max_m = max(masses)
            if max_m > min_m:
                sizes = [5 + 40 * (m - min_m) / (max_m - min_m) for m in masses]
            else:
                sizes = [15] * len(masses)

            fig, ax = plt.subplots(figsize=(6, 6))
            ax.scatter(xs, ys, s=sizes, c='steelblue', alpha=0.7, linewidths=0)
            ax.set_aspect('equal', 'box')
            ax.set_title(f"Frame {frame_idx + 1} / {len(sampled)}  ({len(particles)} particles)")
            ax.set_xlabel("x")
            ax.set_ylabel("y")
            ax.grid(True, linewidth=0.3, alpha=0.5)

            pdf.savefig(fig, bbox_inches='tight')
            plt.close(fig)

            if (frame_idx + 1) % 10 == 0:
                print(f"  {frame_idx + 1}/{len(sampled)} frames done")

    print(f"Saved to {output_file}")

if __name__ == '__main__':
    main()
