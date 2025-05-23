//reishaul1@gmail.com
#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "Player.hpp"

using std::string;
namespace coup{

class Governor : public Player {

public:
    Governor(coup::Game& game, const string& name);
    void tax() override;
    void undo(Player& p)override;
    
};
}


#endif