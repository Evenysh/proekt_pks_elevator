// Сценарий 3: диспетчеризация — какой лифт выберет стратегия «ближайший».
#include "Elevator.h"
#include "ElevatorController.h"
#include "Request.h"
#include <iostream>
#include <memory>

int main() {
    NearestElevatorStrategy strat;
    std::vector<std::shared_ptr<Elevator>> ev = {
        std::make_shared<Elevator>(1, 2),
        std::make_shared<Elevator>(2, 9),
    };
    Request req(5, 10, Direction::UP);
    auto e = strat.selectElevator(ev, req);
    if (!e) {
        std::cerr << "FAIL: no elevator\n";
        return 1;
    }
    if (e->getId() != 1) {
        std::cerr << "FAIL: wrong elevator id=" << e->getId() << "\n";
        return 1;
    }
    std::cout << "OK: selected elevator id=" << e->getId() << "\n";
    return 0;
}
