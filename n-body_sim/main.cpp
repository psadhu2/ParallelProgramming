#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include <cctype>

using namespace std;


struct Vec3 {
    double x;
    double y;
    double z;
};

struct Particle {
    double mass;
    Vec3 positions;
    Vec3 velocity;
    Vec3 force;
};

bool isInteger(const string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') {
        if (s.size() == 1) return false;
        i = 1;
    }
    for (; i < s.size(); ++i) {
        if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

void printUsage(const char* exeName) {
    cerr << "Usage:\n"
         << "  " << exeName << " <init> <dt> <steps> <dumpEvery> [output.tsv]\n\n"
         << "<init> can be:\n"
         << "  - integer N   : random initialization with N particles\n"
         << "  - sem         : predefined Sun-Earth-Moon (3 particles)\n"
         << "  - file path   : load initial state from TSV line format\n\n"
         << "Examples:\n"
         << "  " << exeName << " sem 60 10000 10 n-body_sim/output.tsv\n"
         << "  " << exeName << " 100 1 10000 100 n-body_sim/output.tsv\n"
         << "  " << exeName << " n-body_sim/solar.tsv 200 5000000 1000 n-body_sim/bench.tsv\n";
}

vector<Particle> initRandom(
    int n,
    double minMass = 1e20,
    double maxMass = 1e26,
    double posRange = 1e9,
    double velRange = 1e3,
    unsigned int seed = 42
) {
    mt19937 rng(seed);
    uniform_real_distribution<double> massDist(minMass, maxMass);
    uniform_real_distribution<double> posDist(-posRange, posRange);
    uniform_real_distribution<double> velDist(-velRange, velRange);

    vector<Particle> particles;
    particles.reserve(n);

    for (int i = 0; i < n; ++i) {
        Particle p;
        p.mass = massDist(rng);
        p.positions = {posDist(rng), posDist(rng), posDist(rng)};
        p.velocity = {velDist(rng), velDist(rng), velDist(rng)};
        p.force = {0.0, 0.0, 0.0};
        particles.push_back(p);
    }
    return particles;
}

vector<Particle> initSunEarthMoon() {
    vector<Particle> particles;
    particles.reserve(3);

    // Sun
    particles.push_back({
        1.9891e30,
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0},
        {0.0, 0.0, 0.0}
    });
    // Earth
    particles.push_back({
        5.972e24,
        {1.496e11, 0.0, 0.0},
        {0.0, 29780.0, 0.0},
        {0.0, 0.0, 0.0}
    });
    // Moon
    particles.push_back({
        7.342e22,
        {1.49984e11, 0.0, 0.0},
        {0.0, 30802.0, 0.0},
        {0.0, 0.0, 0.0}
    });

    return particles;
}

vector<Particle> loadStateFromFile(const string& path) {
    ifstream in(path);
    if (!in) {
        throw runtime_error("Cannot open file: " + path);
    }

    string line;
    while (getline(in, line)) {
        if (line.find_first_not_of(" \t\r\n") != string::npos) {
            break;
        }
    }
    if (line.empty()) {
        throw runtime_error("Input file is empty: " + path);
    }

    replace(line.begin(), line.end(), '\t', ' ');
    istringstream iss(line);

    int n = 0;
    if (!(iss >> n) || n <= 0) {
        throw runtime_error("Invalid particle count in file: " + path);
    }

    vector<Particle> particles;
    particles.reserve(static_cast<size_t>(n));

    for (int i = 0; i < n; ++i) {
        Particle p{};
        if (!(iss >> p.mass
                  >> p.positions.x >> p.positions.y >> p.positions.z
                  >> p.velocity.x >> p.velocity.y >> p.velocity.z
                  >> p.force.x >> p.force.y >> p.force.z)) {
            throw runtime_error("Malformed state line in file: " + path);
        }
        particles.push_back(p);
    }

    return particles;
}

void computeForces(std::vector<Particle>& particles) {
    const double G = 6.674e-11;
    const double softening = 1e-9;

    for (auto& p : particles) {
        p.force = {0.0, 0.0, 0.0};
    }

    for (size_t i = 0; i < particles.size(); ++i) {
        for (size_t j = i + 1; j < particles.size(); ++j) {
            double dx = particles[j].positions.x - particles[i].positions.x;
            double dy = particles[j].positions.y - particles[i].positions.y;
            double dz = particles[j].positions.z - particles[i].positions.z;

            double distSq = dx * dx + dy * dy + dz * dz + softening;
            double dist = std::sqrt(distSq);

            double scale = G * particles[i].mass * particles[j].mass / (distSq * dist);

            double fx = scale * dx;
            double fy = scale * dy;
            double fz = scale * dz;

            particles[i].force.x += fx;
            particles[i].force.y += fy;
            particles[i].force.z += fz;

            particles[j].force.x -= fx;
            particles[j].force.y -= fy;
            particles[j].force.z -= fz;
        }
    }
}

void integrate(std::vector<Particle>& particles, double dt) {
    for (auto& p : particles) {
        // a = F / m
        double ax = p.force.x / p.mass;
        double ay = p.force.y / p.mass;
        double az = p.force.z / p.mass;

        // v_new = v_old + a * dt
        p.velocity.x += ax * dt;
        p.velocity.y += ay * dt;
        p.velocity.z += az * dt;

        // x_new = x_old + v_new * dt
        p.positions.x += p.velocity.x * dt;
        p.positions.y += p.velocity.y * dt;
        p.positions.z += p.velocity.z * dt;
    }
}

void dumpStateTSV(const vector<Particle>& particles, ostream& out) {
    out << particles.size();
    out << setprecision(15);

    for (const auto& p : particles) {
        out << '\t' << p.mass
            << '\t' << p.positions.x << '\t' << p.positions.y << '\t' << p.positions.z
            << '\t' << p.velocity.x << '\t' << p.velocity.y << '\t' << p.velocity.z
            << '\t' << p.force.x << '\t' << p.force.y << '\t' << p.force.z;
    }
    out << '\n';
}


int main(int argc, char** argv) {
    try {
        vector<Particle> particles;
        double dt = 0.0;
        int steps = 0;
        int dumpEvery = 0;
        string outputPath = "n-body_sim/output.tsv";

        if (argc < 5) {
            printUsage(argv[0]);
            return 1;
        }

        string mode = argv[1];
        if (mode == "sem" || mode == "sun-earth-moon") {
            particles = initSunEarthMoon();
        } else if (isInteger(mode)) {
            int n = stoi(mode);
            if (n <= 0) {
                throw runtime_error("Number of particles must be positive.");
            }
            particles = initRandom(n);
        } else {
            particles = loadStateFromFile(mode);
        }

        dt = stod(argv[2]);
        steps = stoi(argv[3]);
        dumpEvery = stoi(argv[4]);
        if (argc >= 6) outputPath = argv[5];

        if (steps <= 0 || dumpEvery <= 0 || dt <= 0.0) {
            throw runtime_error("dt, steps, and dumpEvery must be positive.");
        }

        ofstream out(outputPath);
        if (!out) {
            throw runtime_error("Cannot open output file: " + outputPath);
        }

        // Output initial state (step 0), then evolve and dump at interval.
        computeForces(particles);
        dumpStateTSV(particles, out);

        for (int step = 1; step <= steps; ++step) {
            integrate(particles, dt);
            computeForces(particles);

            if (step % dumpEvery == 0) {
                dumpStateTSV(particles, out);
            }
        }

        cout << "Wrote simulation states to " << outputPath << '\n';
    } catch (const exception& ex) {
        cerr << ex.what() << '\n';
        return 1;
    }
    return 0;
}
