#include "ElevatorController.h"
#include <climits>
#include <cstdlib>
#include <iostream>

// ---- NearestElevatorStrategy (стратегия «ближайший лифт») ---------------
std::shared_ptr<Elevator> NearestElevatorStrategy::selectElevator(
    const std::vector<std::shared_ptr<Elevator>>& elevators,
    const Request& request)
{
    std::shared_ptr<Elevator> best;
    int bestDist = INT_MAX;

    for (const auto& e : elevators) {
        int dist = std::abs(e->getCurrentFloor() - request.getSourceFloor());
        if (dist < bestDist) { bestDist = dist; best = e; }
    }
    return best;
}

// ---- ElevatorController (одиночка) -------------------------------------
ElevatorController::ElevatorController() : running(false) {
    strategy = std::make_shared<NearestElevatorStrategy>();
}

ElevatorController& ElevatorController::getInstance() {
    static ElevatorController instance;
    return instance;
}

void ElevatorController::setElevators(std::vector<std::shared_ptr<Elevator>> elevs) {
    elevators = std::move(elevs);
}

void ElevatorController::setStrategy(std::shared_ptr<IDispatchStrategy> strat) {
    strategy = strat;
}

// ------------------------------------------------------------------
void ElevatorController::addRequest(std::shared_ptr<Request> request) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        pendingRequests.push(request);
    }
    cv.notify_one();
}

// ------------------------------------------------------------------
void ElevatorController::assignElevator(std::shared_ptr<Request> request) {
    auto elev = strategy->selectElevator(elevators, *request);
    if (!elev) {
        std::cout << "[Диспетчер] Нет доступных лифтов!\n";
        return;
    }

    request->setStatus(RequestStatus::IN_PROGRESS);
    elev->addTargetFloor(request->getSourceFloor());
    elev->addTargetFloor(request->getTargetFloor());

    std::cout << "[Диспетчер] Лифт " << elev->getId()
              << " назначен: этаж " << request->getSourceFloor()
              << " -> этаж "        << request->getTargetFloor() << "\n";
}

// ------------------------------------------------------------------
void ElevatorController::monitorSystem() {
    running = true;
    while (running) {
        std::shared_ptr<Request> req;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() {
                return !pendingRequests.empty() || !running;
            });
            if (!running) break;
            req = pendingRequests.front();
            pendingRequests.pop();
        }
        if (req) assignElevator(req);
    }
}

// ------------------------------------------------------------------
void ElevatorController::stop() {
    running = false;
    cv.notify_all();
}
