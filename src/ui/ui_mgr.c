/*
 * ui_mgr.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include "ui_mgr.h"
#include "product_cfg.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <dk_buttons_and_leds.h>
#include <zephyr/drivers/gpio.h>

#define RUN_STATUS_LED          DK_LED1
#define CON_STATUS_LED          DK_LED2
#define RUN_LED_BLINK_INTERVAL  1000

#define USER_LED                DK_LED1

#define USER_BUTTON             DK_BTN1_MSK

static bool app_button_state;

LOG_MODULE_REGISTER(ui_mgr, LOG_LEVEL_DBG);

static void button_changed(uint32_t button_state, uint32_t has_changed)
{
	LOG_INF("Button pressed: %d\n", has_changed);
    if (has_changed & USER_BUTTON) {
		uint32_t user_button_state = button_state & USER_BUTTON;

		// bt_lbs_send_button_state(user_button_state);
		app_button_state = user_button_state ? true : false;
	}
}

static int init_button(void)
{
	int err;

	err = dk_buttons_init(button_changed);
	if (err) {
		LOG_ERR("Cannot init buttons (err: %d)\n", err);
	}

	return err;
}


void uiMgr_start(void)
{
    LOG_INF("UI Mgr Started\n");
	int err = dk_leds_init();
	if (err) {
		LOG_ERR("LEDs init failed (err %d)\n", err);
	}

	err = init_button();
	if (err) {
		LOG_ERR("Button init failed (err %d)\n", err);
	}
    for(;;)
    {
        k_msleep(1000);
    }
}

K_THREAD_DEFINE(uiMgr_id, STACKSIZE, uiMgr_start, NULL, NULL, NULL,
		THREAD_MID_PRIORITY, 0, 0);