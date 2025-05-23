//reishaul1@gmail.com
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "Merchant.hpp"
#include "Player.hpp"

using std::string;
using std::cout;
using std::endl;


namespace coup{
Merchant::Merchant(coup::Game& game, const string& name):Player(game,name, "Merchant"){};

void Merchant::merchantBonus(){
    if(coins()>=3){
        addCoins(1);
        cout << getName() << " (Merchant) got a bonus coin at the start of the turn" << endl;
    }
}

}


