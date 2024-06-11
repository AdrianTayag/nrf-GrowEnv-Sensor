target_sources(app 
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/thread_cfg.h
)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_LIST_DIR})