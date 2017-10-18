cmake_minimum_required(VERSION 2.8)

project(Asterius)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES ${SOURCES}
	main.cpp
)

include_directories(src)
include_directories(src/SyntaxAnalizer)
include_directories(src/LexicalAnalizer)

aux_source_directory(src SOURCES)
aux_source_directory(src/SyntaxAnalizer SOURCES)
aux_source_directory(src/LexicalAnalizer SOURCES)

add_executable(${PROJECT_NAME} "main.cpp" ${SOURCES})
