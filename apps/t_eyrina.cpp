#include "catch.hpp"

#include "eyrina.cpp"

TEST_CASE("Eyrina commands test parsing", "[eyrina]"){
    Eyrina *app = new Eyrina();

    REQUIRE(app->Run() == 0);
}
