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
