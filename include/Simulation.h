#pragma once
#include <atomic>
#include <memory>
#include <thread>
#include <vector>
#include "Building.h"
#include "ElevatorController.h"
#include "IObserver.h"
#include "Request.h"

// ============================================================
// Simulation — паттерн Facade
// Единая точка входа, скрывающая Building / Elevator / ElevatorController
// Также выступает Subject в паттерне Observer
// ============================================================
class Simulation {
public:
    Simulation(int floors, int elevatorCount);
    ~Simulation();

    void start();
    void stop();
    bool isRunning() const { return running.load(); }

    // Метод фабрики: создаёт проверенную Request и ставит в очередь
    void submitRequest(int fromFloor, int toFloor);

    // Observer: регистрация / отписка / уведомление наблюдателей
    void attach(IObserver* observer);
    void detach(IObserver* observer);
    void notifyObservers();

    int  getFloorsCount() const;
    const std::vector<std::shared_ptr<Elevator>>& getElevators() const;

private:
    std::shared_ptr<Building>    building;
    ElevatorController&          controller;

    std::atomic<bool>            running;
    std::vector<std::thread>     elevatorThreads;
    std::thread                  controllerThread;

    std::vector<IObserver*>      observers;
};
