#include <iostream>
#include <memory>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif
#include "ConsoleLog.h"
#include "Simulation.h"
#include "UserInterface.h"

namespace {

void printUsage(std::ostream& os) {
    os << "Использование:\n"
       << "  elevator_sim                          — ввод параметров с клавиатуры\n"
       << "  elevator_sim <этажи> <лифты>          — позиционные аргументы (2–20, 1–5)\n"
       << "  elevator_sim --floors N --elevators M\n"
       << "  elevator_sim -f N -e M\n"
       << "  elevator_sim --help\n";
}

bool parseCliParameters(int argc, char* argv[], int& floors, int& elevatorCount) {
    if (argc <= 1) return false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            printUsage(std::cout);
            return true; // выход без ошибки
        }
    }

    auto readValue = [&](int& i, int& out) -> bool {
        if (i + 1 >= argc) return false;
        try {
            const int v = std::stoi(argv[++i]);
            out = v;
            return true;
        } catch (...) {
            return false;
        }
    };

    bool haveFloors = false;
    bool haveElevators = false;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--floors" || arg == "-f") {
            if (!readValue(i, floors)) return false;
            haveFloors = true;
        } else if (arg == "--elevators" || arg == "-e") {
            if (!readValue(i, elevatorCount)) return false;
            haveElevators = true;
        } else if (!arg.empty() && arg[0] != '-') {
            try {
                const int v = std::stoi(arg);
                if (!haveFloors) {
                    floors = v;
                    haveFloors = true;
                } else if (!haveElevators) {
                    elevatorCount = v;
                    haveElevators = true;
                } else {
                    return false;
                }
            } catch (...) {
                return false;
            }
        } else {
            return false;
        }
    }

    if (haveFloors != haveElevators) return false;
    return haveFloors && haveElevators;
}

bool validateParameters(int floors, int elevatorCount) {
    return floors >= 2 && floors <= 20 && elevatorCount >= 1 && elevatorCount <= 5;
}

} // namespace

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    ConsoleLog::syncUi([](std::ostream& os) {
        os << "\n";
        os << "============================================================\n";
        os << "  Симулятор работы лифтов в здании\n";
        os << "============================================================\n\n";
    });

    int floors = 0, elevatorCount = 0;

    if (parseCliParameters(argc, argv, floors, elevatorCount)) {
        if (floors == 0 && elevatorCount == 0) {
            return 0; // --help
        }
        if (!validateParameters(floors, elevatorCount)) {
            std::cerr << "Ошибка: этажи 2–20, лифты 1–5.\n";
            printUsage(std::cerr);
            return 1;
        }
        ConsoleLog::syncUi([&](std::ostream& os) {
            os << "Параметры из командной строки: этажей=" << floors
               << ", лифтов=" << elevatorCount << ".\n\n";
        });
    } else if (argc > 1) {
        std::cerr << "Некорректные аргументы.\n";
        printUsage(std::cerr);
        return 1;
    } else {
        UserInterface::getInputParameters(floors, elevatorCount);
    }

    auto simulation = std::make_shared<Simulation>(floors, elevatorCount);
    simulation->start();

    UserInterface ui(simulation);
    ui.run();

    return 0;
}
