
#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "Game.hpp"

#include <iostream>
#include <string>
#include <stdexcept>
using std::string;
using std::runtime_error;
using std::cout;
using std::endl;

namespace coup{
class Game;

class Player{
protected:
    coup::Game& game;
    string name;
    string role;
    int numCoins;
    bool active;

public:
    Player(coup::Game& game, const string& name, const string& role);  // 

    virtual ~Player()=default;

    bool gatheraccess=true;
    bool taxaccess=true;
    bool coupAccess=true;
    bool arrestAccess=true;

    string lastAction;
    string status;
    

    const string& getName()const{return name;}
    const string& getRole()const{return role;}
    void addCoins(int n){numCoins+=n;}
    void decreaseCoins(int n);

    void setActive(bool b){active=b;}
    bool isActive() const { return active; } //to access 
    int coins(){return numCoins;}

    virtual void gather();
    virtual void tax();
    virtual void bribe();
    virtual void arrest(Player& p);
    virtual void sanction(Player& p);
    virtual void coup(Player& p);

    virtual void merchantBonus(){};

    virtual void undo(Player& p);

    int addDistinct(Player& p);
    int substract(Player& p);

    void check_coins();
    void openAccess();
    void isActiveAndTurn();
    

};
}

#endif 