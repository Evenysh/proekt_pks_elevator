#include <catch2/catch_test_macros.hpp>
#include "Building.h"
#include <stdexcept>

TEST_CASE("Building: минимально допустимая конфигурация 2 этажа, 1 лифт", "[building]")
{
    Building b(2, 1);
    REQUIRE(b.getFloorsCount() == 2);
    REQUIRE(b.getElevators().size() == 1u);
}

TEST_CASE("Building: типовое здание 10 этажей и 2 лифта", "[building]")
{
    Building b(10, 2);
    REQUIRE(b.getFloorsCount() == 10);
    REQUIRE(b.getElevators().size() == 2u);
}

TEST_CASE("Building: максимум этажей из требований UI 20 и 5 лифтов", "[building]")
{
    Building b(20, 5);
    REQUIRE(b.getFloorsCount() == 20);
    REQUIRE(b.getElevators().size() == 5u);
}

TEST_CASE("Building: лифты получают id 1..N", "[building]")
{
    Building b(5, 3);
    REQUIRE(b.getElevators()[0]->getId() == 1);
    REQUIRE(b.getElevators()[1]->getId() == 2);
    REQUIRE(b.getElevators()[2]->getId() == 3);
}

TEST_CASE("Building: исключение при 1 этаже", "[building]")
{
    REQUIRE_THROWS_AS((void)Building(1, 1), std::invalid_argument);
}

TEST_CASE("Building: исключение при 0 этажей", "[building]")
{
    REQUIRE_THROWS_AS((void)Building(0, 1), std::invalid_argument);
}

TEST_CASE("Building: исключение при 0 лифтов", "[building]")
{
    REQUIRE_THROWS_AS((void)Building(5, 0), std::invalid_argument);
}

TEST_CASE("Building: неконстантный getElevators позволяет обход", "[building]")
{
    Building b(4, 2);
    auto& ev = b.getElevators();
    REQUIRE(ev.size() == 2u);
    REQUIRE(ev[0]->getCurrentFloor() == 1);
}
