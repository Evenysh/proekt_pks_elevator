// Сценарий 4: жизненный цикл симуляции — старт, короткая работа, останов.
#include "Simulation.h"
#include <chrono>
#include <iostream>
#include <thread>

int main() {
    try {
        auto sim = std::make_shared<Simulation>(6, 2);
        sim->start();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        sim->submitRequest(2, 5);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        sim->stop();
        std::cout << "OK: simulation lifecycle (start / request / stop)\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL: " << ex.what() << "\n";
        return 1;
    }
}
