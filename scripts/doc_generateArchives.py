import os
import sys
import shutil
import zipfile
import re  #for regular expression handling
import argparse # to dry-run
from dotenv import load_dotenv

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
	

	# Install condacolab then conda 
	cell_text2 = new_markdown_cell(source="## Install marmote on conda")
	cell_text3 = new_markdown_cell(source="To install `condacolab` ")
	cell_code4 = new_code_cell(source="!pip install -q condacolab\nimport condacolab\ncondacolab.install()\n!conda --version")
	cell_text5 = new_markdown_cell(source="To install `marmote`")
	cell_code6 = new_code_cell(source="!conda install -c marmote -c conda-forge marmote")
	cell_text7 = new_markdown_cell(source="You can now use marmote")
	
	# Verify the effective installation of conda 
	cell_text8 = new_markdown_cell(source="## Verify if marmote is well installed")
	cell_text9 = new_code_cell(source="try:\n" \
	"import marmote\n" \
	"	print('Marmote is well installed')\n"\
	"except Exception as e:\n" \
	"	print('Marmote is not available.')\n" \
	"	print(e)")

	# Follow this fallback installation process if an exception is raised
	cell_text10 = new_markdown_cell(source="If Marmote isn't available, execute the cell below.")
	cell_text11 = new_markdown_cell(source="Then restart the runtime and rerun the installation cells above.")
	cell_text12 = new_code_cell(source=
		"!pip uninstall numpy thinc spacy -y\n"
		"!pip uninstall numpy -y\n"
		"!pip install -qq numpy==1.26.4\n"
		"import numpy as np\n"
		"if int(np.__version__[0]) > 1:\n"
		"    import os\n"
		"    os.kill(os.getpid(), 9)\n"
	)




	# Insert the cells at the beginning of the notebook
	notebook["cells"] = [
		cell_text1,
		cell_text2,
		cell_text3,
		cell_code4,
		cell_text5,
		cell_code6,
		cell_text7,
		cell_text8,
		cell_text9,
		cell_text10,
		cell_text11,
		cell_text12
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
			notebook_path = os.path.join(directory, filename)
			replace_images_colab(notebook_path)
	print(" -Notebooks modified with images for colab")


def creating_cpp_archive(archivename,source,destination):
	"""
	Create cpp archive
	
	:param archivename of the archive
	:param source peth of the source directory
	:param directory where are the files to be archived
	---
	Création d'un zip avec des fichiers cpp et mcl (medias)
	"""
	os.chdir(source)
	archivename=archivename+'.zip'
	myZip = zipfile.ZipFile(archivename, 'w', zipfile.ZIP_DEFLATED) # create archive (empty)
	#adding file
	for filename in os.listdir(os.getcwd()):
		if filename.endswith(".cpp"):
			myZip.write(filename)
		if filename.endswith(".mcl"):
			myZip.write(filename)
	myZip.close()
	print(" -Archive", archivename,"created from:", source)
	dest=os.path.join(destination,archivename)
	shutil.move(archivename,dest)
	print(" -Archive", archivename,"moved in:",dest)	
	



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
	shutil.move(archivename, os.path.join(destination, archivename))
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
	


def creating_pythonFiles(source_directory,archive_name_py,destination_directory_pyex):
	"""
	Create python files from notebooks located in source directory and move the python files  in destination directory
	
	:param source_directory
	:param archive_name_py
	:param destination_directory_pyex
	---
	On crée un zip a partir des notebooks convertis en python, on le met dans le dossier destination 
	"""
	os.chdir(source_directory)
	current_directory=os.getcwd()
	# Find all notebook files in the current directory
	notebook_files = []
	for f in os.listdir(current_directory):
		if f.endswith('.ipynb'):
			notebook_files.append(f)
	python_files = []
	# Convert the notebooks to Python files
	for notebook_file in notebook_files:
		notebook_path = os.path.join(current_directory, notebook_file)
		with open(notebook_path) as f:
			nb = nbformat.read(f, as_version=4)
			exporter = PythonExporter()
			script, _ = exporter.from_notebook_node(nb)
    
		python_file_name = notebook_file.replace('.ipynb', '.py')
		python_file_path = os.path.join(current_directory, python_file_name)
		with open(python_file_path, 'w') as f:
			f.write(script)
		python_files.append(python_file_path)
	print(" -Notebooks converted in python files")
	# Create a ZIP archive for the Python files
	with zipfile.ZipFile(archive_name_py, 'w',zipfile.ZIP_DEFLATED) as zipf:
		for python_file in python_files:
			zipf.write(python_file, os.path.basename(python_file))
		zipf.close()
	print(" -Archive",archive_name_py,"created from:",source_directory)
	# Move the ZIP archive to the destination directory
	shutil.move(archive_name_py, os.path.join(destination_directory_pyex, archive_name_py))
	print(" -Archive",archive_name_py,"moved in:",destination_directory_pyex)
	# Delete the Python files
	for python_file in python_files:
		os.remove(python_file)
	print(" -Python files deleted from: ",source_directory)
	for nb_file in notebook_files:
		os.remove(nb_file)
	print(" -Notebooks files deleted from: ",source_directory)

# assia : ajout list and test
def list_and_test(dir, message):
	print(message,'=',dir,'',end='')
	if os.access(dir,os.F_OK):
		print("[Exists]")
	else:
		print("[Does not exist]")
	

 

