cmake_minimum_required(VERSION 3.8)

project(Asterius)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(src)
include_directories(src/Common)
include_directories(src/Program)
include_directories(src/Examples)
include_directories(src/SyntaxAnalizer)
include_directories(src/LexicalAnalizer)

aux_source_directory(src SOURCES)
aux_source_directory(src/Common SOURCES)
aux_source_directory(src/Program SOURCES)
aux_source_directory(src/Examples SOURCES)
aux_source_directory(src/SyntaxAnalyzer SOURCES)
aux_source_directory(src/LexicalAnalyzer SOURCES)

set(EXECUTABLE_OUTPUT_PATH ../bin)

add_executable(${PROJECT_NAME} ${SOURCES})
