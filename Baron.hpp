#ifndef BARON_HPP
#define BARON_HPP

#include "Player.hpp"
#include "Game.hpp"

using std::string;
namespace coup{


class Baron: public Player{
public:
    Baron(coup::Game& game,const string& name);
    
    void invest();

};
}
#endif