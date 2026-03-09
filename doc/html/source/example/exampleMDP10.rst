MDP Example 10
==============

This illustrative example is the same one for C++ than the illustrative example of MDP Lesson 1 for python.

Using the library
-----------------

The different header files to include for using a simple Discounted MDP are :

.. code-block:: c++

    #include <list>
    #include <vector>
    #include <string>

    #include <marmoteCore/marmoteSparseMatrix.h>
    #include <marmoteCore/marmoteInterval.h>
    #include <marmoteMDP/marmoteDiscountedMDP.h>
    #include <marmoteMDP/marmoteFeedbackSolutionMDP.h>
    #include <marmoteMDP/marmoteSolutionMDP.h>




Build a simple MDP
------------------

Reminders about MDP
~~~~~~~~~~~~~~~~~~~

Theoretically  MDP is a tuple *(S,A,P,R)* where :

* *S* is the state space
* *A* is the action space
* *P* is a transition probability to a new state *y* from a state *x* in which action *a* is performed
* *R* is the reward

Description of the exemple implemented here
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We assume a simple model with two states x_1=0 and x_2=1 and in each state: two actions a_1=0 and a_2=1.

The reward matrix is:
::

       |  4.5 |  2   |
       | -1.5 |  3.0 |

where *r(x,a)* is the entry with row coordinate $x$ and column coordinate $a$.
The entry *r(x,a)* represents the reward when in state *x* action *a* is performed.

The transition matrices are:

Transition matrix of action 0:
::

       |  0.6 |  0.4 |
       |  0.5 |  0.5 |

Transition matrix of action 1:
::

       |  0.2 |  0.8 |
       |  0.7 |  0.3 |


Elements of a MDP object
~~~~~~~~~~~~~~~~~~~~~~~~

Attributes of the object
""""""""""""""""""""""""


A MDP receives (at least) four objects:


#.  an object ``MarmoteSet`` for the  state space.
    Note that ``MarmoteSet`` objects are done for handling sets. They can be ``MarmoteInterval``, ``MarmoteBox``.
#.  an object ``MarmoteSet`` for the action space.
    Note that each element in a ``MarmoteSet`` objects has an index.
#.  a vector of transition structures. Each entry in the vector corresponds with a ``TransitionStructure`` associated with a given action.

    - the ``TransitionStructure`` at the *a*-th entry of the vector is the ``TransitionStructure`` associated with the action whose index is *a*.
    - a ``TransitionStructure`` describes the probability of transition from a state *i* into state *j*.
    - the TransitionStructure can be a ``FullMatrix`` or a ``SparseMatrix``. Here the *(i,j)* entry of a matrix gives the probability to move from state with index *i* to state with index *j*.

#.  a ``TransitionStructure`` for the  reward (or costs).
    This is a preferably a ``FullMatrix`` in which the cost for an action *a* in a state *x* is defined at entry
    whose row has index *x* of the states and whose column has index *a* of the action.
    Hence the entry *(x,a)* represents the cost of action of index *a* in state of index *x*


How to build the DiscountedMDP object
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Create state space and action space
"""""""""""""""""""""""""""""""""""

Here we create two ``MarmoteInterval`` objects to hold the state space and the action space.
The simplest ``MarmoteSet`` object is the object ``MarmoteInterval``. Here we use a ``MarmoteInterval`` for the state space.
The action space is  also a ``MarmoteSet`` object and in our case, it's also a ``MarmoteInterval`` object.


.. code-block:: c++

    MarmoteSet* stateSpace = new MarmoteInterval(0, 1);
    MarmoteSet* actionSpace = new MarmoteInterval(0, 1);


Storing matrices
""""""""""""""""


A vector is used to store all transition matrices.
The number of matrices should correspond with the size of the action space.
Now, we create a ``vector<TransitionStructure*>`` (a vector of ``TransitionStructure`` which is the superclass of ``SparseMatrix``) to store the transition matrices.

.. code-block:: c++

    vector<SparseMatrix*> trans(actionSpace->Cardinal());



Build transition matrices
"""""""""""""""""""""""""

Create two ``SparseMatrix`` objects to hold the transition matrices associated with each of the two actions.
Hence, we create *P0*, an object ``SparseMatrix`` with size *2x2* and after *P1*. The *P0* and *P1* matrices are transition matrices.
They are filled in  entry by entry with the ``setEntry()`` function.
Hence, we initialize one entry using ``setEntry(row, column, value)`` syntax. We then add *P0* to the trans vector at index 0 (action 0).


.. code-block:: c++

    SparseMatrix* P0 = new SparseMatrix(2);

    P0->setEntry(0, 0, 0.6);
    P0->setEntry(0, 1, 0.4);
    P0->setEntry(1, 0, 0.5);
    P0->setEntry(1, 1, 0.5);

    trans[0] = P0;

    SparseMatrix* P1 = new SparseMatrix(2);
    P1->setEntry(0, 0, 0.2);
    P1->setEntry(0, 1, 0.8);
    P1->setEntry(1, 0, 0.7);
    P1->setEntry(1, 1, 0.3);

    trans[1] = P1;

Build reward matrix
"""""""""""""""""""

We create a ``FullMatrix`` object with size *2x2* for storing rewards associated with each (state, action) pair. The following lines add non-zero values to the matrix `Reward`.

More precisely:

 - The first line adds the value 4.5 to position (0, 0) in the matrix.
 - The second line adds the value 2 to position (0, 1) in the matrix.
 - The third line adds the value -1.5 to position (1, 0) in the matrix.
 - The fourth line adds the value 3 to position (1, 1) in the matrix.

.. code-block:: c++

    FullMatrix* Reward = new FullMatrix(2, 2);

    Reward->setEntry(0, 0, 4.5);
    Reward->setEntry(0, 1, 2);
    Reward->setEntry(1, 0, -1.5);
    Reward->setEntry(1, 1, 3);


Additional parameters of the discounted MDP
"""""""""""""""""""""""""""""""""""""""""""


Additional parameters are: beta (discount factor) and criterion (optimization criterion).

.. code-block:: c++


    double beta = 0.95;
    string criterion = "max";


Build a discounted MDP
""""""""""""""""""""""


Create an MDP as a ``DiscountedMDP`` object and we build  a `DiscountedMDP` object using the constructor, to which we pass the parameters defined in this page.
We then print the mdp with method ``Write``.

Parameters of a ``DiscountedMDP`` are:

 - criterion either "min" or "max"
 - an object that encodes the state space
 - an object that encodes the action space
 - a list of TransitionStructure
 - a reward
 - the discount factor


.. code-block:: c++

    DiscountedMDP* mdp = new DiscountedMDP(criterion, stateSpace, actionSpace, trans, Reward, beta);
    mdp->Write();


Solve a simple MDP
------------------

Resolution Methods
~~~~~~~~~~~~~~~~~~

Detail of the methods can be found in the literature.  All these methods return a `FeedbackSolutionMDP` object with both the optimal value and the optimal policy.


Solve the MDP using:

- ValueIteration ``ValueIteration(epsilon, maxIter)``
- ValueIterationGS  ``ValueIterationGS(epsilon, maxIter)``
- ValueIterationInit ``ValueIterationInit(epsilon, maxIter, pol)``
- PolicyIterationModified ``PolicyIterationModified(epsilon, maxIter,epsilon2, maxIter2)``
- PolicyIterationModifiedGS ``PolicyIterationModifiedGS(epsilon, maxIter,epsilon2, maxIter2)``


There are mainly two parameters for solving the MDP.

 - ``int maxIter = 700;``  the maximum number of iterations allowed
 - ``double epsilon = 0.00001;`` the precision with which the solution is approximated


Otherwise ``PolicyIterationModified`` and variants receive two other parameters related with the maximum number of iterations and the precision required to solve
the *Evaluation Policy* step. On the other hand ``ValueIterationInit`` receives a parameter *pol* which contains
the initial values from which the iteration of the value is run.


Running Resolution Methods
~~~~~~~~~~~~~~~~~~~~~~~~~~

In what follows we compute the solution and we print it with the method ``Write``. We can obtain additionnal details by changing the verbosity with ``changeVerbosity(true)``

.. code-block:: c++

    FeedbackSolutionMDP* optimum = mdp->ValueIteration(epsilon, maxIter);
    optimum->Write();

    FeedbackSolutionMDP* optimum2 = mdp->ValueIterationGS(epsilon, 10);
    optimum2->Write();

    FeedbackSolutionMDP* optimum3 = mdp->ValueIterationInit(epsilon, 200, optimum2);
    cout << "Optimum3" << std::endl;
    optimum3->Write();

    FeedbackSolutionMDP* optimum4 = mdp->PolicyIterationModified(epsilon, maxIter, 0.001, 100);
    cout << "Optimum4" << std::endl;
    optimum4->Write();

    mdp->changeVerbosity(true);

    FeedbackSolutionMDP* optimum5 = mdp->PolicyIterationModifiedGS(epsilon, maxIter, 0.01, 20);
    cout << "Optimum5" << std::endl;
    optimum5->Write();



About SolutionMDP
~~~~~~~~~~~~~~~~~


The solution is stored with a ``FeedbackSolutionMDP`` object. This object has attributes that store a value and an action for each state.
The printing of a ``FeedbackSolutionMDP`` (e.g. ``optimum5->Write()`` ) gives first information about the policy.
The information for all the states in the state space is then displayed. All the information for a state is shown on one line, starting with the state index, the state value and the action associated with the value.


Clean up
~~~~~~~~

Clean up the memory: first us ``ClearRew`` to clean up the reward (and so matrix `Reward`) and then delete the `mdp`.

.. code-block:: c++

    mdp1->ClearRew();

    delete mdp;
    delete optimum;
    delete optimum2;
    delete optimum3;
    delete optimum4;
    delete optimum5;
    delete actionSpace;
    delete stateSpace;
    delete P0;
    delete P1;


Download
--------

The source file can be downloaded :download:`here <../media/exampleMDP10.cpp>`.

Output
------

.. literalinclude:: ../media/exampleMDP10.res
    :language: text
