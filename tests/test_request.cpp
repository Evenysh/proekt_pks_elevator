#include <catch2/catch_test_macros.hpp>
#include <string>
#include "Request.h"

// ---------- Request::validate (не менее 7 различных проверок) ----------

TEST_CASE("Request::validate: корректная поездка вверх в пределах здания", "[request][validate]")
{
    Request r(1, 5, Direction::UP);
    REQUIRE(r.validate(10));
}

TEST_CASE("Request::validate: корректная поездка вниз", "[request][validate]")
{
    Request r(9, 2, Direction::DOWN);
    REQUIRE(r.validate(10));
}

TEST_CASE("Request::validate: границы этажей 1 и N", "[request][validate]")
{
    Request r(1, 10, Direction::UP);
    REQUIRE(r.validate(10));
    Request r2(10, 1, Direction::DOWN);
    REQUIRE(r2.validate(10));
}

TEST_CASE("Request::validate: этаж источника 0 — недопустимо", "[request][validate]")
{
    Request r(0, 5, Direction::UP);
    REQUIRE_FALSE(r.validate(10));
}

TEST_CASE("Request::validate: этаж назначения больше N", "[request][validate]")
{
    Request r(1, 11, Direction::UP);
    REQUIRE_FALSE(r.validate(10));
}

TEST_CASE("Request::validate: одинаковые этажи", "[request][validate]")
{
    Request r(4, 4, Direction::NONE);
    REQUIRE_FALSE(r.validate(10));
}

TEST_CASE("Request::validate: отрицательный этаж назначения", "[request][validate]")
{
    Request r(3, -1, Direction::DOWN);
    REQUIRE_FALSE(r.validate(10));
}

// ---------- getSourceFloor / getTargetFloor ----------

TEST_CASE("Request: геттеры этажей после конструктора", "[request]")
{
    Request r(2, 8, Direction::UP);
    REQUIRE(r.getSourceFloor() == 2);
    REQUIRE(r.getTargetFloor() == 8);
}

TEST_CASE("Request: setTargetFloor меняет целевой этаж", "[request]")
{
    Request r(1, 5, Direction::UP);
    r.setTargetFloor(7);
    REQUIRE(r.getTargetFloor() == 7);
}

TEST_CASE("Request: направление из конструктора", "[request]")
{
    Request up(1, 2, Direction::UP);
    REQUIRE(up.getDirection() == Direction::UP);
    Request down(5, 3, Direction::DOWN);
    REQUIRE(down.getDirection() == Direction::DOWN);
}

TEST_CASE("Request: статус по умолчанию PENDING", "[request]")
{
    Request r(1, 2, Direction::UP);
    REQUIRE(r.getStatus() == RequestStatus::PENDING);
}

TEST_CASE("Request: смена статуса", "[request]")
{
    Request r(1, 2, Direction::UP);
    r.setStatus(RequestStatus::IN_PROGRESS);
    REQUIRE(r.getStatus() == RequestStatus::IN_PROGRESS);
    r.setStatus(RequestStatus::COMPLETED);
    REQUIRE(r.getStatus() == RequestStatus::COMPLETED);
}

// ---------- getDirectionString (5+ вариантов) ----------

TEST_CASE("Request::getDirectionString: UP", "[request]")
{
    Request r(1, 2, Direction::UP);
    REQUIRE(r.getDirectionString() == "UP");
}

TEST_CASE("Request::getDirectionString: DOWN", "[request]")
{
    Request r(3, 1, Direction::DOWN);
    REQUIRE(r.getDirectionString() == "DOWN");
}

TEST_CASE("Request::getDirectionString: NONE", "[request]")
{
    Request r(1, 2, Direction::NONE);
    REQUIRE(r.getDirectionString() == "NONE");
}

TEST_CASE("Request::getDirectionString: после конструктора с DOWN", "[request]")
{
    Request r(10, 1, Direction::DOWN);
    REQUIRE(r.getDirectionString() != "UP");
    REQUIRE(r.getDirectionString() == "DOWN");
}

TEST_CASE("Request::getDirectionString: не совпадает с UP при NONE", "[request]")
{
    Request r(1, 5, Direction::NONE);
    REQUIRE_FALSE(r.getDirectionString() == "UP");
}

// ---------- getStatusString (5+ вариантов) ----------

TEST_CASE("Request::getStatusString: PENDING", "[request]")
{
    Request r(1, 2, Direction::UP);
    REQUIRE(r.getStatusString() == "PENDING");
}

TEST_CASE("Request::getStatusString: IN_PROGRESS", "[request]")
{
    Request r(1, 2, Direction::UP);
    r.setStatus(RequestStatus::IN_PROGRESS);
    REQUIRE(r.getStatusString() == "IN_PROGRESS");
}

TEST_CASE("Request::getStatusString: COMPLETED", "[request]")
{
    Request r(1, 2, Direction::UP);
    r.setStatus(RequestStatus::COMPLETED);
    REQUIRE(r.getStatusString() == "COMPLETED");
}

TEST_CASE("Request::getStatusString: цепочка смен статусов", "[request]")
{
    Request r(1, 2, Direction::UP);
    REQUIRE(r.getStatusString() == "PENDING");
    r.setStatus(RequestStatus::IN_PROGRESS);
    REQUIRE(r.getStatusString() == "IN_PROGRESS");
    r.setStatus(RequestStatus::COMPLETED);
    REQUIRE(r.getStatusString() == "COMPLETED");
}

TEST_CASE("Request::getStatusString: после IN_PROGRESS снова читается", "[request]")
{
    Request r(4, 9, Direction::UP);
    r.setStatus(RequestStatus::IN_PROGRESS);
    REQUIRE(r.getStatusString().find("PROGRESS") != std::string::npos);
}
