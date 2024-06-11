target_sources(app 
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/ui_mgr.c
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/ui_mgr.h
)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_LIST_DIR})