//reishaul1@gmail.com
/**
 * @file Judge.cpp
 * @brief Implementation of the Judge role in the Coup game.
 */
#include <iostream>
#include <stdexcept>

#include "Judge.hpp"
#include "Player.hpp"

using std::string;

namespace coup{
/**
 * @brief Constructs a Judge player with a given name and links them to the game.
 * @param game Reference to the Game object.
 * @param name The player's name.
 */
Judge::Judge(coup::Game& game,const string& name):Player(game,name, "Judge"){};

/**
 * @brief Cancels a bribe action previously taken by another player.
 * The Judge can only undo the "bribe" action. If the action was already cancelled,
 * or if it's not a bribe, the operation is rejected.
 * @param p The player whose action is to be undone.
 * @throws std::runtime_error if the action is invalid or already undone.
 */
void Judge::undo(Player& p){
    if(p.lastAction=="cancelled"){
        throw runtime_error ("Judge already cancelled this operation");
    }
    else if(p.lastAction=="bribe"){//check if the last action of the player is bribe
        p.lastAction="cancelled";
        game.next_turn();//continue to the next turn
        cout<<getName() <<"("<<getRole()<<") cancelled " << p.getName() << "'s bribe operation"<<endl;
    }
    else if(p.status=="coup"){//neccessary condition for special case
        throw runtime_error ("the player is out of the game");
    }
    else{
        throw runtime_error ("Judge cannot undo " + p.lastAction+" operation");
    }    
}
}