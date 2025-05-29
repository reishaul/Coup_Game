//reishaul1@gmail.com
// Defines the `Governor` class, a subclass of `Player` in the `coup` namespace.
#include <iostream>
#include <stdexcept>

#include "Governor.hpp"
using std::string;
using std::cout;
using std::endl;
using std::runtime_error;

namespace coup{

/**
 * @brief Constructs a Governor player with a reference to the game and a name.
 * @param game Reference to the Game instance.
 * @param name The name of the player.
 */
Governor::Governor(coup::Game& game, const string& name):Player(game,name, "Governor"){};

/**
 * @brief The tax function allows the Governor to take 3 coins from the bank.
 * It checks if the Governor has access to the tax operation.
 * @throws runtime_error if the Governor does not have access for tax operation.
 */
void Governor::tax() {
    if(taxaccess){
        addCoins(3);//get 3 instead of 2 coins
        std::cout<< getName() <<" (Governor) took three coins by using tax operation"<<endl;
        game.next_turn();//move to the next player
        lastAction="tax";//sign the last action of the player
        openAccess();//after one action the player can do all actions again
    }
    else{
        throw runtime_error("no access for tax operation");
    }
}

/**
 * @brief The undo function allows the Governor to undo the tax action on a player.
 * It checks if the player has enough coins and if the last action was tax.
 * @param p The player on whom the tax action is being undone.
 * @throws runtime_error if the player is not active, last action is not tax, or player has less than 2 coins.
 */
void Governor::undo(Player& p){
    if(!p.isActive()){
        throw runtime_error("the player is not active anymore");
    }
    else if(!(p.lastAction=="tax")){//check if the last action of the player is tax for correctness
        throw runtime_error("the last action of  this player is not tax");

    }
    else if(p.coins()<2){//check if the player has at least 2 coins cause he cannot have less than 0 coins
        throw runtime_error("cannot undo tax on a player with less than 2 coins");
    }
    else if(p.status=="undo"){//if already has undo status, we cannot undo again
        throw runtime_error("cannot undo tax on a player who already has undo status");
    }
    else{
        p.decreaseCoins(2);//return the coins that the player get
        cout<< getName() <<"(Governor) undo tax on "<<p.getName()<<endl;
        p.status="undo";
    }

}
}