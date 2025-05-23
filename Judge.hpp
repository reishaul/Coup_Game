//reishaul1@gmail.com
#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"
//#include "Game.hpp"

using std::string;
namespace coup{

class Judge : public Player {

public:
    Judge(coup::Game& game, const string& name);
    //void cancelBribe(Player& p);
    void undo(Player& p)override;
    
};
}


#endif