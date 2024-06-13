/*
 * product_msgs.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include "product_msgs.h"
#include "sensor_mgr.h"
#include "ui_mgr.h"
#include "telemetry_mgr.h"

ZBUS_CHAN_DEFINE(sensor_data_chan,  /* Name */
                 sensor_msg, /* Message type */

                 NULL, /* Validator */
                 NULL, /* User data */
                 ZBUS_OBSERVERS(ui_mgr_sub, telemetry_mgr_sub), /* observers */
                 ZBUS_MSG_INIT(.sensor = 0, .value = 0)		      /* Initial value {0} */
);