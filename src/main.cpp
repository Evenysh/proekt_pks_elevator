#include <iostream>
#include <memory>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Simulation.h"
#include "UserInterface.h"

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::cout << "=== Симулятор работы лифтов в здании ===\n\n";

    int floors = 0, elevatorCount = 0;
    UserInterface::getInputParameters(floors, elevatorCount);

    auto simulation = std::make_shared<Simulation>(floors, elevatorCount);
    simulation->start();

    UserInterface ui(simulation);
    ui.run();

    return 0;
}
