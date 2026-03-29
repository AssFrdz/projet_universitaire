import os 
import sys

# fonction de redirection 
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
	