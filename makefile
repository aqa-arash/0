CXX = g++
CXXFLAGS = -fopenmp -O3
SOURCE = test.cpp

OUTPUT = test

all: $(OUTPUT)

$(OUTPUT): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(SOURCE)

clean:
	rm -f $(OUTPUT)
