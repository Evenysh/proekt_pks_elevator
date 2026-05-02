#pragma once
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <set>
#include <string>

// ============================================================
// ElevatorState — паттерн State
// Поведение лифта зависит от текущего состояния
// ============================================================
enum class ElevatorState {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    DOORS_OPEN
};

// ============================================================
// Elevator — модель одной кабины, работающей в своём потоке
// ============================================================
class Elevator {
public:
    explicit Elevator(int id, int initialFloor = 1);

    // Вызов из других потоков: добавить остановку на этаже
    void addTargetFloor(int floor);

    // Точка входа потока — блокирующий цикл
    void run();
    void stop();

    int           getId()           const { return id; }
    int           getCurrentFloor() const { return currentFloor.load(); }
    ElevatorState getState()        const;
    bool          isIdle()          const;
    std::string   getStateString()  const;

private:
    // Ближайший этаж из targetFloors; вызывать при удерживаемом mtx
    int getNearestTarget() const;

    int                     id;
    std::atomic<int>        currentFloor;
    ElevatorState           state;
    std::set<int>           targetFloors;

    mutable std::mutex      mtx;
    std::condition_variable cv;
    std::atomic<bool>       running;
};
