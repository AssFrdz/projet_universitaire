Install and run instructions
============================


Principle of the software
-------------------------

Marmote consists of a C++ library and its wrapping to Python. 
Programming in either language is made easier using the following technogies:

1. **Conda** is used to create execution environments which are consistent
   with the chosen version of Marmote.

   a. Please note that there are **no commercial restrictions** with our use of *conda*.
   b. *marmote* only uses packages from channel *conda-forge* which is an open source channel managed by the community.
   c. *marmote* is open-source and hosted on an *open-source channel*.
   d. the choice of *miniforge* insures the use of softwares or packages *with no commercial restrictions*.

2. **CMake** is used for C++ progamming as the primary build system generator. 
   It configures the
   build environment and generates platform-specific build files (e.g.,
   Makefiles, Visual Studio project files).

Versions and prerequisites
--------------------------

The current version of Marmote is 1.3.1. 

Versions 1.3.0 and 1.2.0 can still be used but are not recommended.

Version 1.1.1 and version 0.3.0 are **deprecated**.

Requirements for Python programming are:

* Versions 1.3.0 and 1.3.1

  1. depend on python 3.10 or 3.11 or 3.12 or 3.13
  2. depend on numpy 2.X

* Version 1.2.0

  1. depends on python 3.10 or 3.11
  2. depends on numpy 1.2X.

Requirements for C++ programming are:

* Versions 1.3.0 and 1.3.1

  1. depend on GLIBC 15 

Platform-specific instructions
------------------------------

Installing and running marmote on different platforms requires specific instructions. See the instructions dedicated to your platform below.

Install Marmote
~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1
   :name: build_instructions_toc

   Linux <./instructions/linux/install_linux>
   Windows <./instructions/windows/install_windows>
   MacOS <./instructions/macos/install_macos>

Compile C++ Files and run
~~~~~~~~~~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1
   :name: run_instructions_C_toc

   Linux <./instructions/linux/compile_linux>
   Windows <./instructions/windows/compile_windows>
   MacOS <./instructions/macos/compile_macos>

Run python Files
~~~~~~~~~~~~~~~~

.. toctree::
   :maxdepth: 1
   :name: run_instructions_P_toc

   Linux <./instructions/linux/run_python_linux>
   Windows <./instructions/windows/run_python_windows>
   MacOS <./instructions/macos/run_python_macos>
   Colab <./instructions/colab/run_python_colab>

In case of problems
-------------------

Although we have tested the environment on many platforms, you may encounter some difficulties.
If this is the case,

* please take a look at the dedicated :doc:`troubleshooting page <./instructions/faq>`

* or contact us if your problem persists.

Note that in order to make our conda environment as portable as possible, we have chosen not
to overload it with numerous installed packages. Most of the time, the problem originates
in a missing package, or a mismatch between package versions. In that case, you will just need to
(re)install a specific package to resolve the situation.

.. toctree::
    :maxdepth: 1
    :name: faq_instructions_toc
    :hidden:

    instructions/faq
