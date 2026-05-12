#include "UserInterface.h"
#include "ConsoleLog.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

UserInterface::UserInterface(std::shared_ptr<Simulation> sim)
    : simulation(sim) {
    simulation->attach(this);
}

// ------------------------------------------------------------------
void UserInterface::onStatusUpdate() {
    ConsoleLog::sync([](std::ostream& os) {
        os << "  [заявка]   Принята. Команда [2] — таблица статуса лифтов.\n";
    });
}

// ------------------------------------------------------------------
void UserInterface::getInputParameters(int& floors, int& elevators) {
    auto readInt = [](const std::string& prompt, int lo, int hi, int def) -> int {
        int v = 0;
        ConsoleLog::syncUi([&prompt](std::ostream& os) { os << prompt; });
        if (!(std::cin >> v) || v < lo || v > hi) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            ConsoleLog::syncUi([&def](std::ostream& os) {
                os << "  (ввод некорректен, подставлено по умолчанию: " << def << ")\n";
            });
            return def;
        }
        return v;
    };

    ConsoleLog::syncUi([](std::ostream& os) {
        os << "Параметры здания (можно ввести число и Enter):\n";
    });
    floors    = readInt("  Этажей в здании (2–20) [по умолчанию 10]: ", 2, 20, 10);
    elevators = readInt("  Лифтов (1–5) [по умолчанию 2]: ", 1, 5, 2);
}

// ------------------------------------------------------------------
void UserInterface::run() {
    int choice = 0;
    while (simulation->isRunning() && choice != 3) {
        {
            ConsoleLog::UiInputSession inputSession;
            printMenu();
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                ConsoleLog::syncUi([](std::ostream& os) {
                    os << "  (ожидалось число 1–3, попробуйте снова)\n";
                });
                continue;
            }
        }
        switch (choice) {
            case 1: callElevator();  break;
            case 2: displayStatus(); break;
            case 3: break;
            default:
                ConsoleLog::sync([](std::ostream& os) {
                    os << "  Неизвестная команда. Выберите 1, 2 или 3.\n";
                });
        }
    }
    if (simulation->isRunning()) simulation->stop();
}

// ------------------------------------------------------------------
void UserInterface::printMenu() const {
    ConsoleLog::syncUi([](std::ostream& os) {
        os << "\n";
        os << "------------------------------------------------------------\n";
        os << "  Меню\n";
        os << "    [1]  Вызвать лифт (с этажа / на этаж)\n";
        os << "    [2]  Показать статус всех лифтов\n";
        os << "    [3]  Выход\n";
        os << "------------------------------------------------------------\n";
        os << "  Ваш выбор: ";
    });
}

// ------------------------------------------------------------------
void UserInterface::displayStatus() const {
    ConsoleLog::syncUi([this](std::ostream& os) {
        std::ostringstream tab;
        tab.setf(std::ios::left);
        const auto& list = simulation->getElevators();
        tab << "\n";
        tab << "  +--------+--------------+---------------------------+\n";
        tab << "  |  №     | Этаж         | Состояние                 |\n";
        tab << "  +--------+--------------+---------------------------+\n";
        for (const auto& e : list) {
            tab << "  | " << std::setw(6) << e->getId()
                << " | " << std::setw(12) << e->getCurrentFloor()
                << " | " << std::setw(25) << e->getStateString()
                << " |\n";
        }
        tab << "  +--------+--------------+---------------------------+\n";
        os << tab.str();
    });
}

// ------------------------------------------------------------------
void UserInterface::callElevator() {
    int from = 0, to = 0;
    const int n = simulation->getFloorsCount();
    {
        ConsoleLog::UiInputSession inputSession;
        ConsoleLog::syncUi([n](std::ostream& os) {
            os << "  Этаж вызова (1–" << n << "): ";
        });
        std::cin >> from;
        ConsoleLog::syncUi([n](std::ostream& os) {
            os << "  Целевой этаж (1–" << n << "): ";
        });
        std::cin >> to;
    }

    try {
        simulation->submitRequest(from, to);
    } catch (const std::invalid_argument& ex) {
        ConsoleLog::syncUi([&ex](std::ostream& os) {
            os << "  [ошибка] " << ex.what() << "\n";
        });
    }
}
