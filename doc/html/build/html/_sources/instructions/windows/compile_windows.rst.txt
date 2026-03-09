Using the C++ API of Marmote on MS Windows
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. |br| raw:: html

   <br>
  

It is recommended follow the ``cmake`` development framework.
Its steps are:

* specify what are the project's source files, libraries etc. in a single file named ``CMakeLists.txt''
* create a specific directory for building binaries, separated from source files
* execute ``cmake`` to create adequate makefiles
* execute ``make`` to create binaries, perform tests and all other tasks.

Requirements
^^^^^^^^^^^^

For the use of C++ API (and only for it), you will need to install ``Visual Studio`` (which is **not** Visual Studio Code) and some elements of ``Visual C++``:

* install Visual Studio |vs_link|
    * Choose the Visual Studio 2026 Community version

.. |vs_link| raw:: html

   <a href="https://visualstudio.microsoft.com/downloads/" target="_blank">from VS' website</a>

* install ``C++ CMake tools for Windows``
    * This needs both 'workloads' ``Desktop development with C++`` and ``Linux Development with C++`` to be selected (see |explanation_link| for details). 
    * This can be done with the visual studio installer.
    
.. |explanation_link| raw:: html

   <a href="https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170" target="_blank">here</a>

The following steps are executed in a shell window:
open an ``anaconda prompt`` in the start menu.
    

Preparation
^^^^^^^^^^^

| Allow the shell to find Visual Studio's cmake

.. code-block:: winbatch

   $ set PATH=%ProgramFiles%\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin;%PATH%

| Prepare an installation directory, and move into it.  We assume throughout that ``MAR_DIR`` is the path to this directory.  
| Note that the choice of the repository's name is free: you can also call it ``Marmote`` if you prefer.

.. code-block:: sh

    $ mkdir MAR_DIR
    $ cd MAR_DIR

Creation of the makefile and the binaries
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  
Creating a ``cmake`` project simply requires placing there C++ source files and a
``CMakeLists.txt`` configuration file.

Compilation of all examples
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Download the source code files: :download:`as zip file <../all_ex/all_examples.zip>` to the directory ``MAR_DIR``,
and unzip it there with your favorite tool.
Download also the :download:`configuration file <../all_ex/CMakeLists.txt>`.

Next, prepare the build directory, and "make" the project:

.. code-block:: sh
  
    $ mkdir build
    $ cd build
    $ cmake -G "Visual Studio 18 2026" ..
    $ cmake --build . --config Release

If you use a previous version of Visual Studio, you may have to select the "generator" ``Visual Studio 16 2019``
or ``Visual Studio 17 2022``.

The executables will be found in the ``Release`` directory.
To execute it (here for *example1*):

.. code-block:: winbatch

    $ cd Release		
    $ .\example1 3 0.2 0.2 0.6


Some help
^^^^^^^^^

Managing environnement in visual studio:
`see here <https://learn.microsoft.com/en-us/visualstudio/python/managing-python-environments-in-visual-studio?view=vs-2022>`__

Managing Cmake in visual studio:
`see here <https://learn.microsoft.com/fr-fr/cpp/build/cmake-projects-in-visual-studio?view=msvc-170#cmake-partial-activation>`__

