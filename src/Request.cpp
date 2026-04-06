#include "Request.h"

Request::Request(int sourceFloor, int targetFloor, Direction direction)
    : sourceFloor(sourceFloor), targetFloor(targetFloor),
      direction(direction), status(RequestStatus::PENDING) {}

bool Request::validate(int totalFloors) const {
    if (sourceFloor < 1 || sourceFloor > totalFloors) return false;
    if (targetFloor  < 1 || targetFloor  > totalFloors) return false;
    if (sourceFloor == targetFloor) return false;
    return true;
}

std::string Request::getDirectionString() const {
    switch (direction) {
        case Direction::UP:   return "UP";
        case Direction::DOWN: return "DOWN";
        default:              return "NONE";
    }
}

std::string Request::getStatusString() const {
    switch (status) {
        case RequestStatus::PENDING:     return "PENDING";
        case RequestStatus::IN_PROGRESS: return "IN_PROGRESS";
        case RequestStatus::COMPLETED:   return "COMPLETED";
        default:                         return "UNKNOWN";
    }
}
