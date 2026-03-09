Example 4
^^^^^^^^^

Description
"""""""""""

Example that reads a Markov chains from files then outputs them
to other files. Two possibilities are illustrated: either export the whole Markov chains, either only their generators.
Usage:

.. code-block:: bash

   example4

Tasks performed
"""""""""""""""

* create two Markov Chain objects, by reading their entries from files with the MCL format
* write these two Markov chains in files, still with the MCL format: this uses the member function "Write()" of MarmoteMarkovChain objects
* write just the generators of these Markov chains: this uses the member function "Store()" of MarmoteTransitionStructure objects
* cleanup

Code
""""

.. literalinclude:: ../media/example4.cpp
    :language: c++

Output 
""""""

The output of the program depends on whether the data files ``IO_example_in1.mcl`` and ``IO_example_in2.mcl`` are available or not in the directory from which the program is exececuted. The first case is the "normal output". The second one is the "error output".


Normal output
-------------

.. code-block:: bash

    ####################################
    Constructed Chains:
    Example1: IO_example_in1.mcl
    discrete sparse
    8
    0          0 2.000000e-01
    0          1 8.000000e-01
    1          0 2.500000e-01
    1          1 2.500000e-01
    1          2 2.500000e-01
    1          3 2.500000e-01
    2          0 6.000000e-01
    2          2 4.000000e-01
    3          2 3.000000e-01
    3          3 2.000000e-01
    3          4 2.500000e-01
    3          5 2.500000e-01
    4          4 1.000000e-01
    4          5 3.000000e-01
    4          6 3.000000e-01
    4          7 3.000000e-01
    5          5 1.000000e+00
    6          4 5.000000e-01
    6          6 5.000000e-01
    7          4 4.000000e-01
    7          5 2.000000e-01
    7          6 2.000000e-01
    7          7 2.000000e-01
    stop
    ####################################
    Example2: IO_example_in2.mcl
    discrete sparse
    10
    0          0 6.000000e-01
    0          1 4.000000e-01
    1          0 3.000000e-01
    1          1 3.000000e-01
    1          2 4.000000e-01
    2          1 3.000000e-01
    2          2 3.000000e-01
    2          3 4.000000e-01
    3          2 3.000000e-01
    3          3 3.000000e-01
    3          4 4.000000e-01
    4          3 3.000000e-01
    4          4 3.000000e-01
    4          5 4.000000e-01
    5          4 3.000000e-01
    5          5 3.000000e-01
    5          6 4.000000e-01
    6          5 3.000000e-01
    6          6 3.000000e-01
    6          7 4.000000e-01
    7          6 3.000000e-01
    7          7 3.000000e-01
    7          8 4.000000e-01
    8          7 3.000000e-01
    8          8 3.000000e-01
    8          9 4.000000e-01
    9          8 3.000000e-01
    9          9 7.000000e-01
    stop
    ####################################

Files produced
--------------

The normal execution produces 4 files:

 * IO_example_out1.mcl and IO_example_out1.mcl, containing the two chains in MCL format. They should be identical to IO_example_in1.mcl and IO_example_in2.mcl, respectively
 * IO_example_mat1.mmt
 * IO_example_chain2.gen.mmt

Error output
------------

When the input files are not present in the directory where example4 is executed, the output is the following:

.. code-block:: text

    ========= MARMOTE LOGGER IS INITIALIZED ==========
    Error in MarkovChain: could not open input file IO_example_in1.mcl. Generator not initialized.
    Error in MarkovChain: could not open input file IO_example_in2.mcl. Generator not initialized.
    ####################################
    Constructed Chains:
    Example1: IO_example_in1.mcl
    Error in MarkovChain::Write(): chain has no generator. Skipped.
    ####################################
    Example2: IO_example_in2.mcl
    Error in MarkovChain::Write(): chain has no generator. Skipped.
    ####################################
    Error in MarkovChain::Write(): chain has no generator. Skipped.
    Error in MarkovChain::Write(): chain has no generator. Skipped.
    Chain 1 has no generator
    Chain 2 has no generator.
 
Download
""""""""

The source file is :download:`here <../media/example4.cpp>`.
The input files are: :download:`IO_example_in1.mcl <../media/IO_example_in1.mcl>` and :download:`IO_example_in2.mcl <../media/IO_example_in2.mcl>`.

