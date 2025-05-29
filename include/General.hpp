//reishaul1@gmail.com
#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "Player.hpp"
#include "Game.hpp"
#include <string>
using std::string;

namespace coup{
/**
 * @brief Represents the General player role in the game.
 */
class General:public Player{
public:
    /**
     * @brief Constructs a General player with a name linked to a game.
     * @param game Reference to the game instance.
     * @param name The name of the General player.
     */
    General(coup::Game& game, const string& name);

    /**
     * @brief Cancels a coup action against the specified target player.
     * @param target The player whose coup is to be cancelled.
     */
    void cancelCoup(Player& target);

};
}
#endif 