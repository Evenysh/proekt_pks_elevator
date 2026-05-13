#include <catch2/catch_test_macros.hpp>
#include "ElevatorController.h"
#include "Request.h"
#include <memory>
#include <vector>

static std::vector<std::shared_ptr<Elevator>> make_ev(int f1, int f2) {
    return {
        std::make_shared<Elevator>(1, f1),
        std::make_shared<Elevator>(2, f2),
    };
}

TEST_CASE("NearestElevatorStrategy: один лифт — всегда он", "[strategy]")
{
    NearestElevatorStrategy strat;
    std::vector<std::shared_ptr<Elevator>> ev = {std::make_shared<Elevator>(1, 4)};
    Request req(2, 9, Direction::UP);
    auto chosen = strat.selectElevator(ev, req);
    REQUIRE(chosen);
    REQUIRE(chosen->getId() == 1);
}

TEST_CASE("NearestElevatorStrategy: выбирается ближайший к этажу вызова", "[strategy]")
{
    NearestElevatorStrategy strat;
    auto ev = make_ev(2, 9);
    Request req(5, 1, Direction::DOWN);
    auto chosen = strat.selectElevator(ev, req);
    REQUIRE(chosen);
    REQUIRE(chosen->getId() == 1);
}

TEST_CASE("NearestElevatorStrategy: второй лифт ближе", "[strategy]")
{
    NearestElevatorStrategy strat;
    auto ev = make_ev(1, 4);
    Request req(5, 10, Direction::UP);
    auto chosen = strat.selectElevator(ev, req);
    REQUIRE(chosen);
    REQUIRE(chosen->getId() == 2);
}

TEST_CASE("NearestElevatorStrategy: равное расстояние — первый в векторе", "[strategy]")
{
    NearestElevatorStrategy strat;
    auto ev = make_ev(3, 7);
    Request req(5, 10, Direction::UP);
    auto chosen = strat.selectElevator(ev, req);
    REQUIRE(chosen);
    REQUIRE(chosen->getId() == 1);
}

TEST_CASE("NearestElevatorStrategy: пустой список лифтов", "[strategy]")
{
    NearestElevatorStrategy strat;
    std::vector<std::shared_ptr<Elevator>> ev;
    Request req(1, 2, Direction::UP);
    auto chosen = strat.selectElevator(ev, req);
    REQUIRE_FALSE(chosen);
}

TEST_CASE("NearestElevatorStrategy: три лифта на разных этажах", "[strategy]")
{
    NearestElevatorStrategy strat;
    std::vector<std::shared_ptr<Elevator>> ev = {
        std::make_shared<Elevator>(1, 1),
        std::make_shared<Elevator>(2, 8),
        std::make_shared<Elevator>(3, 4),
    };
    Request req(5, 10, Direction::UP);
    auto chosen = strat.selectElevator(ev, req);
    REQUIRE(chosen);
    REQUIRE(chosen->getId() == 3);
}

TEST_CASE("NearestElevatorStrategy: вызов с этажа совпадающего с лифтом", "[strategy]")
{
    NearestElevatorStrategy strat;
    auto ev = make_ev(5, 10);
    Request req(5, 1, Direction::DOWN);
    auto chosen = strat.selectElevator(ev, req);
    REQUIRE(chosen);
    REQUIRE(chosen->getCurrentFloor() == 5);
}
