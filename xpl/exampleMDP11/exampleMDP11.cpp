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
#include <marmoteCore/marmoteInterval.h>
#include <marmoteMDP/marmoteDiscountedMDP.h>
#include <marmoteMDP/marmoteFeedbackSolutionMDP.h>
#include <marmoteMDP/marmoteSolutionMDP.h>


//#include <marmoteLog+init.h>
#include <marmoteCore/marmotePolicy.h>
#include <marmoteLog/marmoteLog.h>

#include <list>
#include <vector>
#include <string>


using namespace std;

/**
 * @brief class to implement an example of a simple discounted MDP
 * @author EH,
 * @date Feb 2025 (created apr 2022)
 * @version 1.2
 *
 * This example create an example of discounted MDP 
 * 
 * The MDP comes from the book of Geron Chapter 16 fig 16.8
 * 
 * The reward matrix should be
 * 
 * [ 7, 0, 0]
 * [0 , -50, penalty]
 * [penalty, penalty, 32]
 * 
 * 
 * *) When the discount factor is 0.95
 * 
 * Optimal policy is [0, 1, 2] 
 * 
 * Value Function is [21.8992,1.1798, 53.8735]
 * 
 * *) When the discount factor is 0.9 
 * 
 * Optimal policy is [0,0,2] 
 * 
 * Value Function is [18.9189  0 50.1337]
 * 
*/


int main( int argc, char** argv )
{
    int i; 
    /* create and initialize the MDP parameters discount factor and criteria. */
    /* create and initialize the discount factor. */
    double beta = 0.95;
    string critere("max");
     
    /*create and initialize the maximum number of iterations allowed end the precision.*/  
    /* create and initialize epsilon. */
    double epsilon = 0.000001;

    /*create and initialize the maximum number of iterations allowed.*/
    int maxIter = 700;
    
    /* create a penalties for non avaiable action (we should use the limit values) but we do not this.*/
    double penalty = -100000;

    /*==================Test new features==================*/

    marmote::log::initialize();
    marmoteLogI << "I give some informations to the log";
    /*Create the MDP object to test 1-dimension example and fill all its fields.*/
    int min = 0;
    int max = 2;
    int dim_SS = (max-min+1); /* dimension of the state space */
    MarmoteSet *actionSpace = new MarmoteInterval(0,2);
    MarmoteSet *stateSpace = new MarmoteInterval(min,max);
        
    vector<TransitionStructure*> trans(actionSpace->Cardinal());
    SparseMatrix *P0 = new SparseMatrix(dim_SS); 
    /* matrix for the a0 action*/
    P0->setEntry(0,0,0.7);
    P0->setEntry(0,1,0.3);
    P0->setEntry(1,1,1.0);
    P0->setEntry(2,2,1.0); /* add virtual action */
    trans.at(0) = P0;

    /* matrix for the a1 action*/
    SparseMatrix *P1 = new SparseMatrix(dim_SS);
    P1->setEntry(0,0,1.0);
    P1->setEntry(1,2,1.0);
    P1->setEntry(2,2,1.0); /* add virtual action */
    trans.at(1) = P1;
    
    /* matrix for the a2 action*/
    SparseMatrix *P2 = new SparseMatrix(dim_SS);
    P2->setEntry(0,0,0.8);
    P2->setEntry(0,1,0.2);
    P2->setEntry(1,1,1.0); /* add virtual action */
    P2->setEntry(2,0,0.8);
    P2->setEntry(2,1,0.1);
    P2->setEntry(2,2,0.1);
    trans.at(2) = P2;
    
    
    /* creating of a vector of cost matrices when cost are determined by transitions */
    vector<TransitionStructure*> rews(actionSpace->Cardinal());
    /* gain matrices for each transitions 
     a matrix per action */
    SparseMatrix *R1 = new SparseMatrix(dim_SS);
    SparseMatrix *R2 = new SparseMatrix(dim_SS);
    SparseMatrix *R3  = new SparseMatrix(dim_SS);
    
    R1->setEntry(0,0,10);
    R1->setEntry(2,2,penalty);

    R2->setEntry(1,2,-50);
    R2->setEntry(2,2,penalty);
    
    R3->setEntry(1,1,penalty);
    R3->setEntry(2,0,40);

    /* Adding reward to collection */
    rews.at(0) = R1;
    rews.at(1) = R2;
    rews.at(2) = R3;    
    std::cout << "Size :\t" << std::endl ;
    std::cout << critere.size() << std::endl;
    std::cout<< "Begining MDP building" <<  std::endl ;
    DiscountedMDP *mdp1 = new DiscountedMDP(critere, stateSpace, actionSpace, trans, rews,beta);
    std::cout<<"End of building MDP" << std::endl ;

    std::cout<<"printing MDP" << std::endl ;
    mdp1->Write();
        
    std::cout<<"Printing solution from value iteration" << std::endl ;
    /* call the function to solve the MDP. */
    FeedbackSolutionMDP *optimum = mdp1->ValueIteration(epsilon, maxIter);
    optimum->Write();
       
    cout << endl << "Checking solutions" << endl; 
    mdp1->PolicyCost(optimum,epsilon, maxIter);
    for(i=0;i<stateSpace->Cardinal();i++){
		cout << "i= " << i << " sol= " << optimum->getValueIndex(i) << endl;
    }
    
    /*call the function to solve the MDP. */
    cout << endl << "Modified Policy iteration"<< std::endl ;
    SolutionMDP *optimum2 = mdp1->PolicyIterationModified(epsilon, maxIter, epsilon*0.01, 100);
    optimum2->Write();
  
    /* call the function to solve the MDP. */
    cout << endl << "Gauss Seidel Value Iteration"<< std::endl ;
    SolutionMDP *optimum3 = mdp1->ValueIterationGS(epsilon, maxIter);
    optimum3->Write();

        /*call the function to solve the MDP. */
    cout << endl << "Modified Policy iteration GS"<< std::endl ;
    SolutionMDP *optimum4 = mdp1->PolicyIterationModifiedGS(epsilon, maxIter, 0.001, 20);
    optimum4->Write();
        
    
    std::cout << std::endl <<"********************************\n" << std::endl ;
        
    cout << "Destructing" << endl;
    
    delete mdp1;
    delete optimum;
    delete optimum2;
    delete optimum3;
    delete optimum4;
        
    std::cout<<"Destructing 2" << std::endl ;        


    delete stateSpace;
    delete actionSpace;
	
    return 0;
}


