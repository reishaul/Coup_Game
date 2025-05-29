//reishaul1@gmail.com
/**
 * @file Player.hpp
 * @brief Defines the Player class and its interface for the Coup game.
 */
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

/**
 * @class Player
 * @brief Represents a player in the Coup game, managing their coins, actions, and status.
 */
class Player{
protected:
    coup::Game& game;// reference to the game object to which the player belongs
    string name;
    string role;
    int numCoins=0;//to store the number of coins the player has. set to 0 at the beginning of the game
    bool active=true; //to check if the player is active in the game

public:

    /**
     * @brief Constructs a Player with game reference, name, and role.
     */
    Player(coup::Game& game, const string& name, const string& role);

    Player(const Player&) = delete;//to prevent copying player (Player p1=p2) or(p1=p1)
    Player& operator=(const Player&) = delete;


    virtual ~Player()=default;//virtual distructor to allow derived classes to clean up properly

    // Access control for player actions
    bool gatheraccess=true;
    bool taxaccess=true;
    bool coupAccess=true;
    bool arrestAccess=true;

    // To store the last action and status of the player
    string lastAction;
    string status;
    
    // Getters 
    const string& getName()const{return name;}
    const string& getRole()const{return role;}

    // Methods to manage coins and player status
    void addCoins(int n){numCoins+=n;}
    void decreaseCoins(int n);

    //setter
    void setActive(bool b){active=b;}
    bool isActive() const { return active; } //to access 
    int coins(){return numCoins;}//getter for coins

    // Methods to ovverride in derived classes
    virtual void gather();
    virtual void tax();
    virtual void bribe();
    virtual void arrest(Player& p);
    virtual void sanction(Player& p);
    virtual void coup(Player& p);

    virtual void merchantBonus(){};//special method for Merchant class to get one more coin if he has 3 or more coins at the beginning of his turn

    virtual void undo(Player& p);

    //specialized methods for Player class
    int addDistinct(Player& p);
    int substract(Player& p);

    void check_coins();//special method to check if it is time to make coup operation 
    void openAccess();//to open all access for the player after one action(for one turn)
    void isActiveAndTurn();//to check if it is the player's turn and also if he is active

    // Overloaded equality operator to compare players by name
    bool operator==(const Player& other) const{
        return this->name==other.name;
    }
    
};
}

#endif 