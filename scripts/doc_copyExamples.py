import os
import shutil
import subprocess
import zipfile

EXE_SUFFIX = ".exe" if os.name == "nt" else ""
"""
Utilities for copying Marmote C++ examples into the documentation assets.

This module is intended to be executed from the HTML documentation source
directory. It copies example sources into ``media``, generates ``.res`` and
``.cmd`` outputs, and updates the example archive CMake file based on the
selected example lists.
"""

# Example identifiers to include in the generated archives.
list1 = [1, 2, 3, 4, 5, 6, 7, 10]  # Markov chain examples. Use -1 to disable one.
list2 = [10, 11, 21, 31, 40]  # MDP examples.

# Mapping between numeric identifiers and example directory names.
corresponding_directory = [
    "example1",
    "example2",
    "example3",
    "example4",
    "example5",
    "example6",
    "example7",
    "example10",
    "exampleMDP10",
    "exampleMDP11",
    "exampleMDP21",
    "exampleMDP31",
    "exampleMDP40",
]

corresponding_executable = [
    "example1.exe",
    "example2.exe",
    "example3.exe",
    "example4_bin.exe",
    "example5.exe",
    "example6.exe",
    "example7.exe",
    "example10.exe",
    "exampleMDP10.exe",
    "exampleMDP11.exe",
    "exampleMDP21.exe",
    "exampleMDP31.exe",
    "exampleMDP40.exe",
]

# Command-line parameters for Markov chain examples.
corresponding_parameters = [
    " 10 0.1 0.2 0.7",
    " 10 0.1 0.2 0.7",
    " 10 0.05 0.05 0.05 0.05 0.05 0.05 0.2 0.5",
    " example4/IO_example_in1.mcl example4/IO_example_in2.mcl",
    "",
    "",
    "",
    "",
    "",
    " -tmax_sim 5",
]


def clean_destination_directory(destination_dir):
    """
    Remove generated example assets from a destination directory.

    :param destination_dir: target directory to clean
    """
    for filename in os.listdir(destination_dir):
        if filename.endswith(".cpp") or filename.endswith(".res") or filename.endswith(".cmd"):
            file_path = os.path.join(destination_dir, filename)
            os.remove(file_path)
            print(f"File removed: {file_path}")


def generate_example_res_files(corresponding_executable, num_list, example_format, parameters):
    """
    Generate ``.res`` and ``.cmd`` files for Markov chain examples.

    :param corresponding_executable: executable names associated with the examples
    :param num_list: selected example numbers
    :param example_format: filename template for generated assets
    :param parameters: command-line arguments indexed by example number
    """
    idx = 0
    for num in num_list:
        execname = corresponding_executable[idx]
        parameters_string = parameters[num - 1]
        example_file = example_format.format(num)
        executable_path = os.path.join("./", execname)
        if os.path.exists(executable_path):
            command = [executable_path] + parameters_string.split()

            command_output = example_file + ".cmd"
            print("Command file", command_output, "command", command)
            with open(command_output, "w") as file:
                file.write("./" + example_file + parameters_string)

            result_output = example_file + ".res"
            print("Output file", result_output, "command", command)

            # Run the executable without a shell to avoid command injection.
            result = subprocess.run(command, check=False, text=True, capture_output=True)
            with open(result_output, "w") as file:
                file.write(result.stdout)
        else:
            print(f"File {executable_path} not found")
        idx += 1


def generate_exampleMDP_res_files(prefix, num_list, example_format):
    """
    Generate ``.res`` files for MDP examples.

    :param prefix: example executable prefix
    :param num_list: selected example numbers
    :param example_format: filename template for generated assets
    """
    for num in num_list:
        executable_name = f"{prefix}{num}{EXE_SUFFIX}"
        example_file = example_format.format(num)
        executable_path = os.path.join("./", executable_name)
        if os.path.exists(executable_path):
            command = executable_path
            result_output = example_file + ".res"
            print("Output file", result_output, "command", command)
            result = subprocess.run(command, check=False, text=True, capture_output=True)
            with open(result_output, "w") as file:
                file.write(result.stdout)
        else:
            print(f"File {executable_path} not found")


def copy_example_files(corresponding_directory, num_list, example_format, destination_dir):
    """
    Copy selected Markov chain example source files.

    :param corresponding_directory: example directory names indexed by example
    :param num_list: selected example numbers
    :param example_format: filename template for source files
    :param destination_dir: destination directory
    """
    idx = 0
    for num in num_list:
        print(num)
        subdir = corresponding_directory[idx]
        example_file = example_format.format(num)
        source_file = os.path.join(subdir, example_file)
        if os.path.exists(source_file):
            shutil.copy(source_file, destination_dir)
            print(source_file, "copied in", destination_dir)
        else:
            print(f"File {source_file} not found")

        idx += 1


def copy_exampleMDP_files(prefix, num_list, example_format, destination_dir):
    """
    Copy selected MDP example source files.

    :param prefix: example directory prefix
    :param num_list: selected example numbers
    :param example_format: filename template for source files
    :param destination_dir: destination directory
    """
    for num in num_list:
        subdir = f"{prefix}{num}"
        example_file = example_format.format(num)
        source_file = os.path.join(subdir, example_file)
        if os.path.exists(source_file):
            shutil.copy(source_file, destination_dir)
            print(source_file, "copied in", destination_dir)
        else:
            print(f"File {source_file} not found")


def copy_example_res_files(corresponding_directory, num_list, example_format, destination_dir):
    """
    Copy generated ``.res`` and ``.cmd`` files for Markov chain examples.

    :param corresponding_directory: example directory names indexed by example
    :param num_list: selected example numbers
    :param example_format: filename template for generated assets
    :param destination_dir: destination directory
    """
    for num in num_list:
        example_file = example_format.format(num)
        if os.path.exists(example_file):
            shutil.move(example_file, destination_dir)
            print(example_file, "copied in", destination_dir)
        else:
            print(f"File {example_file} not found")


def copy_exampleMDP_res_files(num_list, example_format, destination_dir):
    """
    Copy generated ``.res`` files for MDP examples.

    :param num_list: selected example numbers
    :param example_format: filename template for generated assets
    :param destination_dir: destination directory
    """
    for num in num_list:
        example_file = example_format.format(num)
        if os.path.exists(example_file):
            shutil.move(example_file, destination_dir)
            print(example_file, "moved in", destination_dir)
        else:
            print(f"File {example_file} not found")


def modify_cmake_file(source_dir, cmake_file, cmake_file_model, list1, list2):
    """
    Generate a ``CMakeLists.txt`` from the example model file.

    :param source_dir: directory that contains the template and generated file
    :param cmake_file: output CMake file name
    :param cmake_file_model: source CMake template name
    :param list1: selected Markov chain examples
    :param list2: selected MDP examples
    """
    # Resolve both the template path and the generated file path.
    cmake_file_path = os.path.join(source_dir, cmake_file)
    cmake_model_path = os.path.join(source_dir, cmake_file_model)
    print("source", cmake_model_path, "destination", cmake_file_path)
    shutil.copy(cmake_model_path, cmake_file_path)

    with open(cmake_file_path, "r") as file:
        cmake_content = file.readlines()

    counter = 0

    # Replace the two placeholder loops with the selected example identifiers.
    for i, line in enumerate(cmake_content):
        if "EXAMPLE_NUMBER IN ITEMS" in line:
            counter += 1
            if counter == 1:
                filtered_list1 = filter(lambda x: x >= 0, list1)
                cmake_content[i] = (
                    "foreach( EXAMPLE_NUMBER IN ITEMS " + " ".join(map(str, filtered_list1)) + " )\n"
                )
            elif counter == 2:
                filtered_list2 = filter(lambda x: x >= 0, list2)
                cmake_content[i] = (
                    "foreach( EXAMPLE_NUMBER IN ITEMS " + " ".join(map(str, filtered_list2)) + " )\n"
                )
                break

    if counter < 2:
        print("The line 'EXAMPLE_NUMBER IN ITEMS' was not found twice in the file.")

    with open(cmake_file_path, "w") as file:
        file.writelines(cmake_content)

    archive_path = os.path.join(source_dir, "all_examples.zip")
    print("Updating archive", archive_path)
    with zipfile.ZipFile(archive_path, "a") as archive:
        archive.write(cmake_file_path, "cmake/CMakeLists.txt")
