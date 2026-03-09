cmake_minimum_required(VERSION 3.10)
project(xpl_examples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED ENV{CONDA_PREFIX})
  message(FATAL_ERROR "CONDA_PREFIX not set. Active l'environnement conda avant CMake.")
endif()

set(MAR_INC "$ENV{CONDA_PREFIX}/Library/include")
set(MAR_LIB "$ENV{CONDA_PREFIX}/Library/lib")

include_directories(
  "${MAR_INC}"
  "${MAR_INC}/marmoteCore"
  "${MAR_INC}/marmoteMarkovChain"
  "${MAR_INC}/marmoteMDP"
  "${MAR_INC}/marmoteLog"
)

link_directories("${MAR_LIB}")

# Sortie unique des exe dans build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}/bin")

add_executable(example1 example1/example1.cpp)
add_executable(example2 example2/example2.cpp)
add_executable(example3 example3/example3.cpp)

# Ajustement example4: exe renommé pour éviter conflit avec dossier example4/
add_executable(example4_bin example4/example4.cpp)

add_executable(example5 example5/example5.cpp)
add_executable(example6 example6/example6.cpp)
add_executable(example7 example7/example7.cpp)
add_executable(example10 example10/example10.cpp)

add_executable(exampleMDP10 exampleMDP10/exampleMDP10.cpp)
add_executable(exampleMDP11 exampleMDP11/exampleMDP11.cpp)
add_executable(exampleMDP21 exampleMDP21/exampleMDP21.cpp)
add_executable(exampleMDP31 exampleMDP31/exampleMDP31.cpp)
add_executable(exampleMDP40 exampleMDP40/exampleMDP40.cpp)

foreach(tgt
  example1 example2 example3 example4_bin example5 example6 example7 example10
  exampleMDP10 exampleMDP11 exampleMDP21 exampleMDP31 exampleMDP40)
  target_link_libraries(${tgt} PRIVATE marmoteCore marmoteMarkovChain marmoteMDP)
endforeach()

# Copie des fichiers .mcl nécessaires à l'exemple4 dans build/bin/example4/
add_custom_command(TARGET example4_bin POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/example4"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${CMAKE_CURRENT_SOURCE_DIR}/example4/IO_example_in1.mcl"
          "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/example4/IO_example_in1.mcl"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${CMAKE_CURRENT_SOURCE_DIR}/example4/IO_example_in2.mcl"
          "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/example4/IO_example_in2.mcl"
)
