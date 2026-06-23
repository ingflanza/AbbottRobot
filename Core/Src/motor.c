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

static uint8_t motor1_stopped;
static uint8_t motor2_stopped;

/**
 * @brief  Calculate PWM value based on motor, direction, and velocity
 * @param  motor: Motor identifier
 * @param  direction: Motor direction
 * @param  velocity: Velocity offset (0-100)
 * @retval PWM compare value (1000-2000)
 */
static uint16_t motor_calculate_pwm(uint32_t motor, uint8_t direction, uint16_t velocity)
{
  uint16_t pwm_value;

  /* Clamp velocity to valid range */
  if (velocity > MOTOR_VELOCITY_MAX) {
    velocity = MOTOR_VELOCITY_MAX;
  }

  if (motor == MOTOR1)
  {
    pwm_value = MOTOR1_PWM_NEUTRAL;
  }
  else if (motor == MOTOR2)
  {
    pwm_value = MOTOR2_PWM_NEUTRAL;
  }

  if ((direction == MOTOR_DIRECTION_STOP) || (velocity == 0))
  {
    return pwm_value;
  }

  /* Scale velocity from 0-100 range to 0-500 PWM delta */
  velocity = velocity * 5;

  if (motor == MOTOR1)
  {
    /* Motor1: forward increases PWM (1500→2000), backward decreases (1500→1000) */
    if (direction == MOTOR_DIRECTION_FORWARD)
    {
      pwm_value = MOTOR1_PWM_NEUTRAL + velocity;
    }
    else
    {
      pwm_value = MOTOR1_PWM_NEUTRAL - velocity;
    }
  }
  else if (motor == MOTOR2)
  {
    /* Motor2: forward decreases PWM (1500→1000), backward increases (1500→2000) */
    if (direction == MOTOR_DIRECTION_FORWARD)
    {
      pwm_value = MOTOR2_PWM_NEUTRAL - velocity;
    }
    else
    {
      pwm_value = MOTOR2_PWM_NEUTRAL + velocity;
    }
  }

  return pwm_value;
}

static void motor_start(uint32_t motor)
{
  if (motor == MOTOR1)
  {
    HAL_TIM_PWM_Start(&htim3, MOTOR1);
    motor1_stopped = 0;
  }
  else if (motor == MOTOR2)
  {
    HAL_TIM_PWM_Start(&htim3, MOTOR2);
    motor2_stopped = 0;
  }
}

static uint8_t motor_is_stopped(uint32_t motor)
{
  if (motor == MOTOR1)
  {
    return motor1_stopped;
  }
  else if (motor == MOTOR2)
  {
    return motor2_stopped;
  }
  return -1;
}

void motor_init()
{
  motor1_stopped = 1;
  motor2_stopped = 1;
}

void motor_run(uint32_t motor, uint8_t direction, uint16_t velocity)
{
  if ((direction == MOTOR_DIRECTION_STOP) || (velocity == 0))
  {
    /* Stop for the specified motor if direction is STOP or velocity is 0 */
    motor_stop(motor);
    return;
  }

  if (motor_is_stopped(motor))
  {
    motor_start(motor);
  }

  uint16_t pwm_value = motor_calculate_pwm(motor, direction, velocity);
  __HAL_TIM_SET_COMPARE(&htim3, motor, pwm_value);
}

void motor_stop(uint32_t motor)
{
  if (motor == MOTOR1)
  {
    HAL_TIM_PWM_Stop(&htim3, MOTOR1);
    motor1_stopped = 1;
  }
  else if (motor == MOTOR2)
  {
    HAL_TIM_PWM_Stop(&htim3, MOTOR2);
    motor2_stopped = 1;
  }
}

void motor_stop_all()
{
  motor_stop(MOTOR1);
  motor_stop(MOTOR2);
}
