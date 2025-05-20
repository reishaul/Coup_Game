#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "Player.hpp"
//#include "Game.hpp"

using std::string;

namespace coup{

class Merchant : public Player {

public:
    Merchant(coup::Game& game, const string& name);
    void merchantBonus() override;

};
}

#endif