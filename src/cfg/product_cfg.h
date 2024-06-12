/*
 * product_cfg.h
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)

#define STACKSIZE 1024
#define THREAD_LOW_PRIORITY 7
#define THREAD_MID_PRIORITY 6
#define THREAD_HIGH_PRIORITY 5