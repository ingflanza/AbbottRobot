/**
  ******************************************************************************
  * @file           : linesensor.h
  * @brief          : Line sensor (TCRT5000) ADC driver
  * @author         : Federico Lanza
  * @date           : 2026-06-19
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 Federico Lanza.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef __LINESENSOR_H__
#define __LINESENSOR_H__

#include <stdint.h>

/* Sensor identifiers */
#define LINE_SENSOR_1  1U
#define LINE_SENSOR_2  2U

/**
  * @brief Initialize line sensor module state variables
  * @retval None
  */
void linesensor_init(void);

/**
  * @brief Read one line sensor value through ADC1
  * @param sensor: LINE_SENSOR_1 or LINE_SENSOR_2
  * @retval ADC value (0-4095), 0 on invalid sensor or HAL error
  */
uint16_t linesensor_read(uint8_t sensor);

#endif /* __LINESENSOR_H__ */
