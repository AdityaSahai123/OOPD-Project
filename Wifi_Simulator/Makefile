# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# Source files for the shared library
SRC_LIB = wifi_simulator.cpp wifi4.cpp wifi5.cpp wifi6.cpp access_point.cpp channel.cpp user.cpp
OBJ_LIB = $(SRC_LIB:.cpp=.o)
LIBRARY_NAME = libwifi_simulator.so

# Source file for the test program
SRC_TEST = test_program.cpp
OBJ_TEST = $(SRC_TEST:.cpp=.o)
TARGET_TEST = wifi_simulator_test

# Source file for the main program
SRC_MAIN = main.cpp
OBJ_MAIN = $(SRC_MAIN:.cpp=.o)
TARGET_MAIN = wifi_simulator

# Library path (current directory, adjust if needed)
LIBRARY_PATH = .
LIBRARY = -L$(LIBRARY_PATH) -lwifi_simulator  # Link the shared library

# Default target
all: $(TARGET_MAIN) $(TARGET_TEST)

# Build the main program executable
$(TARGET_MAIN): $(OBJ_MAIN) $(OBJ_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_MAIN) $(OBJ_LIB)

# Build the shared library (for common logic)
$(LIBRARY_NAME): $(OBJ_LIB)
	$(CXX) -shared -o $(LIBRARY_NAME) $(OBJ_LIB)

# Build the test program executable
$(TARGET_TEST): $(OBJ_TEST) $(LIBRARY_NAME)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_TEST) -L$(LIBRARY_PATH) $(LIBRARY)

# Rule to compile .cpp files into .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ_LIB) $(OBJ_TEST) $(OBJ_MAIN) $(TARGET_MAIN) $(TARGET_TEST) $(LIBRARY_NAME)

