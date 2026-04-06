#pragma once
#include <memory>
#include "IObserver.h"
#include "Simulation.h"

// ============================================================
// UserInterface — паттерн Observer (конкретный наблюдатель)
// Вывод состояния симуляции и ввод пользователя
// ============================================================
class UserInterface : public IObserver {
public:
    explicit UserInterface(std::shared_ptr<Simulation> simulation);

    // Основной интерактивный цикл
    void run();

    // Колбэк IObserver — вызывается Simulation при смене состояния
    void onStatusUpdate() override;

    // Читает число этажей и лифтов до создания Simulation
    static void getInputParameters(int& floors, int& elevators);

private:
    void displayStatus()  const;
    void callElevator();
    void printMenu()      const;

    std::shared_ptr<Simulation> simulation;
};
