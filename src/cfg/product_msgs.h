/*
 * product_msgs.h
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

// Implement ZBUS for communicating between threads
// https://github.com/zephyrproject-rtos/zephyr/tree/main/samples/subsys/zbus/work_queue
#ifndef _MESSAGES_H_
#define _MESSAGES_H_

#include <zephyr/kernel.h>
#include <zephyr/zbus/zbus.h>
#include <stdint.h>

typedef enum {
    SOIL_MOISTURE = 0,
    TEMPERATURE,
    HUMIDITY,
}sensor_type;

typedef struct {
    sensor_type sensor;
    uint32_t value;
}sensor_msg;

#endif /* _MESSAGES_H_ */