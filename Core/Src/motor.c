/**
  ******************************************************************************
  * @file           : motor.c
  * @brief          : Motor control module source
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
#include "motor.h"
#include "main.h"

/* External timer handle (declared in main.c) */
extern TIM_HandleTypeDef htim3;

/**
 * @brief  Calculate PWM value based on motor, direction, and velocity
 * @param  motor: Motor identifier
 * @param  direction: Motor direction
 * @param  velocity: Velocity offset (0-100)
 * @retval PWM compare value (1000-2000)
 */
static uint16_t motor_calculate_pwm(uint32_t motor, uint8_t direction, uint16_t velocity)
{
  uint16_t pwm_value = MOTOR_PWM_NEUTRAL;

  /* Clamp velocity to valid range */
  if (velocity > MOTOR_VELOCITY_MAX) {
    velocity = MOTOR_VELOCITY_MAX;
  }

  if (direction == MOTOR_DIRECTION_STOP) {
    return MOTOR_PWM_NEUTRAL;
  }

  /* Scale velocity from 0-100 range to 0-500 PWM delta */
  velocity = velocity * 5;

  /* Motor1: forward increases PWM (1500→2000), backward decreases (1500→1000) */
  if (motor == MOTOR1) {
    if (direction == MOTOR_DIRECTION_FORWARD) {
      pwm_value = MOTOR_PWM_NEUTRAL + velocity;
    } else {
      pwm_value = MOTOR_PWM_NEUTRAL - velocity;
    }
  }
  /* Motor2: forward decreases PWM (1500→1000), backward increases (1500→2000) */
  else if (motor == MOTOR2) {
    if (direction == MOTOR_DIRECTION_FORWARD) {
      pwm_value = MOTOR_PWM_NEUTRAL - velocity;
    } else {
      pwm_value = MOTOR_PWM_NEUTRAL + velocity;
    }
  }

  /* Clamp to PWM limits */
  if (pwm_value < MOTOR_PWM_MIN) {
    pwm_value = MOTOR_PWM_MIN;
  } else if (pwm_value > MOTOR_PWM_MAX) {
    pwm_value = MOTOR_PWM_MAX;
  }

  return pwm_value;
}

void motor_init(void)
{
  /* Start PWM on both motor channels */
  HAL_TIM_PWM_Start(&htim3, MOTOR1);
  HAL_TIM_PWM_Start(&htim3, MOTOR2);

  /* Set both motors to neutral position */
  motor_stop(MOTOR1);
  motor_stop(MOTOR2);
}

void motor_run(uint32_t motor, uint8_t direction, uint16_t velocity)
{
  uint16_t pwm_value = motor_calculate_pwm(motor, direction, velocity);
  __HAL_TIM_SET_COMPARE(&htim3, motor, pwm_value);
}

void motor_stop(uint32_t motor)
{
  __HAL_TIM_SET_COMPARE(&htim3, motor, MOTOR_PWM_NEUTRAL);
}
