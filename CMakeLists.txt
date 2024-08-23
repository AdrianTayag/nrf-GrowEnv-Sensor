cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(nrf-GrowEnv)

# NORDIC SDK APP START
add_subdirectory(src)
# add_subdirectory(tests)

# NORDIC SDK APP END
zephyr_library_include_directories(.)
