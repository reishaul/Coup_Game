#include <iostream>
#include <stdexcept>

#include "General.hpp"
#include "Player.hpp"
//#include "Game.hpp"

using std::string;

namespace coup{
General::General(coup::Game& game, const string& name):Player(game, name, "General"){};

void General::cancelCoup(Player& p, Player& target){

    if(coins()<5){
        throw runtime_error("cannot cancel coup there is not enough coins");
    }
    if(!target.isActive()){
        game.set_active(target.getName(), true);
    }

    decreaseCoins(5);
    if(p.status=="arrest"){p.addCoins(1);}//מה קשור במה נפגע השחקן שביצע את ההפיכה
}
}



