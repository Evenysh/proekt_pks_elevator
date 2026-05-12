#include "Elevator.h"
#include "ConsoleLog.h"
#include <chrono>
#include <climits>
#include <cstdlib>
#include <iomanip>
#include <thread>

Elevator::Elevator(int id, int initialFloor)
    : id(id), currentFloor(initialFloor),
      state(ElevatorState::IDLE), running(false) {}

// ------------------------------------------------------------------
void Elevator::addTargetFloor(int floor) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        targetFloors.insert(floor);
    }
    cv.notify_one();
}

// ------------------------------------------------------------------
// Вызывать при удерживаемом mtx
int Elevator::getNearestTarget() const {
    if (targetFloors.empty()) return -1;
    int best = -1, bestDist = INT_MAX;
    int cur  = currentFloor.load();
    for (int f : targetFloors) {
        int d = std::abs(f - cur);
        if (d < bestDist) { bestDist = d; best = f; }
    }
    return best;
}

// ------------------------------------------------------------------
void Elevator::run() {
    running = true;
    while (running) {

        // ---- Ожидание, пока появится работа --------------------------------
        int target;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() {
                return !targetFloors.empty() || !running;
            });
            if (!running) break;
            target = getNearestTarget();
        }

        if (target == -1) continue;

        int cur = currentFloor.load();

        // ---- Уже на целевом этаже — открыть / закрыть двери ----------------
        if (cur == target) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                state = ElevatorState::DOORS_OPEN;
            }
            const int lid = id;
            const int floorOpen = cur;
            ConsoleLog::sync([lid, floorOpen](std::ostream& os) {
                os << "      [лифт " << std::setw(2) << lid << "]  прибыл на этаж "
                   << std::setw(2) << floorOpen << "  |  двери ОТКРЫТЫ\n";
            });

            std::this_thread::sleep_for(std::chrono::milliseconds(1200));

            {
                std::lock_guard<std::mutex> lock(mtx);
                targetFloors.erase(target);
                state = targetFloors.empty()
                        ? ElevatorState::IDLE
                        : ElevatorState::IDLE; // остаёмся в IDLE; направление на следующей итерации
            }
            ConsoleLog::sync([lid, floorOpen](std::ostream& os) {
                os << "      [лифт " << std::setw(2) << lid << "]  этаж "
                   << std::setw(2) << floorOpen << "  |  двери ЗАКРЫТЫ\n";
            });

        // ---- Сдвиг на один этаж к цели --------------------------------------
        } else if (target > cur) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                state = ElevatorState::MOVING_UP;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(700));
            ++currentFloor;
            const int lid = id;
            const int fl = currentFloor.load();
            ConsoleLog::sync([lid, fl](std::ostream& os) {
                os << "      [лифт " << std::setw(2) << lid << "]  ВВЕРХ   ->  этаж "
                   << std::setw(2) << fl << "\n";
            });

        } else {
            {
                std::lock_guard<std::mutex> lock(mtx);
                state = ElevatorState::MOVING_DOWN;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(700));
            --currentFloor;
            const int lid = id;
            const int fl = currentFloor.load();
            ConsoleLog::sync([lid, fl](std::ostream& os) {
                os << "      [лифт " << std::setw(2) << lid << "]  ВНИЗ    ->  этаж "
                   << std::setw(2) << fl << "\n";
            });
        }

        // После шага — IDLE, если очередь этажей пуста
        // (предикат в cv.wait снова заблокирует поток, если пусто)
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (targetFloors.empty()) state = ElevatorState::IDLE;
        }
    }
}

// ------------------------------------------------------------------
void Elevator::stop() {
    running = false;
    cv.notify_all();
}

// ------------------------------------------------------------------
ElevatorState Elevator::getState() const {
    std::lock_guard<std::mutex> lock(mtx);
    return state;
}

bool Elevator::isIdle() const {
    std::lock_guard<std::mutex> lock(mtx);
    return state == ElevatorState::IDLE && targetFloors.empty();
}

std::string Elevator::getStateString() const {
    std::lock_guard<std::mutex> lock(mtx);
    switch (state) {
        case ElevatorState::IDLE:        return "ОЖИДАНИЕ";
        case ElevatorState::MOVING_UP:   return "ДВИЖЕНИЕ ВВЕРХ";
        case ElevatorState::MOVING_DOWN: return "ДВИЖЕНИЕ ВНИЗ";
        case ElevatorState::DOORS_OPEN:  return "ДВЕРИ ОТКРЫТЫ";
        default:                         return "НЕИЗВЕСТНО";
    }
}
