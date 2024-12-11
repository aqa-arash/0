# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -I./eigen -O3 -Wall -Wextra -Wpedantic -Werror

# Target executable
TARGET = IO

# Source files
SRCS = read_dataset.cpp

# Header file
HEADERS = IO.hpp

# Build target
$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up build files
clean:
	rm -f $(TARGET)