//reishaul1@gmail.com
#include <iostream>
#include <stdexcept>
#include "General.hpp"
#include "Player.hpp"


using std::string;

namespace coup{
/**
 * @brief Constructs a General player with a name linked to a game.
 * @param game Reference to the game instance.
 * @param name The name of the General player.
 */
General::General(coup::Game& game, const string& name):Player(game, name, "General"){};

/**
 * @brief Cancels a coup action against the specified target player.
 * @param target The player whose coup is to be cancelled.
 * @throws runtime_error if the General does not have enough coins or if the target player is already active.
 */
void General::cancelCoup(Player& target){

    if(coins()<5){//this operation requires 5 coins
        throw runtime_error("cannot cancel coup there is not enough coins");
    }
    else if(!target.isActive()){//if the target player is not active, we can cancel the coup

        target.setActive(true);//change the target player to active
        decreaseCoins(5);
        cout<<getName()<<"("<<getRole()<<") cancelled the coup on "<<target.getName()<<"("<<target.getRole()<<")"<<endl;
    }
    else{
        throw runtime_error("cannot cancel coup on active player");
    }

}
}



