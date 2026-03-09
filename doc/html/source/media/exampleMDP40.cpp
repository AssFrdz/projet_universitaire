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

/**
 * @brief class to implement an example of a finite Horizon MDP
 * @author Hyon, lip6.
 * @version 0.6
 * @date oct 2022
 * 
 * A finite horizon MDP issued from example 3.2 of Puterman's book.
 * 
 * We should have 
 *  At Step 0
 *   s   v(s)    q(s)
 *   0   4.1875   3
 *   1   8.0625   0
 *   2   12.125   0
 *   3   14.188   0
 *
 *
 */

//#include <list>
#include <vector>
#include <string>

using namespace std;

#include "marmoteCore/marmoteInterval.h"
#include "marmoteCore/marmoteSet.h"
#include "marmoteCore/marmoteSparseMatrix.h"
#include "marmoteCore/marmoteFullMatrix.h"
#include "marmoteMDP/marmoteFiniteHorizonMDP.h"
#include "marmoteMDP/marmoteNonStationarySolutionMDP.h"
#include "marmoteMDP/marmoteSolutionMDP.h"

#include <marmoteCore/marmotePolicy.h>
#include <marmoteLog/marmoteLog.h>



int main( int argc, char** argv )
{
  //==================MDP features==================//
  string critere("max");
  double penalty=-1000;
  int N=3;  //horizon
  double beta=1;

  //==================MDP solving features==================//
  /* create and initialize epsilon nd initialize the maximum number of iterations allowed. */
  double epsilon = 0.01;
  int maxIter = 7;
  /* These two values have no effect on the running of the method */


  marmote::log::initialize();

  //================== Build MDP==================//

  /* Create the MDP object to test 1-dimension example and fill all its fields. */
  int min = 0;
  int max = 3;
  MarmoteSet *actionSpace = new MarmoteInterval(0,3);
  MarmoteSet *stateSpace = new MarmoteInterval(min,max);

  vector<TransitionStructure*> trans(actionSpace->Cardinal());
  
  SparseMatrix *P = new SparseMatrix(stateSpace->Cardinal());
  P->setEntry(0,0,1.0);
  P->setEntry(1,0,0.75);
  P->setEntry(1,1,0.25);
  P->setEntry(2,0,0.25);
  P->setEntry(2,1,0.5);
  P->setEntry(2,2,0.25);
  P->setEntry(3,1,0.25);
  P->setEntry(3,2,0.5);
  P->setEntry(3,3,0.25);
  trans.at(0) = P;
  cout<<"Added transition matrix 0 to vector"<< endl;
  
  P = new SparseMatrix(stateSpace->Cardinal());
  P->setEntry(0,0,0.75);
  P->setEntry(0,1,0.25);
  P->setEntry(1,0,0.25);
  P->setEntry(1,1,0.5);
  P->setEntry(1,2,0.25);
  P->setEntry(2,1,0.25);
  P->setEntry(2,2,0.5);
  P->setEntry(2,3,0.25);
  P->setEntry(3,1,0.25);
  P->setEntry(3,2,0.5);
  P->setEntry(3,3,0.25);
  trans.at(1) = P;
  cout<<"Added transition matrix 1 to vector"<< endl;
  
  P = new SparseMatrix(stateSpace->Cardinal());
  P->setEntry(0,0,0.25);
  P->setEntry(0,1,0.5);
  P->setEntry(0,2,0.25);
  P->setEntry(1,1,0.25);
  P->setEntry(1,2,0.5);
  P->setEntry(1,3,0.25);
  P->setEntry(2,1,0.25);
  P->setEntry(2,2,0.5);
  P->setEntry(2,3,0.25);
  P->setEntry(3,1,0.25);
  P->setEntry(3,2,0.5);
  P->setEntry(3,3,0.25);
  trans.at(2) = P;
  cout<<"Added transition matrix 2 to vector"<< endl;
  
  P = new SparseMatrix(stateSpace->Cardinal());
  P->setEntry(0,1,0.25);
  P->setEntry(0,2,0.5);
  P->setEntry(0,3,0.25);
  P->setEntry(1,1,0.25);
  P->setEntry(1,2,0.5);
  P->setEntry(1,3,0.25);
  P->setEntry(2,1,0.25);
  P->setEntry(2,2,0.5);
  P->setEntry(2,3,0.25);
  P->setEntry(3,1,0.25);
  P->setEntry(3,2,0.5);
  P->setEntry(3,3,0.25);
  trans.at(3) = P;
  cout<<"Added transition matrix 3 to vector"<< endl;

  FullMatrix *R  = new FullMatrix(stateSpace->Cardinal(),actionSpace->Cardinal());
  R->setEntry(0,0,0);
  R->setEntry(0,1,-1);
  R->setEntry(0,2,-2);
  R->setEntry(0,3,-5);
  R->setEntry(1,0,5);
  R->setEntry(1,1,0);
  R->setEntry(1,2,-3);
  R->setEntry(1,3,penalty);
  R->setEntry(2,0,6);
  R->setEntry(2,1,-1);
  R->setEntry(2,2,penalty);
  R->setEntry(2,3,penalty);
  R->setEntry(3,0,5);
  R->setEntry(3,1,penalty);
  R->setEntry(3,2,penalty);
  R->setEntry(3,3,penalty);

  std::cout<<"Begining building MDP" << std::endl;
  FiniteHorizonMDP *mdp1 = new FiniteHorizonMDP(critere, stateSpace, actionSpace, trans, R,N,beta);
  std::cout<<"MDP built" << std::endl;

  std::cout<<"print MDP" << std::endl;
  mdp1->Write();

  std::cout<<"Print solution after value iteration" << std::endl;
  /*call the function to solve the MDP.*/
  NonStationarySolutionMDP *optimum = mdp1->ValueIteration(epsilon, maxIter);
  optimum->Write();

  std::cout<<"\nChecking solutions" << std::endl;
  mdp1->PolicyCost(optimum,epsilon, maxIter);
  for(stateType i=0;i<stateSpace->Cardinal();i++){
         std::cout<<"i=" << i << " N=" << N-1 << " solution= " << optimum->getValueAtStepIndex(N-1,i) << std::endl;
  }


  std::cout<<"\n********************************" << std::endl;
  std::cout<<"Destroying" << std::endl;
  delete optimum;
  delete mdp1;

  std::cout<<"Destroying 2" << std::endl;

  delete stateSpace;
  delete actionSpace;
  
  return 0;
}
