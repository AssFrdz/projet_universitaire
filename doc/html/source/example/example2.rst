Example 2
^^^^^^^^^

Description
"""""""""""

Example that constructs a 3x3 discrete-time Markov chain then computes the transient distribution for a number of steps and an initial distribution given as parameters to the application. Usage:

.. code-block:: bash

   example2 <n> <p1> <p2> <p3>

Here, n is the number of steps for the simulation, and p1, p2, p3 are the respective initial probabilities of the three states.

Tasks performed
"""""""""""""""

* create a ``DiscreteDistribution`` object to hold the initial distribution of the process
* create a ``SparseMatrix`` object to hold the transition matrix of the chain, entry by entry with the ``addToEntry()`` function;
* create a ``MarkovChain`` object and link the previous elements to it;
* output the Markov chain object to the screen;
* compute the distribution of the Markov chain after n steps and store it in a ``Distribution`` object;
* write the distribution to the screen;
* clean up.

Code
""""

.. literalinclude:: ../media/example2.cpp
    :language: c++

Command
"""""""

.. literalinclude:: ../media/example2.cmd
    :language: bash

Output
""""""

The output consists in:

 * the code of the Markov chain: matrix and initial distribution (see also example 1)
 * the representation of the discrete distribution in the ``Marmote`` format: 1) the list of "states" of the distribution (here, the integer interval [0..2]); 2) the list of respective probabilities of these states.

.. code-block:: bash

    $ ./example2 10 0.2 0.3 0.5
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
    discrete values { 0  1  2  } probas { 3.4783e-01 3.3597e-01 3.1621e-01 } 

Download
""""""""

The source file is :download:`here <../media/example2.cpp>`
