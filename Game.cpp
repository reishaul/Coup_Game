#include <iostream>
#include <string>

#include <stdexcept>
//#include "Game.hpp"
#include "Player.hpp"

using std::string;
namespace coup{

Game::Game(): is_over(false),num_of_players(0),winner_index(-1),turn_index(0){
    is_active.resize(MAX_PLAYERS,true);
}

std::string Game::turn()const{
    if(players_names.empty()){
        throw std::runtime_error("there is no players left");
    }
    return players_names[turn_index];
}

std::vector<std::string> Game::players()const{
    std::vector<std::string> vec;

    for(size_t i=0; i<players_names.size(); i++){
        if(is_active[i]==true){
            vec.push_back(players_names[i]);
        }
    }
    return vec;
}


std::string Game::winner() const{

    if(winner_index==-1||!is_over){//check if the game is not over or there is not winner yet
        throw std::runtime_error("there is no winner yet");
       
    }
    return players_names[winner_index];
}



void Game::add_player(const string& name) {//(const string& name, Player* player)
    if(players_names.size()>=MAX_PLAYERS){
        throw std::runtime_error("cannot add more then six players");
    }
    //players_objects.push_back(player); // Create a new Player object and add it to the vector
    players_names.push_back(name);
    is_active.push_back(true);
}

void Game::next_turn(){
    size_t num_active_players =players_names.size();
    if(num_active_players==0){
        throw std::runtime_error("there is no players left");
    }
    do{
        turn_index=(turn_index+1)%num_active_players;
    }while(!is_active[turn_index]);
}

void Game::set_active(const std::string& name, bool active) {
    for (size_t i = 0; i < players_names.size(); ++i) {
        if (players_names[i] == name) {
            is_active[i] = active;
            return;
        }
    }
    throw std::runtime_error("player not found for set_player_active");
}
}





 
