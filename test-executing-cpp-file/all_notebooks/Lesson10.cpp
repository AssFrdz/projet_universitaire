// --- Standard C++ utilities used in this notebook ---
#include <iomanip>
#include <iostream>

// --- Marmote headers used in this lesson ---
#include <marmoteMarkovChain/marmoteMarkovChain.h>
#include <marmoteMarkovChain/marmoteSimulationResult.h>
#include <marmoteMarkovChain/General/marmoteHomogeneousMultidBirthDeath.h>
#include <marmoteMarkovChain/General/marmoteHomogeneousMultidRandomWalk.h>
#include <marmoteCore/marmoteBox.h>
#include <marmoteCore/marmoteDiracDistribution.h>
#include <marmoteCore/marmoteDiscreteDistribution.h>
#include <marmoteCore/marmoteHomogeneousMultidTransition.h>

// --- Convenience declarations for the cells below ---
using namespace std;
using namespace marmote;


// Reprint the stored trajectory with the explicit multidimensional state.
void printTrajectory(SimulationResult* sr, MarmoteSet* sp)
{
    for (unsigned long int i = 0; i < sr->trajectorySize(); i++) {
        cout << setw(8) << i;
        cardinalType zestate = sr->states()[i];
        sp->PrintState(&cout, zestate);
        cout << endl;
    }
}

// The command-line example uses a very long horizon (1e4).
// In the notebook we use shorter runs to keep the output readable.
double tmax = 12.0;
simLenType nsteps = 12;

// Four-dimensional grid: 1000 x 1000 x 100 x 100.
stateType dims[4] = {1000, 1000, 100, 100};
double rateup[4] = {0.125, 0.125, 0.125, 0.125};
double ratedown[4] = {0.125, 0.125, 0.125, 0.125};

// Start all simulations from state (0, 0, 0, 0).
DiscreteDistribution* iDis = new DiracDistribution(0);


// Create the compact multidimensional transition structure in continuous time.
HomogeneousMultidTransition* genCT =
    new HomogeneousMultidTransition(CONTINUOUS, 4, dims, rateup, ratedown);
MarkovChain* mcCT = new MarkovChain(genCT);

// Check that all objects agree on the cardinal of the state space.
MarmoteSet* bigBox = new MarmoteBox(4, dims);
cout << "# Printing the size of the state space" << endl;
cout << "Big box cardinal           = " << bigBox->Cardinal() << endl;
cout << "HomMultidTrans size        = " << genCT->orig_size() << endl;
cout << "MultiDimHom space cardinal = "
     << genCT->orig_state_space()->Cardinal() << endl;
delete bigBox;

// Launch the continuous-time simulation.
mcCT->set_init_distribution(iDis);
cout << "# Execution of the continuous-time simulation with the trajectory printed along the way" << endl;
SimulationResult* srCT =
    mcCT->SimulateChainCT_AllOpt(tmax, false, true, false, true, true, CACHE_NONE);

// Print the stored trajectory again, but with the explicit state vector.
cout << "# Printing again the trajectory" << endl;
printTrajectory(srCT, genCT->orig_state_space());


// Clean up the first experiment before starting the second one.
delete mcCT;
delete srCT;

// Create the compact multidimensional transition structure in discrete time.
HomogeneousMultidTransition* genDT =
    new HomogeneousMultidTransition(DISCRETE, 4, dims, rateup, ratedown);
MarkovChain* mcDT = new MarkovChain(genDT);

// Launch the discrete-time simulation.
mcDT->set_init_distribution(iDis);
cout << "# Execution of the discrete-time simulation with the trajectory printed along the way" << endl;
SimulationResult* srDT =
    mcDT->SimulateChainDT_AllOpt(nsteps, false, true, true, true, CACHE_NONE);

// Print the stored trajectory again.
cout << "# Printing again the trajectory" << endl;
printTrajectory(srDT, genDT->orig_state_space());


// Clean up the second experiment.
delete mcDT;
delete srDT;

// Build the same model directly as a multidimensional birth-death process.
MarkovChain* mcBirthDeath = new HomogeneousMultiDBirthDeath(4, dims, rateup, ratedown);
mcBirthDeath->set_init_distribution(iDis);

cout << "# Execution of the continuous-time simulation with the trajectory printed along the way" << endl;
SimulationResult* srBirthDeath =
    mcBirthDeath->SimulateChainCT_AllOpt(tmax, false, true, false, true, true, CACHE_NONE);

cout << "# Printing again the trajectory" << endl;
printTrajectory(srBirthDeath, mcBirthDeath->generator()->orig_state_space());


// Clean up the third experiment.
delete mcBirthDeath;
delete srBirthDeath;

// Build the same model directly as a multidimensional random walk.
MarkovChain* mcRandomWalk = new HomogeneousMultiDRandomWalk(4, dims, rateup, ratedown);
mcRandomWalk->set_init_distribution(iDis);

cout << "# Execution of the discrete-time simulation with the trajectory printed along the way" << endl;
SimulationResult* srRandomWalk =
    mcRandomWalk->SimulateChainDT_AllOpt(nsteps, false, true, true, true, CACHE_NONE);

cout << "# Printing again the trajectory" << endl;
printTrajectory(srRandomWalk, mcRandomWalk->generator()->orig_state_space());


delete mcRandomWalk;
delete srRandomWalk;
delete iDis;

