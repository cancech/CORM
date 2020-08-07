cmake_minimum_required(VERSION 3.10)
project(CORM)

#Compile the CORM library
include_directories(include)
add_library(corm SHARED
		src/corm/exception/InvalidBeanException.cpp
		src/corm/BeanManager.cpp
		src/corm/CircularDependencyChecker.cpp
		src/corm/Configuration.cpp
		src/corm/Context.cpp
)
target_compile_options(corm PUBLIC -Wall -c -fmessage-length=0 -fPIC)

# Add the unit tests
add_subdirectory(test)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
