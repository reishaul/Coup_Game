//reishaul1@gmail.com

/**
 * @file Game.cpp
 * @brief Implements the Game class methods for managing player turns and game flow.
 */
#include <iostream>
#include <string>

#include <stdexcept>
#include "Player.hpp"

using std::string;
namespace coup{
/**
 * @brief Constructs a new Game object with initial turn index set to 0.
 */
Game::Game(): turn_index(0){}

/**
 * @brief Gets the name of the player whose turn it is.
 * @return The current player's name.
 * @throws std::runtime_error if there are no players.
 */
std::string Game::turn()const{
    if(players_pointers.empty()){// Check if there are no players in the game
        throw std::runtime_error("there is no players left");
    }
    return players_pointers[turn_index]->getName();
}

/**
 * @brief Returns a list of names of active players.
 * @return A vector of active player names.
 */
std::vector<std::string> Game::players()const{
    std::vector<std::string> vec;

    for(const auto& player : players_pointers){
        if(player->isActive()){
            vec.push_back(player->getName());// Add the name of the active player to the vector
        }
    }
    return vec;
}

/**
 * @brief Returns the name of the winning player.
 * @return The winner's name.
 * @throws std::runtime_error if more than one player is still active or all are out.
 */
std::string Game::winner() const{
    Player* winner = nullptr;// Initialize winner pointer to nullptr
    bool found_winner = false;//sign that we found a winner
    for(Player* p:players_pointers) {
        if (p->isActive()) {
            if(found_winner) {
                throw std::runtime_error("there is no winner yet");
            }
            winner = p;// Set the winner pointer to the first active player found
            found_winner = true;
        }
    }
    if(winner==nullptr){
        throw std::runtime_error("All players are out of the game");
    }
    return winner->getName();
}

/**
 * @brief Adds a new player to the game.
 * @param p Reference to the player to add.
 * @throws std::runtime_error if the player limit (6) is exceeded.
 */
void Game::add_player(Player& p) {//(const string& name, Player* player)
    if(players_pointers.size()>=MAX_PLAYERS){
        throw std::runtime_error("cannot add more then six players");
    }
    players_pointers.push_back(&p);
}

/**
 * @brief Advances the game to the next active player's turn.
 * @throws std::runtime_error if no players are available.
 */
void Game::next_turn(){
    size_t num_active_players =players_pointers.size();
    if(num_active_players==0){// Check if there are no active players
        throw std::runtime_error("there is no players left");
    }
    do{// Loop until we find the next active player
        turn_index=(turn_index+1)%num_active_players;
    }while(!players_pointers[turn_index]->isActive());
}

/**
 * @brief Moves the turn back to the previous player.
 */
void Game::back_turn(){
    if(turn_index==0){// If the current turn index is 0, wrap around to the last player
        turn_index=players_pointers.size()-1;
    }
    else{
        turn_index--;
    }
}

}





 
