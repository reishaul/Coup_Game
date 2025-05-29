# reishaul1@gmail.com
CXX = g++
#flags for compilation
CXXFLAGS = -std=c++17 -Wall -g -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

#new
SRC_DIR = src
INC_DIR = include
TEST_DIR = test

# Common object files shared between different executables
COMMON_OBJS = Game.o Player.o Governor.o Judge.o Spy.o Baron.o General.o Merchant.o

#Build the main game executable 'coup_game' from main.o and common objects
coup_game: main.o $(COMMON_OBJS)
	$(CXX) -o coup_game main.o $(COMMON_OBJS) $(LDFLAGS)

# Build the demo executable from Demo.o and common objects
demo: Demo.o $(COMMON_OBJS)
	$(CXX) -o demo Demo.o $(COMMON_OBJS) $(LDFLAGS)

# Compilation rules for each source file to object file

main.o: $(SRC_DIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Demo.o: $(SRC_DIR)/Demo.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Game.o: $(SRC_DIR)/Game.cpp $(INC_DIR)/Game.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Player.o: $(SRC_DIR)/Player.cpp $(INC_DIR)/Player.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Governor.o: $(SRC_DIR)/Governor.cpp $(INC_DIR)/Governor.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Judge.o: $(SRC_DIR)/Judge.cpp $(INC_DIR)/Judge.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Spy.o: $(SRC_DIR)/Spy.cpp $(INC_DIR)/Spy.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Baron.o: $(SRC_DIR)/Baron.cpp $(INC_DIR)/Baron.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

General.o: $(SRC_DIR)/General.cpp $(INC_DIR)/General.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

Merchant.o: $(SRC_DIR)/Merchant.cpp $(INC_DIR)/Merchant.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build and run the game executable directly from main.cpp and common objects
Main: $(SRC_DIR)/main.cpp $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o coup_game $^ $(LDFLAGS)
	./coup_game

# Build and run tests
test: test_runner
	./test_runner

# Build the test runner executable from game_test.o and common objects
test_runner: game_test.o $(COMMON_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)


# Compile game_test.cpp to object file
game_test.o: game_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run tests using valgrind to check for memory leaks
valgrind: test_runner 
	valgrind --leak-check=full --track-origins=yes ./test_runner


# cleaning executable and object files
clean:
	rm -f *.o coup_game demo test_runner game_test.o
