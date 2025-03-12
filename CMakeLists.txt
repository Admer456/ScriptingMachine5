
cmake_minimum_required( VERSION 3.16 )
project( ScriptingMachine5 VERSION 1.0 LANGUAGES CXX )

## std::filesystem needs C++17
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

option( SM5_EXAMPLES "Build examples for ScriptingMachine5" ON )

set( SM5_SOURCES
	src/Assembly.cpp
	src/Assembly.hpp
	src/ErrorWriter.cpp
	src/ErrorWriter.hpp
	src/Host.cpp
	src/Host.hpp
	src/Library.cpp
	src/Library.hpp
	src/Utility.hpp

	src/DotNet/coreclr_delegates.h
	src/DotNet/hostfxr.h
)

## VS filters
source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SM5_SOURCES} )

## ScriptingMachine5 library
add_library( ScriptingMachine5 STATIC ${SM5_SOURCES} )
target_include_directories( ScriptingMachine5 INTERFACE src )

## Examples
add_subdirectory( examples/Example1_BasicUsage )
add_subdirectory( examples/Example2_PassingData )
