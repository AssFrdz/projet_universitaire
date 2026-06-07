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


#include "marmoteCore/marmoteInterval.h"
#include "marmoteCore/marmoteSparseMatrix.h"
#include "marmoteMDP/marmoteAverageMDP.h"
#include "marmoteMDP/marmoteFeedbackSolutionMDP.h"
#include "marmoteMDP/marmoteSolutionMDP.h"

#include <marmoteCore/marmotePolicy.h>
#include <marmoteLog/marmoteLog.h>

#include <list>
#include <vector>
#include <string>
#include <time.h> /* needed for Visual Studio */

using namespace std;


/**
 * @brief code to implement an example of average MDP
 * @author Hyon, 
 * @version 0.1
 * @date May 2022
 *
 * The average value of the optimal policies is 1666.67
 * The average values for the three policies are 
 * 
 * 1923.08 for Ra
 * 1727.27 for Rc
 * 3000    for Rd
 * 
 */




int main( int argc, char** argv )
{
    stateType i;
    string critere("min");

    clock_t end_t, start_t;
    double total_t;

    /* create and initialize epsilon. */
    double epsilon = 0.00001;

    /* create and initialize the maximum number of iterations allowed. */
    int maxIter = 500;

    //==================Test new features==================//
    marmote::log::initialize();

    //Create the MDP object to test 1-dimension example and fill all its fields.
    int dim_SS = 4; /* dimension of the state space */
    int dim_AS = 3; /* dimension of the action space */
    MarmoteSet *actionSpace = new MarmoteInterval(0,2);               // Intervalle from 0 to 2 thus 0, 1, and 2
    MarmoteSet *stateSpace = new MarmoteInterval(0,3);            // Intervalle from 0 to 3 thus 0, 1, et 2 ans 3

    vector<TransitionStructure*> trans(actionSpace->Cardinal());  // vector vect = ( matrice a_0 , matrice a_1 , matrice a_2 )

    SparseMatrix *P0 = new SparseMatrix(dim_SS); /* matrix for the a_0 action*/
    //P0->setEntry(0,0,0);
    P0->setEntry(0,1,0.875);
    P0->setEntry(0,2,0.0625);
    P0->setEntry(0,3,0.0625);
    //P0->setEntry(1,1,0.0);
    P0->setEntry(1,1,0.75);
    P0->setEntry(1,2,0.125);
    P0->setEntry(1,3,0.125);
    P0->setEntry(2,0,0);
    P0->setEntry(2,1,0);
    P0->setEntry(2,2,0.5);
    P0->setEntry(2,3,0.5);
    P0->setEntry(3,0,0);
    P0->setEntry(3,1,0);
    P0->setEntry(3,2,0);
    P0->setEntry(3,3,1.0);
    trans.at(0) = P0;

    SparseMatrix *P1 = new SparseMatrix(dim_SS);
    P1->setEntry(0,0,0);
    P1->setEntry(0,1,0.875);
    P1->setEntry(0,2,0.0625);
    P1->setEntry(0,3,0.0625);
    P1->setEntry(1,0,0);
    P1->setEntry(1,1,0.75);
    P1->setEntry(1,2,0.125);
    P1->setEntry(1,3,0.125);
    P1->setEntry(2,0,0);
    P1->setEntry(2,1,1.0);
    P1->setEntry(2,2,0);
    P1->setEntry(2,3,0);
    P1->setEntry(3,0,0);
    P1->setEntry(3,1,0);
    P1->setEntry(3,2,0);
    P1->setEntry(3,3,1.0);
    trans.at(1) = P1;

    SparseMatrix *P2 = new SparseMatrix(dim_SS);
    P2->setEntry(0,0,0);
    P2->setEntry(0,1,0.875);
    P2->setEntry(0,2,0.0625);
    P2->setEntry(0,3,0.0625);
    P2->setEntry(1,0,1.0);
    P2->setEntry(1,1,0);
    P2->setEntry(1,2,0);
    P2->setEntry(1,3,0);
    P2->setEntry(2,0,1.0);
    P2->setEntry(2,1,0);
    P2->setEntry(2,2,0);
    P2->setEntry(2,3,0);
    P2->setEntry(3,0,1.0);
    P2->setEntry(3,1,0);
    P2->setEntry(3,2,0);
    P2->setEntry(3,3,0);
    trans.at(2) = P2;



    SparseMatrix *Reward  = new SparseMatrix(dim_SS, dim_AS);
    Reward->setEntry(0,0,0);
    Reward->setEntry(0,1,4000);
    Reward->setEntry(0,2,6000);
    Reward->setEntry(1,0,1000);
    Reward->setEntry(1,1,4000);
    Reward->setEntry(1,2,6000);
    Reward->setEntry(2,0,3000);
    Reward->setEntry(2,1,4000);
    Reward->setEntry(2,2,6000);
    Reward->setEntry(3,0,3000);
    Reward->setEntry(3,1,4000);
    Reward->setEntry(3,2,6000);

    cout << "Building MDP" << std::endl;
    AverageMDP *mdp1 = new AverageMDP(critere, stateSpace, actionSpace, trans, Reward);
    cout << "Building MDP finished" << std::endl;

    cout << "Printing MDP" << std::endl;
    mdp1->Write();

    cout << std::endl << std::endl <<"###############################" << std::endl;
    cout << "Printing solution of value iteration " << std::endl;
    /* call the function to solve the MDP.*/
    start_t =clock();
    SolutionMDP *optimum = mdp1->ValueIteration(epsilon, maxIter);
    end_t= clock();
    total_t = (double)(end_t - start_t)/CLOCKS_PER_SEC;
    cout <<"# Done : elapsing time " << total_t << " sec" << std::endl;
    optimum->Write();

    cout << std::endl <<"Checking solutions of VI" << std::endl;
    mdp1->PolicyCost(optimum,epsilon, maxIter);
    FeedbackSolutionMDP *optimumFeed = dynamic_cast <FeedbackSolutionMDP *> (optimum);
    double *sol1 = optimumFeed->getValue();
    for(i=0;i<stateSpace->Cardinal();i++){
        cout << "i= " << i << " sol1= " << sol1[i] << std::endl;
    }

    //call the function to solve the MDP.
    cout << std::endl << std::endl <<"###############################" << std::endl;
    cout << std::endl<<"Computation with Policy Iteration modified" << std::endl;
    start_t =clock();
    SolutionMDP *optimum2 = mdp1->PolicyIterationModified(epsilon, maxIter, 0.00001, 1000);
    end_t= clock();
    total_t = (double)(end_t - start_t)/CLOCKS_PER_SEC;
    cout <<"# Done : elapsing time " << total_t << " sec" << std::endl;
    optimum2->Write();

    cout  << std::endl <<"Checking solutions of PIM" << std::endl;
    mdp1->PolicyCost(optimum2,epsilon, maxIter);
    optimumFeed = dynamic_cast <FeedbackSolutionMDP *> (optimum2);
    sol1 = optimumFeed->getValue();
    for(i=0;i<stateSpace->Cardinal();i++){
      cout << "i= " << i << " sol1= " << sol1[i] << std::endl;
    }

    cout << std::endl<<"Computation with Policy Iteration modified GS" << std::endl;
    start_t =clock();
    SolutionMDP *optimum2b = mdp1->PolicyIterationModifiedGS(epsilon, maxIter, 0.00001, 1000);
    end_t= clock();
    total_t = (double)(end_t - start_t)/CLOCKS_PER_SEC;
    cout <<"# Done : elapsing time " << total_t << " sec" << std::endl;
    optimum2b->Write();
    delete optimum2b;

    //call the function to solve the MDP.
    cout << std::endl << std::endl <<"###############################" << std::endl;
    cout<< std::endl  <<"Computation with relative value Iteration" << std::endl;
    start_t =clock();
    SolutionMDP * optimum3 = mdp1->RelativeValueIteration(epsilon, maxIter);
    end_t= clock();
    total_t = (double)(end_t - start_t)/CLOCKS_PER_SEC;
    std::cout <<"# Done : elapsing time " << total_t << " sec" << std::endl;
    optimum3->Write();

    cout  << std::endl <<"Checking solutions" << std::endl;
    mdp1->PolicyCost(optimum3,epsilon, maxIter);
    optimumFeed = dynamic_cast <FeedbackSolutionMDP *> (optimum3);
    sol1 = optimumFeed->getValue();
    for(i=0;i<stateSpace->Cardinal();i++){
       cout << "i= " << i << " sol1= " << sol1[i] << std::endl;    
    }

    cout << std::endl << std::endl <<"###############################" << std::endl;
    cout << std::endl << std::endl <<"###############################" << std::endl;
    cout <<"Checking Average Cost" << std::endl;
    cout << std::endl <<"Policy Ra" << std::endl;
    FeedbackSolutionMDP* politique = new FeedbackSolutionMDP(stateSpace->Cardinal());
    politique->setActionIndeax(0,0);
    politique->setActionIndex(1,0);
    politique->setActionIndex(2,0);
    politique->setActionIndex(3,2);
    
    cout  << std::endl <<"Checking solutions" << std::endl;
    mdp1->PolicyCost(politique,epsilon, maxIter);
    politique->Write();
    
    cout << std::endl <<"Policy Rc" << std::endl;
    
    stateType* action = new stateType[stateSpace->Cardinal()];
    politique->setAction(action);
    politique->setActionIndex(0,0);
    politique->setActionIndex(1,0);
    politique->setActionIndex(2,2);
    politique->setActionIndex(3,2);
    
    
    cout  << std::endl <<"Checking solutions" << std::endl;
    mdp1->PolicyCost(politique,epsilon, maxIter);
    politique->Write();


    cout << std::endl <<"Policy Rd" << std::endl;
    /* define the policy */
    politique->setActionIndex(0,0);
    politique->setActionIndex(1,2);
    politique->setActionIndex(2,2);
    politique->setActionIndex(3,2);
    /* reset the value to zero*/
    politique->resetValue();
    /* reset the average cost to 0.0*/
    politique->setAvgCost(0.0);
    politique->Write();

    cout << std::endl <<"Solution checking" << std::endl;
    mdp1->PolicyCost(politique,epsilon, maxIter*10);
    politique->Write();


    cout << std::endl << std::endl << "********************************" << std::endl;

    cout <<"Deleting" << std::endl;
    delete mdp1;
    delete optimum;
    delete optimum2;
    delete optimum3;
    delete politique;

    cout <<"Deleting 2" << std::endl;
    delete actionSpace;
    delete stateSpace;

    return 0;
}
