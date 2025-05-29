//reishaul1@gmail.com
//governor.hpp
#ifndef GOVERNOR_HPP
#define GOVERNOR_HPP

#include "Player.hpp"

using std::string;
namespace coup{
/**
 * @brief Represents the Governor player role in the game.
 */
class Governor : public Player {

public:
    /**
     * @brief Constructs a Governor player with a reference to the game and the player's name.
     * @param game Reference to the game instance.
     * @param name Name of the player.
     */
    Governor(coup::Game& game, const string& name);
    void tax() override;// Overrides the tax method to implement the Governor's tax action.
    void undo(Player& p)override;//implements the undo action for the Governor
    
};
}


#endif