#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "Player.hpp"
#include "Game.hpp"
#include <string>
using std::string;

namespace coup{
class General:public Player{
public:
    General(coup::Game& game, const string& name);
    void cancelCoup(Player& p, Player& target);

};
}
#endif 