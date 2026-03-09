Example 5
^^^^^^^^^

Description
"""""""""""

Example that creates a 3x3 Markov chain then tests several methods to compute the same result: its stationary distribution.
Usage:

.. code-block:: bash

   example5

Tasks performed
"""""""""""""""

* create a state space with 3 states, using the ``MarmoteInterval`` object;
* create an ``UniformDistribution`` object, and use it to draw initial distribution at random;
* create a ``MarkovChain`` object with the state space and the initial distribution, and write it to the standard output;
* create a transition matrix for the chain, and associate the matrix to the chain with the ``set_generator()`` method;
* use three methods to compute the stationary distribution of the chain: 

  * the default method ``StationaryDistribution()``;
  * the power method with a maximal number of iterations and a tolerance, starting from the initial distribution;
  * the RLGL (red light green light) method, also with a maximal number of iterations and a tolerance.
  
* write the resulting distributions to the standard output;
* compute the distance between two of the resulting distributions, using the L1 norm;
* check the (approximate) invariance of the distribution obtained with RLGL by:

  * computing the one-step transient distribution with this RLGL distribution as initial distribution;
  * computing the L1 distance between the result and the RLGL distribution.
  
* test, as alternative, the computation of a transient distribution by:

  * computing the transient distribution after 100 steps from the same initial distribution;
  * computing the L1 distance between the result and the distribution obtained with the power method.
  
* cleanup. 

Code
""""

.. literalinclude:: ../media/example5.cpp
    :language: c++

Output
""""""

.. literalinclude:: ../media/example5.res
    :language: text
  
Download
""""""""

The source file is :download:`here <../media/example5.cpp>`
