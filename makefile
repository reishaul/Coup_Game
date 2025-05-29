# reishaul1@gmail.com
CXX = g++
#flags for compilation
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Common object files shared between different executables
COMMON_OBJS = Game.o Player.o Governor.o Judge.o Spy.o Baron.o General.o Merchant.o

#Build the main game executable 'coup_game' from main.o and common objects
coup_game: main.o $(COMMON_OBJS)
	$(CXX) -o coup_game main.o $(COMMON_OBJS) $(LDFLAGS)

# Build the demo executable from Demo.o and common objects
demo: Demo.o $(COMMON_OBJS)
	$(CXX) -o demo Demo.o $(COMMON_OBJS) $(LDFLAGS)

# Compilation rules for each source file to object file

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o


Demo.o: Demo.cpp
	$(CXX) $(CXXFLAGS) -c Demo.cpp -o Demo.o

Game.o: Game.cpp Game.hpp
	$(CXX) $(CXXFLAGS) -c Game.cpp -o Game.o

Player.o: Player.cpp Player.hpp
	$(CXX) $(CXXFLAGS) -c Player.cpp -o Player.o

Governor.o: Governor.cpp Governor.hpp
	$(CXX) $(CXXFLAGS) -c Governor.cpp -o Governor.o

Judge.o: Judge.cpp Judge.hpp
	$(CXX) $(CXXFLAGS) -c Judge.cpp -o Judge.o

Spy.o: Spy.cpp Spy.hpp
	$(CXX) $(CXXFLAGS) -c Spy.cpp -o Spy.o

Baron.o: Baron.cpp Baron.hpp
	$(CXX) $(CXXFLAGS) -c Baron.cpp -o Baron.o

General.o: General.cpp General.hpp
	$(CXX) $(CXXFLAGS) -c General.cpp -o General.o

Merchant.o: Merchant.cpp Merchant.hpp
	$(CXX) $(CXXFLAGS) -c Merchant.cpp -o Merchant.o

# Build and run the game executable directly from main.cpp and common objects
Main: main.cpp $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o coup_game main.cpp $(COMMON_OBJS) $(LDFLAGS)
	./coup_game

# Build and run tests
test: test_runner
	./test_runner

# Build the test runner executable from game_test.o and common objects
test_runner: game_test.o $(COMMON_OBJS)
	$(CXX) -o test_runner game_test.o $(COMMON_OBJS) $(LDFLAGS)


# Compile game_test.cpp to object file
game_test.o: game_test.cpp
	$(CXX) $(CXXFLAGS) -c game_test.cpp -o game_test.o

# Run tests using valgrind to check for memory leaks
valgrind: test_runner 
	valgrind --leak-check=full --track-origins=yes ./test_runner


# cleaning executable and object files
clean:
	rm -f *.o coup_game demo test_runner game_test.o
