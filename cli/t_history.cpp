#include "catch.hpp"

#include "history.cpp"

TEST_CASE("History initialization and listing", "[history]"){
    CLI_history *history = new CLI_history(5);

    SECTION("initial movement"){
        REQUIRE(history->Up() == "");
        REQUIRE(history->Down() == "");
    }

    SECTION("Updating"){
        REQUIRE(history->Update("a") == 0);
        REQUIRE(history->Update("a") == 1);
        REQUIRE(history->Up() == "a");
        REQUIRE(history->Up() == "a");
        REQUIRE(history->Update("b") == 0);
        REQUIRE(history->Up() == "b");
        REQUIRE(history->Up() == "a");
        REQUIRE(history->Up() == "a");
        REQUIRE(history->Down() == "b");
        REQUIRE(history->Down() == "");
    }

    SECTION("Moving above buffer size"){
        history = new CLI_history(3);

        history->Update("a");
        history->Update("b");
        history->Update("c");
        history->Update("d");

        REQUIRE(history->Up() == "d");
        REQUIRE(history->Up() == "c");
        REQUIRE(history->Up() == "b");
        REQUIRE(history->Up() == "b");

        REQUIRE(history->Down() == "c");
        REQUIRE(history->Down() == "d");
        REQUIRE(history->Down() == "");
    }

    SECTION("Reseting counter"){
        history = new CLI_history(3);

        history->Update("a");
        history->Update("b");
        history->Update("c");
        history->Update("d");

        REQUIRE(history->Up() == "d");
        REQUIRE(history->Up() == "c");

        history->Reset_pointer();

        REQUIRE(history->Down() == "");
        REQUIRE(history->Up() == "d");

        history->Reset_pointer();

        REQUIRE(history->Up() == "d");
    }
}
