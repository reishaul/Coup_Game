//reishaul1@gmail.com
//judge.hpp
#ifndef JUDGE_HPP
#define JUDGE_HPP

#include "Player.hpp"


using std::string;
namespace coup{
/**
 * @class Judge
 * @brief Represents a player with the role of Judge.
 *
 * The Judge can perform unique actions, such as undoing specific player actions.
 */
class Judge : public Player {

public:
    /**
     * @brief Constructs a Judge player with a reference to the game and the player's name.
     * @param game Reference to the Game instance.
     * @param name Name of the player.
     */
    Judge(coup::Game& game, const string& name);
    /**
     * @brief Overrides the undo function to cancel a specific player action.
     * @param p Reference to the player whose action is being undone.
     */
    void undo(Player& p)override;
    
};
}


#endif