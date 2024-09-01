/*
 * ess.h
 * Copyright (c) 2024 Adrian Tayag
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef ESS_H_
#define ESS_H_

int ess_init( void );
void ess_update_temp_value(int16_t new_value);
void ess_update_humidity_value(int16_t new_value);
void ess_update_soil_moisture_value(int16_t new_value);

#endif /* ESS_H_ */