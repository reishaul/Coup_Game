//reishaul1@gmail.com
#include <iostream>
#include <stdexcept>
#include "General.hpp"
#include "Player.hpp"


using std::string;

namespace coup{
General::General(coup::Game& game, const string& name):Player(game, name, "General"){};

void General::cancelCoup(Player& target){

    if(coins()<5){
        throw runtime_error("cannot cancel coup there is not enough coins");
    }
    else if(!target.isActive()){

        target.setActive(true);
        decreaseCoins(5);
        cout<<getName()<<"("<<getRole()<<") cancelled the coup on "<<target.getName()<<"("<<target.getRole()<<")"<<endl;
    }
    else{
        throw runtime_error("cannot cancel coup on active player");
    }

}
}



