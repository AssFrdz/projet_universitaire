import os
import shutil
import subprocess
import sys

EXE_SUFFIX = ".exe" if os.name == "nt" else ""
"""
@author = MB et EH with modifications by AJM
@version = 1.0.2
@warning To be executed in the documentation html source directory  
@brief = This script is dedicated to the transfer of example files from xpl or xpl-wrp to the doc repository
It  copies C++ files into the 'media' directory and modifying a Cmakelist.txt file using lists.
"""

# Lists of integers that describes the file to move
list1 = [1, 2, 3, 4, 5, 6, 7, 10] #MC CPP files to transfer - "-1" is used to deactivate some examples
list2 = [10, 11, 21, 31, 40]      #MDP CPP files to transfer
# correspondence between numbers and names of the examples
corresponding_directory=['example1','example2','example3','example4', 
        'example5','example6','example7','example10', 
	'exampleMDP10', 'exampleMDP11','exampleMDP21','exampleMDP31','exampleMDP40']
corresponding_executable=['example1.exe','example2.exe','example3.exe','example4_bin.exe',
    'example5.exe','example6.exe','example7.exe','example10.exe',
    'exampleMDP10.exe','exampleMDP11.exe','exampleMDP21.exe','exampleMDP31.exe','exampleMDP40.exe'
]
#parameter for  MC examples 
corresponding_parameters=[' 10 0.1 0.2 0.7',' 10 0.1 0.2 0.7',' 10 0.05 0.05 0.05 0.05 0.05 0.05 0.2 0.5', \
 ' example4/IO_example_in1.mcl example4/IO_example_in2.mcl','','','','','',' -tmax_sim 5']

def clean_destination_directory(destination_dir):
	"""
	Function to clean up res and cpp files in destination_dir
	
	:param destination_dir the directory
	"""
	# Parcourir tous les fichiers dans le répertoire
	for fichier in os.listdir(destination_dir):
		# Vérifier si le fichier se termine par .cpp ou .res ou .cmd
		if fichier.endswith('.cpp') or fichier.endswith('.res') or fichier.endswith('.cmd'):
			# Construire le chemin complet vers le fichier
			chemin_fichier = os.path.join(destination_dir, fichier)
			# Supprimer le fichier
			os.remove(chemin_fichier)
			print(f"File removed : {chemin_fichier}")

# supprimer les fichiers cpp, res ou cmd pour nettoyer le script


def generate_example_res_files(corresponding_executable, num_list, example_format, parameters):
	"""
	Function to generate exampleX.res and exampleX.cmd files
	
	:param corresponding_directory
	:param num_list selected list of selected examples
	:param example_format prefix that we want to give to examples names
	:parameters for executing the file
	"""
	idx = 0
	for num in num_list:
		#print(num)
		execname = corresponding_executable[idx]
		para = parameters[(num-1)]
		example_file = example_format.format(num)
		subdirectory = os.path.join('./', execname)
		if os.path.exists(subdirectory):
			commande_parametree = str(subdirectory)+str(para)
			commande = commande_parametree.split()
			# Write the command in the .cmd file
			fichier_sortie = example_file+".cmd"
			print("Command is",fichier_sortie,"commande",commande)
			with open(fichier_sortie, 'w') as fichier:
				fichier.write('./'+example_file+para)
			fichier_sortie = example_file+".res"
			print("Output of",fichier_sortie,"commande",commande)
			# Utiliser subprocess.run pour exécuter la commande
			### SECURITE
			### exécution via subprocess.run pour éviter l'injection de cmd non voulues
			### subprocess.run -> exécuter un programme externe à partir de python
			### on passe une ligne d'arguments


			resultat = subprocess.run(commande, check=False, text=True, capture_output=True)
			# Renommer result.stdout en result.res
			with open(fichier_sortie, 'w') as fichier:
				fichier.write(resultat.stdout)
		else:
			print(f"File {subdirectory} not found")
		idx = idx+1

def generate_exampleMDP_res_files(prefix, num_list, example_format):
	"""
	Function to generate exampleMDPX.res files 
	
	:param prefix of the name of the repository
	:param num_list selected list of selected examples
	:param example_format prefix that we want to give to examples names
	"""
	for num in num_list:
		subdir = f'{prefix}{num}{EXE_SUFFIX}'
		example_file = example_format.format(num)
		subdirectory = os.path.join('./',subdir)
		if os.path.exists(subdirectory):
			commande=str(subdirectory)
			fichier_sortie=example_file+".res"
			print("Sortie",fichier_sortie,"commande",commande)
			# Utiliser subprocess.run pour exécuter la commande
			resultat = subprocess.run(commande, check=False, text=True, capture_output=True)
			# Renommer result.stdout en result.res
			with open(fichier_sortie, 'w') as fichier:
				fichier.write(resultat.stdout)
		else:
			print(f"File {subdirectory} not found")


def copy_example_files(corresponding_directory, num_list, example_format, destination_dir):
	"""
	 Function to copy exampleX.cpp files
	 
	:param corresponding_directory list in which at entry i twe found the name of the example
	:param num_list list of numbers of selected examples
	:param example_format prefix that we want to give to examples names
	:param destination_dir directory in which the files should be copied 
	"""
	idx = 0
	for num in num_list:
		print(num)
		subdir = corresponding_directory[idx]
		#print("subdir",subdir)
		example_file = example_format.format(num)
		source_file = os.path.join(subdir, example_file)
		if os.path.exists(source_file):
			shutil.copy(source_file, destination_dir)
			print(source_file," copied in ",destination_dir)
		else:
			print(f"File {source_file} not found")
		
		idx = idx+1
			


def copy_exampleMDP_files(prefix, num_list, example_format,destination_dir):
	"""
	Function to copy exampleMDPX.cpp files 
	
	:param prefix
	:param num_list selected list of selected examples
	:param example_format prefix that we want to give to examples names
	:param destination_dir directory in which the files should be copied 
	"""
	for num in num_list:
		subdir = f'{prefix}{num}'
		example_file = example_format.format(num)
		source_file = os.path.join(subdir, example_file)
		if os.path.exists(source_file):
			shutil.copy(source_file, destination_dir)
			print(source_file," copied in",destination_dir)
		else:
			print(f"File {source_file} not found")

def copy_example_res_files(corresponding_directory, num_list, example_format, destination_dir):
	"""
	 Function to copy exampleX.res and exampleX.cmd files
	 
	:param corresponding_directory list in which at entry i twe found the name of the example
	:param num_list selected list of selected examples
	:param example_format prefix that we want to give to examples names
	:param destination_dir directory in which the files should be copied 
	"""
	for num in num_list:
		#print(num)
		subdir = corresponding_directory[(num-1)]
		#print("subdir",subdir)
		example_file = example_format.format(num)
		if os.path.exists(example_file):
			shutil.move(example_file, destination_dir)
			print(example_file," copied in",destination_dir)
		else:
			print(f"File {example_file} not found")


def copy_exampleMDP_res_files(num_list, example_format,destination_dir):
	"""
	Function to copy exampleMDPX.cpp files 
	
	:param num_list selected list of selected examples
	:param example_format prefix that we want to give to examples names
	:param destination_dir directory in which the files should be copied 
	"""
	for num in num_list:
		example_file = example_format.format(num)
		if os.path.exists(example_file):
			shutil.move(example_file, destination_dir)
			print(example_file," moved in",destination_dir)
		else:
			print(f"File {example_file} not found")


def modify_cmake_file(source_dir, cmake_file, cmake_file_model, list1, list2):
	"""
	Function to generate a CMakeLists.txt for the list of examples above from a Cmakelist Model
	
	:param source_dir the directory where the model and the files to copy are located
	:param cmake_file the name of the cmakefile
	:param cmake_file_model name of the model
	:param list1 list of basic Examples
	:param list2 list of basic ExampleMDP
	"""
	
	#Full path of the CMakeLists.txt file in the source directory 
	cmake_file_path = os.path.join(source_dir,cmake_file)
	cmake_model_path = os.path.join(source_dir, cmake_file_model)
	#change name of model
	print("source",cmake_model_path,"destination",cmake_file_path)
	shutil.copy(cmake_model_path,cmake_file_path)
	
	# Open the CMakeLists.txt file in read mode
	with open(cmake_file_path, 'r') as file:
		cmake_content = file.readlines()
	# Counter to track how many times the line has been found
	counter = 0
	
	# Iterate through the lines to find the one containing "EXAMPLE_NUMBER IN ITEMS"
	for i, line in enumerate(cmake_content):
		if "EXAMPLE_NUMBER IN ITEMS" in line:
			counter += 1
			# Modify the line based on the first or second found list
			if counter == 1:
				# Build the new line using the first list
				flist1 = filter( lambda x: x >= 0, list1 )
				new_line = "foreach( EXAMPLE_NUMBER IN ITEMS " + " ".join(map(str, flist1)) + " )\n"
				cmake_content[i] = new_line
			elif counter == 2:
				# Build the new line using the second list
				flist2 = filter( lambda x: x >= 0, list2 )
				new_line = "foreach( EXAMPLE_NUMBER IN ITEMS " + " ".join(map(str, flist2)) + " )\n"
				cmake_content[i] = new_line
				break  # Stop iterating after the second modification
	# Write the modifications into the file if the line has not been found twice
	if counter < 2:
		print("The line 'EXAMPLE_NUMBER IN ITEMS' was not found twice in the file.")
	
	with open(cmake_file_path, 'w') as file:
		file.writelines(cmake_content)


def list_and_test(dir, message):
	print(message,'=',dir,'',end='')
	if os.access(dir,os.F_OK):
		print("[Exists]")
	else:
		print("[Does not exist]")

#################""
#main code


"""
assia
On veut exécuter le script depuis un répertoire précis, doc/html/scripts
on simule l'exécution du script avant son lancement réel pour s'assurer qu'il n'y a pas de bug
grâce au --dry_run
"""

args = sys.argv
generate_only = False
dry_run = False
if ( len(args) > 1 ):
	if ( args[1] == "-h" ) or ( args[1] == "--help" ):
		print(f"Usage: {args[0]} [-h|--help] [-g|--generate_only] [-n|--dry_run]")
		print("\tThis script is to be stored in .../scripts\n\tand is to be run from directory .../doc/html/source")
		sys.exit(0)
	elif ( args[1] == "-g" ) or ( args[1] == "--generate_only" ):
		generate_only = True
	elif ( args[1] == "-n" ) or ( args[1] == "--dry_run" ):
		dry_run = True


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
	
if dry_run:
	# Skip the rest
	print("Dry run: not executing copy instructions")
	sys.exit(0)

print("1. Cleaning",os.getcwd())
clean_destination_directory(destination)

# Change to the source directory
os.chdir(source_dir)

print("2. Beginning of copying source file")
# Copy exampleJ.cpp files
copy_example_files(corresponding_directory, list1, 'example{}.cpp', destination)
# Copy exampleMDP.cpp files
copy_exampleMDP_files('exampleMDP', list2, 'exampleMDP{}.cpp', destination)

# Change to the bin directory
os.chdir(source_bin)
print("3. Generating res and cmd",os.getcwd())
generate_example_res_files(corresponding_executable, list1, 'example{}',corresponding_parameters)
generate_exampleMDP_res_files('exampleMDP', list2, 'exampleMDP{}')
	
print("4. Copying res",os.getcwd())
# Copy exampleJ.res files
copy_example_res_files(corresponding_directory, list1, 'example{}.res',destination)
# Copy exampleJ.cmd files
copy_example_res_files(corresponding_directory, list1, 'example{}.cmd',destination)
# Copy exampleMDP.res files
copy_exampleMDP_res_files(list2, 'exampleMDP{}.res',destination)


if generate_only:
	# Skip the CMake part
	print("Generate only: skip step 5 (generate cmake)")
	sys.exit(0)

print("5. Generate CMake")
#Copy and modify CMakeLists.txt 
modify_cmake_file(cmake_destination,cmake_file,cmake_archive,list1,list2)
