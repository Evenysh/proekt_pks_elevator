#pragma once
#include <string>

enum class Direction     { UP, DOWN, NONE };
enum class RequestStatus { PENDING, IN_PROGRESS, COMPLETED };

// ============================================================
// Request — одна заявка пассажира
// Создаётся через Simulation::submitRequest() (метод фабрики)
// ============================================================
class Request {
public:
    Request(int sourceFloor, int targetFloor,
            Direction direction = Direction::NONE);

    bool validate(int totalFloors) const;

    int           getSourceFloor()  const { return sourceFloor; }
    int           getTargetFloor()  const { return targetFloor; }
    Direction     getDirection()    const { return direction; }
    RequestStatus getStatus()       const { return status; }

    void setStatus(RequestStatus s) { status = s; }
    void setTargetFloor(int f)      { targetFloor = f; }

    std::string getDirectionString() const;
    std::string getStatusString()    const;

private:
    int           sourceFloor;
    int           targetFloor;
    Direction     direction;
    RequestStatus status;
};
