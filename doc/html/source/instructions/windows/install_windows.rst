Marmote install with conda on MS Windows
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Requirements
^^^^^^^^^^^^

Before installing ``Marmote`` you will need to install a **conda distribution** . You can :

* install Conda with  ``miniconda`` from: |miniconda_link|
    * you will need to create an account for this,
    * follow carrefully the installation instructions of miniconda,
    * this solution could have **licensing restrictions for commercial uses**,
    * Installing miniconda is not required if ``Anaconda`` is previously  installed.

* install Conda with ``miniforge`` from |miniforge_link|
    * you will need to follow carrefully the installation instructions,
    * this solution has **No licensing restrictions for commercial uses**.

.. |miniconda_link| raw:: html

   <a href="https://conda.io/miniconda.html" target="_blank">Anaconda's website</a>

.. |miniforge_link| raw:: html

    <a href="https://github.com/conda-forge/miniforge" target="_blank">Miniforge Repository</a>

.. * [optional, but can facilitate later use of the software] add conda to the *PATH*. This can be done during installation or later (type ``Win + R``, type ``sysdm.cpl``)

Installation of conda packages
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

| The following installation steps should be executed in a *conda prompt* window:
| to this end open an ``anaconda prompt`` in the start menu.


Creating the virtual runtime environment
""""""""""""""""""""""""""""""""""""""""
The next step is to create the conda environment (called *marmote-use*) and to activate it.


.. code-block:: sh

    $ conda create -n marmote-use
    $ conda activate marmote-use

Checking the prerequisites
""""""""""""""""""""""""""

You can check the required version of :doc:`prerequisites <../../build_instructions>`, when the environment *marmote-use* is activated, by typing 

.. code-block:: sh

    $ conda list
    
which returns the list of the installed packages and their versions.

When it is necessary to modify the version of installed libraries, or install new libraries,
use the ``conda install`` instruction. Below is an example which modifies the versions of python and numpy
in the conda environment:

.. code-block:: sh

    $ conda activate marmote-use   # If the environnement marmote-use is not activated
    $ conda install -c conda-forge python=3.XX numpy=Y.ZZ



Install the library
"""""""""""""""""""
In order to install the library in the conda environment it is necessary to specify the channels and the minimal packages on the command line.

.. code-block:: sh

    $ conda activate marmote-use   # If the environnement marmote-use is not activated
    $ conda install -c marmote -c conda-forge marmote


Checking the installation
""""""""""""""""""""""""

In the shell window, type

.. code-block:: sh

    $ conda activate marmote-use # If the environnement marmote-use is not activated
    $ conda list


In the list, find the name ``marmote`` and check the version. If the name does not appear the software is not installed. If the version is not the latest available, then some dependencies were missing. 
Try to identify and fix them.

If both name and version are correct, you are ready to program!
