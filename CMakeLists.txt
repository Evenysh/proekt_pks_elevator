cmake_minimum_required(VERSION 3.14)
project(ElevatorSimulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

# Ядро симуляции (без main) — для unit-тестов и мини-сценариев
set(CORE_SOURCES
    src/Building.cpp
    src/Elevator.cpp
    src/Request.cpp
    src/ElevatorController.cpp
    src/Simulation.cpp
    src/UserInterface.cpp
)

add_library(elevator_core STATIC ${CORE_SOURCES})
target_include_directories(elevator_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(elevator_core PUBLIC Threads::Threads)

add_executable(elevator_sim src/main.cpp)
target_link_libraries(elevator_sim PRIVATE elevator_core)

option(ELEVATOR_BUILD_TESTS "Сборка unit-тестов (Catch2)" ON)
if(ELEVATOR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

option(ELEVATOR_BUILD_SCENARIOS "Сборка мини-программ сценариев" ON)
if(ELEVATOR_BUILD_SCENARIOS)
    add_subdirectory(scenarios)
endif()
