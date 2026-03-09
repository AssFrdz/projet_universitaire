MDP Example 40
==============

A finite horizon MDP

Description
-----------

This is the *stochastic inventory model* studied in Puterman's book (op. cit.).
This model is formally described in in section 3.2.1. Then a numerical example is given and
detailed in section 3.2.2. Finally, in section 4.6.1, the various steps of the solving algorithm are
illustrated.

The maximum stock size is 3 (*i.e.* [0,3]) and we have four actions from 0 to 3.

Expected Results
~~~~~~~~~~~~~~~~

Recal that the horizon is 4. This means that the steps index varies from 0 to 3 and that the value function is equal to 0 at step 3

We should have

+----------------------+
|     At Step 2        |
+----------------------+
+----+--------+--------+
|  s |   v(s) |   q(s) |
+----+--------+--------+
|  0 |   0    |   0    |
+----+--------+--------+
|  1 |   5    |   0    |
+----+--------+--------+
|  2 |   6    |   0    |
+----+--------+--------+
|  3 |   5    |   0    |
+----+--------+--------+


+----------------------+
|    At Step 1         |
+----------------------+
+----+--------+--------+
|  s |  v(s)  |  q(s)  |
+----+--------+--------+
|  0 |   2    |   2    |
+----+--------+--------+
|  1 |  6.25  |   0    |
+----+--------+--------+
|  2 |  10.0  |   0    |
+----+--------+--------+
|  3 |  10.5  |   0    |
+----+--------+--------+


+----------------------+   
|    At Step 0         |
+----------------------+
+----+--------+--------+
| s  |  v(s)  |  q(s)  |
+----+--------+--------+
| 0  | 4.1875 |   3    |
+----+--------+--------+
| 1  | 8.0625 |   0    |
+----+--------+--------+
| 2  | 12.125 |   0    |
+----+--------+--------+
| 3  | 14.188 |   0    |
+----+--------+--------+   



Tasks performed:
----------------

1. We build the different transition matrices as described in the book. e.g. the matrix associated with action 0

.. code-block:: c++

    SparseMatrix *P = new SparseMatrix(stateSpace->Cardinal());
    P->addToEntry(0,0,1.0);
    P->addToEntry(1,0,0.75);
    P->addToEntry(1,1,0.25);
    P->addToEntry(2,0,0.25);
    P->addToEntry(2,1,0.5);
    P->addToEntry(2,2,0.25);
    P->addToEntry(3,1,0.25);
    P->addToEntry(3,2,0.5);
    P->addToEntry(3,3,0.25);
    trans.at(0) = P;
    cout<<"Added transition matrix 0 to vector"<< endl;


2. We build the `FiniteHorizonMDP`

.. code-block:: c++

  FiniteHorizonMDP *mdp1 = new FiniteHorizonMDP(critere, stateSpace, actionSpace, trans, R,N,beta);

where

    * `critere` is the criteria (min or max)
    * `stateSpace` is the state set
    * `actionSpace` is the action set
    * `trans` is the vector of transition matrices
    * `Cost` is the matrix of the rewards
    * `beta` is the discount factor. This discount factor can be equal to 1.

3. We compute the solution by a usual dynamic programming algorithm. 

    * **Be carefull** that the call of the function is done by the name  `ValueIteration(epsilon, maxIter)`. However *epsilon* and *maxIter* are not implied in the algorithm and can be filled in with arbitrary values.
    * **Be carefull** that for a finite horizon the solution returned is not a stationary feedbackpolicy but a non stationary policy. The object returned is than a `NonStationarySolution` and not a `FeedbackSolution`.
    
.. code-block:: c++

  SolutionMDP *optimum = mdp1->ValueIteration(epsilon, maxIter);
  
4. We write the solution. This `NonStationarySolution` writes the policy step by step.

.. code-block:: c++

  optimum->WriteSolution();
  
5. We apply the policy with method `PolicyCost`. This function computes the value of a given `NonStationaryPolicy`

.. code-block:: c++

    double *sol1 = mdp1->PolicyCost(optimum,epsilon, maxIter);
  

Code
----

The whole code is

.. code-block:: c++
    
    #include <list>
    #include <vector>
    #include <string>

    using namespace std;

    #include "marmoteCore/marmoteInterval.h"
    #include "marmoteCore/marmoteSet.h"
    #include "marmoteCore/marmoteSparseMatrix.h"
    #include "marmoteMDP/marmoteFiniteHorizonMDP.h"
    #include "marmoteMDP/marmoteNonStationarySolutionMDP.h"
    #include "marmoteMDP/marmoteSolutionMDP.h"

    int main( int argc, char** argv )
    {
    //==================MDP features==================//
    string critere("max");
    double penalty=-1000;
    int N=3;  //horizon
    double beta=1;

    //==================MDP features==================//
    //create and initialize epsilon.
    double epsilon = 0.1;
    //create and initialize the maximum number of iterations allowed.
    int maxIter = 7;

    //==================Test new features==================//

    //Create the MDP object to test 1-dimension example and fill all its fields.
    int min = 0;
    int max = 3;
    MarmoteSet *actionSpace = new MarmoteInterval(0,3);
    MarmoteSet *stateSpace = new MarmoteInterval(min,max);

    vector<TransitionStructure*> trans(actionSpace->Cardinal());

    SparseMatrix *P = new SparseMatrix(stateSpace->Cardinal());
    P->addToEntry(0,0,1.0);
    P->addToEntry(1,0,0.75);
    P->addToEntry(1,1,0.25);
    P->addToEntry(2,0,0.25);
    P->addToEntry(2,1,0.5);
    P->addToEntry(2,2,0.25);
    P->addToEntry(3,1,0.25);
    P->addToEntry(3,2,0.5);
    P->addToEntry(3,3,0.25);
    trans.at(0) = P;
    cout<<"Added transition matrix 0 to vector"<< endl;

    P = new SparseMatrix(stateSpace->Cardinal());
    P->addToEntry(0,0,0.75);
    P->addToEntry(0,1,0.25);
    P->addToEntry(1,0,0.25);
    P->addToEntry(1,1,0.5);
    P->addToEntry(1,2,0.25);
    P->addToEntry(2,1,0.25);
    P->addToEntry(2,2,0.5);
    P->addToEntry(2,3,0.25);
    P->addToEntry(3,1,0.25);
    P->addToEntry(3,2,0.5);
    P->addToEntry(3,3,0.25);
    trans.at(1) = P;
    cout<<"Added transition matrix 1 to vector"<< endl;
    
    P = new SparseMatrix(stateSpace->Cardinal());
    P->addToEntry(0,0,0.25);
    P->addToEntry(0,1,0.5);
    P->addToEntry(0,2,0.25);
    P->addToEntry(1,1,0.25);
    P->addToEntry(1,2,0.5);
    P->addToEntry(1,3,0.25);
    P->addToEntry(2,1,0.25);
    P->addToEntry(2,2,0.5);
    P->addToEntry(2,3,0.25);
    P->addToEntry(3,1,0.25);
    P->addToEntry(3,2,0.5);
    P->addToEntry(3,3,0.25);
    trans.at(2) = P;
    cout<<"Added transition matrix 2 to vector"<< endl;

    P = new SparseMatrix(stateSpace->Cardinal());
    P->addToEntry(0,1,0.25);
    P->addToEntry(0,2,0.5);
    P->addToEntry(0,3,0.25);
    P->addToEntry(1,1,0.25);
    P->addToEntry(1,2,0.5);
    P->addToEntry(1,3,0.25);
    P->addToEntry(2,1,0.25);
    P->addToEntry(2,2,0.5);
    P->addToEntry(2,3,0.25);
    P->addToEntry(3,1,0.25);
    P->addToEntry(3,2,0.5);
    P->addToEntry(3,3,0.25);
    trans.at(3) = P;
    cout<<"Added transition matrix 3 to vector"<< endl;

    SparseMatrix *R  = new SparseMatrix(stateSpace->Cardinal(),actionSpace->Cardinal());
    R->addToEntry(0,0,0);
    R->addToEntry(0,1,-1);
    R->addToEntry(0,2,-2);
    R->addToEntry(0,3,-5);
    R->addToEntry(1,0,5);
    R->addToEntry(1,1,0);
    R->addToEntry(1,2,-3);
    R->addToEntry(1,3,penalty);
    R->addToEntry(2,0,6);
    R->addToEntry(2,1,-1);
    R->addToEntry(2,2,penalty);
    R->addToEntry(2,3,penalty);
    R->addToEntry(3,0,5);
    R->addToEntry(3,1,penalty);
    R->addToEntry(3,2,penalty);
    R->addToEntry(3,3,penalty);

    std::cout<<"Begining building MDP" << std::endl;
    FiniteHorizonMDP *mdp1 = new FiniteHorizonMDP(critere, stateSpace, actionSpace, trans, R,N,beta);
    std::cout<<"MDP built" << std::endl;
    
    std::cout<<"print MDP" << std::endl;
    mdp1->WriteMDP();
    
    std::cout<<"Print solution after value iteration" << std::endl;
    /*call the function to solve the MDP.*/
    SolutionMDP *optimum = mdp1->ValueIteration(epsilon, maxIter);
    optimum->WriteSolution();

    std::cout<<"\nChecking solutions" << std::endl;
    double *sol1 = mdp1->PolicyCost(optimum,epsilon, maxIter);
    for(stateType i=0;i<stateSpace->Cardinal();i++){
         std::cout<<"i=" << i << " solution= " << sol1[i] << std::endl;
    }
    
    std::cout<<"\n********************************" << std::endl;
    std::cout<<"Destroying" << std::endl;
    mdp1->ClearRew();
    delete optimum;

    std::cout<<"Destroying 2" << std::endl;
    
    for(int i=0;i<actionSpace->Cardinal();i++){
        delete trans.at(i);
    }
    delete mdp1;

    return 0;
    }


Download
--------

The source file is :download:`here <../media/exampleMDP40.cpp>`

Output
------

.. literalinclude:: ../media/exampleMDP40.res
    :language: text
