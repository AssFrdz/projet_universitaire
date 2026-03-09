Using the Python API of Marmote on MS Windows
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. |br| raw:: html

   <br>
  
  
Examples provided are in two forms: 
Python notebooks and simple Python program files.

Preparation
^^^^^^^^^^^
   
Prepare an installation directory, and move into it.
We assume throughout that ``MAR_DIR`` is the path to this directory. |br|
Note that the choice of the repository's name is free you can also 
call it ``Marmote`` if you prefer.

.. code-block:: sh

    $ mkdir MAR_DIR
    $ cd MAR_DIR

The following steps are executed in a shell window:
open an ``anaconda prompt`` or a ``conda powershell`` in the start menu.

Running the notebooks
^^^^^^^^^^^^^^^^^^^^^

The installation of `jupyter` in the conda environement is required. So type

.. code-block:: sh

    $ conda install -c conda-forge jupyter

Download the notebooks files: :download:`as zip file <../all_nb/all_notebooks.zip>` to the directory ``MAR_DIR``.
Unpack the source files:

.. code-block:: sh

    $ unzip all_notebooks.zip

Then move in the directory and type

.. code-block:: sh

    $ jupyter-notebook name_of_notebook.ipynb


Running Python files
^^^^^^^^^^^^^^^^^^^^

Download all the Python files: :download:`as zip file <../all_pyex/all_pythons.zip>` to the directory ``MAR_DIR``.
Unpack the source files:

.. code-block:: sh

    $ unzip all_pythons.zip

Then move in the directory and type

.. code-block:: sh

    $ python name_of_script.py

Running Python files with VSCode
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Once Marmote is installed on the computer it is also possible to load the Marmote python kernel in `vscode`.
All details to manage environmentsin VSCode are given by Microsoft `here <https://code.visualstudio.com/docs/python/environments>`__.

(From the webpage above)
The easy way to access to active conda environments in the shell and VSC is to use a conda prompt and activate the desired environment. Then, you can launch VS Code by entering the
``code . `` command. Once VS Code is open, you can select the interpreter either by using the Command Palette or by clicking on the status bar.
