//reishaul1@gmail.com
#include <iostream>
#include <string>

#include <stdexcept>
#include "Player.hpp"

using std::string;
namespace coup{

Game::Game(): turn_index(0){}
// Game::Game(): is_over(false),turn_index(0){
// }

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
        throw std::runtime_error("All players are out of the game");
    }
    return winner->getName();
}


void Game::add_player(Player& p) {//(const string& name, Player* player)
    if(players_pointers.size()>=MAX_PLAYERS){
        throw std::runtime_error("cannot add more then six players");
    }
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


// void Game::next_turn() {
//     size_t total_players = players_pointers.size();
//     if (total_players == 0) {
//         throw std::runtime_error("there are no players left in the game");
//     }

//     size_t attempts = 0;
//     do {
//         turn_index = (turn_index + 1) % total_players;
//         attempts++;
//     } while (!players_pointers[turn_index]->isActive() && attempts < total_players);

//     if (!players_pointers[turn_index]->isActive()) {
//         throw std::runtime_error("no active players left to take turn");
//     }
// }

}





 
