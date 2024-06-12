/*
 * sensor_mgr.c
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#include "sensor_mgr.h"
#include "product_cfg.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/drivers/adc.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>

static bool soilMoisture_init( void );
static int soilMoisture_read( void );

static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));
static int16_t buf;
static uint32_t count;
struct adc_sequence sequence = {
    .buffer = &buf,
    /* buffer size in bytes, not number of samples */
    .buffer_size = sizeof(buf),
};

LOG_MODULE_REGISTER(sensor_mgr, LOG_LEVEL_DBG);

static bool soilMoisture_init( void )
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

static int soilMoisture_read( void )
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
    } else {
        LOG_INF(" = %d mV", val_mv);
    }

    return val_mv;
}

void sensorMgr_start(void)
{
    LOG_INF("Sensor Mgr Started\n");
    soilMoisture_init();

    for(;;)
    {
        soilMoisture_read();
        k_msleep(1000);
    }
}

K_THREAD_DEFINE(sensorMgr_id, STACKSIZE, sensorMgr_start, NULL, NULL, NULL,
        THREAD_MID_PRIORITY, 0, 0);
