add_subdirectory(cfg)
add_subdirectory(sensor)
add_subdirectory(telemetry)
add_subdirectory(ui)

target_sources(app PRIVATE
  main.c
)
target_include_directories(app PUBLIC ${CMAKE_CURRENT_LIST_DIR})