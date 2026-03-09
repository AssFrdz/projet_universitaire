Troubleshooting
===============

.. contents:: Issues are organised by marmote version
   :local:
   :depth: 2

Problems with marmote-1-3-0
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Installation problem with Linux
-------------------------------

June 2025.  Problem of python 3.13
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For the version of *python3.13* it is not possible to have both *marmote-1-3-0* and *jupyter*. Furthermore, without installing a specific version of python in the conda env
(with  command ``conda install -c conda-forge python=3.1X``) before marmote install, then during the installation of *marmote1-3-0*, the version of python is switched to *python3-13*.
For the time being, we recommend that you install *python3.12*.

June 2025.  Problem of compiling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you encounter the following error message during compilation:

.. code-block:: bash

   undefined reference « std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_replace_cold(char*, unsigned long, char const*, unsigned long, unsigned long)@GLIBCXX_3.4.31

Then you need to install a new version of the library  `GLIBCXX` (marmote-1-3-0 has been compiled with version 15 of GLIBC). This can be done by typing

.. code-block:: bash

    conda install -c conda-forge gxx=15 libstdcxx-ng


Installation problem with MacOS
-------------------------------

June 2025.  Problem of compiling
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If the following error appears
``fatal error: 'string' file not found #include <string>``, this is not an error of *marmote* but a (more general) problem of MacOS.
To resolve the issue you should desinstall and reinstall the command line tool by

.. code-block:: bash


    sudo rm -rf /Library/Developer/CommandLineTools
    xcode-select --install

Installation problem with Windows
---------------------------------

June 2025.  Problem with *mkl* library
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this case, it may be necessary to reinstall *mkl* library after marmote install. To do so ``conda install -c conda-forge mkl``.

Installation problem on colab
-----------------------------

October 2025.  Problem of colab environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



Problems with marmote-1-2-0
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Installation problem on colab
-----------------------------

June 2025.  Problem of numpy version
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Google change the numpy version in colab. The standard version is now *2.0.2* but *marmote-1-2-0* expects an older version of numpy *1.26.4* or older. It must be necessary to change the numpy version.

**Check the installed version** To check the version,

1. Add a new code cell in the notebook juste before the cell with text ``To install marmote`` and fill in with the instructions

.. code-block:: python

    import numpy as np
    print(np.__version__)


2. Execute the cell. The version of numpy used is printed. If the version is not *1.XX.yy*, then you can not use marmote (but you can install it) and you should install the correct version of numpy.

**Install the correct version of numpy** To install the correct version of numpy

1. Add a new code cell just before the cell with text  ``To install condacolab`` and fill in with the instructions

.. code-block:: bash

    !pip uninstall numpy thinc spacy -y
    !pip uninstall numpy -y
    !pip install -qq numpy==1.26.4
    if int(np.__version__[0]) > 1:
        import os
        os.kill(os.getpid(), 9)


2. In ``Runtime`` tab, select ``Disconnect and delete runtime``

3. Execute the notebook.

*N.B.* It may happen than during the installation of the new version of numpy some dependances are not satisfied (this occurs if the message  * `error pip's dependency resolver does not currently take into account all the packages that are installed` * appears). However, even if the message appears you can continue to execute the notebook and the correct version of numpy should be installed.


Installation problem with Linux
-------------------------------

June 2024. Problem of old `libstdcxx` (on debian stable).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you encounter the following error message during compilation:

.. code-block:: bash

    /usr/bin/ld : /miniconda3/envs/marmote-use/lib/libmarmoteMarkovChain.so.1.2.0 : undefined reference « std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::_M_replace_cold(char*, unsigned long, char const*, unsigned long, unsigned long)@GLIBCXX_3.4.31

Then you need to install a new version of the library  `GLIBCXX` (marmote-1-2-0 has been compiled with version 13 of GLIBC). This can be done by typing

.. code-block:: bash

    conda install -c conda-forge libstdcxx-ng
    conda install -c conda-forge gxx=13.2.0
