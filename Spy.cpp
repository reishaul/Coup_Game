//reishaul1@gmail.com
#include <iostream>
#include <stdexcept>

#include "Spy.hpp"
using std::string;
using std::cout;
using std::endl;

namespace coup{
Spy::Spy(coup::Game& game,const string& name):Player(game,name,"Spy"){};

void Spy::blockArrest(Player& p){
    if(!p.isActive()){
        throw runtime_error("the player is not active");
    }
    cout<<getName() << "(spy) has blocked " << p.getName() << "'s arrest" << endl;
    p.arrestAccess=false;
}
 
void Spy::viewCoins(Player& p){
    if(!this->active){
        throw runtime_error("cannot view coins- this player is not active so far");
    }
    else if(!p.isActive()){
        throw runtime_error("this player is not active so far");
    }
    enemyCoins=p.coins();
    cout<<getName() << " has view " << p.getName() << "'s coins: " << enemyCoins << endl;
}
}
