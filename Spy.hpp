#ifndef SPY_HPP
#define SPY_HPP

#include "Player.hpp"
//#include "Game.hpp"

namespace coup{
class Spy : public Player{

private:
int enemyCoins;

public:
    Spy(coup::Game& game, const string& name);
    void cancelAreest(Player& p);
    void viewCoins(Player& p);
};
}

#endif


