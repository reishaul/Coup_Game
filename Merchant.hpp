//reishaul1@gmail.com
/**
 * @file Merchant.hpp
 * @brief Declaration of the Merchant class, a special role in the Coup game.
 */
#ifndef MERCHANT_HPP
#define MERCHANT_HPP

#include "Player.hpp"

using std::string;

namespace coup{
/**
 * @class Merchant
 * @brief Represents a player with the Merchant role.
 * The Merchant has a unique ability to receive a bonus coin at every turn.
 */
class Merchant : public Player {

public:
    /**
     * @brief Constructs a Merchant player.
     * @param game Reference to the Game object.
     * @param name The name of the player.
     */
    Merchant(coup::Game& game, const string& name);

    /**
     * @brief Grants a bonus to the Merchant.
     */
    void merchantBonus() override;

};
}

#endif