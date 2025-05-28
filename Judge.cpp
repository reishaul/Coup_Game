//reishaul1@gmail.com
#include <iostream>
#include <stdexcept>

#include "Judge.hpp"
#include "Player.hpp"

using std::string;

namespace coup{
Judge::Judge(coup::Game& game,const string& name):Player(game,name, "Judge"){};

void Judge::undo(Player& p){
    if(p.lastAction=="cancelled"){
        throw runtime_error ("Judge already cancelled this operation");
    }
    else if(p.lastAction=="bribe"){
        p.lastAction="cancelled";
        game.next_turn();//continue to the next turn
        cout<<getName() <<"("<<getRole()<<") cancelled " << p.getName() << "'s bribe operation"<<endl;
    }
    else if(p.status=="coup"){
        throw runtime_error ("the player is out of the game");
    }
    else{
        throw runtime_error ("Judge cannot undo " + p.lastAction+" operation");
    }    
}
}