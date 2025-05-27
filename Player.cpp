//reishaul1@gmail.com
#include <iostream>
#include <string>
#include "Player.hpp"
#include "Game.hpp"


#include <algorithm>
#include <stdexcept>

namespace coup{
Player::Player(coup::Game& game,const string& name,const string& role)
:game(game),name(name),role(role),numCoins(0),active(true){
    for(auto& p:game.getPlayers()){
        if(p->getName()==name){
            throw runtime_error("This name is already taken- please choose another one");
        }
    }
    if(name.empty()){
        throw runtime_error("the name of the player cannot be empty");
    }
    if(role.empty()){
        throw runtime_error("the role of the player cannot be empty");
    }
    if(role!="Governor" && role!="Spy" && role!="Baron" && role!="General" && role!="Judge" && role!="Merchant"){
        throw runtime_error("the role of the player is not valid");
    }
    game.add_player(*this);
}

void Player::decreaseCoins(int n){
    if(n>numCoins){
        throw runtime_error("the player dosen't have enough coins");
    }
    numCoins-=n;
}

    
void Player::gather(){
    isActiveAndTurn();//check if it is the player turn and also if he is active
    check_coins();
    
    if(gatheraccess){
        merchantBonus();
        numCoins=numCoins+1;
        lastAction="gather";
        game.next_turn();
        cout<< getName() <<"("<<getRole()<<") took one coin by using gather operation"<<endl;
        openAccess();
    }
    else{
        throw runtime_error("no access for gather operation");
    }
}

void Player::tax(){
    isActiveAndTurn();
    check_coins();

    if(taxaccess){
        merchantBonus();
        numCoins=numCoins+2;
        lastAction="tax";
        game.next_turn();
        cout<< getName() <<"("<<getRole()<<") took two coins by using tax operation"<<endl;
        openAccess();
    }
    else{
        throw runtime_error("no access for tax operation");
    }
}

void Player::bribe(){

    if(!isActive()){
        throw runtime_error("cannot perform bribe- the player is not active");
    }
    check_coins();//check if the player have 10 or nore coins
    merchantBonus();

    if(numCoins<4){
        throw runtime_error("the player dosen't have enough coins for bribe operation");
    }
    else if(game.turn()!=this->name){
        numCoins=numCoins-4;
        lastAction="bribe";
        game.back_turn();
        cout<< getName() <<"("<<getRole()<<") do a bribe operation "<<endl;
    }
    else{
        throw runtime_error("this is already the player's turn");
    }
}

void Player::arrest(Player& p){
    isActiveAndTurn();
    check_coins();

    if(!arrestAccess){
        throw runtime_error("no access for arrest operation");
    }
    else if(!p.isActive()){
        throw runtime_error("the target player is already out of the game");
    }
    else if(game.getLastArrest()!=p.getName()){
        merchantBonus();//if the player is merchant he get one coin
        if(p.getRole()=="Merchant"){p.decreaseCoins(2);}
        
        //if it is general he get back his coin back
        else if(p.getRole()=="General"){numCoins+=1;}
        else{
            p.decreaseCoins(1);
            numCoins+=1;
        }
        lastAction="arrest";
        p.status="arrest";
        game.setLastArrest(p.getName());
        cout<< getName() <<"("<<getRole()<<") performed an arrest operation against "<<p.getName()<<"("<<p.getRole()<<")"<<endl;
        game.next_turn();
        openAccess();
    }
    else{
        throw runtime_error("cannot arrest the same player two times in a row");
    }
}

void Player::sanction(Player& p){
    isActiveAndTurn();
    check_coins();
    merchantBonus();
    if(numCoins<3){
        throw runtime_error("the player dosen't have enough coins to perform sanction operation");
    }

    p.gatheraccess=false;
    p.taxaccess=false;
    
    p.addCoins(addDistinct(p));//Baron on sanction get one coin as a compensation and the other no
    numCoins=numCoins-substract(p);

    lastAction="sanction";
    p.status="sanction";
    game.next_turn();
    cout<< getName() <<"("<<getRole()<<") performed a sanction operation against "<<p.getName()<<"("<<p.getRole()<<")"<<endl;
    openAccess();
}

void Player::coup(Player& p){
    isActiveAndTurn();
    if(coupAccess){
        // int a = ((game.turn_index)+1)%(game.players_pointers.size());
        // string name=game.players_pointers[a]->getName();

        if (!p.active) {
            throw runtime_error("the player is already out of the game");
        }

        merchantBonus();
        if(numCoins<7){
            throw runtime_error("the player doesn't have enough coins for a coup operation");
        }

        numCoins=numCoins-7;
        p.setActive(false);
        
        lastAction="coup";
        p.status="coup";
        
        
        game.next_turn();
        //game.back_turn();
        cout<< getName() <<"("<<getRole()<<") performed a coup against "<<p.getName()<<"("<<p.getRole()<<")"<<endl;
        openAccess();
        
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

/*We must check in the begining of each turn if the player has 10 coins-
cause if yes he must perform a coup operation*/
void Player::check_coins(){
    if(coins()>=10){
        throw runtime_error("You are must make a coup operation");
    }
}

void Player::openAccess(){//this fuction create to open the access to the player for the next turn
    arrestAccess=true;
    taxaccess=true;
    gatheraccess=true;
}

void Player::isActiveAndTurn(){
    if(!(game.turn()==this->name)){
    std::string role_lower = this->role;
    std::transform(role_lower.begin(), role_lower.end(), role_lower.begin(), ::tolower);
    throw std::runtime_error("Not " + role_lower + "'s turn");
    }
    else if(!isActive()){
        throw std::runtime_error("Cannot make operation-the player is not active");
    }
}

}



