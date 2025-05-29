//reishaul1@gmail.com
/**
 * @file Game.hpp
 * @brief Declares the Game class for managing the Coup game logic.
 */
#ifndef GAME_HPP
#define GAME_HPP

#include <climits> 
#include <string>
#include <vector>


namespace coup {
class Player;

const int MAX_PLAYERS = 6;

/**
 * @class Game
 * @brief Manages the game state, player turns, and tracks game progress.
 */
class Game {
private:

    int turn_index;// the index of the player in the cuurent turn
    std::vector<Player*> players_pointers;//to store pointer the player object
    std::string lastArrest;// to store the name of the last player that was arrested

public:
    Game(); 

    /**
     * @brief Adds a player to the game.
     * @param p Reference to the player to add.
     */
    void add_player(Player& p); 

    /**
     * @brief Returns the names of the current active players.
     * @return A vector of player names.
     */
    std::vector<std::string> players() const; 

    /**
     * @brief Returns the name of the player whose turn it is.
     * @return Name of the current player.
     */
    std::string turn() const; 

    void next_turn(); //go to the next player that is active
    void back_turn();//return to the previous player that is active

    /**
     * @brief Returns the name of the winning player.
     * @throws std::runtime_error if the game is not finished.
     */
    std::string winner() const;

    const std::vector<Player*>& getPlayers() const { return players_pointers; }//Returns all player pointers.
    std::string getLastArrest() const { return lastArrest; }//Gets the name of the last arrested player
    void setLastArrest(const std::string& n) { lastArrest = n; }//Sets the name of the last arrested player.
};
}

#endif
