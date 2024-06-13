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
    LOG_INF("ADC reading[%u]: %s, channel %d: Raw: %d", count++, adc_channel.dev->name,
        adc_channel.channel_id, val_mv);

    ret = adc_raw_to_millivolts_dt(&adc_channel, &val_mv);
    if (ret < 0) {
        LOG_WRN(" (value in mV not available)\n");
    } 
    else {
        LOG_INF(" = %d mV", val_mv);
        sensor_msg soil_data = {0};

        soil_data.sensor = SOIL_MOISTURE;
        soil_data.value = ret;
        zbus_chan_pub(&sensor_data_chan, &soil_data, K_MSEC(250));
    }
}

void sensorMgr_start(void)
{
    LOG_INF("Sensor Mgr Started\n");
    soilMoisture_init();
    for(;;)
    {
        // Convert to timer based once ZBus is implemented
        soilMoisture_read();
        k_msleep(1000);
    }
}

K_THREAD_DEFINE(sensorMgr_id, STACKSIZE, sensorMgr_start, NULL, NULL, NULL,
        THREAD_MID_PRIORITY, 0, 0);
