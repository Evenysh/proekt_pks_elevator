#include "Simulation.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

Simulation::Simulation(int floors, int elevatorCount)
    : controller(ElevatorController::getInstance()), running(false) {
    building = std::make_shared<Building>(floors, elevatorCount);
    controller.setElevators(building->getElevators());
}

Simulation::~Simulation() {
    if (running) stop();
}

// ------------------------------------------------------------------
void Simulation::start() {
    running = true;

    for (auto& e : building->getElevators())
        elevatorThreads.emplace_back([e]() { e->run(); });

    controllerThread = std::thread([this]() { controller.monitorSystem(); });

    std::cout << "[Симуляция] Запущена: "
              << building->getElevators().size() << " лифт(а), "
              << building->getFloorsCount()       << " этаж(ей).\n";
}

// ------------------------------------------------------------------
void Simulation::stop() {
    running = false;
    controller.stop();
    for (auto& e : building->getElevators()) e->stop();

    if (controllerThread.joinable()) controllerThread.join();
    for (auto& t : elevatorThreads)
        if (t.joinable()) t.join();

    std::cout << "[Симуляция] Остановлена.\n";
}

// ------------------------------------------------------------------
// Метод фабрики: собирает и проверяет Request, затем ставит в очередь
void Simulation::submitRequest(int fromFloor, int toFloor) {
    int total = building->getFloorsCount();
    Direction dir = (toFloor > fromFloor) ? Direction::UP : Direction::DOWN;
    auto req = std::make_shared<Request>(fromFloor, toFloor, dir);

    if (!req->validate(total))
        throw std::invalid_argument(
            "Недопустимые этажи. Должны быть в диапазоне [1, " + std::to_string(total) +
            "] и не совпадать.");

    controller.addRequest(req);
    notifyObservers();
}

// ------------------------------------------------------------------
void Simulation::attach(IObserver* obs)  { observers.push_back(obs); }

void Simulation::detach(IObserver* obs) {
    observers.erase(
        std::remove(observers.begin(), observers.end(), obs),
        observers.end());
}

void Simulation::notifyObservers() {
    for (auto* obs : observers) obs->onStatusUpdate();
}

// ------------------------------------------------------------------
int Simulation::getFloorsCount() const {
    return building->getFloorsCount();
}

const std::vector<std::shared_ptr<Elevator>>& Simulation::getElevators() const {
    return building->getElevators();
}

