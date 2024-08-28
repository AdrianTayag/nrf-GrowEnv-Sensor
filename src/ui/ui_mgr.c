/*
 * ui_mgr.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include "ui_mgr.h"
#include "product_cfg.h"
#include "product_msgs.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <dk_buttons_and_leds.h>
#include <zephyr/drivers/gpio.h>
#include <bluetooth/services/lbs.h>

LOG_MODULE_REGISTER(ui_mgr, LOG_LEVEL_DBG);

ZBUS_MSG_SUBSCRIBER_DEFINE(ui_mgr_sub);

#ifndef CONFIG_BOARD_NATIVE_SIM
static bool app_button_state;

static void button_changed(uint32_t button_state, uint32_t has_changed)
{
	LOG_INF("Button pressed: %d\n", has_changed);
    if (has_changed & USER_BUTTON) {
		uint32_t user_button_state = button_state & USER_BUTTON;

		bt_lbs_send_button_state(user_button_state);

		// TODO: Add button debouncing (40ms) for valid press
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
#endif /* CONFIG_BOARD_NATIVE_SIM */


void uiMgr_start(void)
{
    LOG_INF("UI Mgr Started");
#ifndef CONFIG_BOARD_NATIVE_SIM
	int err = dk_leds_init();
	if (err) {
		LOG_ERR("LEDs init failed (err %d)\n", err);
	}

	err = init_button();
	if (err) {
		LOG_ERR("Button init failed (err %d)\n", err);
	}
#endif /* CONFIG_BOARD_NATIVE_SIM */

	const struct zbus_channel *chan;
	sensor_msg msg = {0};
	while (!zbus_sub_wait_msg(&ui_mgr_sub, &chan, &msg, K_FOREVER)) {
		LOG_INF("UI_MGR: Sensor msg received: Sensor = %u, Value = %u, ",
			msg.sensor, msg.value);
		// TODO: Update display buffer, send work item for LCD display
	}
	LOG_ERR("UI MGR Thread exited.\n");
}

K_THREAD_DEFINE(uiMgr_id, STACKSIZE, uiMgr_start, NULL, NULL, NULL,
		THREAD_MID_PRIORITY, 0, 0);