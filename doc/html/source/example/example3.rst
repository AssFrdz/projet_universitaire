Example 3
^^^^^^^^^

Description
"""""""""""

Example that constructs three 8x8 discrete-time Markov chain then computes the transient distributions.
Usage:

.. code-block:: bash

   example3 <n> <p1> <p2> <p3> <p4> <p5> <p6> <p7> <p8>

Here, n is the number of steps for the simulation, and p1 to p8 are the respective initial probabilities of the eight states.

Tasks performed
"""""""""""""""

* parse the command line to get the parameters of the run;
* create a ``DiscreteDistribution`` object to hold the initial distribution, common to the three Markov chainps;
* create three ``SparseMatrix`` object to hold the transition matrices of the chains, entry by entry with the ``addToEntry()`` function;
* create three ``MarkovChain`` object and link the previous elements to them with the ``set_init_distribution()`` and ``set_generator()`` functions;
* compute the distribution of each Markov chain after n steps;
* write the distributions to the screen;
* clean up.

Code
""""

.. literalinclude:: ../media/example3.cpp
    :language: c++

Command
"""""""

.. literalinclude:: ../media/example3.cmd
    :language: bash

Output
""""""

The output consists in the representation of the three discrete distribution in the ``Marmote`` format: 1) the list of "states" of hte distribution (here, the integer interval [0..7]); 2) the list of respective probabilities of these states. Additional tracing information is inserted at the different phases of the program.
 
.. code-block:: text

	$ ./example3 10 0.4 0.05 0.05 0.1 0.1 0.1 0.1 0.1
	## Displaying transient distributions at time 10
	discrete values { 0  1  2  3  4  5  6  7  } probas {  0.09636   0.1116  0.07247  0.03764  0.04887   0.5676  0.04505  0.02041 } 
	discrete values { 0  1  2  3  4  5  6  7  } probas {  0.09636   0.1116  0.07247  0.03764     0.17   0.1811   0.1598   0.1711 } 
	discrete values { 0  1  2  3  4  5  6  7  } probas {   0.1614    0.171    0.145    0.149   0.1031   0.1528  0.07875  0.03898 } 
	## Destruction of initial and transient distributions
	## Destruction of MCs


Download
""""""""

The source file is :download:`here <../media/example3.cpp>`
