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


// Create the infinite-state birth-death process.
Homogeneous1DBirthDeath* mc = new Homogeneous1DBirthDeath(0.5, 1.0);

// Simulate the chain for 10 time units.
SimulationResult* simres = mc->SimulateChain(10.0, false, false, true, true);

// Stationary distribution.
DiscreteDistribution* stadis = mc->StationaryDistribution();
cout << stadis->toString(FORMAT_MARMOTE) << endl;
cout << stadis->Mean() << endl;
cout << stadis->getProba(0) << endl;
cout << stadis->getProba(1) << endl;
cout << stadis->Ccdf(4) << endl;

// Uniformization and embedding.
MarkovChain* umc = mc->Uniformize();
cout << umc->className() << endl;
MarkovChain* emc = mc->Embed();
if (emc != nullptr) {
    cout << emc->className() << endl;
} else {
    cout << "Embedding is not available for this infinite-state instance in the current C++ API." << endl;
}


// A finite birth-death process with 11 states.
Homogeneous1DBirthDeath* mcf = new Homogeneous1DBirthDeath(11, 0.4, 0.8);
DiscreteDistribution* stadisf = mcf->StationaryDistribution();
cout << stadisf->toString(FORMAT_MARMOTE) << endl;
cout << stadisf->Mean() << endl;

// Hitting times toward states 3 and 7.
bool hitSet[11] = {false, false, false, true, false, false, false, true, false, false, false};
double* avg = mcf->AverageHittingTimes(hitSet);
for (int i = 0; i < 11; i++) cout << avg[i] << " ";
cout << endl;

double** avgcon = mcf->AverageHittingTimes_Conditional(hitSet);
for (int i = 0; i < 10; i++) {
    for (int j = 2; j < 8; j++) cout << avgcon[i][j] << " ";
    cout << endl;
}

SimulationResult* simhit = mcf->SimulateHittingTime(static_cast<cardinalType>(0), hitSet, 20, 10000.0);
simhit->Diagnose(&cout);
for (auto t : simhit->CT_dates()) cout << t << " ";
cout << endl;


// A 3-dimensional birth-death process on a 4 x 4 x 4 box.
stateType dims3[3] = {4, 4, 4};
double lambda3[3] = {1.0, 1.0, 1.0};
double mu3[3] = {0.8, 0.8, 0.2};
HomogeneousMultiDBirthDeath* mdbd = new HomogeneousMultiDBirthDeath(3, dims3, lambda3, mu3);
SimulationResult* simmdbd = mdbd->SimulateChain(10.0, true, false, true, true);
DiscreteDistribution* sd = simmdbd->Distribution();
cout << sd->toString(FORMAT_MARMOTE) << endl;


// Poisson process.
PoissonProcess* poi = new PoissonProcess(1.0);
SimulationResult* simpoi = poi->SimulateChain(10.0, true, true, true);
for (auto t : simpoi->CT_dates()) cout << t << " ";
cout << endl;

// Markov-modulated Poisson process.
double rates[11] = {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0};
MMPP* mmpp = new MMPP(mcf->generator(), rates);
DiracDistribution* zeroCount = new DiracDistribution(0.0);
DiracDistribution* zeroPhase = new DiracDistribution(0.0);
SimulationResult* simmmpp = mmpp->SimulateChain(10.0, zeroCount, zeroPhase, false, true, true);
simmmpp->Diagnose(&cout);


// Release the predefined chain objects created in this lesson.
delete poi;
delete mdbd;
delete mcf;
delete emc;
delete umc;
delete mc;

