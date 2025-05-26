# reishaul1@gmail.com
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

COMMON_OBJS = Game.o Player.o Governor.o Judge.o Spy.o Baron.o General.o Merchant.o

# ברירת מחדל - קומפילציה של main.cpp
coup_game: main.o $(COMMON_OBJS)
	$(CXX) -o coup_game main.o $(COMMON_OBJS) $(LDFLAGS)

# אופציה להריץ את Demo.cpp
demo: Demo.o $(COMMON_OBJS)
	$(CXX) -o demo Demo.o $(COMMON_OBJS) $(LDFLAGS)

# כללים לקימפול
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

# ניקוי קבצים
clean:
	rm -f *.o coup_game demo
