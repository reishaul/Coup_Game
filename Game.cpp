#include <iostream>
#include <string>

#include <stdexcept>
//#include "Game.hpp"
#include "Player.hpp"

using std::string;
namespace coup{

Game::Game(): is_over(false),turn_index(0){
    //is_active.resize(MAX_PLAYERS,true);
}

std::string Game::turn()const{
    if(players_pointers.empty()){
        throw std::runtime_error("there is no players left");
    }
    return players_pointers[turn_index]->getName();
}

std::vector<std::string> Game::players()const{
    std::vector<std::string> vec;

    for(const auto& player : players_pointers){
        if(player->isActive()){
            vec.push_back(player->getName());
        }
    }
    return vec;
}


std::string Game::winner() const{

    if (!is_over) {
        throw std::runtime_error("there is no winner yet");
    }

    Player* winner = nullptr;
    bool found_winner = false;
    for(Player* p:players_pointers) {
        if (p->isActive()) {
            if(found_winner) {
                throw std::runtime_error("there is no winner yet");
            }
            winner = p;
            found_winner = true;
        }
    }
    if(winner==nullptr){
        throw std::runtime_error("everyone is out of the game");
    }
    return winner->getName();
}



void Game::add_player(Player& p) {//(const string& name, Player* player)
    if(players_pointers.size()>=MAX_PLAYERS){
        throw std::runtime_error("cannot add more then six players");
    }
    //players_objects.push_back(player); // Create a new Player object and add it to the vector
    players_pointers.push_back(&p);
}

void Game::next_turn(){
    size_t num_active_players =players_pointers.size();
    if(num_active_players==0){
        throw std::runtime_error("there is no players left");
    }
    do{
        turn_index=(turn_index+1)%num_active_players;
    }while(!players_pointers[turn_index]->isActive());
}

void Game::back_turn(){
    if(turn_index==0){
        turn_index=players_pointers.size()-1;
    }
    else{
        turn_index--;
    }
}

void Game::set_active(const std::string& name, bool active) {
    for (size_t i = 0; i < players_pointers.size(); ++i) {
        if (players_pointers[i]->getName() == name) {
            players_pointers[i]->setActive(active);
            return;
        }
    }
    throw std::runtime_error("player not found for set_player_active");
}
}





 
