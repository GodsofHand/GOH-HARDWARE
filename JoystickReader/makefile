# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -std=c++11
LDFLAGS = -lrt

# Target executable and source file
TARGET1 = mpuSender
TARGET2 = JSSender
SOURCE1 = mpuSender.cpp
SOURCE2 = JSSender.cpp

# Default target
all: run2

# Link the object file to create the executable
$(TARGET1): $(SOURCE1)
	$(CXX) $(CXXFLAGS) $(SOURCE1) -o $(TARGET1) $(LDFLAGS)


# Link the object file to create the executable
$(TARGET2): $(SOURCE2)
	$(CXX) $(CXXFLAGS) $(SOURCE2) -o $(TARGET2) $(LDFLAGS)

# Run the program
run1: $(TARGET1)
	./$(TARGET1)


# Run the program
run2: $(TARGET2)
	./$(TARGET2)

# Clean up the built files
clean:
	rm -f $(TARGET1) $(TARGET2)

# Help target to describe the options
help:
	@echo "Makefile for the project"
	@echo
	@echo "Usage:"
	@echo "  make         # Compile the project"
	@
