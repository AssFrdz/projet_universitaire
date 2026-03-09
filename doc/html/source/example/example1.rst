Example 1
^^^^^^^^^

Description
"""""""""""

This example creates a 3-state, discrete-time Markov chain, then performs a (Monte-Carlo) simulation of it. Usage:

.. code-block:: bash

   example1 <n> <p1> <p2> <p3>

Here, n is the number of steps for the simulation, and p1, p2, p3 are the respective initial probabilities of the three states.

Tasks performed
"""""""""""""""

* create a ``DiscreteDistribution`` object to hold the initial distribution of the process
* create a ``SparseMatrix`` object to hold the transition matrix of the chain, entry by entry with the ``addToEntry()`` function;
* create a ``MarkovChain`` object and link the previous elements to it;
* output the Markov chain object to the screen;
* create a simulation of a trajectory and store it in a ``SimulationResult`` object;
* write the trajectory to the screen;
* clean up.

Code
""""

.. literalinclude:: ../media/example1.cpp
    :language: c++

Command
"""""""

.. literalinclude:: ../media/example1.cmd
    :language: bash

Output
""""""

The output consists in:

 * the code of the Markov chain, which itself consists in:
 
   + the description of the matrix, here an entry-by-entry format
   + the description of the initial distribution
   
 * the trajectory, where each line represents a time instant: three numbers appear: 1) the time step; 2) the state index; 3) the state description (in this case, the state and the index are the same).

.. code-block:: bash

    $ ./example1 10 0.2 0.3 0.5
    discrete sparse
    3
         0          0 2.500000e-01
         0          1 5.000000e-01
         0          2 2.500000e-01
         1          0 4.000000e-01
         1          1 2.000000e-01
         1          2 4.000000e-01
         2          0 4.000000e-01
         2          1 3.000000e-01
         2          2 3.000000e-01
    stop
    discrete values { 0 1 2 } probas {      0.2      0.3      0.5 }
    Trajectory 
         0        0 0
         1        2 2
         2        2 2
         3        1 1
         4        0 0
         5        0 0
         6        1 1
         7        0 0
         8        1 1
         9        2 2
        10        2 2

Download
""""""""

The source file is :download:`here <../media/example1.cpp>`

