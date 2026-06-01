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


// Recreate the 3-state discrete-time Markov chain of Lesson 1.
double states[3] = {0.0, 1.0, 2.0};
FullMatrix* P = new FullMatrix(3);
P->set_type(DISCRETE);
P->setEntry(0,0,0.25); P->setEntry(0,1,0.50); P->setEntry(0,2,0.25);
P->setEntry(1,0,0.40); P->setEntry(1,1,0.20); P->setEntry(1,2,0.40);
P->setEntry(2,0,0.40); P->setEntry(2,1,0.30); P->setEntry(2,2,0.30);
double initial_prob[3] = {0.2, 0.2, 0.6};
DiscreteDistribution* initial = new DiscreteDistribution(3, states, initial_prob);
MarkovChain* c1 = new MarkovChain(P);
c1->set_init_distribution(initial);
c1->set_model_name("Demo");


// Compute transient distributions after 1, 2 and 3 steps.
DiscreteDistribution* pi1 = c1->TransientDistributionDT(1);
DiscreteDistribution* pi2 = c1->TransientDistributionDT(2);
DiscreteDistribution* pi3 = c1->TransientDistributionDT(3);
cout << *pi1 << endl;
cout << *pi2 << endl;
cout << *pi3 << endl;

// Change the initial distribution to a Dirac mass at state 0.
DiracDistribution* pi0_dirac = new DiracDistribution(0.0);
c1->set_init_distribution(pi0_dirac);
cout << *(c1->TransientDistributionDT(1)) << endl;

// Use a uniform discrete distribution instead.
UniformDiscreteDistribution* pi0_uniform = new UniformDiscreteDistribution(0, 2);
c1->set_init_distribution(pi0_uniform);
cout << *(c1->TransientDistributionDT(1)) << endl;


// Default stationary distribution.
DiscreteDistribution* pista = c1->StationaryDistribution();
cout << *pista << endl;

// RLGL stationary distribution.
UniformDiscreteDistribution* u0 = new UniformDiscreteDistribution(0, 2);
DiscreteDistribution* pista2 = c1->StationaryDistributionRLGL(100, 1e-10, u0, false);
cout << *pista2 << endl;
cout << "Distance L1 between both approximations = "
     << Distribution::DistanceL1(pista, pista2) << endl;

// Exact stationary distribution.
double prosta_ex[3] = {8.0/23.0, 85.0/253.0, 80.0/253.0};
DiscreteDistribution* pista_ex = new DiscreteDistribution(3, states, prosta_ex);
cout << *pista_ex << endl;
cout << "Distance L1 between default and exact pi = "
     << Distribution::DistanceL1(pista, pista_ex) << endl;


// Simulate a trajectory of 10 steps and keep it in memory.
SimulationResult* simRes = c1->SimulateChainDT(10, false, true, false);
simRes->Diagnose(&cout);
for (auto d : simRes->DT_dates()) {
    cout << d << " ";
}
cout << endl;

// Run a second simulation with occupancy statistics and no stored trajectory.
SimulationResult* simRes2 = c1->SimulateChainDT(10, true, false, true);
DiscreteDistribution* trDis = simRes2->Distribution();
cout << *trDis << endl;


// Recreate the continuous-time chain of Lesson 1.
SparseMatrix* Q = new SparseMatrix(6);
Q->set_type(CONTINUOUS);
Q->setEntry(0,1,1.0);
Q->setEntry(0,0,-1.0);
for (stateType i = 1; i < 6; i++) {
    if (i > 0) {
        Q->setEntry(i,0,1.0);
        Q->addToEntry(i,i,-1.0);
    }
    if (i < 5) {
        Q->setEntry(i,i+1,1.0);
        Q->addToEntry(i,i,-1.0);
    }
}
MarkovChain* c2 = new MarkovChain(Q);
c2->set_init_distribution(initial);
c2->set_model_name("Demo_Continuous");

// Stationary distribution and simulation.
DiscreteDistribution* stadis = c2->StationaryDistribution();
cout << *stadis << endl;
SimulationResult* simresCT = c2->SimulateChainCT(10.0, false, true, true, true);
simresCT->Diagnose(&cout);
for (auto t : simresCT->CT_dates()) {
    cout << t << " ";
}
cout << endl;

// Hitting time simulation toward state 5.
bool hitset[6] = {false, false, false, false, false, true};
SimulationResult* hitSim = c2->SimulateHittingTime(static_cast<cardinalType>(0), hitset, 50, 100.0);
for (auto t : hitSim->CT_dates()) {
    cout << t << " ";
}
cout << endl;

// Average hitting times for all starting states.
double* avghit = c2->AverageHittingTimes(hitset);
for (int i = 0; i < 6; i++) {
    cout << avghit[i] << " ";
}
cout << endl;


// Release the main objects created in this lesson.
delete c2;
delete simRes;
delete simRes2;
delete c1;

