// Сценарий 2: проверка заявок Request (валидация границ).
#include "Request.h"
#include <iostream>

int main() {
    int failed = 0;
    if (!Request(1, 5, Direction::UP).validate(10)) {
        std::cerr << "FAIL: valid trip\n";
        ++failed;
    }
    if (Request(1, 1, Direction::NONE).validate(10)) {
        std::cerr << "FAIL: same floor must be invalid\n";
        ++failed;
    }
    if (Request(0, 5, Direction::UP).validate(10)) {
        std::cerr << "FAIL: source 0\n";
        ++failed;
    }
    if (failed == 0)
        std::cout << "OK: request validation scenarios passed\n";
    return failed;
}
