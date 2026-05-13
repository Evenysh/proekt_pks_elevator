include(FetchContent)
FetchContent_Declare(
    Catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.5.4.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(Catch2)

add_executable(elevator_tests
    test_request.cpp
    test_building.cpp
    test_elevator.cpp
    test_strategy.cpp
    test_simulation.cpp
    test_singleton.cpp
)
target_link_libraries(elevator_tests PRIVATE elevator_core Catch2::Catch2WithMain)
# Тестовые .cpp, подключающие ConsoleLog.h, не шлют шум в cout при фильтрах Catch2.
target_compile_definitions(elevator_tests PRIVATE ELEVATOR_QUIET_CONSOLE=1)

include(CTest)
add_test(NAME elevator_unit_tests COMMAND elevator_tests)
set_tests_properties(elevator_unit_tests PROPERTIES TIMEOUT 180)
