/*
 * main.c
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
#include <zephyr/logging/log.h>

#include <soc.h>

LOG_MODULE_REGISTER(main_func, LOG_LEVEL_DBG);

int main(void)
{
	LOG_INF("Starting NRF GrowEnv Sensor Application...");
	
	// Main thread exits, freeing up memory
	return 0;
}
