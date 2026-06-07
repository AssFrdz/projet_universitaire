# Marmote Documentation Developer Guide / Guide developpeur

This repository contains the assets and automation used to build the Marmote teaching documentation: Python notebooks, C++ notebooks for Xeus-cling, downloadable archives, generated example outputs, and the Sphinx website.

Ce depot contient les ressources et scripts de generation utilises pour produire la documentation pedagogique Marmote : notebooks Python, notebooks C++ pour Xeus-cling, archives telechargeables, sorties d'exemples generees et site Sphinx.

- [Version francaise](#version-francaise)
- [English version](#english-version)

---

## Version francaise

### 1. Objet du depot

Ce depot sert a maintenir et generer la documentation Marmote autour de trois familles de contenus :

- les notebooks Python publies dans `doc/html/source/pytutos/`
- les notebooks C++ publies dans `doc/html/source/cpptutos/`
- les exemples C++ officiels presents dans `xpl/`, compiles puis copies dans la documentation

Le workflow actuel repose principalement sur trois scripts Python :

- `scripts/total.py`
- `scripts/doc_generateArchives.py`
- `scripts/doc_copyExamples.py`

Le point d'entree a utiliser est `scripts/total.py`.

### 2. Arborescence utile

```text
project-root/
|-- .env
|-- build/
|-- doc/
|   `-- html/
|       |-- build/
|       |   `-- html/
|       `-- source/
|           |-- cpptutos/
|           |-- pytutos/
|           |-- media/
|           |-- instructions/
|           |-- conf.py
|           |-- cpp_examples.rst
|           `-- python_examples.rst
|-- scripts/
|   |-- total.py
|   |-- doc_generateArchives.py
|   `-- doc_copyExamples.py
`-- xpl/
```

Role des dossiers principaux :

- `xpl/` : sources C++ officielles Marmote
- `build/` : arborescence de compilation CMake des exemples C++
- `doc/html/source/pytutos/` : notebooks Python publies par Sphinx
- `doc/html/source/cpptutos/` : notebooks C++ publies par Sphinx
- `doc/html/source/media/` : fichiers `.cpp`, `.res`, `.cmd`, images et ressources copiees pour la documentation
- `doc/html/source/instructions/` : archives ZIP generees pour le telechargement
- `doc/html/build/html/` : site HTML genere

### 3. Dependances necessaires

Le depot n'embarque pas encore de fichier d'environnement unique. Les dependances sont donc a installer manuellement.

#### 3.1. Dependances Python pour les scripts et Sphinx

Installer au minimum :

```powershell
python -m pip install python-dotenv nbformat nbconvert sphinx nbsphinx recommonmark sphinx-rtd-theme
```

Selon votre usage, vous pouvez aussi installer :

```powershell
python -m pip install jupyterlab
```

#### 3.2. Dependances systeme

`pandoc` doit etre disponible dans le `PATH` pour que `nbsphinx` puisse traiter correctement les notebooks.

Verification :

```powershell
pandoc --version
```

#### 3.3. Environnement Marmote pour compiler les exemples C++

Les `CMakeLists.txt` presents dans `xpl/` et `build/` s'appuient sur `CONDA_PREFIX`. Il faut donc activer un environnement conda contenant Marmote avant de compiler les exemples.

Verification :

```powershell
echo $env:CONDA_PREFIX
```

Sur Windows, les scripts CMake actuels supposent une arborescence Conda de type :

- `%CONDA_PREFIX%\Library\include`
- `%CONDA_PREFIX%\Library\lib`

Si vous travaillez sous Linux ou macOS, adaptez au besoin les variables `MAR_INC` et `MAR_LIB` dans le `CMakeLists.txt` concerne.

#### 3.4. Environnement Xeus-cling pour les notebooks C++

Les notebooks C++ de `cpptutos/` sont prevus pour un kernel Jupyter `xcpp17`.

Les premieres cellules cachees de ces notebooks contiennent des directives `#pragma cling` qui referencent explicitement un environnement Conda. Dans le depot actuel, les chemins sont de la forme :

```text
/home/assia/miniconda3/envs/xeus-cpp-env/include
/home/assia/miniconda3/envs/xeus-cpp-env/lib
```

Si votre environnement differe, vous devez remplacer ces chemins dans les notebooks C++.

### 4. Configuration obligatoire du fichier `.env`

Le script `scripts/total.py` charge la configuration via `python-dotenv`. Le fichier `.env` a la racine du depot est obligatoire.

Contenu actuel :

```dotenv
PROJECT_ROOT="project-root"

LIST1=[1,2,3,4,5,6,7,10]
LIST2=[10,11,21,31,40]

CORRESPONDING_DIRECTORY=["example1","example2","example3","example4","example5","example6","example7","example10","exampleMDP10","exampleMDP11","exampleMDP21","exampleMDP31","exampleMDP40"]

CORRESPONDING_EXECUTABLE=["example1.exe","example2.exe","example3.exe","example4_bin.exe","example5.exe","example6.exe","example7.exe","example10.exe","exampleMDP10.exe","exampleMDP11.exe","exampleMDP21.exe","exampleMDP31.exe","exampleMDP40.exe"]

CORRESPONDING_PARAMETERS=["10 0.1 0.2 0.7","10 0.1 0.2 0.7","10 0.05 0.05 0.05 0.05 0.05 0.05 0.2 0.5","example4/IO_example_in1.mcl example4/IO_example_in2.mcl","","","","","","-tmax_sim 5"]
```

#### 4.1. Ce qu'il faut remplacer

- `PROJECT_ROOT`
  - valeur recherchee dans le chemin courant pour verifier que le script est bien lance dans le bon depot
  - valeur recommandee : garder `project-root`

- `LIST1`
  - numeros des exemples chaines de Markov a inclure dans l'archive et dans le `CMakeLists.txt` genere

- `LIST2`
  - numeros des exemples MDP a inclure dans l'archive et dans le `CMakeLists.txt` genere

- `CORRESPONDING_DIRECTORY`
  - liste des dossiers source dans `xpl/`

- `CORRESPONDING_EXECUTABLE`
  - liste des executables attendus dans `build/bin/`

- `CORRESPONDING_PARAMETERS`
  - parametres utilises pour executer les exemples qui produisent des `.res` et `.cmd`

#### 4.2. Contraintes importantes

- Les listes `CORRESPONDING_DIRECTORY`, `CORRESPONDING_EXECUTABLE` et `CORRESPONDING_PARAMETERS` doivent rester coherentes entre elles.
- `example4` est un cas special : l'executable attendu est `example4_bin.exe`, pas `example4.exe`, afin d'eviter le conflit avec le dossier `example4/`.
- La logique actuelle de `total.py` suppose encore que le depot s'appelle `project-root` lorsqu'il remonte l'arborescence :

```python
while(os.path.basename(os.getcwd())!="project-root"):
```

Le plus simple est donc de garder le nom du dossier racine a `project-root`. Si vous renommez le depot, il faut aussi adapter cette condition dans `scripts/total.py`.

### 5. Workflow developpeur recommande

#### Etape 1. Activer l'environnement Marmote

```powershell
conda activate marmote-use
```

Adaptez le nom de l'environnement a votre machine.

#### Etape 2. Compiler les exemples C++

Depuis la racine du depot :

```powershell
cmake -S xpl -B build
cmake --build build --config Release
```

Les executables attendus sont produits dans `build/bin/`.

Exemples :

```powershell
cd build\bin
.\example1.exe
.\exampleMDP10.exe
```

#### Etape 3. Generer les ressources de documentation

Depuis la racine du depot :

```powershell
python scripts/total.py
```

Options disponibles :

```powershell
python scripts/total.py -n
python scripts/total.py -g
```

Signification :

- `-n`, `--dry_run`
  - verifie les chemins et l'environnement sans generer les fichiers

- `-g`, `--generate_only`
  - genere les fichiers de sortie des exemples sans reconstruire le `CMakeLists.txt` final des exemples

#### Etape 4. Construire la documentation Sphinx

Depuis `doc/html/` :

```powershell
python -m sphinx -b html source build\html
```

Le site est genere dans :

```text
doc/html/build/html/
```

### 6. Ce que produit `scripts/total.py`

Le script global enchaine deux familles d'operations :

#### 6.1. Generation d'archives et d'exports

Portee par `doc_generateArchives.py`.

Resultats :

- `doc/html/source/instructions/all_nb_colab/all_notebooks_colab.zip`
  - notebooks Python adaptes a Colab

- `doc/html/source/instructions/all_nb_cpp/all_notebooks.zip`
  - exports `.cpp` generes a partir des notebooks C++
  - inclut aussi un `CMakeLists.txt` genere automatiquement

- `doc/html/source/instructions/all_ex/all_examples.zip`
  - archive des exemples C++ copies depuis `media/`

- `doc/html/source/instructions/all_nb_py/all_notebooks.zip`
  - archive des notebooks Python

- `doc/html/source/instructions/all_pyex/all_pythons.zip`
  - archive des fichiers `.py` exportes depuis les notebooks Python

#### 6.2. Copie et execution des exemples C++

Portee par `doc_copyExamples.py`.

Resultats :

- copie des fichiers `.cpp` selectionnes dans `doc/html/source/media/`
- generation des fichiers `.res`
- generation des fichiers `.cmd` pour certains exemples chaines de Markov
- mise a jour de `doc/html/source/instructions/all_ex/CMakeLists.txt`
- ajout de `cmake/CMakeLists.txt` dans `all_examples.zip`

### 7. Details importants sur les notebooks C++

#### 7.1. Cellules cachees et export `.cpp`

La generation des fichiers `.cpp` a partir de `cpptutos/` est geree par :

- `creating_cppFiles(...)`
- `_prepare_notebook_for_cpp_export(...)`
- `_strip_xeus_lines(...)`

Le comportement actuel est le suivant :

- les cellules taguees `hide-cell` ou `nbsphinx-hidden` sont exclues de l'export
- les lignes `#pragma cling ...` sont supprimees du `.cpp` exporte
- les `#include` utiles restent presents dans le `.cpp`
- un `CMakeLists.txt` est genere automatiquement pour l'archive C++

#### 7.2. Kernel Jupyter attendu

Les notebooks `cpptutos/*.ipynb` referencent le kernel :

```json
"name": "xcpp17"
```

Si le kernel n'existe pas sur votre machine :

- installez Xeus-cling avec ce kernel
- ou mettez a jour les metadonnees des notebooks avec le nom de votre kernel

#### 7.3. Chemins `#pragma cling` a adapter

Les notebooks contiennent des chemins absolus qui doivent correspondre a votre installation. Rechercher au minimum :

```text
/home/assia/miniconda3/envs/xeus-cpp-env/include
/home/assia/miniconda3/envs/xeus-cpp-env/lib
```

Si vous changez le nom de l'environnement ou l'utilisateur, remplacez ces chemins dans chaque notebook C++ concerne.

### 8. CMake : deux cas a distinguer

#### Cas 1. Compilation des exemples `xpl/`

Le `CMakeLists.txt` de `xpl/` n'utilise pas `find_package(marmote)`. Il s'appuie directement sur :

- `CONDA_PREFIX`
- `%CONDA_PREFIX%\Library\include`
- `%CONDA_PREFIX%\Library\lib`

Dans ce cas, il suffit en principe d'activer le bon environnement conda avant de lancer CMake.

#### Cas 2. Compilation des `.cpp` exportes depuis `cpptutos/`

L'archive generee dans `instructions/all_nb_cpp/all_notebooks.zip` contient un `CMakeLists.txt` base sur :

```cmake
find_package(marmote REQUIRED COMPONENTS marmoteCore marmoteLog marmoteMarkovChain marmoteMDP)
```

Si CMake ne trouve pas Marmote sur Windows, cela ne signifie pas que Marmote ne fournit pas de configuration CMake. Il faut souvent indiquer explicitement ou la trouver :

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:\path\to\marmote"
```

ou

```powershell
cmake -S . -B build -Dmarmote_DIR="C:\path\to\marmote\lib\cmake\marmote"
```

Remplacez ces chemins par ceux de votre installation Marmote.

### 9. Depannage

#### `Pandoc wasn't found`

Cause :

- `pandoc` n'est pas installe ou n'est pas visible dans le `PATH`

Verification :

```powershell
pandoc --version
```

#### `No such kernel named xcpp17`

Cause :

- le kernel Xeus-cling attendu par les notebooks C++ n'est pas enregistre dans Jupyter

Verification :

```powershell
jupyter kernelspec list
```

#### `download file not readable: ... manual.pdf`

Cause :

- `doc/html/source/cpp_api.rst` reference `media/manual.pdf`
- le fichier n'existe pas a cet emplacement

Solutions :

- ajouter le fichier PDF manquant
- ou supprimer/corriger le lien dans le `.rst`

#### Le kernel C++ meurt pendant un notebook

Cause frequente :

- reexecution hors ordre de cellules manipulant des pointeurs, objets detruits ou structures Marmote lourdes

Conseil :

- redemarrer le kernel
- executer `Run All`
- eviter de relancer une cellule de test apres une cellule de nettoyage/desallocation

#### `total.py` ne trouve pas les chemins attendus

Verifications :

- le depot s'appelle bien `project-root`
- vous avez un fichier `.env` valide a la racine
- les dossiers `xpl/`, `build/bin/`, `doc/html/source/pytutos/`, `doc/html/source/cpptutos/` et `doc/html/source/media/` existent

### 10. Maintenance courante

Quand vous ajoutez un nouvel exemple ou une nouvelle lecon, pensez a mettre a jour :

- les notebooks dans `pytutos/` ou `cpptutos/`
- les index `.rst` concernes
  - `doc/html/source/python_examples.rst`
  - `doc/html/source/cpp_examples.rst`
- le fichier `.env` si un nouvel exemple C++ doit etre compile/execute
- les listes et executables attendus si `build/bin/` change

Checklist minimale avant livraison :

- `python scripts/total.py -n`
- `python scripts/total.py`
- `python -m sphinx -b html source build\html` depuis `doc/html/`
- verification des archives dans `doc/html/source/instructions/`
- verification des pages HTML generees dans `doc/html/build/html/`

---

## English version

### 1. Repository purpose

This repository contains the automation and source assets used to build the Marmote teaching documentation:

- Python notebooks published from `doc/html/source/pytutos/`
- C++ notebooks for Xeus-cling published from `doc/html/source/cpptutos/`
- official Marmote C++ examples stored in `xpl/`, compiled and then copied into the documentation assets

The main scripts are:

- `scripts/total.py`
- `scripts/doc_generateArchives.py`
- `scripts/doc_copyExamples.py`

The script to run is `scripts/total.py`.

### 2. Useful repository structure

```text
project-root/
|-- .env
|-- build/
|-- doc/
|   `-- html/
|       |-- build/
|       |   `-- html/
|       `-- source/
|           |-- cpptutos/
|           |-- pytutos/
|           |-- media/
|           |-- instructions/
|           |-- conf.py
|           |-- cpp_examples.rst
|           `-- python_examples.rst
|-- scripts/
|   |-- total.py
|   |-- doc_generateArchives.py
|   `-- doc_copyExamples.py
`-- xpl/
```

Main directories:

- `xpl/`: official Marmote C++ sources
- `build/`: CMake build tree for the C++ examples
- `doc/html/source/pytutos/`: Python notebooks published by Sphinx
- `doc/html/source/cpptutos/`: C++ notebooks published by Sphinx
- `doc/html/source/media/`: copied `.cpp`, `.res`, `.cmd`, images, and shared documentation assets
- `doc/html/source/instructions/`: generated ZIP archives for download
- `doc/html/build/html/`: generated HTML website

### 3. Required dependencies

There is no single pinned environment file yet, so dependencies must currently be installed manually.

#### 3.1. Python dependencies for scripts and Sphinx

Minimum:

```powershell
python -m pip install python-dotenv nbformat nbconvert sphinx nbsphinx recommonmark sphinx-rtd-theme
```

Optional:

```powershell
python -m pip install jupyterlab
```

#### 3.2. System dependency

`pandoc` must be available in `PATH` for `nbsphinx` to render notebooks correctly.

Check:

```powershell
pandoc --version
```

#### 3.3. Marmote conda environment for C++ example compilation

The `CMakeLists.txt` files used for `xpl/` and `build/` rely on `CONDA_PREFIX`. Activate a conda environment containing Marmote before running CMake.

Check:

```powershell
echo $env:CONDA_PREFIX
```

On Windows, the current CMake scripts assume the Conda layout:

- `%CONDA_PREFIX%\Library\include`
- `%CONDA_PREFIX%\Library\lib`

If you work on Linux or macOS, adapt `MAR_INC` and `MAR_LIB` in the relevant `CMakeLists.txt` file if needed.

#### 3.4. Xeus-cling environment for C++ notebooks

The C++ notebooks in `cpptutos/` are designed for a Jupyter kernel named `xcpp17`.

Their hidden setup cells also contain hard-coded `#pragma cling` paths. In the current repository they look like:

```text
/home/assia/miniconda3/envs/xeus-cpp-env/include
/home/assia/miniconda3/envs/xeus-cpp-env/lib
```

Replace those paths if your local environment is different.

### 4. Mandatory `.env` configuration

`scripts/total.py` loads configuration from the root `.env` file. This file is required.

Current content:

```dotenv
PROJECT_ROOT="project-root"

LIST1=[1,2,3,4,5,6,7,10]
LIST2=[10,11,21,31,40]

CORRESPONDING_DIRECTORY=["example1","example2","example3","example4","example5","example6","example7","example10","exampleMDP10","exampleMDP11","exampleMDP21","exampleMDP31","exampleMDP40"]

CORRESPONDING_EXECUTABLE=["example1.exe","example2.exe","example3.exe","example4_bin.exe","example5.exe","example6.exe","example7.exe","example10.exe","exampleMDP10.exe","exampleMDP11.exe","exampleMDP21.exe","exampleMDP31.exe","exampleMDP40.exe"]

CORRESPONDING_PARAMETERS=["10 0.1 0.2 0.7","10 0.1 0.2 0.7","10 0.05 0.05 0.05 0.05 0.05 0.05 0.2 0.5","example4/IO_example_in1.mcl example4/IO_example_in2.mcl","","","","","","-tmax_sim 5"]
```

#### 4.1. What you may need to replace

- `PROJECT_ROOT`
  - substring searched in the current working directory path
  - recommended value: keep `project-root`

- `LIST1`
  - Markov chain example numbers included in the example archive and generated `CMakeLists.txt`

- `LIST2`
  - MDP example numbers included in the example archive and generated `CMakeLists.txt`

- `CORRESPONDING_DIRECTORY`
  - source directories inside `xpl/`

- `CORRESPONDING_EXECUTABLE`
  - executable names expected inside `build/bin/`

- `CORRESPONDING_PARAMETERS`
  - command-line parameters used to generate `.res` and `.cmd` files

#### 4.2. Important constraints

- `CORRESPONDING_DIRECTORY`, `CORRESPONDING_EXECUTABLE`, and `CORRESPONDING_PARAMETERS` must stay aligned.
- `example4` is special: the executable is expected to be `example4_bin.exe`, not `example4.exe`, to avoid a name collision with the `example4/` folder.
- The current relocation logic in `scripts/total.py` still assumes the repository root is literally named `project-root`:

```python
while(os.path.basename(os.getcwd())!="project-root"):
```

The safest option is to keep the repository folder name as `project-root`. If you rename the repository directory, update that condition in `scripts/total.py`.

### 5. Recommended developer workflow

#### Step 1. Activate the Marmote environment

```powershell
conda activate marmote-use
```

Replace the environment name with your own local one.

#### Step 2. Compile the C++ examples

From the repository root:

```powershell
cmake -S xpl -B build
cmake --build build --config Release
```

Expected executables are generated in `build/bin/`.

Examples:

```powershell
cd build\bin
.\example1.exe
.\exampleMDP10.exe
```

#### Step 3. Generate documentation assets

From the repository root:

```powershell
python scripts/total.py
```

Available options:

```powershell
python scripts/total.py -n
python scripts/total.py -g
```

Meaning:

- `-n`, `--dry_run`
  - validate paths and environment without creating files

- `-g`, `--generate_only`
  - generate example output assets without rebuilding the final example `CMakeLists.txt`

#### Step 4. Build the Sphinx documentation

From `doc/html/`:

```powershell
python -m sphinx -b html source build\html
```

The generated website is written to:

```text
doc/html/build/html/
```

### 6. What `scripts/total.py` produces

The global script chains two categories of work.

#### 6.1. Archive generation and notebook export

Handled by `doc_generateArchives.py`.

Outputs:

- `doc/html/source/instructions/all_nb_colab/all_notebooks_colab.zip`
  - Python notebooks adapted for Colab

- `doc/html/source/instructions/all_nb_cpp/all_notebooks.zip`
  - exported `.cpp` files generated from C++ notebooks
  - also contains an auto-generated `CMakeLists.txt`

- `doc/html/source/instructions/all_ex/all_examples.zip`
  - archive of example C++ source files copied from `media/`

- `doc/html/source/instructions/all_nb_py/all_notebooks.zip`
  - archive of Python notebooks

- `doc/html/source/instructions/all_pyex/all_pythons.zip`
  - archive of `.py` files exported from Python notebooks

#### 6.2. C++ example copy and execution

Handled by `doc_copyExamples.py`.

Outputs:

- selected `.cpp` files copied to `doc/html/source/media/`
- generated `.res` files
- generated `.cmd` files for some Markov chain examples
- updated `doc/html/source/instructions/all_ex/CMakeLists.txt`
- `cmake/CMakeLists.txt` added into `all_examples.zip`

### 7. Important details about the C++ notebooks

#### 7.1. Hidden cells and `.cpp` export

`.cpp` export from `cpptutos/` is handled by:

- `creating_cppFiles(...)`
- `_prepare_notebook_for_cpp_export(...)`
- `_strip_xeus_lines(...)`

Current behavior:

- cells tagged `hide-cell` or `nbsphinx-hidden` are excluded from export
- `#pragma cling ...` lines are removed from exported `.cpp` files
- useful `#include` lines stay in the exported `.cpp`
- a `CMakeLists.txt` file is generated automatically for the C++ archive

#### 7.2. Expected Jupyter kernel

The notebooks in `cpptutos/*.ipynb` currently reference:

```json
"name": "xcpp17"
```

If that kernel does not exist on your machine:

- install Xeus-cling with that kernel name
- or update the notebook metadata to match your local kernel

#### 7.3. `#pragma cling` paths to update

At minimum, search for:

```text
/home/assia/miniconda3/envs/xeus-cpp-env/include
/home/assia/miniconda3/envs/xeus-cpp-env/lib
```

If your environment name or username is different, replace those paths in each relevant C++ notebook.

### 8. CMake: two separate cases

#### Case 1. Compiling the `xpl/` examples

The `xpl/CMakeLists.txt` file does not use `find_package(marmote)`. It relies directly on:

- `CONDA_PREFIX`
- `%CONDA_PREFIX%\Library\include`
- `%CONDA_PREFIX%\Library\lib`

In that case, activating the correct Conda environment before running CMake is usually enough.

#### Case 2. Compiling the `.cpp` files exported from `cpptutos/`

The archive generated in `instructions/all_nb_cpp/all_notebooks.zip` contains a `CMakeLists.txt` based on:

```cmake
find_package(marmote REQUIRED COMPONENTS marmoteCore marmoteLog marmoteMarkovChain marmoteMDP)
```

If CMake cannot find Marmote on Windows, it does not mean Marmote lacks a CMake package. In practice, you often need to tell CMake where the Marmote config files are located:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:\path\to\marmote"
```

or

```powershell
cmake -S . -B build -Dmarmote_DIR="C:\path\to\marmote\lib\cmake\marmote"
```

Replace those paths with the actual Marmote location on your machine.

### 9. Troubleshooting

#### `Pandoc wasn't found`

Cause:

- `pandoc` is not installed or not available in `PATH`

Check:

```powershell
pandoc --version
```

#### `No such kernel named xcpp17`

Cause:

- the expected Xeus-cling kernel is not registered in Jupyter

Check:

```powershell
jupyter kernelspec list
```

#### `download file not readable: ... manual.pdf`

Cause:

- `doc/html/source/cpp_api.rst` references `media/manual.pdf`
- that file is missing

Possible fixes:

- add the missing PDF file
- or remove/fix the link in the `.rst` file

#### The C++ kernel dies while running a notebook

Common cause:

- cells are re-run out of order while the notebook manages pointers, deleted objects, or heavy Marmote objects

Recommended workflow:

- restart the kernel
- run all cells in order
- avoid re-running test cells after cleanup/deallocation cells

#### `total.py` cannot find the expected directories

Check:

- the repository folder is still named `project-root`
- a valid `.env` file exists at the repository root
- `xpl/`, `build/bin/`, `doc/html/source/pytutos/`, `doc/html/source/cpptutos/`, and `doc/html/source/media/` all exist

### 10. Routine maintenance

When you add a new example or lesson, remember to update:

- the notebooks in `pytutos/` or `cpptutos/`
- the relevant `.rst` indexes
  - `doc/html/source/python_examples.rst`
  - `doc/html/source/cpp_examples.rst`
- the `.env` file if a new C++ example must be compiled and executed
- the expected binary names if `build/bin/` changes

Minimum release checklist:

- `python scripts/total.py -n`
- `python scripts/total.py`
- `python -m sphinx -b html source build\html` from `doc/html/`
- verify archives in `doc/html/source/instructions/`
- verify generated HTML pages in `doc/html/build/html/`
