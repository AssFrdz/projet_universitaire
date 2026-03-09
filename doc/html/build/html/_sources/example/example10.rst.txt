Example 10
^^^^^^^^^^

Description
"""""""""""

This example simulates two Markov chains on a very big grid,
one in continuous time, one in discrete time.

The grid has four dimensions. It is of size 1000 x 1000 x 100 x 100 = 10^10.
Storing such a large matrix would not be feasible. Marmote uses a specialized
object to represent it in a compact way. The general simulation procedure for
Markov chains uses this object as if it were a regular matrix.

Usage:

.. code-block:: bash

   example10 -tmax_sim <time>

Here, 'time' specifies the duration for the simulation: maximal time in continuous time,
and the number of steps in discrete time.

Tasks performed
"""""""""""""""

* Phase 1:

  * create a continuous-time ``HomogeneousMultidTransition`` object with dimensions 1000 x 1000 x 100 x 100, and transition rates equal to 1/8 in each direction;
  * create a continuous-time ``MarkovChain`` object using this HomogeneousMultidTransition object as transition structure;
  * check the consistency of the state space in different objects, by printing its size to the screen;
  * create a simulation of a trajectory, starting from state (0,0,0,0), and store it in a ``SimulationResult`` object:

    * the states are printed along the way
    * the extensive form of the state is printed, in addition to its state index
    
  * write again the trajectory to the screen using the method of ``SimulationResult`` objects;
  
* Phase 2:

  * create a discrete-time `HomogeneousMultidTransition`` object with dimensions 1000 x 1000 x 100 x 100, and transition probabilities equal to 1/8 in each direction;
  * create a discrete-time ``MarkovChain`` object using this HomogeneousMultidTransition object as transition structure;
  * create a simulation of a trajectory, starting from state (0,0,0,0) and store it in a ``SimulationResult`` object:
  
    - the states are printed along the way
    - the extensive form of the state is printed, in addition to its state index
    
  * write again the trajectory to the screen using the method of ``SimulationResult`` objects;
  
* Phase 3:

  * create directly a continuous-time Markov chain object ``HomogeneousMultiDBirthDeath`` with the same state space and transition rates as in Phase 1;
  * simulate it and print trajectories;
  
* Phase 4:

  * create directly a discrete-time Markov chain object ``HomogeneousMultiDRandomWalk`` with the same state space and transition probabilities as in Phase 2;
  * simulate it and print trajectories;
  
* clean up.

Code
""""

.. literalinclude:: ../media/example10.cpp
    :language: c++
    
Command
"""""""

.. literalinclude:: ../media/example10.cmd
    :language: bash

Output
""""""

The output consists in:

* the cardinal of the state space, obtained in three different ways
* for Phases 1 and 3 (continuous time), 

   * the trajectory where each line represents a time instant with four parts: 
   
     1) the time step within brackets; 
     2) the time of the event; 
     3) the state index; 
     4) the state description as a vector of 4 integer numbers within parentheses;
     
   * the trajectory again, but only with the time step and the state description;
   
* for Phases 2 and 4 (discrete time), 

   * the trajectory where each line represents a time instant with three parts: 
   
     1) the time step; 
     2) the state index; 
     3) the state description as a vector of 4 integer numbers within parentheses;
     
   * the trajectory again, but only with the time step and the state description.
 

.. literalinclude:: ../media/example10.res
    :language: bash
 
Download
""""""""

The source file is :download:`here <../media/example10.cpp>`

