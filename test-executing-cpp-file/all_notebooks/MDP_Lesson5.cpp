// --- Standard C++ utilities used in this notebook ---
#include <iostream>
#include <string>
#include <vector>

// --- Marmote headers used in this lesson ---
#include <marmoteCore/marmoteFullMatrix.h>
#include <marmoteCore/marmoteInterval.h>
#include <marmoteCore/marmoteSet.h>
#include <marmoteCore/marmoteSparseMatrix.h>
#include <marmoteMDP/marmoteFiniteHorizonMDP.h>
#include <marmoteMDP/marmoteNonStationarySolutionMDP.h>
#include <marmoteMDP/marmoteSolutionMDP.h>

// --- Convenience declarations for the cells below ---
using namespace std;
using namespace marmote;


// Define the main features of the finite-horizon MDP.
string criterion = "max";
double penalty = -1000.0;
int horizon = 3;
double discountFactor = 1.0;

// The solver signature still requires epsilon and maxIter, even though they
// do not drive a standard fixed-point iteration in the finite-horizon case.
double epsilon = 0.01;
int maxIter = 7;


// Create the one-dimensional state and action spaces.
int minState = 0;
int maxState = 3;
MarmoteSet* actionSpace = new MarmoteInterval(0, 3);
MarmoteSet* stateSpace = new MarmoteInterval(minState, maxState);

// Prepare the collection of transition matrices, one for each action.
vector<TransitionStructure*> trans(actionSpace->Cardinal());
SparseMatrix* P = nullptr;


// Transition matrix for action 0.
P = new SparseMatrix(stateSpace->Cardinal());
P->setEntry(0, 0, 1.0);
P->setEntry(1, 0, 0.75);
P->setEntry(1, 1, 0.25);
P->setEntry(2, 0, 0.25);
P->setEntry(2, 1, 0.5);
P->setEntry(2, 2, 0.25);
P->setEntry(3, 1, 0.25);
P->setEntry(3, 2, 0.5);
P->setEntry(3, 3, 0.25);
trans.at(0) = P;
cout << "Added transition matrix 0 to vector" << endl;

// Transition matrix for action 1.
P = new SparseMatrix(stateSpace->Cardinal());
P->setEntry(0, 0, 0.75);
P->setEntry(0, 1, 0.25);
P->setEntry(1, 0, 0.25);
P->setEntry(1, 1, 0.5);
P->setEntry(1, 2, 0.25);
P->setEntry(2, 1, 0.25);
P->setEntry(2, 2, 0.5);
P->setEntry(2, 3, 0.25);
P->setEntry(3, 1, 0.25);
P->setEntry(3, 2, 0.5);
P->setEntry(3, 3, 0.25);
trans.at(1) = P;
cout << "Added transition matrix 1 to vector" << endl;

// Transition matrix for action 2.
P = new SparseMatrix(stateSpace->Cardinal());
P->setEntry(0, 0, 0.25);
P->setEntry(0, 1, 0.5);
P->setEntry(0, 2, 0.25);
P->setEntry(1, 1, 0.25);
P->setEntry(1, 2, 0.5);
P->setEntry(1, 3, 0.25);
P->setEntry(2, 1, 0.25);
P->setEntry(2, 2, 0.5);
P->setEntry(2, 3, 0.25);
P->setEntry(3, 1, 0.25);
P->setEntry(3, 2, 0.5);
P->setEntry(3, 3, 0.25);
trans.at(2) = P;
cout << "Added transition matrix 2 to vector" << endl;

// Transition matrix for action 3.
P = new SparseMatrix(stateSpace->Cardinal());
P->setEntry(0, 1, 0.25);
P->setEntry(0, 2, 0.5);
P->setEntry(0, 3, 0.25);
P->setEntry(1, 1, 0.25);
P->setEntry(1, 2, 0.5);
P->setEntry(1, 3, 0.25);
P->setEntry(2, 1, 0.25);
P->setEntry(2, 2, 0.5);
P->setEntry(2, 3, 0.25);
P->setEntry(3, 1, 0.25);
P->setEntry(3, 2, 0.5);
P->setEntry(3, 3, 0.25);
trans.at(3) = P;
cout << "Added transition matrix 3 to vector" << endl;


// Build the reward matrix.
FullMatrix* R = new FullMatrix(stateSpace->Cardinal(), actionSpace->Cardinal());
R->setEntry(0, 0, 0);
R->setEntry(0, 1, -1);
R->setEntry(0, 2, -2);
R->setEntry(0, 3, -5);
R->setEntry(1, 0, 5);
R->setEntry(1, 1, 0);
R->setEntry(1, 2, -3);
R->setEntry(1, 3, penalty);
R->setEntry(2, 0, 6);
R->setEntry(2, 1, -1);
R->setEntry(2, 2, penalty);
R->setEntry(2, 3, penalty);
R->setEntry(3, 0, 5);
R->setEntry(3, 1, penalty);
R->setEntry(3, 2, penalty);
R->setEntry(3, 3, penalty);

// Build the finite-horizon MDP.
cout << "Beginning building MDP" << endl;
FiniteHorizonMDP* mdp1 = new FiniteHorizonMDP(criterion, stateSpace, actionSpace, trans, R, horizon, discountFactor);
cout << "MDP built" << endl;
mdp1->Write();


// Solve the model by dynamic programming.
cout << "Print solution after value iteration" << endl;
NonStationarySolutionMDP* optimum = mdp1->ValueIteration(epsilon, maxIter);
optimum->Write();


// Evaluate the computed policy and inspect the values at step horizon - 1.
cout << endl << "Checking solutions" << endl;
mdp1->PolicyCost(optimum, epsilon, maxIter);
for (stateType i = 0; i < stateSpace->Cardinal(); i++) {
    cout << "i = " << i << " N = " << horizon - 1
         << " solution = " << optimum->getValueAtStepIndex(horizon - 1, i) << endl;
}


// Release the MDP objects created in this lesson.
delete optimum;
delete mdp1;
delete stateSpace;
delete actionSpace;

