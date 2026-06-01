// --- Standard C++ utilities used in this notebook ---
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// --- Marmote headers used in this lesson ---
#include <marmoteCore/marmoteCore>
#include <marmoteMarkovChain/marmoteMarkovChain>

// --- Convenience declarations for the cells below ---
using namespace std;
using namespace marmote;


// Uniform distribution on [4,10].
UniformDistribution* udis = new UniformDistribution(4.0, 10.0);
cout << udis->className() << endl;
cout << udis->Mean() << endl;
cout << udis->Rate() << endl;
cout << udis->Variance() << endl;
cout << udis->Cdf(6.0) << endl;
cout << udis->Ccdf(6.0) << endl;
cout << udis->Laplace(0.01) << endl;
cout << udis->DLaplace(0.01) << endl;

// Uniform discrete distribution on {4,...,10}.
UniformDiscreteDistribution* uddis = new UniformDiscreteDistribution(4, 10);
cout << uddis->className() << endl;
cout << uddis->Mean() << endl;
cout << uddis->Rate() << endl;
cout << uddis->Variance() << endl;
cout << uddis->Cdf(6.0) << endl;
cout << uddis->Ccdf(6.0) << endl;
cout << uddis->Laplace(0.01) << endl;
cout << uddis->DLaplace(0.01) << endl;

// Exponential distribution with mean 4.0.
ExponentialDistribution* edis = new ExponentialDistribution(4.0);
cout << edis->Mean() << endl;
cout << edis->Rate() << endl;
cout << edis->Variance() << endl;
cout << edis->Cdf(6.0) << endl;
cout << edis->Ccdf(6.0) << endl;
cout << edis->Laplace(0.0) << endl;
cout << edis->DLaplace(0.0) << endl;


// Test a few structural properties.
cout << edis->HasProperty("integerValued") << endl;
cout << edis->HasProperty("continuous") << endl;
cout << uddis->HasProperty("integerValued") << endl;
cout << uddis->HasProperty("compactSupport") << endl;

// Sampling.
for (int i = 0; i < 4; i++) cout << edis->Sample() << endl;
cout << endl;
for (int i = 0; i < 4; i++) cout << udis->Sample() << endl;
cout << endl;
for (int i = 0; i < 4; i++) cout << uddis->Sample() << endl;


// Rescale several distributions exactly as in the Python lesson.
cout << udis->toString(FORMAT_MARMOTE) << endl;
cout << udis->Rescale(0.5)->toString(FORMAT_MARMOTE) << endl;
cout << edis->toString(FORMAT_MARMOTE) << endl;
cout << edis->Rescale(5.0)->toString(FORMAT_MARMOTE) << endl;

double vals[4] = {3.4, 4.5, 6.7, 8.9};
double probs[4] = {0.1, 0.2, 0.3, 0.4};
DiscreteDistribution* didis = new DiscreteDistribution(4, vals, probs);
cout << didis->toString(FORMAT_MARMOTE) << endl;
cout << didis->Rescale(10.0)->toString(FORMAT_MARMOTE) << endl;
cout << uddis->toString(FORMAT_MARMOTE) << endl;


// Compare a few pairs of distributions with standard distances.
UniformDiscreteDistribution* d1 = new UniformDiscreteDistribution(0, 19);
GeometricDistribution* d2 = new GeometricDistribution(0.5);
UniformDiscreteDistribution* d3 = new UniformDiscreteDistribution(0, 24);
GeometricDistribution* d4 = new GeometricDistribution(0.55);

cout << "L1 = " << Distribution::DistanceL1(d1, d2) << endl;
cout << "L2 = " << Distribution::DistanceL2(d1, d2) << endl;
cout << "Linf = " << Distribution::DistanceLInfinity(d1, d2) << endl;
cout << "TV = " << Distribution::DistanceTV(d1, d2) << endl;
cout << "Computable L-infinity distance: " << Distribution::DistanceLInfinity(d1, d3) << endl;


// A 4-state continuous birth-death chain.
Homogeneous1DBirthDeath* four = new Homogeneous1DBirthDeath(4, 3.0, 1.0);
cout << four->generator()->TransDistrib(0)->toString(FORMAT_MARMOTE) << endl;
cout << four->generator()->TransDistrib(1)->toString(FORMAT_MARMOTE) << endl;
cout << four->TransientDistribution(4.0)->toString(FORMAT_MARMOTE) << endl;
cout << four->StationaryDistribution()->toString(FORMAT_MARMOTE) << endl;
SimulationResult* simres = four->SimulateChain(20.0, true, false, false, false);
cout << simres->Distribution()->toString(FORMAT_MARMOTE) << endl;


// Two-state continuous chain.
TwoStateContinuous* two = new TwoStateContinuous(5.0, 1.0);
bool hitset2[2] = {false, true};
vector<Distribution*> hd2 = two->HittingTimeDistributions(hitset2);
for (Distribution* d : hd2) {
    cout << d->toString(FORMAT_MARMOTE) << endl;
}

// Felsenstein 81 model.
double f81prob[4] = {0.1, 0.2, 0.3, 0.4};
Felsenstein81* f81 = new Felsenstein81(f81prob, 1.0);
bool hitset4[4] = {false, false, true, false};
vector<Distribution*> hd4 = f81->HittingTimeDistributions(hitset4);
for (Distribution* d : hd4) {
    cout << d->toString(FORMAT_MARMOTE) << endl;
}

// Phase-type distribution returned for a general Markov chain.
bool hitSetGen[4] = {false, false, false, true};
Distribution* htd = four->HittingTimeDistribution(0, hitSetGen);
cout << htd->toString(FORMAT_MARMOTE) << endl;
cout << htd->Mean() << endl;

// Another example with a 1-dimensional random walk.
Homogeneous1DRandomWalk* bd = new Homogeneous1DRandomWalk(11, 0.1, 0.2);
bool hitset11[11] = {false, false, false, false, false, false, false, false, false, false, true};
Distribution* ht = bd->HittingTimeDistribution(0, hitset11);
cout << ht->toString(FORMAT_MARMOTE) << endl;
cout << ht->Mean() << endl;
cout << ht->Variance() << endl;


// Release the chain objects used in the hitting-time examples.
delete bd;
delete f81;
delete two;
delete four;

