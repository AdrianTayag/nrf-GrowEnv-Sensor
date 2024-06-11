/*
 * telemetry_mgr.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include "telemetry_mgr.h"

void telemetryMgr_start(void)
{
    printk("Telemetry Mgr Started\n");
    for(;;)
    {
        k_msleep(1000);
    }
}