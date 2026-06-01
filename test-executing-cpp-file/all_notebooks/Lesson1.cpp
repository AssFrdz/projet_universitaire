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


// The Python notebook first creates the vector of state labels.
double states[3] = {0.0, 1.0, 2.0};
stateType n = 3;


// Create a full transition matrix and declare that it represents a discrete-time chain.
FullMatrix* P = new FullMatrix(n);
P->set_type(DISCRETE);

// Fill the transition probabilities.
P->setEntry(0,0,0.25);
P->setEntry(0,1,0.50);
P->setEntry(0,2,0.25);
P->setEntry(1,0,0.40);
P->setEntry(1,1,0.20);
P->setEntry(1,2,0.40);
P->setEntry(2,0,0.40);
P->setEntry(2,1,0.30);
P->setEntry(2,2,0.30);

// Inspect the matrix.
P->Write(&cout, "", FORMAT_MARMOTE);


// Create the initial distribution.
double initial_prob[3] = {0.2, 0.2, 0.6};
DiscreteDistribution* initial = new DiscreteDistribution(3, states, initial_prob);
cout << *initial << endl;


// Build the Markov chain from the transition matrix.
MarkovChain* c1 = new MarkovChain(P);
c1->set_init_distribution(initial);
c1->set_model_name("Demo_Discrete");

// Print the chain and inspect its time type.
c1->Write(&cout);
cout << "Type = " << c1->type() << " ; DISCRETE = " << DISCRETE << endl;


// Display the same matrix in several exchange formats.
cout << c1->generator()->toString(FORMAT_MATLAB_SPARSE) << endl;
cout << c1->generator()->toString(FORMAT_NUMPY) << endl;
cout << c1->generator()->toString(FORMAT_R) << endl;
cout << c1->generator()->toString(FORMAT_MAPLE) << endl;


// Create a sparse infinitesimal generator with 6 states.
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
Q->Write(&cout, "", FORMAT_MARMOTE);

// Build the continuous-time chain.
MarkovChain* c2 = new MarkovChain(Q);
c2->set_init_distribution(initial);
c2->set_model_name("Demo_Continuous");
c2->Write(&cout);
cout << "Type = " << c2->type() << " ; CONTINUOUS = " << CONTINUOUS << endl;


// Uniformize the chain with the automatically chosen rate.
MarkovChain* c2uni = c2->Uniformize();
c2uni->Write(&cout);
cout << "Uniformization rate = " << c2uni->generator()->uniformization_rate() << endl;

// Redo uniformization with a larger rate.
MarkovChain* c2uni2 = new MarkovChain(c2->generator()->Uniformize(4.0));
cout << c2uni2->generator()->toString(FORMAT_NUMPY) << endl;

// Embed the chain at jump times.
MarkovChain* c2embed = c2->Embed();
cout << c2embed->generator()->toString(FORMAT_MARMOTE) << endl;


// Release the Markov chain objects created in this lesson.
delete c2embed;
delete c2uni2;
delete c2uni;
delete c2;
delete c1;

