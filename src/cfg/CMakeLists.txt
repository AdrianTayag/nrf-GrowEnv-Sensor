target_sources(app 
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/product_cfg.h
        ${CMAKE_CURRENT_LIST_DIR}/product_msgs.h
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/product_msgs.c

)

target_include_directories(app PUBLIC ${CMAKE_CURRENT_LIST_DIR})