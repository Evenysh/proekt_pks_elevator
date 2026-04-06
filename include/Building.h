#pragma once
#include <memory>
#include <vector>
#include "Elevator.h"

// ============================================================
// Building — здание: этажи и набор лифтов
// ============================================================
class Building {
public:
    Building(int floorsCount, int elevatorCount);

    int getFloorsCount() const { return floorsCount; }

    const std::vector<std::shared_ptr<Elevator>>& getElevators() const { return elevators; }
    std::vector<std::shared_ptr<Elevator>>&       getElevators()       { return elevators; }

private:
    int                                    floorsCount;
    std::vector<std::shared_ptr<Elevator>> elevators;
};
