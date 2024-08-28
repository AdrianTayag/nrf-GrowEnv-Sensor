/*
 * product_cfg.h
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _PRODUCT_CFG_H_
#define _PRODUCT_CFG_H_


#define DEVICE_NAME             CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN         (sizeof(DEVICE_NAME) - 1)

#define STACKSIZE 1024
#define THREAD_LOW_PRIORITY 7
#define THREAD_MID_PRIORITY 6
#define THREAD_HIGH_PRIORITY 5

#define RUN_STATUS_LED          DK_LED1
#define CON_STATUS_LED          DK_LED2
#define RUN_LED_BLINK_INTERVAL  1000

#define USER_LED                DK_LED1

#define USER_BUTTON             DK_BTN1_MSK

#endif /* _PRODUCT_CFG_H_ */