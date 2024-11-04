cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MyProject)

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

# Custom target to mimic the original Makefile's behavior
add_custom_target(makefile_target ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Building using the original Makefile"
        COMMAND make -f Makefile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
