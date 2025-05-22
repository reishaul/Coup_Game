
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

    //cout<<"the turn is: "<<game_1.turn()<<endl;
    
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
    spy2.blockArrest(governor2);

    baron2.invest();//baron=9
    baron2.bribe();//baron=5
    baron2.gather();//baron=6

    general2.tax();//general=7
    judge2.tax();//judge=7
    merchant2.tax();//Merchant=9

    for(Player* p : game_2.getPlayers()){
    cout << p->getName() << " " << p->coins() << endl;
    }

    try{//try not to make coup with player that have 10 coins
        governor2.arrest(baron2);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    governor2.coup(spy2);//gov=2, spy is offline

    try{//try to make operation with offline player
        spy2.viewCoins(governor2);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    try{//try to make arrest twice on the same player
        baron2.arrest(governor2);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    cout<<"the turn:"<<game_2.turn()<<endl;

    baron2.tax();//baron=7

    cout<<"spy2 is active: "<<spy2.isActive()<<endl;

    
    general2.cancelCoup(spy2);//general=3, spy=7
    cout<<"is active: "<<spy2.isActive()<<endl;
    
    general2.tax();//general=4

    spy2.viewCoins(governor2);//now the spy can see the coins of the governor

    judge2.tax();//judge=9
    merchant2.tax();//Merchant=12

    for(Player* p : game_2.getPlayers()){
    cout << p->getName() << " " << p->coins() << endl;
    }

    governor2.tax();//gov=4
    cout<<"the turn is: "<<game_2.turn()<<endl;

    spy2.gather();//spy=8
    spy2.bribe();//spy=4
    
    judge2.undo(spy2);//turn continues to the next player
    cout<<"the turn is: "<<game_2.turn()<<endl;//baron

    
    baron2.sanction(general2);//baron= 6

    try{//try to make a finacnce operation
    general2.tax();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    try{//try to make a finacnce operation
        general2.gather();
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
    }

    cout<<boolalpha<<general2.arrestAccess<<endl;
    general2.arrest(judge2);//general=4,judge=8
    judge2.tax();//judge=10

    try{//cannot have 10 or more coins and not make coup operation
        merchant2.tax();

    }catch(const std::exception &e){
        std::cerr<<e.what()<<'\n';
    }
    merchant2.coup(judge2);//Merchant=6;

    for(Player* p : game_2.getPlayers()){
    cout << p->getName() << " " << p->coins() << " " << p->isActive() << endl;
    }


   
    try{//try to arrest the same player two times in sequence
        governor2.arrest(judge2);
    }catch(const std::exception &e){
        std::cerr<<e.what()<<'\n';
    }
    governor2.tax();//gov=8
    spy2.tax();//spy=6
    governor2.undo(spy2);//spy=4
    baron2.sanction(spy2);//baron=3
    general2.tax();//general=7
    merchant2.gather();//merchant=8

    for(Player* p : game_2.getPlayers()){
    cout << p->getName() << " " << p->coins() << " " << p->isActive() << endl;
    }

    governor2.coup(baron2);//gov=0
    spy2.arrest(governor2);//spy=5
       
    try{//try to make operation with offline player
        baron.gather();
    }catch(const std::exception &e){
        std::cerr<<e.what()<<'\n';
    }
    general2.gather();//gen=8
    merchant2.gather();//mer=10

    for(Player* p : game_2.getPlayers()){
    cout << p->getName() << " " << p->coins() << " " << p->isActive() << endl;
    }
    //יש להוסיף בדיקה בכל פונקציה האם התור הנוכחי הוא של השחקן המבצע


}
