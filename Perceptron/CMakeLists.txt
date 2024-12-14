cmake_minimum_required(VERSION 3.27.9)
project(Perceptron LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_Fortran_COMPILER "")  # Disable Fortran checks (from Eigen)

# FetchContent module to automatically download Eigen if not present
include(FetchContent)

FetchContent_Declare(
        Eigen3
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 3.4.0
)

# Make Eigen available
FetchContent_MakeAvailable(Eigen3)

add_executable(read_dataset read_dataset.cpp
        IO.cpp
        IO.hpp)

# Link Eigen to the project
target_link_libraries(read_dataset PRIVATE Eigen3::Eigen)

# Platform-specific configurations
if (WIN32)
    # Windows specific configurations can be added here
    message(STATUS "Configuring for Windows platform")
elseif (UNIX)
    # Linux/Unix specific configurations can be added here
    message(STATUS "Configuring for Linux/Unix platform")
endif ()

# Install rules
install(TARGETS read_dataset RUNTIME DESTINATION bin)
install(DIRECTORY ${EIGEN3_SOURCE_DIR}/Eigen DESTINATION include)

# Message to indicate completion
message(STATUS "CMake setup complete for Perceptron")
