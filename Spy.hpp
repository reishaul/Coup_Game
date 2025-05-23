//reishaul1@gmail.com
#ifndef SPY_HPP
#define SPY_HPP

#include "Player.hpp"

namespace coup{
class Spy : public Player{

private:
int enemyCoins;

public:
    Spy(coup::Game& game, const string& name);
    void blockArrest(Player& p);
    void viewCoins(Player& p);
};
}

#endif


