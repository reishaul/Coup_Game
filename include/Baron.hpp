//reishaul1@gmail.com
//baron.hpp
#ifndef BARON_HPP
#define BARON_HPP

#include "Player.hpp"
#include "Game.hpp"

using std::string;
namespace coup{

/**
 * @brief Represents the Baron player role in the game.
 */
class Baron: public Player{
public:
    /**
     * @brief Constructs a Baron player with a reference to the game and the player's name.
     * @param game Reference to the game instance.
     * @param name Name of the player.
     */
    Baron(coup::Game& game,const string& name);
    /**
     * @brief Performs the invest action for the Baron.
     */
    void invest();
};
}
#endif