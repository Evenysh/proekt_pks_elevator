#include <catch2/catch_test_macros.hpp>
#include "Elevator.h"

TEST_CASE("Elevator: id и начальный этаж из конструктора", "[elevator]")
{
    Elevator e(3, 7);
    REQUIRE(e.getId() == 3);
    REQUIRE(e.getCurrentFloor() == 7);
}

TEST_CASE("Elevator: по умолчанию этаж 1", "[elevator]")
{
    Elevator e(1);
    REQUIRE(e.getCurrentFloor() == 1);
}

TEST_CASE("Elevator: начальное состояние IDLE и строка ОЖИДАНИЕ", "[elevator]")
{
    Elevator e(2, 1);
    REQUIRE(e.getState() == ElevatorState::IDLE);
    REQUIRE(e.getStateString() == "ОЖИДАНИЕ");
}

TEST_CASE("Elevator: isIdle истина без целей и без run", "[elevator]")
{
    Elevator e(1, 5);
    REQUIRE(e.isIdle());
}

TEST_CASE("Elevator: после addTargetFloor isIdle ложь", "[elevator]")
{
    Elevator e(1, 1);
    REQUIRE(e.isIdle());
    e.addTargetFloor(4);
    REQUIRE_FALSE(e.isIdle());
}

TEST_CASE("Elevator: addTargetFloor не меняет текущий этаж до run", "[elevator]")
{
    Elevator e(2, 3);
    e.addTargetFloor(10);
    REQUIRE(e.getCurrentFloor() == 3);
}

TEST_CASE("Elevator: несколько целевых этажей в очереди", "[elevator]")
{
    Elevator e(1, 1);
    e.addTargetFloor(2);
    e.addTargetFloor(5);
    e.addTargetFloor(5);
    REQUIRE_FALSE(e.isIdle());
}
