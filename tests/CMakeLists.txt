cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(sensor_mgr_tests)

target_sources(app PRIVATE test_sensor_mgr.c)