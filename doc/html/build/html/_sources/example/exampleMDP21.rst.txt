MDP Example 21
==============

Description
-----------

This is an Avreage cost MDP


| This example assumed a single machine which has 4 states
| — 0 = new
| — 1 = usable with minor damage
| — 2 = usable with major damage
| — 3 = Out of order

| This example has 3 actions
| — 0 = Do nothing and the system remains in the same state. 
|      Cost of action 0 in state 0 = 0. 
|      Cost of action 0 in state 1 = 1000 
|      Cost of action 0 in state 2 = 3000
|      Cost of action 0 in state 3 = 3000
| — 1 = Fine tuning and the system jumps to state  Cost in any state = 4000
| — 2 = Total repair: return to state 0. Cost in any state = 6000

This example comes from a MDP course of J.Y. Potvin.

The optimal expected value is 1666.67 and the optimal policy is (0,0,1,2)

| Some others policies are also considered and evaluated :
| — Ra = (1, 1, 1, 3) with average cost = 1923.
| — Rc = (1, 1, 3, 3) with average cost = 1727.
| — Rd = (1, 3, 3, 3) with average cost = 3000.

Tasks performed:
----------------

We create P0, P1, P2 ``SparseMatrix`` matrices that are the transition matrices associated with decision 0, 1, 2

We create Reward the matrix of the cost. 

| In line with explanation above it is equal to:
| [    0  4000 6000 ]
| [ 1000  4000 6000 ] 
| [ 3000  4000 6000 ]
| [ 3000  4000 6000 ]

For each of the solving method we measure the running time

We consider policy Ra, Rc, Rd and we evaluate their costs by mean of function ``PolicyCost()``

Clean up.

Code
----

We build the AverageMDP. The criteria is a string equal to `min` defined line 52, 

.. code-block:: c++
    
    cout << "Building MDP" << std::endl;
    AverageMDP *mdp1 = new AverageMDP(critere, stateSpace, actionSpace, trans, Reward);
    cout << "Building MDP finished" << std::endl;


Here is the code to launch ``ValueIteration(epsilon, maxIter)`` : *epsilon* is the precision, *maxIter* the maximum number of allowed iterations. 
We then measure the running time. 
    
.. code-block:: c++

    start_t =clock();
    SolutionMDP *optimum = mdp1->ValueIteration(epsilon, maxIter);
    end_t= clock();
    total_t = (double)(end_t - start_t)/CLOCKS_PER_SEC;
    cout <<"# Done : elapsing time " << total_t << " sec" << std::endl;
    optimum->WriteSolution();    
 
| Here is the code to define a policy. 
| A stationary policy is an object ``FeedbackSolutionMDP`` created with the constructor ``FeedbackSolutionMDP(dimension)``. 
| We fix the value of the action with ``setActionIndex(index,value)``. 
| Here the policy is *Ra=[0,0,0,2]*

.. code-block:: c++ 

    cout << std::endl <<"Politicy Ra" << std::endl;
    FeedbackSolutionMDP* politique = new FeedbackSolutionMDP(stateSpace->Cardinal());
    politique->setActionIndex(0,0);
    politique->setActionIndex(1,0);
    politique->setActionIndex(2,0);
    politique->setActionIndex(3,2);
    
    

| We now evaluate (or compute)  the cost of a policy. 
| This is done with ``PolicyCost(politique,epsilon, maxIter)``
| The method ``setValue(solpol)`` fix the averagevalue to ``solpol``

.. code-block:: c++  

    /* reset the value to zero*/
    politique->resetValue();
    /* reset the average cost to 0.0*/
    politique->setAvgCost(0.0);
    
    cout  << std::endl <<"Checking solutions" << std::endl;
    double *solpol = mdp1->PolicyCost(politique,epsilon, maxIter);
    politique->setValue(solpol);
    politique->WriteSolution();  
    
Output
------

.. literalinclude:: ../media/exampleMDP21.res
    :language: text

Download
--------

The source file is :download:`here <../media/exampleMDP21.cpp>`

