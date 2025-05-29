//reishaul1@gmail.com
/**
 * @file Spy.hpp
 * @brief Declaration of the Spy class, a special role in the Coup game.
 */
#ifndef SPY_HPP
#define SPY_HPP

#include "Player.hpp"

namespace coup{
/**
 * @class Spy
 * @brief Represents a Spy player with special abilities in the Coup game.
 */
class Spy : public Player{

private:
int enemyCoins;//Stores the number of coins seen from the target player

public:
    /**
     * @brief Constructs a Spy with a reference to the game and a name.
     * @param game Reference to the Game object.
     * @param name Name of the player.
     */
    Spy(coup::Game& game, const string& name);

    /**
     * @brief Blocks the arrest action from another player.
     * @param p Reference to the player whose arrest action is need to block.
     */
    void blockArrest(Player& p);

    /**
     * @brief Views the number of coins of another player.
     * @param p Reference to the target player.
     */
    void viewCoins(Player& p);
};
}

#endif


