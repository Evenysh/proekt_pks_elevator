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
├── Dockerfile              # контейнеризация (ПЗ «Контейнеризация»)
├── .dockerignore
├── README.md
├── include/
│   ├── ConsoleLog.h
│   ├── IObserver.h
│   ├── Request.h
│   ├── Elevator.h
│   ├── Building.h
│   ├── ElevatorController.h
│   ├── Simulation.h
│   └── UserInterface.h
├── src/
│   ├── main.cpp
│   ├── Request.cpp
│   ├── Elevator.cpp
│   ├── Building.cpp
│   ├── ElevatorController.cpp
│   ├── Simulation.cpp
│   └── UserInterface.cpp
├── tests/                    # unit-тесты Catch2 (ПЗ №4)
│   ├── CMakeLists.txt
│   ├── test_*.cpp
│   └── Тестовые_сценарии.txt
└── scenarios/                # мини-программы сценариев (ПЗ №4)
    ├── CMakeLists.txt
    └── scenario_*.cpp
```

---

## Инструкция по сборке

### Требования

| Инструмент | Версия |
|---|---|
| CMake | >= 3.14 (нужен для FetchContent Catch2) |
| Компилятор C++ | GCC 7+ / Clang 5+ / MSVC 2017+ (C++17) |

### Команды сборки

```bash
cmake -S . -B build
cmake --build build
```

### Запуск

Без аргументов программа спрашивает число этажей и лифтов с клавиатуры.
Можно передать параметры сразу:

```bash
./build/elevator_sim 10 2
./build/elevator_sim --floors 10 --elevators 2
./build/elevator_sim --help
```

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

Первый запуск `cmake` скачивает Catch2 с GitHub (нужен интернет). Отключить тесты: `-DELEVATOR_BUILD_TESTS=OFF`; отключить мини-сценарии: `-DELEVATOR_BUILD_SCENARIOS=OFF`.

### Тестирование (практическое занятие №4)

```bash
cmake -S . -B build
cmake --build build
cd build
ctest --output-on-failure    # или ./tests/elevator_tests
./scenarios/scenario_building
./scenarios/scenario_request
./scenarios/scenario_strategy
./scenarios/scenario_simulation_lifecycle
```

Список сценариев и пояснения: `tests/Тестовые_сценарии.txt`.

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

В Docker Catch2 подтягивается при сборке через `FetchContent` (см. `tests/CMakeLists.txt`).

---

## Контейнеризация (Docker)

Практическое задание: сборка и тесты в **чистой Linux** (Ubuntu 22.04), без установки
компилятора на хост-машину. Используется **multi-stage**-сборка: этап `builder` компилирует
проект и прогоняет Catch2; этап `runtime` содержит только готовые бинарники и `libstdc++6`.

### Требования на хосте

| Инструмент | Назначение |
|---|---|
| [Docker](https://www.docker.com/) | Сборка и запуск образа |
| Интернет | При `docker build` скачивается Catch2 с GitHub |

Все зависимости для **сборки внутри контейнера** перечислены в `Dockerfile`
(`build-essential`, `cmake`, `git`, `ca-certificates`).

### 1. Сборка образа

Выполнять из **корня репозитория** (где лежат `Dockerfile` и `CMakeLists.txt`):

```bash
docker build -t elevator-sim:latest .
```

**Что происходит:** Docker создаёт образ `elevator-sim:latest`. На этапе `builder` выполняются
`cmake`, компиляция и **`ctest --output-on-failure`**. Если хотя бы один тест Catch2 падает,
сборка образа **прерывается с ошибкой** — это и есть проверка тестов в контейнере.
В финальный образ попадают только `elevator_sim`, `elevator_tests` и мини-сценарии, без
исходников и папки `build/`.

### 2. Запуск симулятора (основная функция)

Интерактивный режим (`-it` — терминал + ввод с клавиатуры). Флаг `--init` рекомендуется,
чтобы Ctrl+C корректно завершал процесс в контейнере:

```bash
docker run -it --rm --init elevator-sim:latest --floors 10 --elevators 2
```

Альтернативные форматы аргументов (то же, что у локального `./elevator_sim`):

```bash
docker run -it --rm --init elevator-sim:latest 10 2
docker run -it --rm --init elevator-sim:latest -f 10 -e 2
```

| Флаг / аргумент | Пояснение |
|---|---|
| `-it` | Интерактивный TTY: меню, ввод этажей |
| `--rm` | Удалить контейнер после выхода |
| `--init` | Корректная передача сигналов (Ctrl+C); без него удобнее выходить через меню `3` |
| `--floors 10 --elevators 2` | Параметры здания без запроса с клавиатуры |

**Штатный выход из программы:** в меню выбрать `3` и Enter.

Быстрая демонстрация без ручного ввода в меню (сразу выход):

```bash
printf '3\n' | docker run -i --rm elevator-sim:latest 10 2
```

Здесь `-i` достаточно (без `-t`), потому что ввод идёт через pipe, а не с клавиатуры.

### 3. Повторный запуск unit-тестов Catch2

Тесты уже выполняются при `docker build`; отдельно их можно показать в **готовом** образе:

```bash
docker run --rm --entrypoint /usr/local/bin/elevator_tests elevator-sim:latest
```

`--entrypoint` подменяет точку входа (по умолчанию — `elevator_sim`), чтобы запустить
только тестовый исполняемый файл. Ожидаемый результат — сообщение Catch2 об успешном
прохождении всех тестов (код выхода 0).

### 4. Мини-сценарии (ПЗ №4) внутри контейнера

В образе лежат бинарники в `/usr/local/bin/scenarios/`:

```bash
docker run --rm --entrypoint /usr/local/bin/scenarios/scenario_building elevator-sim:latest
docker run --rm --entrypoint /usr/local/bin/scenarios/scenario_request elevator-sim:latest
docker run --rm --entrypoint /usr/local/bin/scenarios/scenario_strategy elevator-sim:latest
docker run --rm --entrypoint /usr/local/bin/scenarios/scenario_simulation_lifecycle elevator-sim:latest
```

Каждая команда запускает одну мини-программу и завершается; в stdout должно быть `OK: ...`.

### 5. Справка по аргументам в контейнере

```bash
docker run --rm elevator-sim:latest --help
```

### Соответствие заданию (кратко)

| Требование | Реализация |
|---|---|
| Dockerfile, автосборка | `Dockerfile`, `docker build` |
| Catch2 в контейнере | `ctest` при сборке; опционально `elevator_tests` при `docker run` |
| Аргументы командной строки | `main.cpp`, передаются после имени образа в `docker run` |
| Multi-stage, без артефактов сборки | Стадии `builder` / `runtime` |
| Чистая Linux, явные зависимости | `FROM ubuntu:22.04`, `apt-get install ...` |
| Рабочий контейнер | `docker run -it ... elevator-sim:latest` |