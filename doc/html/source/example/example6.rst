Example 6
^^^^^^^^^

Description
"""""""""""

Example that creates MarmoteSet objects and enumerates them. Objects tested are MarmoteBox and MarmoteInterval.
Usage:

.. code-block:: bash

   example6

Tasks performed
"""""""""""""""

* create a 1-dimensional ``MarmoteBox`` object with size 2;
* create a 2-dimensional ``MarmoteBox`` object with size 2x3;
* for each of them:

  * perform a test on the object;
  * perform the same test on the object cast to a generic ``MarmoteSet`` object;
  * cleanup.
      
* the test consists in:

  * writing the cardinal of the set, and the number of its dimensions;
  * enumerating the set with the ``Enumerate()`` member function;
  * enumerating it again, via a walk through it, using the ``FirstState()`` and ``NextState()`` member functions;
  * enumerating it again, via the indices of the states and the ``DecodeState()`` member function.

Code
""""

.. literalinclude:: ../media/example6.cpp
    :language: c++

Output
""""""

.. literalinclude:: ../media/example6.res
  
Download
""""""""

The source file is :download:`here <../media/example6.cpp>`
