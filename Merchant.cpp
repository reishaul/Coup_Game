//reishaul1@gmail.com
/**
 * @file Merchant.cpp
 * @brief Implementation of the Merchant class for the Coup game.
 */
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "Merchant.hpp"
#include "Player.hpp"

using std::string;
using std::cout;
using std::endl;


namespace coup{
/**
 * @brief Constructs a Merchant player with a reference to the game and the player's name.
 * @param game Reference to the Game object.
 * @param name Name of the player.
 */
Merchant::Merchant(coup::Game& game, const string& name):Player(game,name, "Merchant"){};

/**
 * @brief Gives the Merchant a bonus coin if they have 3 or more coins.
 * This function is intended to be called at the start of the Merchant's turn.
 */
void Merchant::merchantBonus(){
    if(coins()>=3){
        addCoins(1);
        cout << getName() << " (Merchant) got a bonus coin at the start of the turn" << endl;
    }
}

}


