add_executable(scenario_building scenario_building.cpp)
target_link_libraries(scenario_building PRIVATE elevator_core)

add_executable(scenario_request scenario_request.cpp)
target_link_libraries(scenario_request PRIVATE elevator_core)

add_executable(scenario_strategy scenario_strategy.cpp)
target_link_libraries(scenario_strategy PRIVATE elevator_core)

add_executable(scenario_simulation_lifecycle scenario_simulation_lifecycle.cpp)
target_link_libraries(scenario_simulation_lifecycle PRIVATE elevator_core)
