/*
 * telemetry_mgr.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include "telemetry_mgr.h"
#include "product_cfg.h"
#include "product_msgs.h"
#include "ess.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/settings/settings.h>
#include <dk_buttons_and_leds.h>

static void connected(struct bt_conn *conn, uint8_t err);
static void disconnected(struct bt_conn *conn, uint8_t reason);

LOG_MODULE_REGISTER(telemetry_mgr, LOG_LEVEL_DBG);

ZBUS_MSG_SUBSCRIBER_DEFINE(telemetry_mgr_sub);

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected        = connected,
	.disconnected     = disconnected,
};

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_ESS_VAL)),
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_ERR("Connection failed (err %u)\n", err);
		return;
	}

	LOG_INF("Connected\n");

	dk_set_led_on(CON_STATUS_LED);
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason %u)\n", reason);

	dk_set_led_off(CON_STATUS_LED);
}

void telemetryMgr_start(void)
{
    LOG_INF("Telemetry Mgr Started\n");

    int err;
	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
	}

	LOG_INF("Bluetooth initialized\n");
	err = ess_init();
	if (err) {
		LOG_ERR("Failed to init ESS (err:%d)\n", err);
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad),
			      sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)\n", err);
	}

	LOG_INF("Advertising successfully started\n");

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	const struct zbus_channel *chan;
	sensor_msg msg = {0};
	while (!zbus_sub_wait_msg(&telemetry_mgr_sub, &chan, &msg, K_FOREVER)) {
		LOG_INF("TELEMETRY_MGR: Sensor msg received: Sensor = %u, Value = %u, ",
			msg.sensor, msg.value);

		switch (msg.sensor) {
		case TEMPERATURE:
			ess_update_temp_value(msg.value);
			break;
		case RELATIVE_HUMIDITY:
			ess_update_humidity_value(msg.value);
			break;
		case SOIL_MOISTURE:
			ess_update_soil_moisture_value(msg.value);
			break;
		default:
			LOG_ERR("TELEMETRY_MGR: Invalid sensor type\n");
			break;
		}
	}
	LOG_ERR("TELEMETRY MGR Thread exited.\n");
}

K_THREAD_DEFINE(telemetryMgr_id, STACKSIZE, telemetryMgr_start, NULL, NULL, NULL,
		THREAD_MID_PRIORITY, 0, 0);
