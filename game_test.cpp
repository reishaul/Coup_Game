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


}
