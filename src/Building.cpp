#include "Building.h"
#include <stdexcept>

Building::Building(int floorsCount, int elevatorCount)
    : floorsCount(floorsCount) {
    if (floorsCount < 2)
        throw std::invalid_argument("Building must have at least 2 floors.");
    if (elevatorCount < 1)
        throw std::invalid_argument("Building must have at least 1 elevator.");

    for (int i = 0; i < elevatorCount; ++i)
        elevators.push_back(std::make_shared<Elevator>(i + 1, 1));
}
