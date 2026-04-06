# Elevator Building Simulator

Многопоточный симулятор работы нескольких лифтов в многоэтажном здании.  
Дисциплина: Программирование корпоративных систем.  
Автор: Разина Евгения Андреевна, группа ЭФБО-02-24.

---

## Краткая суть проекта

Программа моделирует работу лифтовой системы: принимает вызовы с этажей,
распределяет их между лифтами и отображает текущее состояние системы.
Каждый лифт выполняется в отдельном потоке (`std::thread`), что обеспечивает
параллельную обработку заявок.

Применяемые паттерны: **Singleton**, **Strategy**, **State**, **Observer**, **Factory Method**, **Facade**.

---

## Структура проекта

```
.
├── CMakeLists.txt
├── README.md
├── include/
│   ├── IObserver.h
│   ├── Request.h
│   ├── Elevator.h
│   ├── Building.h
│   ├── ElevatorController.h
│   ├── Simulation.h
│   └── UserInterface.h
└── src/
    ├── main.cpp
    ├── Request.cpp
    ├── Elevator.cpp
    ├── Building.cpp
    ├── ElevatorController.cpp
    ├── Simulation.cpp
    └── UserInterface.cpp
```

---

## Инструкция по сборке

### Требования

| Инструмент | Версия |
|---|---|
| CMake | >= 3.10 |
| Компилятор C++ | GCC 7+ / Clang 5+ / MSVC 2017+ (C++17) |

### Команды сборки

```bash
cmake -S . -B build
cmake --build build
```

### Запуск

```bash
# Linux / macOS
cd build
./elevator_sim

# Windows — Ninja / MinGW Makefiles и т.п. (exe сразу в build/)
cd build
elevator_sim.exe

# Windows — генератор Visual Studio (exe в подпапке конфигурации)
# по умолчанию после cmake --build build собирается Debug:
build/Debug/elevator_sim.exe
# Release: cmake --build build --config Release
build/Release/elevator_sim.exe
```

В Git Bash из корня проекта: `./build/Debug/elevator_sim.exe` (или `./build/Release/elevator_sim.exe`).

---

## Пример запуска

```
=== Elevator Building Simulator ===

Number of floors    (2-20): 10
Number of elevators (1-5):  2
[Simulation] Started: 2 elevator(s), 10 floors.

[1] Call elevator   [2] Show status   [3] Exit
Choice: 1
From floor (1-10): 1
To   floor (1-10): 7
[UI] Request accepted. Enter '2' to check elevator status.
[Controller] Elevator 1 assigned: floor 1 -> floor 7
  [Elevator 1] Moving UP   -> Floor 2
  [Elevator 1] Moving UP   -> Floor 3
  ...
  [Elevator 1] Arrived at floor 7  --  DOORS OPEN
  [Elevator 1] Floor 7  --  DOORS CLOSED

[1] Call elevator   [2] Show status   [3] Exit
Choice: 2

--- Elevator Status ---
  Elevator 1  |  Floor:  7  |  IDLE
  Elevator 2  |  Floor:  1  |  IDLE
-----------------------
```

---

## Использованные сторонние библиотеки

| Библиотека | Назначение | Подключение |
|---|---|---|
| **Threads** (pthreads / WinThreads) | Многопоточность (`std::thread`, `std::mutex`, `std::condition_variable`) | `find_package(Threads REQUIRED)` в CMakeLists.txt |

Все остальные зависимости — стандартная библиотека C++17.


