#ifndef GAME_HPP
#define GAME_HPP

#include <climits> 
#include <string>
#include <vector>


namespace coup {
class Player;

const int MAX_PLAYERS = 6;

class Game {
private:
    bool is_over;  
    //int num_of_players;
    //int winner_index;
    int turn_index;// the index of the player in the cuurent turn
    std::vector<Player*> players_pointers;//to store pointer the player object
    std::string lastArrest;

public:
    Game(); 

    void add_player(Player& p); // ניתן להוסיף שחקן למשחק

    std::vector<std::string> players() const; 
    std::string turn() const;  
    void next_turn(); //go to the next turn
    void back_turn();//return to the previous turn

    bool gameOver()const{return is_over;}

    std::string winner() const;               // מחזיר את שם המנצח, או שגיאה אם המשחק לא נגמר
    
    ///void set_active(const std::string& name, bool active);

    const std::vector<Player*>& getPlayers() const { return players_pointers; }
    std::string getLastArrest() const { return lastArrest; }
    void setLastArrest(const std::string& n) { lastArrest = n; }
};
}

#endif
