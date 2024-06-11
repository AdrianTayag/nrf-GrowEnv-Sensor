/*
 * thread_cfg.h
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */
#include <zephyr/kernel.h>

#include "sensor_mgr.h"
#include "ui_mgr.h"
#include "telemetry_mgr.h"

#define STACKSIZE 1024
#define THREAD_LOW_PRIORITY 7
#define THREAD_MID_PRIORITY 6
#define THREAD_HIGH_PRIORITY 5

K_THREAD_DEFINE(sensorMgr_id, STACKSIZE, sensorMgr_start, NULL, NULL, NULL,
        THREAD_MID_PRIORITY, 0, 0);
K_THREAD_DEFINE(uiMgr_id, STACKSIZE, uiMgr_start, NULL, NULL, NULL,
		THREAD_MID_PRIORITY, 0, 0);
K_THREAD_DEFINE(telemetryMgr_id, STACKSIZE, telemetryMgr_start, NULL, NULL, NULL,
		THREAD_MID_PRIORITY, 0, 0);