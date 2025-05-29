//reishaul1@gmail.com
#include <iostream>
#include <stdexcept>

#include "Baron.hpp"
using std::string;

//Defines the `Baron` class, a subclass of `Player` in the `coup` namespace.
namespace coup{
// Constructor for the `Baron` class, initializing the player with a game reference and a name.
Baron::Baron(coup::Game& game, const string& name):Player(game,name, "Baron"){};

/**
 * @brief The invest function allows the Baron to trade 3 coins for 6 coins.
 * * It checks if the Baron has at least 3 coins and if it is the Baron's turn.
 * * @throws runtime_error if the Baron has less than 3 coins or if it is not the Baron's turn.
 */
void Baron::invest(){
    if(coins()<3){
        throw runtime_error("cannot invest with less then 3 coins");
    }
    else if(game.turn() != getName()){
        throw runtime_error("Not baron's turn");
    }
    addCoins(3);//add 3 coins to the baron
    game.next_turn();//move to the next player
    lastAction="invest";//sign 
    cout<< getName() <<" (Baron) traded its 3 coins and got 6 "<<endl;
    openAccess();//after one action the player can do all actions again

}
}

