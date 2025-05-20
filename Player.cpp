//reishaul1@gmail.com
#include <iostream>
#include <string>
#include "Player.hpp"
//#include "Game.hpp"

#include <algorithm>
#include <stdexcept>

namespace coup{
Player::Player(coup::Game& game,const string& name,const string& role)
:game(game),name(name),role(role),numCoins(0),active(true){
    game.add_player(name);
}

void Player::decreaseCoins(int n){
    if(n>numCoins){
        throw runtime_error("the player dosen't have enough coins");
    }
    numCoins-=n;
}

    
void Player::gather(){
    
    if(!(game.turn()==this->name)){
        std::string role_lower = this->role;
        std::transform(role_lower.begin(), role_lower.end(), role_lower.begin(), ::tolower);
        //throw runtime_error("Not " + this->role + "'s turn");
        throw std::runtime_error("Not " + role_lower + "'s turn");
    }
    
    if(gatheraccess){
        merchantBonus();
        numCoins=numCoins+1;
        lastAction="gather";
        game.next_turn();
        cout<< getName() <<"("<<getRole()<<") took one coin by using gather operation"<<endl;
    }
    else{
        throw runtime_error("no access for gather operation");
    }
}

void Player::tax(){
    if(taxaccess){
        merchantBonus();
        numCoins=numCoins+2;
        lastAction="tax";
        game.next_turn();
        cout<< getName() <<"("<<getRole()<<") took two coins by using tax operation"<<endl;
    }
    else{
        throw runtime_error("no access for tax operation");
    }
}

void Player::bribe(){
    merchantBonus();
    if(numCoins<4){
        throw runtime_error("the player dosen't have enough coins for bribe operation");
    }
    numCoins=numCoins-4;
    lastAction="bribe";
    //game.next_turn();//without this line the turn is not continue
    
}

void Player::arrest(Player& p){
    if(arrestAccess){
        if(p.active){
            if(p.lastArrest!=p.getName()){
                merchantBonus();
                if(p.getRole()=="Merchant"){
                    p.decreaseCoins(2);
                }
                else{
                    p.numCoins--;
                    numCoins+=1;
                }
                lastAction="arrest";
                p.status="arrest";
                game.next_turn();

            }
            else throw runtime_error("cannot perform this operation two times constantly");
        }
        else throw runtime_error("the player is not active anymore");
    }
    throw runtime_error("there is no access to perform this operation");
}

void Player::sanction(Player& p){
    merchantBonus();
    if(numCoins<3){
        throw runtime_error("the player dosen't have enough coins for bribe operation");
    }
    else if(p.active){
        p.gatheraccess=false;
        p.taxaccess=false;
        
        p.addCoins(addDistinct(p));//Baron on sanction get one coin as a compensation and the other no
        numCoins=numCoins-substract(p);

        lastAction="sanction";
        p.status="sanction";
        game.next_turn();
        
    }
    else throw runtime_error("the player is not active anymore");
}

void Player::coup(Player& p){
    if(coupAccess){
        merchantBonus();
        if(numCoins<7){
            throw runtime_error("the player doesn't have enough coins for a coup operation");
        }
        if (!p.active) {
            throw runtime_error("the player is already out of the game");
        }
        numCoins=numCoins-7;
        p.active=false;//לשאול האם צריך גם את השורה הזו?
        p.game.set_active(p.getName(), false);//the player isn't active anymore
        
        lastAction="coup";
        p.status="coup";
        game.next_turn();
        cout<< getName() <<"("<<getRole()<<") performed a coup against "<<p.getRole()<<endl;
    }
    else{
        throw runtime_error("no access for coup operation");
    }
}

int Player::addDistinct(Player& p){
    if(p.getRole()=="Baron"){return 1;}
    else{return 0;}
}

int Player::substract(Player& p){
    if(p.getRole()=="Judge"){
        return 4;
    }
    else{return 3;}
}

void Player::undo(Player& p){
    cout<<"this role diden't have the ability to undo";
}


}



