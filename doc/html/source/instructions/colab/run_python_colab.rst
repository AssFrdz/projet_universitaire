Using the Python API of Marmote on google colab
===============================================

Python notebooks can be run in `google colab <https://colab.google/>`_

Please note that with the proposed solution you should reinstall the library each time you restart the kernel.

Preparation
-----------

  1. Download the set of notebooks on your own computer
  2. Open  `Google colab <https://colab.google/>`_   with your google account
  3. Upload the notebook that you want to use (or all the notebooks)
  4. Execute the uploaded notebook. The first cells installs the software and can take up to several minutes.


Running the notebooks and select colab runtime version
------------------------------------------------------

Runtime latest version
^^^^^^^^^^^^^^^^^^^^^^

Marmote 1.3.1 works with the latest version of the Runtime, released at the end of January 2026.
You should add the following two cells immediately after installing *marmote*.


.. code-block:: python

  !conda list marmote
  !python --version

and

.. code-block:: python

  import sys
  site_packages_path = '/usr/local/lib/python3.11/site-packages'
  if site_packages_path not in sys.path:
      sys.path.insert(0, site_packages_path)
  print(f"sys.path updated: {site_packages_path in sys.path}")

where the value of ``site_packages_path`` is deduced from a call to ``conda list``
that gives you the path to site-package of your python version.

Runtime version 2025.07
^^^^^^^^^^^^^^^^^^^^^^^

Marmote 1.30 and 1.3.1 works with Runtime version 2025.07. To select the correct runtime.

.. line-block::

  Go to Tab "Runtime",
  then tab "Change Runtime type",
  then tab "Runtime version"
  then select "2025.07"


In case of problems
^^^^^^^^^^^^^^^^^^^

Have a look also in the :doc:`dedicated FAQ page for problems <../faq>` **for a patch**).

