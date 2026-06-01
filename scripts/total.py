import doc_copyExamples as c
import doc_generateArchives as g
import os, json
import sys
import argparse
from dotenv import load_dotenv

load_dotenv()
# Load the project configuration from the environment.
PROJECT_ROOT = os.getenv("PROJECT_ROOT")
list1= json.loads(os.getenv("LIST1"))
list2= json.loads(os.getenv("LIST2"))
corresponding_parameters= json.loads(os.getenv("CORRESPONDING_PARAMETERS"))
corresponding_executable= json.loads(os.getenv("CORRESPONDING_EXECUTABLE"))
corresponding_directory= json.loads(os.getenv("CORRESPONDING_DIRECTORY"))

def list_and_test(dir, message):
    """
    Print a status line showing whether a path exists.
    """
    print(message,'=',dir,'',end='')
    if os.access(dir,os.F_OK):
        print("[Exists]")
    else:
        print("[Does not exist]")


def parse_arguments():
    """
    Parse command-line arguments.

    :return: argparse.Namespace containing the parsed options

    Available options:
    - ``-n`` / ``--dry_run``: validate paths without generating files
    - ``-g`` / ``--generate_only``: generate example assets without rebuilding CMake
    """
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--dry_run", action="store_true")
    parser.add_argument("-g", "--generate_only", action="store_true")
    return parser.parse_args()


def stay_or_change_directory():
    """
    Ensure that the current working directory is ``doc/html/source``.

    If the script is launched from another location inside the project tree, it
    walks up to the project root and redirects to ``doc/html/source``. If the
    current directory is outside the configured project root, the script stops.
    """
    root_dir = os.path.abspath(os.path.join(__file__, '..',".."))
    print(f"root {root_dir}")

    current_directory=os.getcwd()

    if(PROJECT_ROOT in current_directory):
        if(os.path.basename(current_directory)=="source"):
            print("No directory change needed",os.getcwd())
        else:
            print("Unexpected working directory. Redirecting to the correct location...")
            while(os.path.basename(os.getcwd())!="project-root"):
                print("Moving up...",os.getcwd())
                os.chdir("..")
            
            path = os.path.join("doc","html","source")
            os.chdir(path)
            print("Redirection completed successfully : ",os.getcwd())
    else:
        print("Wrong directory. Program stops")
        sys.exit()
		
def generateArchive(args):
    """
    Generate all documentation archives.

    :param args: argparse.Namespace parsed command-line options

    The function validates the source directories, prepares temporary working
    directories, and then generates:
    - Colab notebooks and their archive
    - C++ files exported from C++ notebooks and their archive
    - the archive of example C++ source files
    - the archive of Python notebooks
    - the archive of Python files exported from notebooks

    If ``dry_run`` is enabled, the function stops after the path checks.
    """

    stay_or_change_directory()
    current_directory=os.getcwd()
    print("Manage working directories and source directories")
    # Define the main source and temporary directories.
    source_d_py = os.path.join(current_directory,"pytutos")
    source_d_cpp = os.path.join(current_directory,"cpptutos")
    notebooks_d = os.path.join(current_directory,"pytutos_colab")
    # Generated example source files are stored in media.
    examples_directory = os.path.join(current_directory,'media')

    list_and_test(source_d_py,"Python notebooks source")
    if (not os.path.exists(source_d_py)):
        print("No source directory for Python notebooks. Program stops.")
        sys.exit()

    list_and_test(source_d_cpp,"C++ notebooks source")
    if (not os.path.exists(source_d_cpp)):
        print("No source directory for C++ notebooks. Program stops.")
        sys.exit()

    list_and_test(examples_directory,"Example cpp source")
    if (not os.path.exists(examples_directory)):
        print("No source directory for example C++ files. Program stops.")
        sys.exit()

    if (not os.path.exists(notebooks_d)):
        os.makedirs(notebooks_d, exist_ok=True)

    if args.dry_run:
        print("Dry run: archive and copy steps skipped.")
        return

    print("1 - Generate Notebook for colab")
    g.copy_rename_notebooks(source_d_py,notebooks_d)
    os.chdir(current_directory)
    g.add_cells_condacolab_directory(notebooks_d)
    print("1 - Updating image links Colab")
    os.chdir(current_directory)
    g.replace_images_colab_directory(notebooks_d)
    name="all_notebooks_colab"
    g.creating_colab_archive(name,notebooks_d)
    destination=os.path.join(current_directory,"instructions","all_nb_colab")
    if (not os.path.exists(destination)):
        os.makedirs(destination, exist_ok=True)

    g.moving_cleaning_colab_archive(name,notebooks_d,destination)


    print("2 - Generate cpp files archive from cpp notebooks")
    os.chdir(current_directory)
    destination_directory = os.path.join(current_directory,"instructions","all_nb_cpp")
    archive = 'all_notebooks.zip'  
    os.makedirs(destination_directory, exist_ok=True)
    g.creating_cppFiles(source_d_cpp,archive,destination_directory)


    print("3 - Generate cpp archive")
    os.chdir(current_directory)
    name="all_examples"
    destination=os.path.join(current_directory,"instructions","all_ex")
    os.makedirs(destination, exist_ok=True)
    g.creating_cpp_archive(name,examples_directory,destination)


    print("4 - Generate nb archive for python")
    os.chdir(current_directory)
    destination_directory = os.path.join(current_directory,"instructions","all_nb_py")
    archive = 'all_notebooks.zip'  
    os.makedirs(destination_directory, exist_ok=True)
    g.creating_nb_archive(archive,source_d_py,destination_directory)


    print("5 - Generate python Files archive")
    os.chdir(current_directory)
    g.copy_notebooks(source_d_py,notebooks_d)
    destination_directory_pyex = os.path.join(current_directory,'instructions','all_pyex')  
    os.makedirs(destination_directory_pyex, exist_ok=True)
    archive_name = 'all_pythons.zip'  
    g.creating_pythonFiles(notebooks_d,archive_name,destination_directory_pyex)


    # Remove the temporary Colab notebook directory when it is empty.
    os.chdir(current_directory)
    try: 
        os.rmdir(notebooks_d)
        print("Directory",notebooks_d,"deleted")
    except OSError as e:
        print("Error: %s - %s." % (e.filename, e.strerror))

def copyExample(args,corresponding_directory,corresponding_executable,corresponding_parameters,list1,list2):
    """
    Copy example C++ files, generate result files, and rebuild the CMake archive.

    :param args: argparse.Namespace parsed command-line options
    :param corresponding_directory: list of source example directories
    :param corresponding_executable: executable names associated with examples
    :param corresponding_parameters: execution parameters for the examples
    :param list1: selected standard example identifiers
    :param list2: selected MDP example identifiers

    The function locates the required directories (``xpl``, ``media``,
    ``build/bin``, ``instructions/all_ex``), then:
    - copies the selected C++ source files
    - generates the associated ``.res`` and ``.cmd`` files
    - copies the generated files into ``media``
    - updates ``CMakeLists.txt`` from the model and the selected example lists

    If ``dry_run`` is enabled, the function stops after the path checks. If
    ``generate_only`` is enabled, the example assets are generated but the
    CMake file is not rebuilt.
    """

    root_dir = os.path.abspath(os.path.join(__file__, '..',".."))
    print(f"root {root_dir}")

    current_directory=os.getcwd()

    stay_or_change_directory()

    print("Beginning of script")
    source_dir = os.path.join(os.getcwd(),'..','..','..','xpl')
    destination = os.path.join(os.getcwd(),'media')
    cmake_destination = os.path.join(os.getcwd(),'instructions','all_ex')
    source_bin = os.path.join(os.getcwd(),'..','..','..','build','bin')
    cmake_file = 'CMakeLists.txt'
    cmake_archive = 'CMakeLists_model.txt'

    list_and_test(destination,"Source files destination")
    list_and_test(cmake_destination,"CMakeFile destination")
    list_and_test(source_bin,"Binary")
    list_and_test(source_dir,"Code")
        
    if args.dry_run:
        print("Dry run: not executing copy instructions")
        return

    print("1. Cleaning",os.getcwd())
    c.clean_destination_directory(destination)

    os.chdir(source_dir)

    print("2. Beginning of copying source file")
    c.copy_example_files(corresponding_directory, list1, 'example{}.cpp', destination)
    c.copy_exampleMDP_files('exampleMDP', list2, 'exampleMDP{}.cpp', destination)

    os.chdir(source_bin)
    print("3. Generating res and cmd",os.getcwd())
    c.generate_example_res_files(corresponding_executable, list1, 'example{}',corresponding_parameters)
    c.generate_exampleMDP_res_files('exampleMDP', list2, 'exampleMDP{}')
        
    print("4. Copying res",os.getcwd())
    c.copy_example_res_files(corresponding_directory, list1, 'example{}.res',destination)
    c.copy_example_res_files(corresponding_directory, list1, 'example{}.cmd',destination)
    c.copy_exampleMDP_res_files(list2, 'exampleMDP{}.res',destination)


    if args.generate_only:
        print("Generate only: skip step 5 (generate cmake)")
        sys.exit(0)

    print("5. Generate CMake")
    c.modify_cmake_file(cmake_destination,cmake_file,cmake_archive,list1,list2)


print("Welcome to the Marmote documentation generation tool.")
args_parse = parse_arguments()
generateArchive(args_parse)
copyExample(args_parse,corresponding_directory,corresponding_executable,corresponding_parameters,list1,list2)

		


	

	
