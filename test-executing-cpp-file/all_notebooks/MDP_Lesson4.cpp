// --- Standard C++ utilities used in the notebook ---
// These headers support formatted output and basic containers.
#include <iostream>
#include <string>

// --- Marmote headers used in this lesson ---
// They provide the C++ counterparts of the Python Marmote objects.
#include "marmoteCore/marmoteBox.h"
#include "marmoteCore/marmoteFullMatrix.h"
#include "marmoteCore/marmoteInterval.h"
#include "marmoteCore/marmoteSparseMatrix.h"
#include "marmoteMDP/marmoteFeedbackSolutionMDP.h"
#include "marmoteMDP/marmoteTotalRewardMDP.h"

// --- Convenience declarations for the notebook cells below ---
// They simplify the pedagogical examples without changing the model.
using namespace std;


// Define the sizes of the two dimensions of the box.
stateType dims[2] = {11, 10};


// Create the two-dimensional state space and display its main features.
MarmoteBox *stateSpace = new MarmoteBox(2, dims);

stateType dim_SS = stateSpace->Cardinal();
cout << "State Space cardinal " << dim_SS << endl;
cout << "State Space dimension " << stateSpace->tot_nb_dims() << endl;
cout << "State Space type " << stateSpace->toString() << endl;


// Create the action space with the four admissible moves.
MarmoteInterval *actionSpace = new MarmoteInterval(0, 3);
stateType dim_AS = actionSpace->Cardinal();


// First buffer allows us to manage the initial state.
MarmoteState etat = stateSpace->StateBuffer();
// Second buffer allows us to manage the final state (after transition).
MarmoteState sortie = stateSpace->StateBuffer();

etat[0] = 0;
etat[1] = 0;
sortie[0] = 0;
sortie[1] = 0;

stateType k = 0;
stateType indexO = 0;
stateType indexD = 0;
int l = 0;
int c = 0;


cout << "Fill in Cost Matrix" << endl;
FullMatrix *CostMat = new FullMatrix(dim_SS, dim_AS);

stateSpace->FirstState(etat);
for (k = 0; k < dim_SS; k++) {
    // Compute the index of the state.
    indexO = stateSpace->Index(etat);
    // For each state we give a value to every action.
    for (stateType a = 0; a < dim_AS; a++) {
        CostMat->setEntry(indexO, a, 1.0);
    }
    stateSpace->NextState(etat);
}

// Replace the term in (9,2) for action UP by -1.
etat[0] = 9;
etat[1] = 2;
indexO = stateSpace->Index(etat);
cout << "index of state (9,2) " << indexO << endl << endl;
CostMat->setEntry(indexO, 0, -1.0);

// Fill line 10: all costs are equal to zero.
etat[0] = 10;
for (k = 0; k < 10; k++) {
    etat[1] = k;
    indexO = stateSpace->Index(etat);
    CostMat->setEntry(indexO, 0, 0.0);
    CostMat->setEntry(indexO, 1, 0.0);
    CostMat->setEntry(indexO, 2, 0.0);
    CostMat->setEntry(indexO, 3, 0.0);
}


// Set the optimisation criterion and build the total reward MDP.
string criterion = "min";

cout << "Begining of building MDP" << endl;
TotalRewardMDP *mdpSSP = new TotalRewardMDP(criterion, stateSpace, actionSpace, CostMat);
cout << "End of building MDP" << endl;


cout << "Add matrices" << endl;

double p = 0.9;

SparseMatrix *P0 = new SparseMatrix(dim_SS);
for (l = 0; l < 10; l++) {
    for (c = 0; c < 10; c++) {
        // Define a state and get its index.
        etat[0] = l;
        etat[1] = c;
        indexO = stateSpace->Index(etat);
        if ((l == 4) || (l == 9)) {
            if ((l == 4) && ((c == 2) || (c == 7))) {
                // I am on a door: either I move up with probability p, or I stay.
                sortie[0] = l + 1;
                sortie[1] = c;
                indexD = stateSpace->Index(sortie);
                P0->setEntry(indexO, indexD, p);
                P0->setEntry(indexO, indexO, 1 - p);
            } else {
                if ((l == 9) && (c == 2)) {
                    // Special door leading to the absorbing line.
                    sortie[0] = l + 1;
                    sortie[1] = c;
                    indexD = stateSpace->Index(sortie);
                    P0->setEntry(indexO, indexD, p);
                    P0->setEntry(indexO, indexO, 1 - p);
                } else {
                    // I am on the wall l=4 or l=9: I stay in the same state.
                    P0->setEntry(indexO, indexO, 1.0);
                }
            }
        } else {
            // I am in a room: either I move up with probability p, or I stay.
            sortie[0] = l + 1;
            sortie[1] = c;
            indexD = stateSpace->Index(sortie);
            P0->setEntry(indexO, indexD, p);
            P0->setEntry(indexO, indexO, 1 - p);
        }
    }
}

// Fill in the last line.
for (c = 0; c < 10; c++) {
    etat[0] = 10;
    etat[1] = c;
    indexO = stateSpace->Index(etat);
    P0->setEntry(indexO, indexO, 1.0);
}

mdpSSP->AddMatrix(0, P0);
cout << "Added matrix (action 0)" << endl;


// Complete matrix for action 1 (DOWN).
SparseMatrix *P1 = new SparseMatrix(dim_SS);

for (l = 0; l < 10; l++) {
    for (c = 0; c < 10; c++) {
        etat[0] = l;
        etat[1] = c;
        indexO = stateSpace->Index(etat);
        if ((l == 5) || (l == 0)) {
            if ((l == 5) && ((c == 2) || (c == 7))) {
                // I am on a door: either I move down with probability p, or I stay.
                sortie[0] = l - 1;
                sortie[1] = c;
                indexD = stateSpace->Index(sortie);
                P1->setEntry(indexO, indexD, p);
                P1->setEntry(indexO, indexO, 1 - p);
            } else {
                // I am on the wall l=5 or l=0: I stay in the same state.
                P1->setEntry(indexO, indexO, 1.0);
            }
        } else {
            // I am in a room: either I move down with probability p, or I stay.
            sortie[0] = l - 1;
            sortie[1] = c;
            indexD = stateSpace->Index(sortie);
            P1->setEntry(indexO, indexD, p);
            P1->setEntry(indexO, indexO, 1 - p);
        }
    }
}

// Fill in the last line.
for (c = 0; c < 10; c++) {
    etat[0] = 10;
    etat[1] = c;
    indexO = stateSpace->Index(etat);
    P1->setEntry(indexO, indexO, 1.0);
}

mdpSSP->AddMatrix(1, P1);
cout << "Added matrix (action 1)" << endl;

// Define matrix for action 2 (LEFT).
SparseMatrix *P2 = new SparseMatrix(dim_SS);
for (l = 0; l < 10; l++) {
    for (c = 0; c < 10; c++) {
        etat[0] = l;
        etat[1] = c;
        indexO = stateSpace->Index(etat);
        if ((c == 5) || (c == 0)) {
            if ((c == 5) && ((l == 2) || (l == 7))) {
                // I am on a door: either I move left with probability p, or I stay.
                sortie[0] = l;
                sortie[1] = c - 1;
                indexD = stateSpace->Index(sortie);
                P2->setEntry(indexO, indexD, p);
                P2->setEntry(indexO, indexO, 1 - p);
            } else {
                // I am on the wall c=5 or c=0: I stay in the same state.
                P2->setEntry(indexO, indexO, 1.0);
            }
        } else {
            // I am in a room: either I move left with probability p, or I stay.
            sortie[0] = l;
            sortie[1] = c - 1;
            indexD = stateSpace->Index(sortie);
            P2->setEntry(indexO, indexD, p);
            P2->setEntry(indexO, indexO, 1 - p);
        }
    }
}

// Fill in the last line.
for (c = 0; c < 10; c++) {
    etat[0] = 10;
    etat[1] = c;
    indexO = stateSpace->Index(etat);
    P2->setEntry(indexO, indexO, 1.0);
}

mdpSSP->AddMatrix(2, P2);
cout << "Added matrix (action 2)" << endl;

// Define matrix for action 3 (RIGHT).
SparseMatrix *P3 = new SparseMatrix(dim_SS);
for (l = 0; l < 10; l++) {
    for (c = 0; c < 10; c++) {
        etat[0] = l;
        etat[1] = c;
        indexO = stateSpace->Index(etat);
        if ((c == 4) || (c == 9)) {
            if ((c == 4) && ((l == 2) || (l == 7))) {
                // I am on a door: either I move right with probability p, or I stay.
                sortie[0] = l;
                sortie[1] = c + 1;
                indexD = stateSpace->Index(sortie);
                P3->setEntry(indexO, indexD, p);
                P3->setEntry(indexO, indexO, 1 - p);
            } else {
                // I am on the wall c=4 or c=9: I stay in the same state.
                P3->setEntry(indexO, indexO, 1.0);
            }
        } else {
            // I am in a room: either I move right with probability p, or I stay.
            sortie[0] = l;
            sortie[1] = c + 1;
            indexD = stateSpace->Index(sortie);
            P3->setEntry(indexO, indexD, p);
            P3->setEntry(indexO, indexO, 1 - p);
        }
    }
}

// Fill in the last line.
for (c = 0; c < 10; c++) {
    etat[0] = 10;
    etat[1] = c;
    indexO = stateSpace->Index(etat);
    P3->setEntry(indexO, indexO, 1.0);
}

mdpSSP->AddMatrix(3, P3);
cout << "Added matrix (action 3)" << endl;

cout << "Finishing Adding matrices MDP" << endl;
cout << "Writing MDP" << endl;
cout << *mdpSSP << endl;


// Set the solver parameters and compute the optimal policy by value iteration.
double epsilon = 0.0001;
int maxIter = 250;

cout << "\nPrinting solution from value iteration" << endl;
FeedbackSolutionMDP *optimum2 = mdpSSP->ValueIteration(epsilon, maxIter);
optimum2->Write();


// Display the policy line by line by scanning the second dimension.
cout << "Print solution by dimension (line by line)" << endl;
string line = optimum2->SolutionByDim(1, stateSpace);
cout << line << endl;


// Create the buffer.
MarmoteState bbuf = stateSpace->StateBuffer();
cout << "Printing State Space Path and value function with a browsing by iterating space" << endl;

// Initial state: bbuf receives the value of the first state of the state space.
stateSpace->FirstState(bbuf);

// Scan the whole state space.
for (k = 0; k < stateSpace->Cardinal(); k++) {
    indexO = stateSpace->Index(bbuf);
    l = bbuf[0];
    c = bbuf[1];

    cout << "--State in line=" << l << " column=" << c;
    if ((c <= 4) && (l <= 4)) {
        cout << " --in Room at Bottom Left  ";
    }
    if ((c <= 4) && (l >= 5)) {
        cout << " --in Room at Top Left     ";
    }
    if ((c >= 5) && (l <= 4)) {
        cout << " --in Room at Bottom Right ";
    }
    if ((c >= 5) && (l >= 5)) {
        cout << " --in Room at Top Right    ";
    }

    cout << " --Optimal Action=" << optimum2->getActionIndex(indexO)
         << " --Value=" << optimum2->getValueIndex(indexO) << endl;

    stateSpace->NextState(bbuf);
}


// Release the objects allocated in this notebook.
delete optimum2;
delete mdpSSP;
delete stateSpace;
delete actionSpace;
delete[] etat;
delete[] sortie;
delete[] bbuf;

