// --- Standard C++ utilities used in the notebook ---
// These headers support formatted output and basic containers.
#include <iostream>
#include <string>
#include <vector>

// --- Marmote headers used in this lesson ---
// They provide the C++ counterparts of the Python Marmote objects.
#include "marmoteCore/marmoteFullMatrix.h"
#include "marmoteCore/marmoteInterval.h"
#include "marmoteCore/marmoteSparseMatrix.h"
#include "marmoteMDP/marmoteAverageMDP.h"
#include "marmoteMDP/marmoteFeedbackSolutionMDP.h"
#include "marmoteMDP/marmotePropertiesPolicy.h"
#include "marmoteMDP/marmotePropertiesValue.h"

// --- Convenience declarations for the notebook cells below ---
// They simplify the pedagogical examples without changing the model.
using namespace std;


int dim_SS = 4; // dimension of the state space
int dim_AS = 3; // dimension of the action space

MarmoteSet* stateSpace = new MarmoteInterval(0, 3);
MarmoteSet* actionSpace = new MarmoteInterval(0, 2);


// Matrix for action a_0: do nothing.
SparseMatrix* P0 = new SparseMatrix(dim_SS);
P0->setEntry(0, 1, 0.875);
P0->setEntry(0, 2, 0.0625);
P0->setEntry(0, 3, 0.0625);
P0->setEntry(1, 1, 0.75);
P0->setEntry(1, 2, 0.125);
P0->setEntry(1, 3, 0.125);
P0->setEntry(2, 2, 0.5);
P0->setEntry(2, 3, 0.5);
P0->setEntry(3, 3, 1.0);

// Matrix for action a_1: tune-up.
SparseMatrix* P1 = new SparseMatrix(dim_SS);
P1->setEntry(0, 1, 0.875);
P1->setEntry(0, 2, 0.0625);
P1->setEntry(0, 3, 0.0625);
P1->setEntry(1, 1, 0.75);
P1->setEntry(1, 2, 0.125);
P1->setEntry(1, 3, 0.125);
P1->setEntry(2, 1, 1.0);
P1->setEntry(3, 3, 1.0);

// Matrix for action a_2: total repair.
SparseMatrix* P2 = new SparseMatrix(dim_SS);
P2->setEntry(0, 1, 0.875);
P2->setEntry(0, 2, 0.0625);
P2->setEntry(0, 3, 0.0625);
P2->setEntry(1, 0, 1.0);
P2->setEntry(2, 0, 1.0);
P2->setEntry(3, 0, 1.0);

vector<TransitionStructure*> trans = {P0, P1, P2};


// In the Python notebook, the reward structure is a full (state, action) matrix.
// We keep the same modelling choice here.
FullMatrix* Reward = new FullMatrix(dim_SS, dim_AS);
Reward->setEntry(0, 0, 0);
Reward->setEntry(0, 1, 4000);
Reward->setEntry(0, 2, 6000);
Reward->setEntry(1, 0, 1000);
Reward->setEntry(1, 1, 4000);
Reward->setEntry(1, 2, 6000);
Reward->setEntry(2, 0, 3000);
Reward->setEntry(2, 1, 4000);
Reward->setEntry(2, 2, 6000);
Reward->setEntry(3, 0, 3000);
Reward->setEntry(3, 1, 4000);
Reward->setEntry(3, 2, 6000);


// Set the optimisation criterion and the common numerical parameters.
string criterion = "min";

AverageMDP* mdp1 = new AverageMDP(criterion, stateSpace, actionSpace, trans, Reward);
cout << "The AverageMDP object has been built." << endl;
mdp1->Write();


// Create and initialize epsilon.
double epsilon = 0.00001;
// Create and initialize the maximum number of iterations allowed.
int maxIter = 500;

cout << "Compute with value iteration" << endl;
FeedbackSolutionMDP* optimum = mdp1->ValueIteration(epsilon, maxIter);
optimum->Write();

cout << endl << "Computation with Policy Iteration modified" << endl;
FeedbackSolutionMDP* optimum2 = mdp1->PolicyIterationModified(epsilon, maxIter, 0.001, 1000);
optimum2->Write();


// Solve the model with relative value iteration.
cout << "Computation with relative value iteration" << endl;
FeedbackSolutionMDP* optimum3 = mdp1->RelativeValueIteration(epsilon, maxIter);
optimum3->Write();


// Create an explicit feedback policy that we will analyse and evaluate.
FeedbackSolutionMDP* policy = new FeedbackSolutionMDP(stateSpace->Cardinal());


// Assign one action to each state in the policy object.
policy->setActionIndex(0, 0);
policy->setActionIndex(1, 0);
policy->setActionIndex(2, 1);
policy->setActionIndex(3, 2);


// Reset the value function stored in the policy before evaluation.
policy->resetValue();
policy->Write();


// Display the average cost currently stored in the policy object.
cout << "Getting Average Cost of policy " << policy->getAvgCost() << endl;
cout << "Getting value in 0: " << policy->getValueIndex(0) << endl;
cout << "Getting value in 1: " << policy->getValueIndex(1) << endl;
cout << "Getting value in 2: " << policy->getValueIndex(2) << endl;
cout << "Getting value in 3: " << policy->getValueIndex(3) << endl;
cout << "Getting action in 0: " << policy->getActionIndex(0) << endl;


// Evaluate the policy on the MDP to compute its values and average cost.
mdp1->PolicyCost(policy, epsilon, maxIter);
policy->Write();


// Define a second policy used to study structural properties.
cout << "Define Policy Ra" << endl;
FeedbackSolutionMDP* politique = new FeedbackSolutionMDP(stateSpace->Cardinal());
politique->setActionIndex(0, 0);
politique->setActionIndex(1, 0);
politique->setActionIndex(2, 0);
politique->setActionIndex(3, 2);

cout << "Print solution Ra" << endl;
mdp1->PolicyCost(politique, epsilon, maxIter);
politique->Write();

cout << endl << "Modify the previous Policy and define a new policy Rc" << endl;
politique->setActionIndex(0, 0);
politique->setActionIndex(1, 0);
politique->setActionIndex(2, 2);
politique->setActionIndex(3, 2);
politique->resetValue();

cout << "Print solution of Rc" << endl;
mdp1->PolicyCost(politique, epsilon, maxIter);
politique->Write();

cout << endl << "Define Policy Rd" << endl;
politique->setActionIndex(0, 0);
politique->setActionIndex(1, 2);
politique->setActionIndex(2, 2);
politique->setActionIndex(3, 2);

cout << "Print solution of Rd" << endl;
mdp1->PolicyCost(politique, epsilon, maxIter);
politique->Write();


// Check whether the value function satisfies standard structural properties.
PropertiesValue checkValue(stateSpace);
checkValue.avoidDetail();
int monotoneValue = checkValue.Monotonicity(optimum);
cout << "Printing monotonicity property of value function (1 if increasing -1 if decreasing 0 otherwise): "
     << monotoneValue << endl;

cout << "Verify convexity with details" << endl;
checkValue.getDetail();
int convexValue = checkValue.Convexity(optimum);
cout << "Printing convexity property of value function (1 if convex -1 concave 0 otherwise): "
     << convexValue << endl;


// Check whether the policy satisfies standard structural properties.
cout << "Checking Structural Properties of policy" << endl;
PropertiesPolicy checkPolicy(stateSpace);
int monotonePolicy = checkPolicy.Monotonicity(optimum);
cout << "PropertiesPolicy::MonotonicityOptimalPolicy=" << monotonePolicy
     << " (1 if increasing -1 if decreasing 0 otherwise)." << endl;


// Clean up the objects created in this notebook.
delete optimum;
delete optimum2;
delete optimum3;
delete policy;
delete politique;
delete mdp1;
delete actionSpace;
delete stateSpace;

