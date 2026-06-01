import os
import json
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
from nbconvert import PythonExporter, ScriptExporter # Converts Jupyter notebooks to Python/C++ script files.

"""
Utilities for preparing notebook-based documentation assets.

This module copies notebooks, adapts them for Google Colab, generates zip
archives, and exports notebook content to Python and C++ source files.
"""
	
def copy_rename_notebooks(source_directory,dest_directory):
	"""
	Copy notebooks from ``source_directory`` to ``dest_directory`` and rename
	them with a ``_colab`` suffix.

	:param source_directory: source notebook directory
	:param dest_directory: destination directory
	"""
	os.chdir(source_directory)
	for filename in os.listdir(os.getcwd()):
		if filename.endswith(".ipynb"):
			name=filename.rstrip(".ipynb")
			name=name+"_colab.ipynb"
			dest=os.path.join(dest_directory,name)
			shutil.copy(filename,dest)
	print(" -Notebooks copied and renamed in:",dest_directory)
	


def add_cells_condacolab_directory(directory):
	"""
	Apply the Colab preparation step to every notebook in a directory.

	:param directory: directory containing the notebooks to update
	"""
	for filename in os.listdir(directory):
		if filename.endswith(".ipynb"):
			notebook_path = os.path.join(directory, filename)
			add_cells_condacolab(notebook_path)
	print(" -Notebooks modified for colab")
	
	
	

def add_cells_condacolab(notebook_path):
	"""
	Insert the Colab-specific setup cells at the beginning of a notebook.

	:param notebook_path: path to the notebook file
	"""
	with open(notebook_path, "r", encoding="utf-8") as f:
		notebook = nbformat.read(f, as_version=4)
	
	cellule=notebook["cells"][0]
	titre=cellule['source']
	titre="# Colab worksheet for "+titre[2:]
	
	cell_text1 = new_markdown_cell(source=titre)
	
	# Install condacolab first, then install Marmote with conda.
	cell_text2 = new_markdown_cell(source="## Install marmote on conda")
	cell_text3 = new_markdown_cell(source="To install `condacolab` ")
	cell_code4 = new_code_cell(source="!pip install -q condacolab\nimport condacolab\ncondacolab.install()\n!conda --version")
	cell_text5 = new_markdown_cell(source="To install `marmote`")
	cell_code6 = new_code_cell(source="!conda install -c marmote -c conda-forge marmote")
	cell_text7 = new_markdown_cell(source="You can now use marmote")
	
	# Add a lightweight installation check.
	cell_text8 = new_markdown_cell(source="## The following cell allows you to test the Marmote installation")
	cell_text9 = new_code_cell(source="""
	# Comment out the line below if you no longer want the installation test
	!conda list marmote
	# Uncomment the line below to find out your python's version
	#!python --version
	""")

	# Provide the runtime fallback instructions used on Colab.
	cell_text10 = new_markdown_cell(source="""## There are two ways to run Marmote

	a) Use the Runtime version 2025.07. To select the correct runtime : Go to Tab “Runtime”, then tab “Change Runtime type”, then tab “Runtime version” then select “2025.07”

	b) Include site_packages_path to sys path with the following instructions

	If Marmote isn't available, execute the cell below.""")
	cell_text11 = new_code_cell(source="""import sys
	site_packages_path = '/usr/local/lib/python3.11/site-packages'
	if site_packages_path not in sys.path:
		sys.path.insert(0, site_packages_path)
	print(f"sys.path updated: {site_packages_path in sys.path}")

	# the value of site_packages_path is deduced from a call to conda list
	# that gives you the path to the site-package associated with your python"""
	)




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
		cell_text11
	] + notebook["cells"]
	
	with open(notebook_path, "w", encoding="utf-8") as f:
		nbformat.write(notebook, f)

def creating_colab_archive(archivename,directory):
	"""
	Create the zip archive containing the Colab notebooks.

	:param archivename: base archive name, without extension
	:param directory: directory containing the Colab notebooks
	"""
	os.chdir(directory)
	archivename=archivename+'.zip'
	myZip = zipfile.ZipFile('./'+archivename, 'w', zipfile.ZIP_DEFLATED)
	for filename in os.listdir(os.getcwd()):
		if filename.endswith(".ipynb"):
			 myZip.write('./'+filename)
	myZip.close()
	print(" -Archive colab ", archivename ,"created from:",directory)	

	
def moving_cleaning_colab_archive(archivename,position,dest_directory):
	"""
	Move the Colab archive and delete the temporary Colab notebooks.

	:param archivename: base archive name, without extension
	:param position: directory containing the temporary notebooks
	:param dest_directory: archive destination directory
	"""
	os.chdir(position)	
	archivename=archivename+'.zip'
	dest=os.path.join(dest_directory,archivename)
	shutil.move(archivename,dest)
	print(" -Archive", archivename ,"moved in:",dest)
	for filename in os.listdir(os.getcwd()):
		if filename.endswith(".ipynb"):
			os.remove(filename)
	print(" -All colab.ipynb files removed")


def replace_images_colab(notebook_path):
	"""
	Replace local images with URLs in marmote site in a Colab notebook.
	
	:param notebook_path:  notebook path
	"""

	url_marmote='https://marmote.gitlabpages.inria.fr/marmote/_images/'
	
	if not notebook_path.endswith("_colab.ipynb"):
		print("Error in replace images colab: not a colab notebook")
		return
	
	with open(notebook_path, "r", encoding="utf-8") as f:
		notebook = nbformat.read(f, as_version=4)

	replacements = False
	for cell in notebook["cells"]:
		if cell["cell_type"] == "markdown" :
			image_pattern = r'<img\s+[^>]*src="[^"]*"[^>]*>'
			if re.search(image_pattern, cell["source"]) :
				match = re.search(r'<img\s+[^>]*src="[./]*([^"]*)"[^>]*>',cell["source"])
				if match :
					local_img=match.group(1)
				remote_img = url_marmote+local_img
				cell["source"] = cell["source"].replace(f"<img src=\"./{local_img}\">",f"<img src=\"{remote_img}\">")
				replacements = True
				print("This cell contains an image",cell["source"])
	if replacements:
		with open(notebook_path, "w", encoding="utf-8") as f:
			nbformat.write(notebook, f)



def replace_images_colab_directory(directory):
	"""
	Replace local image paths with public URLs in every notebook of a directory.

	:param directory: notebook directory
	"""
	
	for filename in os.listdir(directory):
		if filename.endswith(".ipynb"): 
			notebook_path = os.path.join(directory, filename)
			replace_images_colab(notebook_path)
	print(" -Notebooks modified with images for colab")


def creating_cpp_archive(archivename,source,destination):
	"""
	Create an archive from the example C++ and MCL files stored in ``media``.

	:param archivename: base archive name, without extension
	:param source: source directory
	:param destination: destination directory
	"""
	os.chdir(source)
	archivename=archivename+'.zip'
	myZip = zipfile.ZipFile(archivename, 'w', zipfile.ZIP_DEFLATED)
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
	Create an archive of notebooks and image assets.

	:param archivename: archive file name
	:param source: source directory
	:param destination: destination directory
	"""
	image_extensions=('.png','jpg')  # more general ('.png', '.jpg', '.jpeg', '.gif', '.svg')

	os.chdir(source)
	files_to_archive = [] 
	current_directory = os.getcwd()

	for f in os.listdir(current_directory):
		if f.endswith('.ipynb') or f.lower().endswith(image_extensions):
			files_to_archive.append(f)
	with zipfile.ZipFile(archivename, 'w', zipfile.ZIP_DEFLATED) as zipf:
		for file in files_to_archive:
			zipf.write(file)
		zipf.close()
	print(" -Archive", archivename, "created from:",source)
	shutil.move(archivename, os.path.join(destination, archivename))
	print(" -Archive", archivename,"moved in:",destination)
	

	
def copy_notebooks(source_directory,dest_directory):
	"""
	Copy all notebooks from one directory to another.

	:param source_directory: source notebook directory
	:param dest_directory: destination directory
	"""
	os.chdir(source_directory)
	for filename in os.listdir(os.getcwd()):
		if filename.endswith(".ipynb"): 
			dest=os.path.join(dest_directory,filename)
			shutil.copy(filename,dest)
	print(" -Notebooks copied from:",source_directory)
	print(" -Notebooks copied in:",dest_directory)
	


def creating_pythonFiles(source_directory,archive_name_py,destination_directory_pyex):
	"""
	Export notebooks to Python files, archive them, and move the archive.

	:param source_directory: source notebook directory
	:param archive_name_py: Python archive name
	:param destination_directory_pyex: destination directory for the archive
	"""
	os.chdir(source_directory)
	current_directory=os.getcwd()
	notebook_files = []
	for f in os.listdir(current_directory):
		if f.endswith('.ipynb'):
			notebook_files.append(f)
	python_files = []
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
	with zipfile.ZipFile(archive_name_py, 'w',zipfile.ZIP_DEFLATED) as zipf:
		for python_file in python_files:
			zipf.write(python_file, os.path.basename(python_file))
		zipf.close()
	print(" -Archive",archive_name_py,"created from:",source_directory)
	shutil.move(archive_name_py, os.path.join(destination_directory_pyex, archive_name_py))
	print(" -Archive",archive_name_py,"moved in:",destination_directory_pyex)
	for python_file in python_files:
		os.remove(python_file)
	print(" -Python files deleted from: ",source_directory)
	for nb_file in notebook_files:
		os.remove(nb_file)
	print(" -Notebooks files deleted from: ",source_directory)




def _strip_xeus_lines(source):
	"""
	Remove Xeus/Cling-specific directives from a notebook code cell while keeping
	the standard C++ and Marmote includes needed in the exported .cpp file.

	:param source source code of one notebook cell
	:return filtered source code
	"""
	if isinstance(source, list):
		source = "".join(source)

	lines = source.splitlines()
	filtered_lines = []
	for line in lines:
		stripped = line.strip()
		if stripped.startswith("#pragma cling"):
			continue
		if stripped in {"#ifdef _WIN32", "#else", "#endif"}:
			continue
		if "Xeus-cling" in stripped:
			continue
		if stripped.startswith("//") and "shared libraries" in stripped:
			continue
		filtered_lines.append(line)

	cleaned_lines = []
	previous_blank = False
	for line in filtered_lines:
		is_blank = (line.strip() == "")
		if is_blank and previous_blank:
			continue
		cleaned_lines.append(line)
		previous_blank = is_blank

	while cleaned_lines and cleaned_lines[0].strip() == "":
		cleaned_lines.pop(0)
	while cleaned_lines and cleaned_lines[-1].strip() == "":
		cleaned_lines.pop()

	if not cleaned_lines:
		return ""

	return "\n".join(cleaned_lines) + "\n"


def _cell_is_hidden_for_export(cell):
	"""
	Return True when a notebook cell is tagged as hidden and should therefore
	be excluded from the exported C++ file.

	:param cell notebook cell
	:return bool
	"""
	tags = cell.get("metadata", {}).get("tags", [])
	return ("hide-cell" in tags) or ("nbsphinx-hidden" in tags)







def _prepare_notebook_for_cpp_export(nb):
	"""
	Create an in-memory notebook that only contains the code cells to be exported
	as C++, after filtering out Xeus-specific setup directives.

	:param nb notebook object
	:return notebook object suitable for ScriptExporter
	"""
	export_nb = nbformat.v4.new_notebook(metadata=nb.metadata)
	export_nb.cells = []

	for cell in nb.cells:
		if cell.cell_type != "code":
			continue
		if _cell_is_hidden_for_export(cell):
			continue
		

		cpp_source = _strip_xeus_lines(cell.source)
		if cpp_source.strip() == "":
			continue

		export_nb.cells.append(new_code_cell(source=cpp_source))

	return export_nb


def _build_cpp_notebooks_cmakelists(cpp_files):
	"""
	Build the content of a CMakeLists.txt file suited for the C++ files exported
	from cpptutos notebooks.

	:param cpp_files list of absolute paths to generated .cpp files
	:return string content of a CMakeLists.txt file
	"""
	source_names = [os.path.basename(path) for path in cpp_files]
	lines = [
		"# This CMake file is automatically generated for the C++ files",
		"# exported from the Marmote cpptutos notebooks.",
		"#",
		"cmake_minimum_required(VERSION 3.17)",
		"",
		"project(marmote_cpp_notebooks)",
		"",
		"set(CMAKE_CXX_STANDARD 20)",
		"set(CMAKE_CXX_STANDARD_REQUIRED ON)",
		"",
		"find_package(marmote REQUIRED COMPONENTS marmoteCore marmoteLog marmoteMarkovChain marmoteMDP)",
		"",
		"set(MARMOTE_CPP_NOTEBOOK_SOURCES",
	]

	for source_name in source_names:
		lines.append(f"    {source_name}")

	lines.extend([
		")",
		"",
		"foreach(PROJ_MAIN IN LISTS MARMOTE_CPP_NOTEBOOK_SOURCES)",
		'    get_filename_component(PROJ_N "${PROJ_MAIN}" NAME_WE)',
		'    message("Building project ${PROJ_N} from source ${PROJ_MAIN}")',
		'    add_executable(${PROJ_N} ${PROJ_MAIN})',
		'    target_link_libraries(${PROJ_N} PUBLIC marmoteCore marmoteLog marmoteMarkovChain marmoteMDP)',
		"endforeach()",
		"",
	])

	return "\n".join(lines)


def creating_cppFiles(source_directory, archive_name_cpp, destination_directory_cppex):
	"""
	Convert notebooks in source_directory to C++ files with nbconvert, exclude
	Xeus-specific directives, archive the generated .cpp files, and move the
	archive to destination_directory_cppex.
	:param source_directory
	:param archive_name_py
	:param destination_directory_cppex
	"""
	notebook_files = [
		f for f in os.listdir(source_directory)
		if f.endswith(".ipynb")
	]
	os.makedirs(destination_directory_cppex, exist_ok=True)

	cpp_files = []
	exporter = ScriptExporter()

	for notebook_file in notebook_files:
		notebook_path = os.path.join(source_directory, notebook_file)

		with open(notebook_path, "r", encoding="utf-8") as f:
			nb = nbformat.from_dict(json.load(f))

		export_nb = _prepare_notebook_for_cpp_export(nb)
		cpp_code, resources = exporter.from_notebook_node(export_nb)

		output_extension = resources.get("output_extension", ".cpp")
		cpp_file_name = os.path.splitext(notebook_file)[0] + output_extension
		cpp_file_path = os.path.join(source_directory, cpp_file_name)

		with open(cpp_file_path, "w", encoding="utf-8") as f:
			f.write(cpp_code)

		cpp_files.append(cpp_file_path)

	print(" -Notebooks converted in C++ files with nbconvert")

	cmake_file_path = os.path.join(source_directory, "CMakeLists.txt")
	with open(cmake_file_path, "w", encoding="utf-8") as f:
		f.write(_build_cpp_notebooks_cmakelists(cpp_files))

	print(" -CMakeLists.txt generated for exported C++ files")

	archive_path = os.path.join(source_directory, archive_name_cpp)
	with zipfile.ZipFile(archive_path, "w", zipfile.ZIP_DEFLATED) as zipf:
		for cpp_file in cpp_files:
			zipf.write(cpp_file, os.path.basename(cpp_file))
		zipf.write(cmake_file_path, "CMakeLists.txt")

	print(" -Archive", archive_name_cpp, "created from:", source_directory)

	shutil.move(archive_path, os.path.join(destination_directory_cppex, archive_name_cpp))
	print(" -Archive", archive_name_cpp, "moved in:", destination_directory_cppex)

	for cpp_file in cpp_files:
		os.remove(cpp_file)

	print(" -C++ files deleted from:", source_directory)

	os.remove(cmake_file_path)
	print(" -CMakeLists.txt deleted from:", source_directory)





	

 
