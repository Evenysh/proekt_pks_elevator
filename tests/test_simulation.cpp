#include <catch2/catch_test_macros.hpp>
#include "Simulation.h"

// Потоки start/stop + синглтон ElevatorController в unit-тестах дают нестабильные
// зависания при полном прогоне Catch2/ctest; жизненный цикл с потоками — в
// scenarios/scenario_simulation_lifecycle.cpp

TEST_CASE("Simulation: число этажей совпадает с параметром", "[simulation]")
{
    auto sim = std::make_shared<Simulation>(12, 2);
    REQUIRE(sim->getFloorsCount() == 12);
}

TEST_CASE("Simulation: число лифтов совпадает с параметром", "[simulation]")
{
    auto sim = std::make_shared<Simulation>(10, 3);
    REQUIRE(sim->getElevators().size() == 3u);
}

TEST_CASE("Simulation: до start потоки не считаются запущенными", "[simulation]")
{
    auto sim = std::make_shared<Simulation>(5, 1);
    REQUIRE(sim->isRunning() == false);
}

TEST_CASE("Simulation: первый лифт на этаже 1", "[simulation]")
{
    auto sim = std::make_shared<Simulation>(8, 2);
    REQUIRE(sim->getElevators()[0]->getCurrentFloor() == 1);
}
