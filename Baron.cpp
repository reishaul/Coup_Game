#include <iostream>
#include <stdexcept>

#include "Baron.hpp"
using std::string;

namespace coup{
Baron::Baron(coup::Game& game, const string& name):Player(game,name, "Baron"){};

void Baron::invest(){
    if(coins()<3){
        throw runtime_error("cannot invest with less then 3 coins");
    }
    addCoins(3);
    game.next_turn();
    lastAction="invest";
    cout<< getName() <<" (Baron) traded its 3 coins and got 6 "<<endl;
    openAccess();

}
}

