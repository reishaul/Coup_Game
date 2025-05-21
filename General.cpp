#include <iostream>
#include <stdexcept>

#include "General.hpp"
#include "Player.hpp"
//#include "Game.hpp"

using std::string;

namespace coup{
General::General(coup::Game& game, const string& name):Player(game, name, "General"){};

void General::cancelCoup(Player& target){

    if(coins()<5){
        throw runtime_error("cannot cancel coup there is not enough coins");
    }
    else if(!target.isActive()){
        game.set_active(target.getName(), true);
        decreaseCoins(5);
    }
    else{
        throw runtime_error("cannot cancel coup on active player");
    }

   

}
}



