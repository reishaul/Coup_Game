//reishaul1@gmail.com
#include <iostream>
#include <string>
#include "Player.hpp"
#include "Game.hpp"

#include <algorithm>
#include <stdexcept>

namespace coup{
    /**
     * @brief Constructs a Player with game reference, name, and role.
     */
Player::Player(coup::Game& game,const string& name,const string& role)
:game(game),name(name),role(role),numCoins(0),active(true){
    for(auto& p:game.getPlayers()){
        if(p->getName()==name){//not permit to create a player with the same name
            throw runtime_error("This name is already taken- please choose another one");
        }
    }
    if(name.empty()){//not permit to create a player with empty name
        throw runtime_error("the name of the player cannot be empty");
    }
    if(role.empty()){//not permit to create a player with empty role
        throw runtime_error("the role of the player cannot be empty");
    }
    if(role!="Governor" && role!="Spy" && role!="Baron" && role!="General" && role!="Judge" && role!="Merchant"){//only these roles are valid
        throw runtime_error("the role of the player is not valid");
    }
    game.add_player(*this);
}


/**
 * @brief methode to decrease the number of coins the player has.
 * @param n The number of coins to decrease.
 * @throws runtime_error if the player does not have enough coins to decrease.
 */
void Player::decreaseCoins(int n){
    if(n>numCoins){
        throw runtime_error("the player dosen't have enough coins");
    }
    numCoins-=n;
}

/**
 * @brief Method to gather coins.(get one coine from the bank)
 * * This method checks if the player is active and it's their turn.
 * * If the player has access to gather, they receive one coin and the turn passes to the next player.
 * * If the player does not have access to gather, an exception is thrown.
 * * @throws runtime_error if the player is not active, not their turn, or does not have access to gather.
 * 
 */
void Player::gather(){
    isActiveAndTurn();//check if it is the player turn and also if he is active
    check_coins();//for coup check
    
    if(gatheraccess){
        merchantBonus();//if the player is merchant he get one coin for free 
        numCoins=numCoins+1;
        lastAction="gather";//sign
        game.next_turn();//continue to the next player
        cout<< getName() <<"("<<getRole()<<") took one coin by using gather operation"<<endl;
        openAccess();//after one action the player can do all actions again
    }
    else{
        throw runtime_error("no access for gather operation");
    }
}


/**
 * @brief Method to perform a tax operation.
 * * This method checks if the player is active and it's their turn.
 * * If the player has access to tax, they receive two coins and the turn passes to the next player.
 * * If the player does not have access to tax, an exception is thrown.
 * * @throws runtime_error if the player is not active, not their turn, or does not have access to tax.
 */
void Player::tax(){
    isActiveAndTurn();//check if it is the player turn and also if he is active
    check_coins();//for coup check

    if(taxaccess){//not in sanction or something like that
        merchantBonus();
        numCoins=numCoins+2;//this operation give the player two coins from the bank
        lastAction="tax";
        game.next_turn();
        cout<< getName() <<"("<<getRole()<<") took two coins by using tax operation"<<endl;
        openAccess();//after one action the player can do all actions again
    }
    else{
        throw runtime_error("no access for tax operation");
    }
}


/**
 * @brief Method to perform a bribe operation. after this operation the player get one more turn
 * * This method checks if the player is active and it's their turn.
 * * If the player has at least 4 coins, they can perform a bribe operation, which costs 4 coins.
 * * If the player does not have enough coins or it is not their turn, an exception is thrown.
 * * @throws runtime_error if the player is not active, not their turn, or does not have enough coins for the bribe operation.
 */
void Player::bribe(){

    if(!isActive()){//check if the player is active
        throw runtime_error("cannot perform bribe- the player is not active");
    }
    check_coins();//check if the player have 10 or nore coins
    merchantBonus();

    if(numCoins<4){//this operation requires 4 coins
        throw runtime_error("the player dosen't have enough coins for bribe operation");
    }
    else if(game.turn()!=this->name){
        numCoins=numCoins-4;
        lastAction="bribe";//sign
        game.back_turn();//instesd of next turn we go back to the previous player
        cout<< getName() <<"("<<getRole()<<") do a bribe operation "<<endl;
    }
    else{
        throw runtime_error("this is already the player's turn");
    }
}

/**
 * @brief Method to perform an arrest operation against another player.
 * * This method checks if the player is active and it's their turn.
 * * If the player has access to arrest, they can arrest another player, taking one coin from them.
 * * If the target player does not have enough coins or has already been arrested in the last turn, an exception is thrown.
 * * @param p The player to be arrested.
 * @throws runtime_error if the player is not active, not their turn, does not have access to arrest, or if the target player does not have enough coins.
 */
void Player::arrest(Player& p){
    isActiveAndTurn();
    check_coins();//for coup check

    if(!arrestAccess){//check if the player has access to arrest
        throw runtime_error("no access for arrest operation");
    }
    else if(!p.isActive()){//check if the target player is active
        throw runtime_error("the target player is already out of the game");
    }
    else if(p.coins()<1){//check if the target player has enough coins
        throw runtime_error("the target player dosen't have enough coins, please choose another player");
    }
    else if(game.getLastArrest()!=p.getName()){//necessary to avoid arresting the same player two times in a row
        merchantBonus();//if the player is merchant he get one coin
        if(p.getRole()=="Merchant"){p.decreaseCoins(2);}//merchent pay 2 coins to the bank instead of lose one to the perforner player
        
        //if it is general he get back his coin back
        else if(p.getRole()=="General"){numCoins+=1;}
        else{
            p.decreaseCoins(1);
            numCoins+=1;
        }
        lastAction="arrest";//sign
        p.status="arrest";
        game.setLastArrest(p.getName());//set the new last arrested player
        cout<< getName() <<"("<<getRole()<<") performed an arrest operation against "<<p.getName()<<"("<<p.getRole()<<")"<<endl;
        game.next_turn();
        openAccess();
    }
    else{
        throw runtime_error("cannot arrest the same player two times in a row");
    }
}

/**
 * @brief Method to perform a sanction operation against another player.
 * * This method checks if the player is active and it's their turn.
 * * If the player has at least 3 coins, they can perform a sanction operation, which costs 3 coins.
 * * The target player loses access to gather and tax operations for the next turn.
 * * If the target player does not have enough coins or is already out of the game, an exception is thrown.
 * * @param p The player to be sanctioned.
 * @throws runtime_error if the player is not active, not their turn, does not have enough coins for the sanction operation, or if the target player is already out of the game.
 */
void Player::sanction(Player& p){
    isActiveAndTurn();//check if it is the player turn and also if he is active
    check_coins();//for coup check
    merchantBonus();
    if(numCoins<3){//this operation requires 3 coins
        throw runtime_error("the player dosen't have enough coins to perform sanction operation");
    }
    else if(!p.isActive()){//check if the target player is active
        throw runtime_error("the target player is already out of the game");
    }

    //block accesse to finance operations
    p.gatheraccess=false;
    p.taxaccess=false;
    
    p.addCoins(addDistinct(p));//Baron on sanction get one coin as a compensation and the other no
    numCoins=numCoins-substract(p);

    lastAction="sanction";//sign
    p.status="sanction";
    game.next_turn();
    cout<< getName() <<"("<<getRole()<<") performed a sanction operation against "<<p.getName()<<"("<<p.getRole()<<")"<<endl;
    openAccess();//after one action the player can do all actions again
}


/**
 * @brief Method to perform a coup operation against another player.
 * * This method checks if the player is active and it's their turn.
 * * If the player has access to coup, they can perform a coup operation, which costs 7 coins.
 * * The target player is set to inactive.
 * * If the target player does not have enough coins or is already out of the game, an exception is thrown.
 * * @param p The player to be couped.
 * @throws runtime_error if the player is not active, not their turn, does not have enough coins for the coup operation, or if the target player is already out of the game.
 */
void Player::coup(Player& p){
    isActiveAndTurn();
    if(coupAccess){
        if (!p.active) {//check if the target player is active
            throw runtime_error("the player is already out of the game");
        }

        merchantBonus();//if the player is merchant he get one coin for free
        if(numCoins<7){//this operation requires 7 coins
            throw runtime_error("the player doesn't have enough coins for a coup operation");
        }

        numCoins=numCoins-7;//decrease the number of coins the player has
        p.setActive(false);//make the target offline
        
        lastAction="coup";
        p.status="coup";//set the status of the target player to coup
        
        game.next_turn();//continue to the next player
        cout<< getName() <<"("<<getRole()<<") performed a coup against "<<p.getName()<<"("<<p.getRole()<<")"<<endl;
        openAccess();
        
    }
    else{
        throw runtime_error("no access for coup operation");
    }
}

/**
 * @brief Method to add a distinct value based on the player's role.
 * * If the player is a Baron, it returns 1; otherwise, it returns 0.
 * @param p The player whose role is checked.
 * @return 1 if the player is a Baron, otherwise 0.
 */
int Player::addDistinct(Player& p){
    if(p.getRole()=="Baron"){return 1;}
    else{return 0;}
}

/**
 * @brief Method to subtract a value based on the player's role.
 * * If the player is a Judge, it returns 4; otherwise, it returns 3.
 * @param p The player whose role is checked.
 * @return 4 if the player is a Judge, otherwise 3.
 */
int Player::substract(Player& p){
    if(p.getRole()=="Judge"){
        return 4;
    }
    else{return 3;}
}

/**
 * @brief Method to undo the last action of the player.
 * * This method is implemented for the Player class who only have this operation
 * the methode will throw an exception if called.
 * @param p The player whose action is to be undone.
 * @throws runtime_error indicating that this role does not have the ability to undo actions.
 */
void Player::undo(Player& p){
    cout<<"this role diden't have the ability to undo";
}

/**
 * @brief Method to check if the player has 10 or more coins.
 * * If the player has 10 or more coins, an exception is thrown indicating that a coup operation must be made.
 * @throws runtime_error if the player has 10 or more coins.
 */
void Player::check_coins(){
    if(coins()>=10){
        throw runtime_error("You are must make a coup operation");
    }
}

/**
 * @brief Method to open access for the player for the next turn.
 * * This method sets all access flags (arrestAccess, taxaccess, gatheraccess) to true,
 * allowing the player to perform all actions in the next turn.
 */
void Player::openAccess(){
    arrestAccess=true;
    taxaccess=true;
    gatheraccess=true;
}

/**
 * @brief Method to check if it is the player's turn and if the player is active.
 * * If it is not the player's turn, an exception is thrown indicating that it is not their turn.
 * * If the player is not active, an exception is thrown indicating that the player is not active.
 * @throws runtime_error if it is not the player's turn or if the player is not active.
 */
void Player::isActiveAndTurn(){
    if(!(game.turn()==this->name)){
    std::string role_lower = this->role;
    std::transform(role_lower.begin(), role_lower.end(), role_lower.begin(), ::tolower);//for print in lower case
    throw std::runtime_error("Not " + role_lower + "'s turn");
    }
    else if(!isActive()){
        throw std::runtime_error("Cannot make operation-the player is not active");
    }
}

}



