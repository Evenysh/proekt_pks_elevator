cmake_minimum_required(VERSION 3.10)
project(ElevatorSimulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Потоки: pthreads на Linux/macOS; встроенно в Windows (MSVC)
find_package(Threads REQUIRED)

set(SOURCES
    src/main.cpp
    src/Building.cpp
    src/Elevator.cpp
    src/Request.cpp
    src/ElevatorController.cpp
    src/Simulation.cpp
    src/UserInterface.cpp
)

add_executable(elevator_sim ${SOURCES})

target_include_directories(elevator_sim PRIVATE include)

target_link_libraries(elevator_sim PRIVATE Threads::Threads)

