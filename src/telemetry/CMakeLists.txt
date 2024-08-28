target_sources(app 
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/telemetry_mgr.h
)

target_sources_ifndef(CONFIG_BOARD_NATIVE_SIM app
                     PRIVATE ${CMAKE_CURRENT_LIST_DIR}/telemetry_mgr.c)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_LIST_DIR})