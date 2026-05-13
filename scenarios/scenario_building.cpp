// Сценарий 1: создание здания с допустимыми параметрами (мини-программа).
#include "Building.h"
#include <iostream>

int main() {
    try {
        Building b(10, 2);
        std::cout << "OK: floors=" << b.getFloorsCount() << " elevators=" << b.getElevators().size() << "\n";
        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "FAIL: " << ex.what() << "\n";
        return 1;
    }
}
