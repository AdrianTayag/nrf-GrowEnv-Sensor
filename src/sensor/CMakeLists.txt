target_sources(app 
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/sensor_mgr.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/sensor_mgr.h
)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_LIST_DIR})