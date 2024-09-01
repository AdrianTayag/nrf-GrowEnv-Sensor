/*
 * ess.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/kernel.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "ess.h"

#include <zephyr/logging/log.h>

#define SENSOR_1_NAME				"DHT Temperature"
#define SENSOR_2_NAME				"DHT Humidity"
#define SENSOR_3_NAME				"Soil Moisture"

LOG_MODULE_DECLARE(ESS);

struct es_measurement {
	uint16_t flags; /* Reserved for Future Use */
	uint8_t sampling_func;
	uint32_t meas_period;
	uint32_t update_interval;
	uint8_t application;
	uint8_t meas_uncertainty;
};

struct read_es_measurement_rp {
	uint16_t flags; /* Reserved for Future Use */
	uint8_t sampling_function;
	uint8_t measurement_period[3];
	uint8_t update_interval[3];
	uint8_t application;
	uint8_t measurement_uncertainty;
} __packed;

static int16_t temp_value;
static int16_t humidity_value;
static int16_t soil_moisture_value;
struct es_measurement meas;

static ssize_t read_es_measurement(struct bt_conn *conn,
				   const struct bt_gatt_attr *attr, void *buf,
				   uint16_t len, uint16_t offset)
{
	const struct es_measurement *value = attr->user_data;
	struct read_es_measurement_rp rsp;

	rsp.flags = sys_cpu_to_le16(value->flags);
	rsp.sampling_function = value->sampling_func;
	sys_put_le24(value->meas_period, rsp.measurement_period);
	sys_put_le24(value->update_interval, rsp.update_interval);
	rsp.application = value->application;
	rsp.measurement_uncertainty = value->meas_uncertainty;

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &rsp,
				 sizeof(rsp));
}

static ssize_t read_u16(struct bt_conn *conn, const struct bt_gatt_attr *attr,
			void *buf, uint16_t len, uint16_t offset)
{
	const uint16_t *u16 = attr->user_data;
	uint16_t value = sys_cpu_to_le16(*u16);

	return bt_gatt_attr_read(conn, attr, buf, len, offset, &value,
				 sizeof(value));
}


/* Environmental Sensing Service Declaration */
BT_GATT_SERVICE_DEFINE(ess_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_ESS),
    // Temperature
    // Characteristic + Value?
    BT_GATT_CHARACTERISTIC(BT_UUID_TEMPERATURE,
                BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                BT_GATT_PERM_READ,
                read_u16, NULL, &temp_value),
    // Descriptor?
    BT_GATT_DESCRIPTOR(BT_UUID_ES_MEASUREMENT,
                BT_GATT_PERM_READ, read_es_measurement, NULL, &meas),
    BT_GATT_CUD(SENSOR_1_NAME, BT_GATT_PERM_READ),

    // Humidity
    // Characteristic + Value?
    BT_GATT_CHARACTERISTIC(BT_UUID_HUMIDITY,
                BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                BT_GATT_PERM_READ,
                read_u16, NULL, &humidity_value),
    // Descriptor?
    BT_GATT_DESCRIPTOR(BT_UUID_ES_MEASUREMENT,
                BT_GATT_PERM_READ, read_es_measurement, NULL, &meas),
    BT_GATT_CUD(SENSOR_2_NAME, BT_GATT_PERM_READ),

    // Soil Moisture
    // Characteristic + Value?
    BT_GATT_CHARACTERISTIC(BT_UUID_HUMIDITY,
                BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                BT_GATT_PERM_READ,
                read_u16, NULL, &soil_moisture_value),
    // Descriptor?
    BT_GATT_DESCRIPTOR(BT_UUID_ES_MEASUREMENT,
                BT_GATT_PERM_READ, read_es_measurement, NULL, &meas),
    BT_GATT_CUD(SENSOR_3_NAME, BT_GATT_PERM_READ),
);

int ess_init( void )
{
	return 0;
}