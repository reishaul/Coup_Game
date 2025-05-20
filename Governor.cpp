#include <iostream>
#include <stdexcept>

#include "Governor.hpp"
using std::string;
using std::cout;
using std::endl;
using std::runtime_error;

namespace coup{

Governor::Governor(coup::Game& game, const string& name):Player(game,name, "Governor"){};

void Governor::tax() {
    if(taxaccess){
        addCoins(3);
        std::cout<< getName() <<" (Governor) took three coins by using tax operation"<<endl;
        game.next_turn();
        lastAction="tax";
    }
    else{
        throw runtime_error("no access for tax operation");
    }
}

void Governor::undo(Player& p){
    if(!p.isActive()){
        throw runtime_error("the player is not active anymore");
    }
    if(!(p.lastAction=="tax")){
        throw runtime_error("the last action of  this player is not tax");

    }
    else{
        p.decreaseCoins(2);//return the coins that the player get 
        cout<< getName() <<"(Governor) undo tax"<<endl;
    }

}
}