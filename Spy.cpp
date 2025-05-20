#include <iostream>
#include <stdexcept>

#include "Spy.hpp"
using std::string;
using std::cout;
using std::endl;

namespace coup{
Spy::Spy(coup::Game& game,const string& name):Player(game,name,"Spy"){};

void Spy::cancelAreest(Player& p){
    if(!p.isActive()){
        throw runtime_error("the player is not active");
    }
    p.arrestAccess=false;
}
 
void Spy::viewCoins(Player& p){
    if(!p.isActive()){
        throw runtime_error("this player is not active so far");
    }
    enemyCoins=p.coins();
    cout<<getName() << " has view " << p.getName() << "'s coins: " << enemyCoins << endl;
}
}
