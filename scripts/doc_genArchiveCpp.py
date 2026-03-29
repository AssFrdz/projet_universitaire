import os
import sys
import shutil
import zipfile
import re  #for regular expression handling
import argparse # to dry-run

import nbformat
from nbformat.v4 import new_code_cell, new_markdown_cell
# Reads and writes Jupyter notebook files.
# The nbformat library is a Python library used to work with Jupyter notebook files in various formats.
# It provides functions for reading, writing, and manipulating Jupyter notebooks.
from nbconvert import PythonExporter # Converts Jupyter notebooks to Python script files.

"""
@author = MB and PL and EH and MS
@version = 1.0.3
@warning To be executed in the script's source directory  
@brief = Copying notebooks from the pytutos directory, rename them, adding cells 
         to manipulate marmote packages on Google Colab, create an archive. 
"""
	
def copy_rename_notebooks(source_directory,dest_directory):
	"""
	copy all the notebooks from source_directory in a directory dest_directory and rename them for be transformed in colab notebooks
	
	:param source_directory directory in which norebooks are
	:param dest_directory destination directory
	---
	on copie les notebooks avant de les modifier et on les convertit en collab
	"""
	#go to source_directory aka ../pytutos
	os.chdir(source_directory)
	#list and copy files
	for filename in os.listdir(os.getcwd()):
		if filename.endswith(".ipynb"):
			name=filename.rstrip(".ipynb")
			name=name+"_colab.ipynb"
			#print(name) 
			dest=os.path.join(dest_directory,name)
			shutil.copy(filename,dest)
	print(" -Notebooks copied and renamed in:",dest_directory)
	


def add_cells_condacolab_directory(directory):
	"""
	Modifies the whole notebooks (for condacolab) of a directory
	
	:param directory where are the notebooks
	---
	On fait appel à la fonction d'ajout de cellule conda colab pour 
	tout les fichiers notebook d'un dossier
	"""
	# Iterate through all files in the directory
	for filename in os.listdir(directory):
		if filename.endswith(".ipynb"):  # Check if the file is a notebook
			notebook_path = os.path.join(directory, filename)
			add_cells_condacolab(notebook_path)
	print(" -Notebooks modified for colab")
	
	
	

def add_cells_condacolab(notebook_path):
	"""
	Modifies a single notebook for condacolab
	
	:param notebook_path the path of the notebook
	---
	a partir du chemin du notebook on crée les cellules et on met les commandes conda colab dans les cellules
	"""
	# Load the existing notebook
	with open(notebook_path, "r", encoding="utf-8") as f:
		notebook = nbformat.read(f, as_version=4)
	
	#read the title 
	cellule=notebook["cells"]
	cellule=cellule[0]
	#print("cellule",cellule,"type",type(cellule))
	titre=cellule['source']
	titre="# Colab worksheet for "+titre[2:]
	
	# Create the cells
	cell_text1 = new_markdown_cell(source=titre)
	cell_text2 = new_markdown_cell(source="## Install marmote on conda")
	cell_text3 = new_markdown_cell(source="To install `condacolab` ")
	cell_code4 = new_code_cell(source="!pip install -q condacolab\nimport condacolab\ncondacolab.install()\n!conda --version")
	cell_text5 = new_markdown_cell(source="To install `marmote`")
	cell_code6 = new_code_cell(source="!conda install -c marmote -c conda-forge marmote")
	cell_text7 = new_markdown_cell(source="You can now use marmote")
	
	# Insert the cells at the beginning of the notebook
	notebook["cells"] = [
		cell_text1,
		cell_text2,
		cell_text3,
		cell_code4,
		cell_text5,
		cell_code6,
		cell_text7,
	] + notebook["cells"]
	
	# Save the modified notebook
	with open(notebook_path, "w", encoding="utf-8") as f:
		nbformat.write(notebook, f)



def creating_colab_archive(archivename,directory):
	"""
	create the colab archive
	
	:param archivename of the archive
	:param directory where are the files to be archived
	---
	on cree un fichier zip avec les fichiers de collab
	"""
	#go into directory where are the files
	os.chdir(directory)
	archivename=archivename+'.zip'
	myZip = zipfile.ZipFile('./'+archivename, 'w', zipfile.ZIP_DEFLATED) # create archive (empty)
	#adding file
	for filename in os.listdir(os.getcwd()):
		if filename.endswith(".ipynb"):
			 myZip.write('./'+filename)
	myZip.close()
	print(" -Archive colab ", archivename ,"created from:",directory)	

	
def moving_cleaning_colab_archive(archivename,position,dest_directory):
	"""
	move the colab archive and delete old files
	
	:param archivename of the archive
	:param position path of the directory to be placed to launch the system
	:param directory where are the files to be archived
	---
	 deplace les fichiers colab de l'archive vers la destination
	"""
	# go to the correct directory
	os.chdir(position)	
	#copy archive in the right place
	archivename=archivename+'.zip'
	dest=os.path.join(dest_directory,archivename)
	shutil.move(archivename,dest)
	print(" -Archive", archivename ,"moved in:",dest)
	for filename in os.listdir(os.getcwd()):
		if filename.endswith(".ipynb"):
			os.remove(filename)
			#print(filename+" removed")
	print(" -All colab.ipynb files removed")


def replace_images_colab(notebook_path):
	"""
	Replace local images with URLs in marmote site in a Colab notebook.
	
	:param notebook_path:  notebook path
	---
	protection s'il ne s'agit pas de condacolab, pas de remplacement possible,sinon on utilise les img de l'inria
	"""
	
	url_marmote='https://marmote.gitlabpages.inria.fr/marmote/_images/'
	
	if not notebook_path.endswith("_colab.ipynb"):
		print("Error in replace images colab: not a colab notebook")
		return
	

	with open(notebook_path, "r", encoding="utf-8") as f:
		notebook = nbformat.read(f, as_version=4)

	replacements = False
	for cell in notebook["cells"]:
		if cell["cell_type"] == "markdown" : #cellule de type markdwon
			image_pattern = r'<img\s+[^>]*src="[^"]*"[^>]*>'
			if re.search(image_pattern, cell["source"]) :
				match = re.search(r'<img\s+[^>]*src="[./]*([^"]*)"[^>]*>',cell["source"])
				if match :
					local_img=match.group(1)
				remote_img = url_marmote+local_img
				cell["source"] = cell["source"].replace(f"<img src=\"./{local_img}\">",f"<img src=\"{remote_img}\">")
				replacements = True
				print("Cette cellule contient une image",cell["source"])
	if replacements:
		with open(notebook_path, "w", encoding="utf-8") as f:
			nbformat.write(notebook, f)



def replace_images_colab_directory(directory):
	"""
	Modifies the whole notebooks (for condacolab) of a directory
	
	:param directory where are the notebooks
	---
	mêmes actions que précédemment, pr tout un dossier cette fois
	"""
	
	# Iterate through all files in the directory
	for filename in os.listdir(directory):
		if filename.endswith(".ipynb"): 
			notebook_path = os.path.join(notebooks_d, filename)
			replace_images_colab(notebook_path)
	print(" -Notebooks modified with images for colab")





def creating_nb_archive(archivename,source,destination): 
	"""
	Create an archive of notebook files in source and put the archive in destination.
	An archive includes ipynb files as well as images
	
	:param archivename of the archive
	:param source peth of the source directory
	:param directory where are the files to be archived
	"""

	""" 
	Configuring Image extension
	---
	crée une archive de notebooks à partir d'une source vers une destination avec des images cette fois
	"""
	image_extensions=('.png','jpg')  # more general ('.png', '.jpg', '.jpeg', '.gif', '.svg')

	os.chdir(source)
	# Find all notebook files and images in the current directory
	files_to_archive = [] 
	current_directory = os.getcwd()

	for f in os.listdir(current_directory):
		if f.endswith('.ipynb') or f.lower().endswith(image_extensions):
			files_to_archive.append(f)
	# Create a ZIP archive
	with zipfile.ZipFile(archivename, 'w', zipfile.ZIP_DEFLATED) as zipf:
		for file in files_to_archive:
			zipf.write(file)
		zipf.close()
	print(" -Archive", archivename, "created from:",source)
	# Move the ZIP archive to the destination directory
	shutil.move(archivename, os.path.join(destination_directory, archive))
	print(" -Archive", archivename,"moved in:",destination)
	

	
def copy_notebooks(source_directory,dest_directory):
	"""
	Copy notebooks from source to destination
	
	:param source_directory
	:param dest_directory
	---
	on copie les notebook d'un dossier vers un autre chemin
	"""
	os.chdir(source_directory)
	#list and copy files
	for filename in os.listdir(os.getcwd()):
		if filename.endswith(".ipynb"): 
			dest=os.path.join(dest_directory,filename)
			shutil.copy(filename,dest)
	print(" -Notebooks copied from:",source_directory)
	print(" -Notebooks copied in:",dest_directory)
	


def creating_cppFiles(source_directory, archive_name_cpp, destination_directory_cppex):
    """
    Convert all .ipynb notebooks in source_directory into .cpp files
    using read_write_from_ipynb_to_cpp(), compress them into a ZIP,
    move the ZIP to destination_directory_cppex, and keep the .ipynb files.
    """

    # Se placer dans le dossier source
    os.chdir(source_directory)
    current_directory = os.getcwd()

    # Récupérer tous les notebooks
    notebook_files = [
        f for f in os.listdir(current_directory)
        if f.endswith('.ipynb')
    ]

    if not notebook_files:
        print("Aucun notebook trouvé dans :", source_directory)
        return

    print(" -Notebooks trouvés :", notebook_files)

    # Liste des fichiers .cpp générés
    cpp_files = []

    # Conversion en .cpp
    for nb_file in notebook_files:
        nb_path = os.path.join(current_directory, nb_file)

        # Appel à TA fonction
        read_write_from_ipynb_to_cpp(nb_path, current_directory)

        # Nom du fichier généré
        cpp_name = nb_file.replace(".ipynb", ".cpp")
        cpp_files.append(os.path.join(current_directory, cpp_name))

    print(" -Tous les fichiers .cpp ont été générés.")

    # Création du ZIP
    with zipfile.ZipFile(archive_name_cpp, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for cpp_file in cpp_files:
            zipf.write(cpp_file, os.path.basename(cpp_file))

    print(" -Archive ZIP créée :", archive_name_cpp)

    # Déplacement du ZIP vers le dossier destination
    shutil.move(archive_name_cpp, os.path.join(destination_directory_cppex, archive_name_cpp))
    print(" -Archive déplacée dans :", destination_directory_cppex)

    # Suppression des .cpp temporaires
    for cpp_file in cpp_files:
        os.remove(cpp_file)

    print(" -Fichiers .cpp supprimés du dossier source.")

def read_write_from_ipynb_to_cpp(file, destination_directory):
    code = ""

    # Lecture du notebook
    with open(file, "r", encoding="utf-8") as f:
        nb = nbformat.read(f, as_version=4)

    # Parcours des cellules
    for i, cell in enumerate(nb.cells):
        if cell.cell_type == 'code':
            if "include" in cell.source:
                code += cell.source + """
int main(int argc, char** argv)
{
"""
            elif "pragma" in cell.source:
                continue
            else:
                code += cell.source + "\n"

    # Fermeture du main
    code += "}\n"

    # Création du fichier .cpp
    new_file_name = os.path.splitext(os.path.basename(file))[0] + ".cpp"
    path = os.path.join(destination_directory, new_file_name)

    with open(path, "w", encoding="utf-8") as f:
        f.write(code)

    print(f"{new_file_name} file created successfully")
	
                
			
			
	

	

#################""
#main code

"""
assia
We allow the script to run in dry‑run mode,
so it can verify whether the required files are correctly detected.
We also warn the user about the directory location constraints.
"""
args = sys.argv
generate_only = False
dry_run = False
if ( len(args) > 1 ):
	if ( args[1] == "-h" ) or ( args[1] == "--help" ):
		print(f"Usage: {args[0]} [-h|--help] [-n|--dry_run]")
		print("\tThis script is to be stored in .../scripts\n\tand is to be run from directory .../doc/html/source")
		sys.exit(0)
	elif ( args[1] == "-n" ) or ( args[1] == "--dry_run" ):
		dry_run = True

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

if("project-root" in current_directory):
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
	
current_directory=os.getcwd()
print("Manage working directories and source directories")
#create and test the directory
source_d = os.path.join(current_directory,"cpptutos")
notebooks_d = os.path.join(current_directory,"cpptutos_colab")

### on verifie que les dossiers pytutos et pytutos_collab, on recup le dossier média avc les fichiers cpp


if (not os.path.exists(source_d)):
	print("No source directory for Notebooks. Stop the programm")
	sys.exit()


if (not os.path.exists(notebooks_d)):
	os.makedirs(notebooks_d, exist_ok=True)

# ajout assia : dry-run
if dry_run:
	print("Dry run : not executing archive/copy instructions")

print("1 - Generate Notebook for colab")
#copy and rename the file
copy_rename_notebooks(source_d,notebooks_d)
# go in directory source
os.chdir(current_directory)
# Call the function to add cells to all notebooks in the directory
add_cells_condacolab_directory(notebooks_d)
print("1 - Updating image links Colab")
# go in directory source
os.chdir(current_directory)
# Call the function to replace images in all notebooks in the directory
replace_images_colab_directory(notebooks_d)
name="all_notebooks_colab"
creating_colab_archive(name,notebooks_d)
destination=os.path.join(current_directory,"instructions","all_nb_cpp_colab")
if (not os.path.exists(destination)):
	os.makedirs(destination, exist_ok=True)

### on crée les notebooks en condacolab a partir des notebook classiques et on cree le zip que lon met ds instructions/all_nb_colab
### condacolab = preparer des formats utilisables dans Google Colab, en mettant les entêtes conda permettant lexécution
### car Ggl Colab n'intègre pas directement conda


moving_cleaning_colab_archive(name,notebooks_d,destination)

print("3 - Generate cpp nb archive")
# go in directory : "source"
os.chdir(current_directory)
#manage destination directory
destination_directory = os.path.join(current_directory,"instructions","all_nb_cpp")
# Name of the archive
archive = 'all_notebooks.zip'  
# Ensure the destination directory exists
os.makedirs(destination, exist_ok=True)
creating_nb_archive(archive,source_d,destination_directory)



### on crée l'archive de notebooks

print("4 - Generate cpp Files archive")
os.chdir(current_directory)
#copy notebooks
copy_notebooks(source_d,notebooks_d)
# Destination directory for Python files
destination_directory_cppex = os.path.join(current_directory,'instructions','all_cppex')  
# Ensure the destination directory exists
os.makedirs(destination_directory_cppex, exist_ok=True)
# Name of the archive for Python files
archive_name = 'all_cpps.zip'  
creating_cppFiles(notebooks_d,archive_name,destination_directory_cppex)

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
	
