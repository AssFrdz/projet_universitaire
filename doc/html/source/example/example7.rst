Example 7
^^^^^^^^^

Description
"""""""""""

Example that constructs four 8x8 discrete-time Markov chain then performs structural analysis on them.
Usage:

.. code-block:: bash

   example7

Tasks performed
"""""""""""""""

* create 4 Markov chains with 8 states each, but different transition structures (see figure below)
* for each of them:

    * perform a quick diagnostic of the generator
    * compute the absorbing states
    * compute the communicating classes
    * list the recurrent classes
    * compute the period

.. image:: ../_images/example7.png

Code
""""

.. literalinclude:: ../media/example7.cpp
    :language: c++

Output
""""""

.. literalinclude:: ../media/example7.res
  
Download
""""""""

The source file is :download:`here <../media/example7.cpp>`
