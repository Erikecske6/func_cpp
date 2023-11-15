#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <algorithm>
#include <array>
#include <numeric>

TEST_CASE("test accumulate on integers") {
    std::array a{0, 1, 2, 3, 4, 5, 6, 7};
    REQUIRE(std::accumulate(a.begin(), a.end(), 0) == 7 * 8 / 2);
    SECTION("dummy section") { REQUIRE_FALSE(true); }
    SECTION("demo throw") { REQUIRE_THROWS_AS(operator new(-1), std::bad_alloc); }
}

TEST_CASE("bdd demo", "[bdd]") {
    GIVEN("a list of numbers") {
        std::array a{0, 1, 2, 3, 4, 5, 6, 7};
        WHEN("my function is called") {
            const auto result = std::accumulate(a.begin(), a.end(), 0);

            THEN("the result is the n* n+1 /2") { REQUIRE(result == 7 * 8 / 2); }
        }
    }
}
