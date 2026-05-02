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
=== Симулятор работы лифтов в здании ===

Количество этажей    (2-20): 10
Количество лифтов     (1-5): 2
[Симуляция] Запущена: 2 лифт(а), 10 этаж(ей).

[1] Вызвать лифт   [2] Показать статус   [3] Выход
Выбор: 1
С этажа (1-10): 1
На этаж (1-10): 7
[UI] Заявка принята. Введите '2' для просмотра статуса лифтов.
[Диспетчер] Лифт 1 назначен: этаж 1 -> этаж 7
  [Лифт 1] Движение ВВЕРХ  -> Этаж 2
  [Лифт 1] Движение ВВЕРХ  -> Этаж 3
  ...
  [Лифт 1] Прибыл на этаж 7  --  ДВЕРИ ОТКРЫТЫ
  [Лифт 1] Этаж 7  --  ДВЕРИ ЗАКРЫТЫ

[1] Вызвать лифт   [2] Показать статус   [3] Выход
Выбор: 2

--- Статус лифтов ---
  Лифт 1  |  Этаж:  7  |  ОЖИДАНИЕ
  Лифт 2  |  Этаж:  1  |  ОЖИДАНИЕ
---------------------
```

---

## Использованные сторонние библиотеки

| Библиотека | Назначение | Подключение |
|---|---|---|
| **Threads** (pthreads / WinThreads) | Многопоточность (`std::thread`, `std::mutex`, `std::condition_variable`) | `find_package(Threads REQUIRED)` в CMakeLists.txt |

Все остальные зависимости — стандартная библиотека C++17.


