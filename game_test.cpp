#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <vector>
#include <string>

#include "Game.hpp"
#include "Baron.hpp"
#include "Spy.hpp"
#include "Governor.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"

#include <sstream>

using namespace coup;
// Test case for the Game class

TEST_CASE("Game Initialization and unregular operations") {
    // Initialize the game and players
    Game game;

    Governor governor1(game, "Haniel");
    Spy spy1(game, "Mom");
    Baron baron1(game, "Dad");
    General general1(game, "Dagan");
    Judge judge1(game, "Hadar");
    Merchant merchant1(game, "Ron");

    // Check that the game has been initialized correctly
    CHECK(game.players().size() == 6);

    CHECK_THROWS_AS(spy1.tax(), std::runtime_error);
    CHECK_THROWS_WITH(spy1.tax(), "Not spy's turn");

    // הפעולה החוקית היחידה כרגע היא של השחקן הראשון
    CHECK_NOTHROW(governor1.tax());

    //CHECK(game.players().empty());
    CHECK(game.turn() == "Mom");
    //vector<string> players2 = game_1.players();

    spy1.gather();//spy=1
    baron1.gather();//baron=1
    general1.gather();//general=1
    judge1.gather();//judge=1
    merchant1.gather();//Merchant=1

    CHECK(governor1.coins() == 3);

    //try to perform action with not enough coins
    CHECK_THROWS_AS(governor1.bribe(), std::runtime_error);
    CHECK_THROWS_WITH(governor1.bribe(), "the player dosen't have enough coins for bribe operation");

    governor1.tax();//gov=6
    spy1.tax();//spy=3
    baron1.tax();//baron=3
    general1.tax();//general=3
    judge1.tax();//judge=3
    merchant1.tax();//Merchant=3

    governor1.tax();//gov=9
    spy1.tax();//spy=5
    baron1.invest();//baron=6
    general1.tax();//general=5
    judge1.tax();//judge=5
    merchant1.tax();//Merchant=6

    governor1.coup(merchant1);//gov=2
    //try to perform action on an offline player
    CHECK_THROWS_AS(spy1.arrest(merchant1), std::runtime_error);
    CHECK_THROWS_WITH(spy1.arrest(merchant1), "the target player is already out of the game");

    spy1.blockArrest(baron1);
    //לנסות לראות מטבעות עם שחקן מת
    spy1.tax();//spy=7
    //try to make arrest during block
    CHECK_THROWS_AS(baron1.arrest(merchant1), std::runtime_error);
    CHECK_THROWS_WITH(baron1.arrest(merchant1), "no access for arrest operation");

    baron1.sanction(general1);//baron=3

    //try to make finance operation during sanction
    CHECK_THROWS_AS(general1.gather(), std::runtime_error);
    CHECK_THROWS_WITH(general1.gather(), "no access for gather operation");

    CHECK_THROWS_AS(general1.tax(), std::runtime_error);
    CHECK_THROWS_WITH(general1.tax(), "no access for tax operation");


    general1.arrest(governor1);//general=6, governor=1
    CHECK(governor1.coins() == 1);

    //try to make arrest on the same player twice in a row
    CHECK_THROWS_AS(judge1.arrest(governor1), std::runtime_error);
    CHECK_THROWS_WITH(judge1.arrest(governor1), "cannot arrest the same player two times in a row");

    judge1.gather();//judge=6

    //try to make operation with offline player
    CHECK_THROWS_AS(merchant1.gather(), std::runtime_error);
    CHECK_THROWS_WITH(merchant1.gather(), "Not merchant's turn");

    //
    governor1.tax();//gov=4
    spy1.tax();//spy=9
    baron1.invest();//baron=6

    //try to cancel coup on player who is already active
    CHECK_THROWS_AS(general1.cancelCoup(spy1), std::runtime_error);
    CHECK_THROWS_WITH(general1.cancelCoup(spy1), "cannot cancel coup on active player");


    general1.cancelCoup(merchant1);//general=1
    CHECK(merchant1.isActive() == true);//check that the merchant is active after canceling the coup
    CHECK(merchant1.coins() == 6);//check that the merchant has still 6 coins after canceling the coup

    general1.gather();//general=2
    judge1.tax();//judge=8
    merchant1.gather();//Merchant=8

    governor1.arrest(general1);//gov=5 general=2
    spy1.tax();//spy=11
    baron1.gather();//baron=7
    general1.tax();//general=4
    judge1.tax();//judge=10
    merchant1.gather();//Merchant=10

    CHECK(governor1.coins() == 5);
    CHECK(spy1.coins() == 11);
    CHECK(baron1.coins() == 7);
    CHECK(general1.coins() == 4);
    CHECK(judge1.coins() == 10);
    CHECK(merchant1.coins() == 10);

    governor1.gather();//gov=6

    //try to make operation with player that have 10 or more coins
    CHECK_THROWS_AS(spy1.gather(), std::runtime_error);
    CHECK_THROWS_WITH(spy1.gather(), "You are must make a coup operation");

    //try to make operation with player that have 10 or more coins
    CHECK_THROWS_AS(spy1.arrest(baron1), std::runtime_error);
    CHECK_THROWS_WITH(spy1.arrest(baron1), "You are must make a coup operation");

    spy1.coup(baron1);//spy=4
    CHECK(game.turn() == "Dagan");//check that the turn is continue well (jump baron)

    CHECK(baron1.isActive() == false);//check that the baron is offline after coup
    general1.tax();//general=6
    judge1.coup(spy1);//judge=3
    CHECK(spy1.isActive() == false);//check that the spy is offline after coup

    //try to make coup on offline player
    CHECK_THROWS_AS(merchant1.coup(spy1), std::runtime_error);
    CHECK_THROWS_WITH(merchant1.coup(spy1), "the player is already out of the game");

    merchant1.coup(governor1);//merchant=4
    CHECK(merchant1.coins() == 4);//check that he get one coin for free

    CHECK(governor1.isActive() == false);//check that the governor is offline after coup

    CHECK(game.turn() == "Dagan");//check that the turn is continue well (jump over gover, spy and baron that is offline)

    general1.cancelCoup(baron1);//general=1
    CHECK(baron1.isActive() == true);//check that the baron is active after canceling the coup
    CHECK(baron1.coins() == 7);//check that the baron has still 6 coins after canceling the coup

    //try to view coins with offline player
    CHECK_THROWS_AS(spy1.viewCoins(baron1), std::runtime_error);
    CHECK_THROWS_WITH(spy1.viewCoins(baron1), "cannot view coins with no active player");

    //try to block arrest with offline player
    CHECK_THROWS_AS(spy1.blockArrest(baron1), std::runtime_error);
    CHECK_THROWS_WITH(spy1.blockArrest(baron1), "cannot block arrest with no active player");

    CHECK(game.turn() == "Dagan");//cancel coup is not considered as turn

    general1.tax();//gen=3
    judge1.tax();//judge=5
    merchant1.tax();//Merchant=7

    baron1.tax();//baron=9
    general1.tax();//general=5
    judge1.tax();//judge=7
    merchant1.tax();//Merchant=10

    //check the coins of each player
    CHECK(baron1.coins() == 9);
    CHECK(general1.coins() == 5);
    CHECK(judge1.coins() == 7);
    //NOTE that the merchant get one more coin if he had 3 or more coins in the beginning of his turn
    CHECK(merchant1.coins() == 10);

    CHECK(governor1.isActive() == false);
    CHECK(spy1.isActive() == false);

    baron1.coup(general1);//baron=2
    judge1.coup(baron1);//judge=0

    //before the last coup- let check if there is a winner
    CHECK_THROWS_AS(game.winner(), std::runtime_error);
    CHECK_THROWS_WITH(game.winner(), "there is no winner yet");

    //let the merchant coup the judge(the last player who is active)
    merchant1.coup(judge1);//Merchant=4

    //check that the game is over and there is a winner
    CHECK(governor1.isActive() == false);
    CHECK(spy1.isActive() == false);
    CHECK(baron1.isActive() == false);
    CHECK(general1.isActive() == false);
    CHECK(judge1.isActive() == false);
    CHECK(merchant1.isActive() == true);

    CHECK(game.winner() == "Ron");//check that the winner is the merchant

}

TEST_CASE("test governor operations") {
    // Initialize the game and players
    Game game;

    Governor governor(game, "Haniel");
    Spy spy(game, "Mom");
    Baron baron(game, "Dad");
    General general(game, "Dagan");
    Judge judge(game, "Hadar");
    Merchant merchant(game, "Ron");

    governor.tax();//gov=3
    spy.tax();//spy=2
    baron.gather();//baron=1
    general.gather();//general=1
    judge.gather();//judge=1
    merchant.tax();//Merchant=2

    CHECK(game.turn() == "Haniel");
    CHECK(governor.coins() == 3);// check that the governor has 3 coins after tax and not 2 as every one else

    governor.arrest(merchant);//gov=4, merchant=1

    //try to make undo in case the target doesn't have enough coins
    CHECK_THROWS_AS(governor.undo(merchant), std::runtime_error);
    CHECK_THROWS_WITH(governor.undo(merchant), "cannot undo tax on a player with less than 2 coins");


    spy.tax();//spy=4
    baron.tax();//baron=3
    general.gather();//general=2
    judge.tax();//judge=2
    merchant.tax();//Merchant=4

    governor.undo(spy);// spy=2
    CHECK(spy.coins() == 2); // check that the spy has 2 coins after undo

    //try to make undo twice in a row on the same player
    CHECK_THROWS_AS(governor.undo(spy), std::runtime_error);
    CHECK_THROWS_WITH(governor.undo(spy), "cannot undo tax on a player who already has undo status");

}

TEST_CASE("test spy operations") {
    // Initialize the game and players
    Game game2;

    Governor governor2(game2, "Haniel");
    Spy spy2(game2, "Mom");
    Baron baron2(game2, "Dad");
    General general2(game2, "Dagan");
    Judge judge2(game2, "Hadar");
    Merchant merchant2(game2, "Ron");

    governor2.gather();//gov=1
    spy2.gather();//spy=1
    baron2.gather();//baron=1
    general2.gather();//general=1
    judge2.gather();//judge=1
    merchant2.tax();//Merchant=2

    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf()); // capture std::cout

    CHECK(game2.turn() == "Haniel");
    spy2.viewCoins(merchant2);  // print to the buffer
    CHECK(game2.turn() == "Haniel");//check that the turn is not continue after view coins

    std::cout.rdbuf(oldCout); // return std::cout to its original state
    std::string output = buffer.str();
    CHECK(output == "Mom has view Ron's coins: 2\n");//check that the output of 'view coins' is correct

    governor2.tax();//gov=4

    CHECK(game2.turn() == "Mom");
    spy2.blockArrest(baron2);//spy=3
    CHECK(game2.turn() == "Mom");//check that the turn is not continue after block arrest

    spy2.tax();//spy=3

    //check that the spy can block arrest
    CHECK_THROWS_AS(baron2.arrest(merchant2), std::runtime_error);
    CHECK_THROWS_WITH(baron2.arrest(merchant2), "no access for arrest operation");

}

TEST_CASE("test baron operations") {
    // Initialize the game and players
    Game game;

    Governor governor(game, "Haniel");
    Spy spy(game, "Mom");
    Baron baron(game, "Dad");
    General general(game, "Dagan");
    Judge judge(game, "Hadar");
    Merchant merchant(game, "Ron");

    governor.gather();//gov=1
    spy.gather();//spy=1
    baron.tax();//baron=2
    general.gather();//general=1
    judge.gather();//judge=1
    merchant.tax();//Merchant=2

    //try to invest with less than 3 coins
    CHECK_THROWS_AS(baron.invest(), std::runtime_error);
    CHECK_THROWS_WITH(baron.invest(), "cannot invest with less then 3 coins");

    governor.gather();//gov=2
    spy.gather();//spy=2
    baron.tax();//baron=4
    general.gather();//general=2
    judge.gather();//judge=2
    merchant.tax();//Merchant=4

    governor.gather();//gov=3
    spy.gather();//spy=3


    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf()); // capture std::cout

    baron.invest(); // print to the buffer- baron=7
    std::cout.rdbuf(oldCout); // return std::cout to its original state
    std::string output = buffer.str();
    CHECK(output == "Dad (Baron) traded its 3 coins and got 6 \n");//check that the output of 'invest' is correct

    general.gather();//general=3
    judge.gather();//judge=3

    merchant.sanction(baron);//general=1, baron=8
    CHECK(baron.coins() == 8); // check that the baron have got one coin as a compensation

}

TEST_CASE("test general operations") {
    // Initialize the game and players
    Game game;

    Governor governor(game, "Haniel");
    Spy spy(game, "Mom");
    Baron baron(game, "Dad");
    General general(game, "Dagan");
    Judge judge(game, "Hadar");
    Merchant merchant(game, "Ron");

    governor.tax();//gov=3
    spy.tax();//spy=2
    baron.tax();//baron=2
    general.gather();//general=1
    judge.tax();//judge=2
    merchant.tax();//Merchant=2

    governor.tax();//gov=6
    spy.tax();//spy=4
    baron.tax();//baron=4
    general.gather();//general=2
    judge.tax();//judge=4
    merchant.tax();//Merchant=4


    governor.tax();//gov=9
    spy.tax();//spy=6
    baron.tax();//baron=6
    general.tax();//general=4
    judge.tax();//judge=6
    merchant.tax();//Merchant=7

    governor.coup(spy);//gov=2

    baron.gather();//baron=7
    //try to cancel coup with less than 5 coins
    CHECK_THROWS_AS(general.cancelCoup(spy), std::runtime_error);
    CHECK_THROWS_WITH(general.cancelCoup(spy), "cannot cancel coup there is not enough coins");
    general.tax();//general=6
    judge.arrest(general);//judge=7, general=6

    //check that the general got his coin back after arrest
    CHECK(general.coins() == 6);
    merchant.tax();//mer=10
    governor.tax();//gov=5
    baron.gather();//baron=8
    //check that the general cannot cancel coup on active player
    CHECK_THROWS_AS(general.cancelCoup(baron), std::runtime_error);
    CHECK_THROWS_WITH(general.cancelCoup(baron), "cannot cancel coup on active player");

    general.cancelCoup(spy);//general=1
    CHECK(spy.isActive() == true);//check that the spy is active after canceling the coup

    //new game
    Game game2;

    Governor governor2(game2, "Haniel");
    Spy spy2(game2, "Mom");
    Baron baron2(game2, "Dad");
    General general2(game2, "Dagan");
    Judge judge2(game2, "Hadar");
    Merchant merchant2(game2, "Ron");

    governor2.tax();//gov=3
    spy2.tax();//spy=2
    baron2.tax();//baron=2
    general2.tax();//general=2
    judge2.tax();//judge=2
    merchant2.tax();//Merchant=2

    governor2.tax();//gov=6
    spy2.tax();//spy=4
    baron2.tax();//baron=4
    general2.tax();//general=4
    judge2.tax();//judge=4
    merchant2.tax();//Merchant=4

    governor2.tax();//gov=9
    spy2.tax();//spy=6
    baron2.tax();//baron=6
    general2.tax();//general=6
    judge2.tax();//judge=6
    merchant2.tax();//Merchant=7

    governor2.coup(general2);//gov=2
    spy2.gather();//spy=7
    baron2.gather();//baron=7
    //operate cancel coup on himself
    general2.cancelCoup(general2);//general=1

    CHECK(general2.isActive() == true);//check that the general is active after canceling the coup on himself
    CHECK(general2.coins() == 1);//check that the general has 1 coin after canceling the coup on himself

}

TEST_CASE("test judge operations") {
    // Initialize the game and players
    Game game;

    Governor governor(game, "Haniel");
    Spy spy(game, "Mom");
    Baron baron(game, "Dad");
    General general(game, "Dagan");
    Judge judge(game, "Hadar");
    Merchant merchant(game, "Ron");

    governor.gather();//gov=1
    spy.gather();//spy=1
    baron.tax();//baron=2
    general.gather();//general=1
    judge.gather();//judge=1
    merchant.tax();//Merchant=2

    governor.gather();//gov=2
    spy.gather();//spy=2
    baron.tax();//baron=4
    general.gather();//general=2
    judge.gather();//judge=2
    merchant.tax();//Merchant=4

    governor.tax();//gov=5
    spy.tax();//spy=4
    baron.tax();//baron=6
    general.tax();//general=4
    judge.tax();//judge=4
    merchant.tax();//Merchant=6

    governor.gather();//gov=6

    governor.bribe();//gov=2
    CHECK(game.turn() == "Haniel");//check that the turn is still on "Haniel" after bribe
    judge.undo(governor);//gov=2
    CHECK(game.turn() == "Mom");//check that the turn is continue to Mom after undo
    CHECK(governor.coins() == 2); // check that the governor has 2 coins after undo

    //try to bribe wehen it is already the player turn
    CHECK_THROWS_AS(spy.bribe(), std::runtime_error);
    CHECK_THROWS_WITH(spy.bribe(), "this is already the player's turn");

    spy.gather();//spy=5
    //try to undo on a player that does not bribed
    CHECK_THROWS_AS(judge.undo(spy), std::runtime_error);
    CHECK_THROWS_WITH(judge.undo(spy), "Judge cannot undo gather operation");

    //try to undo on a player that already has undo status
    CHECK_THROWS_AS(judge.undo(governor), std::runtime_error);
    CHECK_THROWS_WITH(judge.undo(governor), "Judge already cancelled this operation");

}

TEST_CASE("test merchant operations") {
    // Initialize the game and players
    Game game;

    Baron baron(game, "Dad");
    General general(game, "Dagan");
    Judge judge(game, "Hadar");
    Merchant merchant(game, "Ron");

    baron.tax();//baron=2
    general.gather();//general=1
    judge.gather();//judge=1
    merchant.gather();//Merchant=1

    CHECK(merchant.coins() == 1); // check that the merchant has 1 coins after gather

    baron.tax();//baron=4
    general.gather();//general=2
    judge.gather();//judge=2
    merchant.tax();//Merchant=3
    CHECK(baron.coins() == 4); // get one coin more only if he has 3 or more coins in the beginning of his turn

    baron.tax();//baron=6
    general.gather();//general=3
    judge.gather();//judge=3

    merchant.gather();//Merchant=5
    CHECK(merchant.coins() == 5); //5- one from gather and one for free

    //make arrest on merchant
    baron.arrest(merchant);//baron=6, merchant=3

    CHECK(baron.coins() == 6); // check that the baron has  still 6 coins after arresting the merchant
    CHECK(merchant.coins() == 3); // check that the merchant lose 2 coins to the box 
}

TEST_CASE("special cases") {
    // Initialize the game and players
    Game game;

    Governor governor(game, "Haniel");
    Spy spy(game, "Mom");
    Baron baron(game, "Dad");
    General general(game, "Dagan");
    Judge judge(game, "Hadar");
    Merchant merchant(game, "Ron");

    //try to make arrest on player with no coins
    CHECK_THROWS_AS(governor.arrest(merchant), std::runtime_error);
    CHECK_THROWS_WITH(governor.arrest(merchant), "the target player dosen't have enough coins, please choose another player");

    governor.tax();//gov=3
    spy.tax();//spy=2
    baron.tax();//baron=2
    general.tax();//general=2
    judge.tax();//judge=2
    merchant.tax();//Merchant=2

    governor.tax();//gov=6
    spy.tax();//spy=4
    baron.tax();//baron=4
    general.tax();//general=4
    judge.tax();//judge=4
    merchant.tax();//Merchant=4

    governor.tax();//gov=9
    governor.bribe();//gov=5
    CHECK(game.turn() == "Haniel");//check that the turn is still on "Haniel" after bribe

    //try to bribe twice in a row
    CHECK_THROWS_AS(governor.bribe(), std::runtime_error);
    CHECK_THROWS_WITH(governor.bribe(), "this is already the player's turn");

    governor.tax();//gov=8
    spy.tax();//spy=6
    baron.tax();//baron=6
    general.tax();//general=6
    judge.tax();//judge=6
    merchant.tax();//Merchant=7

    governor.coup(baron);//gov=1, baron=6

    //try to make operation on offline player
    CHECK_THROWS_AS(spy.arrest(baron), std::runtime_error);
    CHECK_THROWS_WITH(spy.arrest(baron), "the target player is already out of the game");

    CHECK_THROWS_AS(spy.sanction(baron), std::runtime_error);
    CHECK_THROWS_WITH(spy.sanction(baron), "the target player is already out of the game");

    CHECK_THROWS_AS(spy.coup(baron), std::runtime_error);
    CHECK_THROWS_WITH(spy.coup(baron), "the player is already out of the game");

    CHECK_THROWS_AS(spy.blockArrest(baron), std::runtime_error);
    CHECK_THROWS_WITH(spy.blockArrest(baron), "the player is not active");

    CHECK_THROWS_AS(spy.viewCoins(baron), std::runtime_error);
    CHECK_THROWS_WITH(spy.viewCoins(baron), "this player is not active so far");

    spy.gather();//spy=7
    general.gather();//general=7


    CHECK_THROWS_AS(judge.undo(baron), std::runtime_error);
    CHECK_THROWS_WITH(judge.undo(baron), "the player is out of the game");

    judge.gather();//judge=7
    merchant.gather();//Merchant=8

    CHECK_THROWS_AS(governor.undo(baron), std::runtime_error);
    CHECK_THROWS_WITH(governor.undo(baron), "the player is not active anymore");
}

TEST_CASE("operations with no coins") {
    // Initialize the game and players
    Game game;

    Governor governor(game, "Haniel");
    Spy spy(game, "Mom");
    Baron baron(game, "Dad");
    General general(game, "Dagan");
    Judge judge(game, "Hadar");
    Merchant merchant(game, "Ron");

    // Try to perform operations with no coins
    CHECK_THROWS_AS(governor.bribe(), std::runtime_error);
    CHECK_THROWS_WITH(governor.bribe(), "the player dosen't have enough coins for bribe operation");

    CHECK_THROWS_AS(governor.sanction(spy), std::runtime_error);
    CHECK_THROWS_WITH(governor.sanction(spy), "the player dosen't have enough coins to perform sanction operation");

    CHECK_THROWS_AS(governor.coup(spy), std::runtime_error);
    CHECK_THROWS_WITH(governor.coup(spy), "the player doesn't have enough coins for a coup operation");

    governor.gather();//gov=1
    spy.gather();//spy=1
    baron.gather();//baron=1

    CHECK_THROWS_AS(general.cancelCoup(spy), std::runtime_error);
    CHECK_THROWS_WITH(general.cancelCoup(spy), "cannot cancel coup there is not enough coins");
}

TEST_CASE("operations with no turn& other cases") {
    // Initialize the game and players
    Game game;

    Governor governor(game, "Haniel");
    Spy spy(game, "Mom");
    Baron baron(game, "Dad");
    General general(game, "Dagan");
    Judge judge(game, "Hadar");
    Merchant merchant(game, "Ron");

    CHECK_THROWS_AS(spy.gather(), std::runtime_error);
    CHECK_THROWS_WITH(spy.gather(), "Not spy's turn");

    CHECK_THROWS_AS(spy.tax(), std::runtime_error);
    CHECK_THROWS_WITH(spy.tax(), "Not spy's turn");

    CHECK_THROWS_AS(spy.arrest(baron), std::runtime_error);
    CHECK_THROWS_WITH(spy.arrest(baron), "Not spy's turn");

    governor.tax();//gov=3
    spy.tax();//spy=2
    baron.tax();//baron=2
    general.tax();//general=2
    judge.tax();//judge=2
    merchant.tax();//Merchant=2

    governor.tax();//gov=6
    spy.tax();//spy=4
    baron.tax();//baron=4
    general.tax();//general=4
    judge.tax();//judge=4
    merchant.tax();//Merchant=4

    governor.tax();//gov=9
    spy.tax();//spy=6
    baron.tax();//baron=6
    general.tax();//general=6
    judge.tax();//judge=6
    merchant.tax();//Merchant=7

    //TURN=GOVERNOR

    CHECK_THROWS_AS(baron.sanction(general), std::runtime_error);
    CHECK_THROWS_WITH(baron.sanction(general), "Not baron's turn");

    CHECK_THROWS_AS(merchant.coup(judge), std::runtime_error);
    CHECK_THROWS_WITH(merchant.coup(judge), "Not merchant's turn");

    governor.gather();//gov=10

    CHECK_THROWS_AS(baron.invest(), std::runtime_error);
    CHECK_THROWS_WITH(baron.invest(), "Not baron's turn");

    spy.tax();//spy=8
    baron.invest();//baron=9

    //try to undo on a player that not make the action that expected to undo
    CHECK_THROWS_AS(judge.undo(baron), std::runtime_error);//undo on bribe only
    CHECK_THROWS_WITH(judge.undo(baron), "Judge cannot undo invest operation");

    CHECK_THROWS_AS(governor.undo(baron), std::runtime_error);//undo on bribe only
    CHECK_THROWS_WITH(governor.undo(baron), "the last action of  this player is not tax");

    general.gather();//general=7
    judge.gather();//judge=7
    merchant.coup(general);//Merchant=0
    governor.coup(merchant);//gov=3, merchant=0
    spy.tax();//spy=9
    baron.gather();//baron=10
    general.cancelCoup(general);//general=2

    CHECK(general.isActive() == true);//check that the general is active after canceling the coup on himself
    CHECK(general.coins() == 2);//check that the general has 2 coins after canceling the coup on himself

    judge.coup(general);//judge=0
    CHECK(general.isActive() == false);//check that the general is offline after coup
    
    governor.gather();//gov=4
    spy.coup(judge);//spy=10
    baron.coup(governor);//baron=3, gov=0

    //try to cancel coup with not enough coins
    CHECK_THROWS_AS(general.cancelCoup(general), std::runtime_error);//undo on bribe only
    CHECK_THROWS_WITH(general.cancelCoup(general), "cannot cancel coup there is not enough coins");
 
}

TEST_CASE("two players with same name") {
    // Initialize the game and players
    Game game;

    Governor governor(game, "Haniel");

    CHECK_THROWS_AS(Spy spy(game, "Haniel"), std::runtime_error);
    CHECK_THROWS_WITH(Spy spy(game, "Haniel"), "This name is already taken- please choose another one");

}
TEST_CASE("operate when block") {
    Game game;

    Governor governor(game, "Haniel");
    Spy spy(game, "Mom");
    Baron baron(game, "Dad");
    General general(game, "Dagan");
    Judge judge(game, "Hadar");
    Merchant merchant(game, "Ron");

    governor.tax();//gov=3
    spy.tax();//spy=2
    baron.tax();//baron=2
    general.tax();//general=2
    judge.tax();//judge=2
    merchant.tax();//Merchant=2

    governor.tax();//gov=6
    spy.tax();//spy=4
    baron.tax();//baron=4
    general.tax();//general=4
    judge.tax();//judge=4
    merchant.tax();//Merchant=4

    governor.tax();//gov=9
    spy.blockArrest(baron);//spy=4
    spy.gather();//spy=4

    //try to arrest by baron
    CHECK_THROWS_AS(baron.arrest(merchant), std::runtime_error);
    CHECK_THROWS_WITH(baron.arrest(merchant), "no access for arrest operation");

    baron.sanction(general);//baron=1
    
    //try to gather when sanctioned
    CHECK_THROWS_AS(general.gather(), std::runtime_error);
    CHECK_THROWS_WITH(general.gather(), "no access for gather operation");

    //try to tax when sanctioned
    CHECK_THROWS_AS(general.tax(), std::runtime_error);
    CHECK_THROWS_WITH(general.tax(), "no access for tax operation");

    general.arrest(governor);//general=5
    general.bribe();//general=1
    CHECK(game.turn() == "Dagan");//check that the turn is still on "Dagan" after bribe

    //check the undo operation on bribe
    judge.undo(general);//general=1
    CHECK(game.turn() == "Hadar");//check that the turn is continue to Hadar(Judge) after undo
    CHECK(general.coins() == 1); // check that the general has 1 coins after undo
}









