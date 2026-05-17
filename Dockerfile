# =============================================================================
# Практическое задание «Контейнеризация» — симулятор лифтов
#
# Многоэтапная сборка (multi-stage):
#   builder  — чистая Linux, явные зависимости, cmake, Catch2-тесты
#   runtime  — только готовые бинарники, без объектников и CMake-кэша
# =============================================================================

# -----------------------------------------------------------------------------
# Этап 1: BUILDER — сборка и прогон unit-тестов Catch2
# -----------------------------------------------------------------------------
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

# Все зависимости для сборки перечислены явно (требование ТЗ)
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

# FetchContent скачивает Catch2 при конфигурации — нужен интернет на этапе build
RUN cmake -S . -B build \
        -DCMAKE_BUILD_TYPE=Release \
        -DELEVATOR_BUILD_TESTS=ON \
        -DELEVATOR_BUILD_SCENARIOS=ON \
    && cmake --build build --parallel "$(nproc)" \
    && cd build && ctest --output-on-failure

# -----------------------------------------------------------------------------
# Этап 2: RUNTIME — минимальный образ для запуска программы
# -----------------------------------------------------------------------------
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Runtime: только то, что нужно для запуска скомпилированного C++17-бинарника
RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /src/build/elevator_sim              /usr/local/bin/elevator_sim
COPY --from=builder /src/build/tests/elevator_tests      /usr/local/bin/elevator_tests
COPY --from=builder /src/build/scenarios/                /usr/local/bin/scenarios/

WORKDIR /usr/local/bin

# Основная функция контейнера — интерактивный симулятор (ТЗ п.6)
# Аргументы командной строки передаются в elevator_sim (см. main.cpp)
ENTRYPOINT ["/usr/local/bin/elevator_sim"]
