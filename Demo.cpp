
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Game.hpp"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
using namespace std;//לעשות יוזינג סטידי וקואופ גם בגוי
using namespace coup;

//לא לשכוח לעשות copy constructor ואופרטור השמה
int main() {
    Game game_1{};

    Governor governor(game_1, "Moshe");
    Spy spy(game_1, "Yossi");
    Baron baron(game_1, "Meirav");
    General general(game_1, "Reut");
    Judge judge(game_1, "Gilad");

    vector<string> players = game_1.players();
    
    // Expected output:
    // Moshe
    // Yossi
    // Meirav
    // Reut
    // Gilad
    for(string name : players){
        cout << name << endl;
    }

    // Expected output: Moshe
    cout << game_1.turn() << endl;

    governor.gather();
    spy.gather();
    baron.gather();
    general.gather();
    judge.gather();

    // Expected exception - Not spy's turn
    try{
        spy.gather();
    } catch (const std::exception &e){
        std::cerr << e.what() << '\n';
    }

    governor.gather();
    spy.tax();

    // Expected exception - Judge cannot undo gather
    try{
        judge.undo(governor);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    cout << governor.coins() << endl; // Expected: 2
    cout << spy.coins() << endl; // Expected: 3

    governor.undo(spy); // Governor undo tax
    cout << spy.coins() << endl; // Expected: 1

    baron.tax();
    general.gather();
    judge.gather(); 

    governor.tax();
    spy.gather();
    baron.invest(); // Baron traded its 3 coins and got 6 
    general.gather();
    judge.gather();
    
    cout << baron.coins() << endl; // Expected: 6

    governor.tax();
    spy.gather();
    baron.gather();
    general.gather();
    judge.gather();

    governor.tax();
    spy.gather();
    cout << baron.coins() << endl; // Expected: 7
    baron.coup(governor); // Coup against governor
    
    general.gather();
    judge.gather();
    
    players = game_1.players();
    // Since no one blocked the Baron, the expected output is:
    // Yossi
    // Meirav
    // Reut
    // Gilad
    for (string name : players) {
        cout << name << endl;
    }

    // ADDITIONS
    cout<<"starting the second game\n"<<endl;
    Game game_2{};

    Governor governor2(game_2, "Haniel");
    Spy spy2(game_2, "Mom");
    Baron baron2(game_2, "Dad");
    General general2(game_2, "Dagan");
    Judge judge2(game_2, "Hadar");
    Merchant merchant2(game_2, "Ron");

    vector<string> players2 = game_2.players();
    
    governor2.gather();//gov=1
    spy2.gather();//spy=1
    baron2.gather();//baron=1
    general2.gather();//general=1
    judge2.gather();//judge=1
    merchant2.gather();//Merchant=1

    cout << "coins of the players after gathering" << endl;
    for(Player* p : game_2.getPlayers()){
        cout << p->getName() << " " << p->coins() << endl;
    }
    

    governor2.tax();//gov=4
    spy2.tax();//spy=1
    baron2.tax();//baron=1
    general2.tax();//general=1
    judge2.tax();//judge=1
    merchant2.tax();//Merchant=1

}
