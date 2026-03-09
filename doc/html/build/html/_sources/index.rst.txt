.. MARMOTE documentation master file, created by
   sphinx-quickstart on Thu Feb 13 10:21:40 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. meta::
   :description: The documentation of marmote (Markovian Modelling Tools Environement) software includings numerous tutorials and installation procedure
   :keywords: Markov Chain, Markov Decision Process, solver, marmote, modelling tools


Marmote's documentation
=======================

.. toctree::
   :maxdepth: 1
   :caption: Table of Contents
   :name: mastertoc
   :hidden:

   build_instructions
   cpp_api
   python_api
   about


Marmote
-------

Marmote is a programming platform for building and solving Markov models with discrete state spaces.

.. raw:: html

   <style>
   .center-table {
       margin-left: auto;
       margin-right: auto;
       width: 75%;
   }
   </style>

.. container:: center-table

   .. table::

   =====================  ======================  ===============  ================
     :ref:`Description`     main :ref:`Features`    :ref:`Latest`    :ref:`Licence`
   =====================  ======================  ===============  ================



.. _Description:

Description
-----------

Marmote (MARkovian MOdelling Tools and Environements) integrates a Markov chain solver, a Markov Decision Process solver and consists in a library of objects:

* allowing to construct custom Markov Chain models, analyze their structure and solve them
* allowing to construct custom Markov Decision Process models, solve them and analyze the solution
* representing well-known Markovian models from the literature.

This library comes with two API: one for C++ development, one for Python development (that encapsulates C++ objects).

With this website you can learn how to

* :doc:`build <./build_instructions>` a consistent development environment for contributing to marmote, using conda
* program your models with the :doc:`C++ API <./cpp_api>`
* program your models with the :doc:`Python API <./python_api>`


.. _Features:

Features
--------

* Predefined Spaces (Interval, Box, Composite Set, Simplex, Binary Sequence) and tools for constructing complex state spaces
* Easy handling of Transition Structures
* Support for sparse matrices

Markov Chain Features
~~~~~~~~~~~~~~~~~~~~~

* Graph Analysis of transitions
* Discrete and Continuous Time Chains
* Predefined Chains (Birth and Death, Random Walk, Poisson Process, MMPP)
* Usual solving methods (`Power Method`)
* Advanced solving methods (`RLGL`)

Markov Decision Process Features
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* Criteria implemented are: `infinite horizon discounted`, `infinite horizon average`, `infinite horizon total`, `finite horizon discounted`
* Solving Methods are `Value Iteration` and `Policy Iteration Modified`
* Structural properties analysis

.. _documentation:

For more details about features, you have access to the :doc:`c++ Documentation <./cpp_index>` including the user/reference :download:`manual <./media/manual.pdf>` and the :doc:`Python Documentation <./python_index>`   with the detailed API documentations.

.. _Latest:

Latest Versions
---------------

* Version 1.3.1 released on 2025-12-10
* Version 1.3.0 released on 2025-07-17
* Version 1.2.0 released on 2024-05-31
* Version 1.1.1 released on 2024-05-21 (deprecated)
* Version 0.3.0 released on 2023-10-12 (deprecated)

.. _Licence:

Licence
-------

This software is distributed under the terms of the GNU General Public License (GPL) version 3. 
By using, modifying or distributing this software, you agree to the terms and conditions of the GPL.


Documentation creation date: |today|
