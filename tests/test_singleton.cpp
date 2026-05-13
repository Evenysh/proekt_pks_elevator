#include <catch2/catch_test_macros.hpp>
#include "Elevator.h"
#include "ElevatorController.h"
#include <memory>

TEST_CASE("ElevatorController::getInstance возвращает один и тот же объект", "[singleton]")
{
    ElevatorController& a = ElevatorController::getInstance();
    ElevatorController& b = ElevatorController::getInstance();
    REQUIRE(&a == &b);
}

TEST_CASE("ElevatorController: повторный getInstance после setElevators", "[singleton]")
{
    ElevatorController& c = ElevatorController::getInstance();
    c.setElevators({
        std::make_shared<Elevator>(1, 1),
        std::make_shared<Elevator>(2, 2),
    });
    ElevatorController& d = ElevatorController::getInstance();
    REQUIRE(&c == &d);
}

TEST_CASE("ElevatorController: три последовательных вызова getInstance", "[singleton]")
{
    void* p1 = static_cast<void*>(&ElevatorController::getInstance());
    void* p2 = static_cast<void*>(&ElevatorController::getInstance());
    void* p3 = static_cast<void*>(&ElevatorController::getInstance());
    REQUIRE(p1 == p2);
    REQUIRE(p2 == p3);
}

TEST_CASE("ElevatorController: ссылка на singleton не nullptr-адрес", "[singleton]")
{
    ElevatorController& ref = ElevatorController::getInstance();
    REQUIRE(static_cast<void*>(&ref) != nullptr);
}

TEST_CASE("ElevatorController: getInstance после getInstance стабилен", "[singleton]")
{
    ElevatorController* p = &ElevatorController::getInstance();
    for (int i = 0; i < 5; ++i) {
        REQUIRE(&ElevatorController::getInstance() == p);
    }
}
