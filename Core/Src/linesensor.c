/**
  ******************************************************************************
  * @file           : linesensor.c
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
#include "linesensor.h"
#include "main.h"

/* External ADC handle (declared in main.c) */
extern ADC_HandleTypeDef hadc1;

#define LINE_SENSOR_ADC_TIMEOUT_MS  10U

static uint16_t line_sensor_last_1 = 0U;
static uint16_t line_sensor_last_2 = 0U;
static uint8_t line_sensor_initialized = 0U;

/**
  * @brief Initialize line sensor module state variables
  * @retval None
  */
void linesensor_init(void)
{
  line_sensor_last_1 = 0U;
  line_sensor_last_2 = 0U;
  line_sensor_initialized = 1U;
}

/**
  * @brief Read one line sensor value through ADC1
  * @param sensor: LINE_SENSOR_1 or LINE_SENSOR_2
  * @retval ADC value (0-4095), 0 on invalid sensor or HAL error
  */
uint16_t linesensor_read(uint8_t sensor)
{
  ADC_ChannelConfTypeDef sConfig = {0};
  uint16_t value = 0U;

  if (line_sensor_initialized == 0U)
  {
    linesensor_init();
  }

  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

  switch (sensor)
  {
    case LINE_SENSOR_1:
      sConfig.Channel = ADC_CHANNEL_4;
      break;
    case LINE_SENSOR_2:
      sConfig.Channel = ADC_CHANNEL_1;
      break;
    default:
      return 0u;
  }

  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) return 0u;
  if (HAL_ADC_Start(&hadc1) != HAL_OK) return 0u;
  if (HAL_ADC_PollForConversion(&hadc1, LINE_SENSOR_ADC_TIMEOUT_MS) != HAL_OK)
  {
    HAL_ADC_Stop(&hadc1);
    return 0u;
  }

  value = (uint16_t)HAL_ADC_GetValue(&hadc1);

  if (HAL_ADC_Stop(&hadc1) != HAL_OK) return 0u;

  if (sensor == LINE_SENSOR_1)
  {
    line_sensor_last_1 = value;
  }
  else
  {
    line_sensor_last_2 = value;
  }

  return value;
}
