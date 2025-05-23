#reishaul1@gmail.com
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

OBJS = Demo.o Game.o Player.o Governor.o Judge.o Spy.o Baron.o General.o Merchant.o

coup_game: $(OBJS)
	$(CXX) -o coup_game $(OBJS) $(LDFLAGS)

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

clean:
	rm -f *.o coup_game
