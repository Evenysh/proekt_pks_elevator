#pragma once
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>
#include "Elevator.h"
#include "Request.h"

// ============================================================
// IDispatchStrategy — интерфейс паттерна Strategy
// Задаёт алгоритм выбора лифта для обработки заявки
// ============================================================
class IDispatchStrategy {
public:
    virtual ~IDispatchStrategy() = default;
    virtual std::shared_ptr<Elevator> selectElevator(
        const std::vector<std::shared_ptr<Elevator>>& elevators,
        const Request& request) = 0;
};

// Конкретная стратегия: лифт, ближайший к этажу вызова
class NearestElevatorStrategy : public IDispatchStrategy {
public:
    std::shared_ptr<Elevator> selectElevator(
        const std::vector<std::shared_ptr<Elevator>>& elevators,
        const Request& request) override;
};

// ============================================================
// ElevatorController — паттерн Singleton
// Один экземпляр распределяет все заявки по лифтам
// ============================================================
class ElevatorController {
public:
    static ElevatorController& getInstance();

    void setElevators(std::vector<std::shared_ptr<Elevator>> elevs);
    void setStrategy(std::shared_ptr<IDispatchStrategy> strat);

    void addRequest(std::shared_ptr<Request> request);
    void monitorSystem();   // точка входа потока (блокирующий цикл)
    void stop();

private:
    ElevatorController();
    ElevatorController(const ElevatorController&)            = delete;
    ElevatorController& operator=(const ElevatorController&) = delete;

    void assignElevator(std::shared_ptr<Request> request);

    std::vector<std::shared_ptr<Elevator>> elevators;
    std::queue<std::shared_ptr<Request>>   pendingRequests;
    std::shared_ptr<IDispatchStrategy>     strategy;

    std::mutex              mtx;
    std::condition_variable cv;
    std::atomic<bool>       running;
};
