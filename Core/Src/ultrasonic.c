/**
  ******************************************************************************
  * @file           : ultrasonic.c
  * @brief          : Ultrasonic distance sensor driver
  ******************************************************************************
  */
#include "ultrasonic.h"
#include "main.h"
#include "elapsed.h"

/* Static variables for state machine */
static uint8_t us_state = 0;           // State machine state
static uint32_t us_distance = 0;       // Measured distance (pulse width in ticks)
static uint32_t us_capture_value = 0;  // First capture value

/**
  * @brief Initialize ultrasonic module
  * @retval None
  */
void ultrasonic_init(void)
{
  us_state = 0;
  us_distance = 0;
  us_capture_value = 0;
}

/**
  * @brief Trigger ultrasonic measurement pulse
  * @retval None
  */
void ultrasonic_trigger(void)
{
  /* Set port as output for trigger pulse */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = ULTRASONIC_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStruct);

  /* Trigger pulse */
  HAL_GPIO_WritePin(ULTRASONIC_PORT, ULTRASONIC_PIN, GPIO_PIN_RESET);
  elapsed_delayus(2);
  HAL_GPIO_WritePin(ULTRASONIC_PORT, ULTRASONIC_PIN, GPIO_PIN_SET);
  elapsed_delayus(8);
  HAL_GPIO_WritePin(ULTRASONIC_PORT, ULTRASONIC_PIN, GPIO_PIN_RESET);

  /* Set port/pin as input for echo reception */
  GPIO_InitStruct.Pin = ULTRASONIC_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStruct);

  /* Reset state machine and set polarity to rising edge */
  us_state = 0;
  __HAL_TIM_SET_COUNTER(&ULTRASONIC_TIMER, 0);
  __HAL_TIM_SET_CAPTUREPOLARITY(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
  /* Start timer with input capture interrupt */
  HAL_TIM_IC_Start_IT(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL);
}

/**
  * @brief Process ultrasonic measurement (called from timer interrupt callback)
  * Implements state machine:
  *   State 0: Wait for rising edge
  *   State 1: Calculate pulse width from falling edge
  * @retval None
  */
void ultrasonic_measure(void)
{
  uint32_t capture_value = HAL_TIM_ReadCapturedValue(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL);

  if (us_state == 0)
  {
    /* First rising edge captured */
    us_capture_value = capture_value;
    us_state = 1;

    /* Switch to falling edge detection */
    __HAL_TIM_SET_CAPTUREPOLARITY(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL, TIM_INPUTCHANNELPOLARITY_FALLING);
  }
  else if (us_state == 1)
  {
    __HAL_TIM_SET_COUNTER(&ULTRASONIC_TIMER, 0);
    
    /* Falling edge captured - calculate pulse width */
    us_distance = capture_value - us_capture_value;
    us_state = 0;

    /* Switch back to rising edge detection and disable interrupt */
    __HAL_TIM_SET_CAPTUREPOLARITY(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
  }
}

/**
  * @brief Get last measured distance
  * @retval Distance in centimeters
  */
uint32_t ultrasonic_distance(void)
{
  //return ((float)us_distance / 2.0) * 0.0343;
    return us_distance;
}
