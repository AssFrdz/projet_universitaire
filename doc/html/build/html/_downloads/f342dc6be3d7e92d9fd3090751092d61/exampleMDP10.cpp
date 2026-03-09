/* Marmote and MarmoteMDP are free softwares: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Marmote is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Marmote. If not, see <http://www.gnu.org/licenses/>.

   Copyright 2022 Emmanuel Hyon, Alain Jean-Marie*/

#include <marmoteCore/marmoteSparseMatrix.h>
#include <marmoteCore/marmoteFullMatrix.h>
#include <marmoteCore/marmoteInterval.h>
#include <marmoteCore/marmoteSet.h>

#include <marmoteMDP/marmoteDiscountedMDP.h>
#include <marmoteMDP/marmoteFeedbackSolutionMDP.h>
#include <marmoteMDP/marmoteSolutionMDP.h>

#include <marmoteCore/marmotePolicy.h>

#include <list>
#include <vector>
#include <string>


using namespace std;

/**
 * @brief code to implement an example of a simple discounted MDP
 * @author Hyon, lip6
 * @date March 2024 (created Apr 2022)
 * @version 1.3
 *
 * This example create two example of discounted MDP 
 * 
 * First MDP :
 * 
 * one gives only only one reward matrix and we construct on object MDP
 * 
 * We should obtain
 * 
 * v= (12,28763; 11,31989)
 * 
 */

int main( int argc, char** argv )
{

/* create and initialize the MDP parameters discount factor and criteria. */
    double beta = 0.7;
    string critere("max");

/*create and initialize the maximum number of iterations allowed end the precision.*/
    int maxIter = 700;
    double epsilon = 0.00001;




    /* Create the MDP object to test 1-dimension example and fill all its fields.*/
    int min = 0;
    int max = 1;

    /* initialize the log system */
    marmote::log::initialize();

    /* Create the spaces */
    MarmoteSet *actionSpace = new MarmoteInterval(0,1);
    MarmoteSet *stateSpace = new MarmoteInterval(min,max);

/*Create the vector for the transition structures (here matrices) */
    vector<TransitionStructure*> trans(actionSpace->Cardinal());

/*create transition matrices */
    SparseMatrix *P1 = new SparseMatrix(2);
    P1->setEntry(0,0,0.2);
    P1->setEntry(0,1,0.8);
    P1->setEntry(1,0,0.4);
    P1->setEntry(1,1,0.6);
/* ajout a la collection */
    trans.at(0) = P1;

    SparseMatrix *P2 = new SparseMatrix(2);
    P2->setEntry(0,0,0.5);
    P2->setEntry(0,1,0.5);
    P2->setEntry(1,0,0.7);
    P2->setEntry(1,1,0.3);
    trans.at(1) = P2;

/* input of the reward matrix
 * one entry is r(x,a)
 * with x in line the index of the state
 * with a in column the index of the action
 */
    FullMatrix *R1  = new FullMatrix(2);
    
    R1->setEntry(0,0,0.725);
    R1->setEntry(0,1,4.025);
    R1->setEntry(1,0,3.125);
    R1->setEntry(1,1,2.025);
    
    std::cout << critere.size() << std::endl;
    cout << "Begining MDP building" << std::endl ;
/* Create MDP object */
    DiscountedMDP *mdp1 = new DiscountedMDP(critere, stateSpace, actionSpace, trans, R1,beta);
    cout << "End of building MDP" << endl ;

    cout << "printing MDP" << endl;
    mdp1->Write();
        
    cout << "Printing solution from value iteration" << endl;
    /*call the function to solve the MDP.*/
    FeedbackSolutionMDP *optimum = mdp1->ValueIteration(epsilon, maxIter);
    optimum->Write();
        
    cout << endl << "Checking solutions" << endl;
    mdp1->PolicyCost(optimum,epsilon, maxIter);
    double *solV = optimum->getValue();
    stateType *solA=optimum->getAction();
    for(int i=0;i<stateSpace->Cardinal();i++){
        cout << "i= " << i << " val= " << solV[i] << " action=" << solA[i] << endl;
    }
    cout << endl;
        
    /*call the function to solve the MDP.*/
    cout << "Printing solution from modified value iteration" << endl;
    FeedbackSolutionMDP *optimum2 = mdp1->PolicyIterationModified(epsilon, maxIter, 0.01, 100);
    optimum2->Write();

    cout << endl << "*********************************************" << endl;

    cout << "Destructing" << endl;
    delete mdp1;

    delete optimum;
    delete optimum2;
    cout << "Destructing 2" << endl;
    delete actionSpace;
    delete stateSpace;

    return 0;
}
