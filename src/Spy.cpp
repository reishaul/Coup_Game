//reishaul1@gmail.com
/**
 * @file Spy.cpp
 * @brief Implementation of the Spy class methods.
 */
#include <iostream>
#include <stdexcept>

#include "Spy.hpp"
using std::string;
using std::cout;
using std::endl;

namespace coup{
    /**
 * @brief Constructs a Spy with a reference to the game and player name.
 * @param game Reference to the Game object.
 * @param name Name of the player.
 */
Spy::Spy(coup::Game& game,const string& name):Player(game,name,"Spy"){};


/**
 * @brief Blocks the arrest attempt of another player.
 * @param p Reference to the player attempting the arrest.
 * @throws std::runtime_error if either player is not active.
 */
void Spy::blockArrest(Player& p){
    if(!this->active){//first check if the performer player is active
        throw runtime_error("cannot block arrest with no active player");
    }
    else if(!p.isActive()){//check if the target player is active
        throw runtime_error("the player is not active");
    }
    cout<<getName() << "(spy) has blocked " << p.getName() << "'s arrest" << endl;
    p.arrestAccess=false;//block the arrest access of the target player
}


 /**
 * @brief Views the number of coins another player has.
 * @param p Reference to the target player.
 * @throws std::runtime_error if either player is not active.
 */
void Spy::viewCoins(Player& p){
    if(!this->active){//first check if the performer player is active
        throw runtime_error("cannot view coins with no active player");
    }
    else if(!p.isActive()){//check if the target player is active
        throw runtime_error("this player is not active so far");
    }
    enemyCoins=p.coins();
    cout<<getName() << " has view " << p.getName() << "'s coins: " << enemyCoins << endl;
}
}
