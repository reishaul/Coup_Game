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
    int num_of_players;
    int winner_index;
    int turn_index;// the index of the player in the cuurent turn

    int box=INT_MAX;

    std::vector<std::string>players_names;   
    std::vector<bool>is_active;// true אם השחקן עדיין במשחק
    std::vector<Player*> players_objects;//to store pointer the player object
            
public:
    Game(); 

    void add_player(const std::string& name); // ניתן להוסיף שחקן למשחק

    std::vector<std::string> players() const; 
    std::string turn() const;  
    void next_turn(); 

    bool gameOver()const{return is_over;}

    std::string winner() const;               // מחזיר את שם המנצח, או שגיאה אם המשחק לא נגמר

    static std::string lastArrest;
    void set_active(const std::string& name, bool active);

    const std::vector<Player*>& getPlayers() const { return players_objects; }
};
}

#endif
