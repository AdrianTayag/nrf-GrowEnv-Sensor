/*
 * sensor_mgr.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include "sensor_mgr.h"

void sensorMgr_start(void)
{
    printk("Sensor Mgr Started\n");
    for(;;)
    {
        k_msleep(1000);
    }
}