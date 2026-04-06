#include "UserInterface.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

UserInterface::UserInterface(std::shared_ptr<Simulation> sim)
    : simulation(sim) {
    simulation->attach(this);
}

// ------------------------------------------------------------------
// Колбэк IObserver — вызывается из Simulation::notifyObservers()
void UserInterface::onStatusUpdate() {
    std::cout << "[UI] Заявка принята. Введите '2' для просмотра статуса лифтов.\n";
}

// ------------------------------------------------------------------
void UserInterface::getInputParameters(int& floors, int& elevators) {
    auto readInt = [](const std::string& prompt, int lo, int hi, int def) -> int {
        int v = 0;
        std::cout << prompt;
        if (!(std::cin >> v) || v < lo || v > hi) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  Некорректный ввод. Используется значение по умолчанию: " << def << "\n";
            return def;
        }
        return v;
    };

    floors    = readInt("Количество этажей    (2-20): ", 2, 20, 10);
    elevators = readInt("Количество лифтов     (1-5): ", 1,  5,  2);
}

// ------------------------------------------------------------------
void UserInterface::run() {
    int choice = 0;
    while (simulation->isRunning() && choice != 3) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        switch (choice) {
            case 1: callElevator();  break;
            case 2: displayStatus(); break;
            case 3: break;
            default:
                std::cout << "Неизвестная команда. Попробуйте снова.\n";
        }
    }
    if (simulation->isRunning()) simulation->stop();
}

// ------------------------------------------------------------------
void UserInterface::printMenu() const {
    std::cout << "\n[1] Вызвать лифт   [2] Показать статус   [3] Выход\n"
              << "Выбор: ";
}

// ------------------------------------------------------------------
void UserInterface::displayStatus() const {
    std::cout << "\n--- Статус лифтов ---\n";
    for (const auto& e : simulation->getElevators()) {
        std::cout << "  Лифт " << e->getId()
                  << "  |  Этаж: " << std::setw(2) << e->getCurrentFloor()
                  << "  |  "       << e->getStateString() << "\n";
    }
    std::cout << "---------------------\n";
}

// ------------------------------------------------------------------
void UserInterface::callElevator() {
    int from = 0, to = 0;
    std::cout << "С этажа (1-" << simulation->getFloorsCount() << "): ";
    std::cin >> from;
    std::cout << "На этаж (1-" << simulation->getFloorsCount() << "): ";
    std::cin >> to;

    try {
        simulation->submitRequest(from, to);
    } catch (const std::invalid_argument& ex) {
        std::cout << "[Ошибка] " << ex.what() << "\n";
    }
}

