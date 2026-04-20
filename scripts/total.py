import doc_copyExamples as c
import doc_generateArchives as g
import os, json
import sys
import argparse
from dotenv import load_dotenv

load_dotenv()
# récupérer le nom du projet
PROJECT_ROOT = os.getenv("PROJECT_ROOT")
list1= json.loads(os.getenv("LIST1"))
list2= json.loads(os.getenv("LIST2"))
corresponding_parameters= json.loads(os.getenv("CORRESPONDING_PARAMETERS"))
corresponding_executable= json.loads(os.getenv("CORRESPONDING_EXECUTABLE"))
corresponding_directory= json.loads(os.getenv("CORRESPONDING_DIRECTORY"))

def list_and_test(dir, message):
	print(message,'=',dir,'',end='')
	if os.access(dir,os.F_OK):
		print("[Exists]")
	else:
		print("[Does not exist]")


def parse_arguments():
    parser = argparse.ArgumentParser()
    parser.add_argument("-n", "--dry_run", action="store_true")
    parser.add_argument("-g", "--generate_only", action="store_true")
    return parser.parse_args()


def stay_or_change_directory():
    #Test if we are in the correct directory
    # get the absolute path
    root_dir = os.path.abspath(os.path.join(__file__, '..',".."))
    print(f"root {root_dir}")

    # Get the current working directory path
    current_directory=os.getcwd()
    
    """
    We expect to run the script from ../script directory
    in working directory ../doc/html/source/
    If we are not in the direct directory
    position="../doc/html/source/"        
    os.chdir(position)
    ---
    assia
    Si on n'est pas dans un dossier du projet, on quitte
    Si on se trouve dans source, on poursuit le script
    Sinon, on remonte jusqu'à la racine du projet contenue dans le fichier .env
    la variable d'environnement PROJECT_ROOT
    Puis on redirige vers doc/html/source
    """
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
    stay_or_change_directory()
    current_directory=os.getcwd()
    print("Manage working directories and source directories")
    #create and test the directory
    source_d = os.path.join(current_directory,"pytutos")
    notebooks_d = os.path.join(current_directory,"pytutos_colab")
    # Get the directory where cpp are 
    examples_directory = os.path.join(current_directory,'media')

    """On vérifie que les dossiers pytutos et pytutos_collab, 
    on récupère le dossier média avec les fichiers cpp"""

    list_and_test(source_d,"Notebooks source")
    if (not os.path.exists(source_d)):
        print("No source directory for Notebooks. Stop the programm")
        sys.exit()

    list_and_test(examples_directory,"Example cpp source")
    if (not os.path.exists(examples_directory)):
        print("No source directory for cpp files. Stop the programm")
        sys.exit()

    if (not os.path.exists(notebooks_d)):
        os.makedirs(notebooks_d, exist_ok=True)

    # ajout assia : dry-run
    if args.dry_run:
        print("Dry run : not executing archive/copy instructions")
        return

    print("1 - Generate Notebook for colab")
    #copy and rename the file
    g.copy_rename_notebooks(source_d,notebooks_d)
    # go in directory source
    os.chdir(current_directory)
    # Call the function to add cells to all notebooks in the directory
    g.add_cells_condacolab_directory(notebooks_d)
    print("1 - Updating image links Colab")
    # go in directory source
    os.chdir(current_directory)
    # Call the function to replace images in all notebooks in the directory
    g.replace_images_colab_directory(notebooks_d)
    name="all_notebooks_colab"
    g.creating_colab_archive(name,notebooks_d)
    destination=os.path.join(current_directory,"instructions","all_nb_colab")
    if (not os.path.exists(destination)):
        os.makedirs(destination, exist_ok=True)

    ### on crée les notebooks en condacolab a partir des notebook classiques et on cree le zip que lon met ds instructions/all_nb_colab
    ### condacolab = preparer des formats utilisables dans Google Colab, en mettant les entêtes conda permettant lexécution
    ### car Ggl Colab n'intègre pas directement conda


    g.moving_cleaning_colab_archive(name,notebooks_d,destination)


    print("2 - Generate cpp archive")
    # go in directory : "source"
    os.chdir(current_directory)
    # give the name of the archive
    name="all_examples"
    #manage destination directory
    destination=os.path.join(current_directory,"instructions","all_ex")
    # Ensure the destination directory exists
    os.makedirs(destination, exist_ok=True)
    g.creating_cpp_archive(name,examples_directory,destination)
    ### on cree le zip nommé all_examples avec les fichiers cpp 

    print("3 - Generate nb archive")
    # go in directory : "source"
    os.chdir(current_directory)
    #manage destination directory
    destination_directory = os.path.join(current_directory,"instructions","all_nb")
    # Name of the archive
    archive = 'all_notebooks.zip'  
    # Ensure the destination directory exists
    os.makedirs(destination, exist_ok=True)
    g.creating_nb_archive(archive,source_d,destination_directory)



    ### on crée l'archive de notebooks

    print("4 - Generate python Files archive")
    os.chdir(current_directory)
    #copy notebooks
    g.copy_notebooks(source_d,notebooks_d)
    # Destination directory for Python files
    destination_directory_pyex = os.path.join(current_directory,'instructions','all_pyex')  
    # Ensure the destination directory exists
    os.makedirs(destination_directory_pyex, exist_ok=True)
    # Name of the archive for Python files
    archive_name = 'all_pythons.zip'  
    g.creating_pythonFiles(notebooks_d,archive_name,destination_directory_pyex)

    ### on crée l'archive de fichiers pythons

    #delete the temporary pytutos_colab
    os.chdir(current_directory)
    try: 
        os.rmdir(notebooks_d)
        print("Directory",notebooks_d,"deleted")
    except OSError as e:
        # If it fails, inform the user.
        print("Error: %s - %s." % (e.filename, e.strerror))
        
    ### on supprime les fichiers collab

def copyExample(args,corresponding_directory,corresponding_executable,corresponding_parameters,list1,list2):
    """
    We expect to run the script from ../script directory
    in working directory ../doc/html/source/
    to run the file
    python ../../../scripts/doc_copyExamples.py
    If we are not in the direct directory
    position="../doc/html/source/"        
    os.chdir(position)
    """

    """
    We expect to run the script from ../script directory
    in working directory ../doc/html/source/
    If we are not in the direct directory
    position="../doc/html/source/"        
    os.chdir(position)
    ---
    Si on n'est pas dans un dossier du projet, on quitte
    Si on se trouve dans source, on poursuit le script
    Sinon, on remonte jusqu'à la racine project-root (à remplacer) 
    Puis on redirige vers doc/html/source
    """

    #Test if we are in the correct directory
    # get the absolute path
    root_dir = os.path.abspath(os.path.join(__file__, '..',".."))
    print(f"root {root_dir}")

    # Get the current working directory path
    current_directory=os.getcwd()

    stay_or_change_directory()

    # Directories paths
    print("Beginning of script")
    source_dir = os.path.join(os.getcwd(),'..','..','..','xpl')  # Source directory
    destination = os.path.join(os.getcwd(),'media')  # Source/res files destination directory
    cmake_destination = os.path.join(os.getcwd(),'instructions','all_ex')  # CMakeLists destination directory
    source_bin = os.path.join(os.getcwd(),'..','..','..','build','bin') # binary sources
    cmake_file = 'CMakeLists.txt'  # CMakeLists.txt file
    cmake_archive = 'CMakeLists_model.txt'  # Archive file name for CMakeLists.txt

    list_and_test(destination,"Source files destination")
    list_and_test(cmake_destination,"CMakeFile destination")
    list_and_test(source_bin,"Binary")
    list_and_test(source_dir,"Code")
        
    if args.dry_run:
        # Skip the rest
        print("Dry run: not executing copy instructions")
        return

    print("1. Cleaning",os.getcwd())
    c.clean_destination_directory(destination)

    # Change to the source directory
    os.chdir(source_dir)

    print("2. Beginning of copying source file")
    # Copy exampleJ.cpp files
    c.copy_example_files(corresponding_directory, list1, 'example{}.cpp', destination)
    # Copy exampleMDP.cpp files
    c.copy_exampleMDP_files('exampleMDP', list2, 'exampleMDP{}.cpp', destination)

    # Change to the bin directory
    os.chdir(source_bin)
    print("3. Generating res and cmd",os.getcwd())
    c.generate_example_res_files(corresponding_executable, list1, 'example{}',corresponding_parameters)
    c.generate_exampleMDP_res_files('exampleMDP', list2, 'exampleMDP{}')
        
    print("4. Copying res",os.getcwd())
    # Copy exampleJ.res files
    c.copy_example_res_files(corresponding_directory, list1, 'example{}.res',destination)
    # Copy exampleJ.cmd files
    c.copy_example_res_files(corresponding_directory, list1, 'example{}.cmd',destination)
    # Copy exampleMDP.res files
    c.copy_exampleMDP_res_files(list2, 'exampleMDP{}.res',destination)


    if args.generate_only:
        # Skip the CMake part
        print("Generate only: skip step 5 (generate cmake)")
        sys.exit(0)

    print("5. Generate CMake")
    #Copy and modify CMakeLists.txt 
    c.modify_cmake_file(cmake_destination,cmake_file,cmake_archive,list1,list2)



#################""
#main code
print("Bienvenue dans l'outil de génération de documentation marmote.")
args_parse = parse_arguments()
generateArchive(args_parse)
copyExample(args_parse,corresponding_directory,corresponding_executable,corresponding_parameters,list1,list2)

		


	

	
