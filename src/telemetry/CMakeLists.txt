target_sources(app 
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/telemetry_mgr.h
        ${CMAKE_CURRENT_LIST_DIR}/ess.h
)

target_sources_ifndef(CONFIG_BOARD_NATIVE_SIM app
                     PRIVATE
                        ${CMAKE_CURRENT_LIST_DIR}/telemetry_mgr.c
                        ${CMAKE_CURRENT_LIST_DIR}/ess.c)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_LIST_DIR})