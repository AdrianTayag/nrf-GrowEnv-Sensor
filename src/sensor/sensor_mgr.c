/*
 * sensor_mgr.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include "sensor_mgr.h"
#include "product_cfg.h"
#include "product_msgs.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>

static int soilMoisture_init( void );
static void soilMoisture_read( void );

static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));
static int16_t buf;
static uint32_t count;
struct adc_sequence sequence = {
    .buffer = &buf,
    /* buffer size in bytes, not number of samples */
    .buffer_size = sizeof(buf),
};

LOG_MODULE_REGISTER(sensor_mgr, LOG_LEVEL_DBG);

ZBUS_CHAN_DECLARE(sensor_data_chan);

static int soilMoisture_init( void )
{
    int ret = false;

	if (!adc_is_ready_dt(&adc_channel)) {
		LOG_ERR("ADC controller devivce %s not ready", adc_channel.dev->name);
		return 0;
	}

	ret = adc_channel_setup_dt(&adc_channel);
	if (ret < 0) {
		LOG_ERR("Could not setup channel #%d (%d)", 0, ret);
		return 0;
	}

	ret = adc_sequence_init_dt(&adc_channel, &sequence);
	if (ret < 0) {
		LOG_ERR("Could not initalize sequnce");
		return 0;
	}
}

static void soilMoisture_read( void )
{
    int ret = 0;
    int val_mv;

    ret = adc_read(adc_channel.dev, &sequence);
    if (ret < 0) {
        LOG_ERR("Could not read (%d)", ret);
    }

    val_mv = (int)buf;
    // LOG_INF("ADC reading[%u]: %s, channel %d: Raw: %d", count++, adc_channel.dev->name,
    //     adc_channel.channel_id, val_mv);

    ret = adc_raw_to_millivolts_dt(&adc_channel, &val_mv);
    if (ret < 0) {
        LOG_WRN(" (value in mV not available)\n");
    } 
    else {
        LOG_INF("Soil ADC Reading = %d mV", val_mv);
        sensor_msg soil_data = {0};

        soil_data.sensor = SOIL_MOISTURE;
        soil_data.value = ret;
        zbus_chan_pub(&sensor_data_chan, &soil_data, K_MSEC(250));
    }
}

static void dht_init( void )
{
    const struct device *const dht22 = DEVICE_DT_GET_ONE(aosong_dht);
	if (!device_is_ready(dht22)) {
		printf("Device %s is not ready\n", dht22->name);
	}
}

void sensorMgr_start(void)
{
    LOG_INF("Sensor Mgr Started\n");
    soilMoisture_init();
    dht_init();
    for(;;)
    {
        // Convert to timer based once ZBus is implemented
        k_msleep(2000);
        soilMoisture_read();

        const struct device *const dht22 = DEVICE_DT_GET_ONE(aosong_dht);
        int rc = sensor_sample_fetch(dht22);
        if (rc != 0) {
            LOG_INF("fetch failed: %d\n", rc);
            continue;
        }
        struct sensor_value temperature = { 0 };
        struct sensor_value humidity = { 0 };

        rc = sensor_channel_get(dht22, SENSOR_CHAN_AMBIENT_TEMP,
                    &temperature);
        if (rc == 0) {
            rc = sensor_channel_get(dht22, SENSOR_CHAN_HUMIDITY,
                        &humidity);
        }
        if (rc != 0) {
            LOG_INF("get failed: %d\n", rc);
            continue;
        }
        else
        {
            LOG_INF("DHT %d Cel ; %d %%RH\n\n",
                    temperature.val1,
                    humidity.val1);
        }

    }
}

K_THREAD_DEFINE(sensorMgr_id, STACKSIZE, sensorMgr_start, NULL, NULL, NULL,
        THREAD_MID_PRIORITY, 0, 0);
