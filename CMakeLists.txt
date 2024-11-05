cmake_minimum_required(VERSION 3.10)

# Set the project name
project(AdvPTTestingGround VERSION 1.2
        DESCRIPTION "Just used for testing"
        LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add OpenMP support
find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3")
endif()

# Add the executable
add_executable(test test.cpp)
# Link OpenMP libraries
target_link_libraries(test OpenMP::OpenMP_CXX)
