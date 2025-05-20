#include <iostream>
#include <stdexcept>

#include "Judge.hpp"
#include "Player.hpp"

using std::string;

namespace coup{
Judge::Judge(coup::Game& game,const string& name):Player(game,name, "Judge"){};

void Judge::undo(Player& p){//יש ליישם ביטול של קבלת תור נוסף
    if(p.lastAction=="bribe"){
        p.lastAction="cancelled";
    }
    throw runtime_error ("Judge cannot undo " + p.lastAction);
}
}