
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
    spy2.tax();//spy=3
    baron2.tax();//baron=3
    general2.tax();//general=3
    judge2.tax();//judge=3
    merchant2.tax();//Merchant=3


    for(Player* p : game_2.getPlayers()){
    cout << p->getName() << " " << p->coins() << endl;
    }

    governor2.tax();//gov=7
    spy2.tax();//spy=5
    baron2.invest();//baron=6
    general2.tax();//general=5
    judge2.tax();//judge=5
    merchant2.tax();//Merchant=6

    for(Player* p : game_2.getPlayers()){
    cout << p->getName() << " " << p->coins() << endl;
    }

    governor2.tax();//gov=10
    spy2.tax();//spy=7
    spy2.blockArrest(governor2);//spy=7

    baron2.invest();//baron=9
    baron2.bribe();//baron=5
    baron2.gather();//baron=6

    general2.tax();//general=7
    judge2.tax();//judge=7
    merchant2.tax();//Merchant=9

    for(Player* p : game_2.getPlayers()){
    cout << p->getName() << " " << p->coins() << endl;
    }

    try{
        governor2.arrest(baron2);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    governor2.coup(spy2);//gov=3, spy is offline

    try{//try to make operation with offline player
        spy2.viewCoins(governor2);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    try{//try to make arre
        baron2.arrest(governor2);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    baron2.tax();//baron=7
    general2.cancelCoup(spy2);//general=2, spy=7

    spy2.viewCoins(governor2);//now the spy can see the coins of the governor

    judge2.tax();//judge=9
    merchant2.tax();//Merchant=12

    for(Player* p : game_2.getPlayers()){
    cout << p->getName() << " " << p->coins() << endl;
    }

    //לעשות את החובה לבצע coup אם עשר מטבעות




}
