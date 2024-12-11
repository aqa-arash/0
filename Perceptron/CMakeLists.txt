cmake_minimum_required(VERSION 3.29)
project(Perceptron)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable warnings for all compilers
if(MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# FetchContent module to automatically download Eigen if not present
include(FetchContent)

FetchContent_Declare(
        Eigen3
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG 3.4.0 # Change this to the desired version
)

# Make Eigen available
FetchContent_MakeAvailable(Eigen3)

add_executable(Perceptron IO.cpp)

# Link Eigen to the project
target_link_libraries(Perceptron PRIVATE Eigen3::Eigen)

# Platform-specific configurations
if(WIN32)
    # Windows specific configurations can be added here
    message(STATUS "Configuring for Windows platform")
elseif(UNIX)
    # Linux/Unix specific configurations can be added here
    message(STATUS "Configuring for Linux/Unix platform")
endif()

# Install rules
install(TARGETS Perceptron RUNTIME DESTINATION bin)
install(DIRECTORY ${EIGEN3_SOURCE_DIR}/Eigen DESTINATION include)

# Message to indicate completion
message(STATUS "CMake setup complete for Perceptron")
