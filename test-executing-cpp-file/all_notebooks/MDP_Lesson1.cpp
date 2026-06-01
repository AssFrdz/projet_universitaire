#include <iostream>
#include <string>
#include <vector>

#include <marmoteCore/marmoteFullMatrix.h>
#include <marmoteCore/marmoteInterval.h>
#include <marmoteCore/marmoteSparseMatrix.h>
#include <marmoteMDP/marmoteDiscountedMDP.h>
#include <marmoteMDP/marmoteFeedbackSolutionMDP.h>

using namespace std;


MarmoteSet* actionSpace = new MarmoteInterval(0, 1);
MarmoteSet* stateSpace = new MarmoteInterval(0, 1);


vector<TransitionStructure*> trans(actionSpace->Cardinal());


SparseMatrix* P0 = new SparseMatrix(2);
P0->setEntry(0, 0, 0.6);
P0->setEntry(0, 1, 0.4);
P0->setEntry(1, 0, 0.5);
P0->setEntry(1, 1, 0.5);
cout << "Matrix P0" << endl;
P0->Write(&cout);
trans.at(0) = P0;


SparseMatrix* P1 = new SparseMatrix(2);
P1->setEntry(0, 0, 0.2);
P1->setEntry(0, 1, 0.8);
P1->setEntry(1, 0, 0.7);
P1->setEntry(1, 1, 0.3);
trans.at(1) = P1;


FullMatrix* Reward = new FullMatrix(2, 2);
Reward->setEntry(0, 0, 4.5);
Reward->setEntry(0, 1, 2.0);
Reward->setEntry(1, 0, -1.5);
Reward->setEntry(1, 1, 3.0);


double discountFactor = 0.95;
string criterion = "max";


DiscountedMDP* mdp = new DiscountedMDP(criterion, stateSpace, actionSpace, trans, Reward, discountFactor);
mdp->Write();


double epsilon = 0.00001;
int maxIter = 700;


FeedbackSolutionMDP* optimum = mdp->ValueIteration(epsilon, maxIter);
optimum->Write();


FeedbackSolutionMDP* optimum2 = mdp->ValueIterationGS(epsilon, 10);
optimum2->Write();


FeedbackSolutionMDP* optimum3 = mdp->ValueIterationInit(epsilon, 200, optimum2);
optimum3->Write();


FeedbackSolutionMDP* optimum4 = mdp->PolicyIterationModified(epsilon, maxIter, 0.001, 100);
optimum4->Write();


FeedbackSolutionMDP* optimum5 = mdp->PolicyIterationModifiedGS(epsilon, maxIter, 0.001, 100);
optimum5->Write();


mdp->PolicyCost(optimum, epsilon, maxIter);
double* values = optimum->getValue();
stateType* actions = optimum->getAction();
for (int i = 0; i < stateSpace->Cardinal(); ++i) {
    cout << "i= " << i << " value= " << values[i] << " action= " << actions[i] << endl;
}


delete optimum;
delete optimum2;
delete optimum3;
delete optimum4;
delete optimum5;
delete mdp;
delete stateSpace;
delete actionSpace;

