/**
  ******************************************************************************
  * @file           : ultrasonic.h
  * @brief          : Ultrasonic distance sensor driver
  ******************************************************************************
  */
#ifndef __ULTRASONIC_H__
#define __ULTRASONIC_H__

#include <stdint.h>

/* Defines for ultrasonic sensor configuration */
#define ULTRASONIC_PORT           GPIOA
#define ULTRASONIC_PIN            GPIO_PIN_0
#define ULTRASONIC_TIMER          htim2
#define ULTRASONIC_TIMER_CHANNEL  TIM_CHANNEL_1

/* Time between measurements in milliseconds */
#define ULTRASONIC_TRIGGER_INTERVAL        500

/**
  * @brief Initialize ultrasonic module
  * @retval None
  */
void ultrasonic_init(void);

/**
  * @brief Process ultrasonic measurement state machine
  * @retval None
  */
void ultrasonic_measure(void);

/**
  * @brief Get last measured distance
  * @retval Distance in millimeters
  */
uint16_t ultrasonic_distance(void);

/**
  * @brief Get the error flag
  * @retval 1 if measurement has failed, 0 otherwise
  */
uint8_t ultrasonic_error(void);

#endif /* __ULTRASONIC_H__ */
