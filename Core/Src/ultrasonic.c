/**
  ******************************************************************************
  * @file           : ultrasonic.c
  * @brief          : Ultrasonic distance sensor driver
  ******************************************************************************
  */
#include "ultrasonic.h"
#include "main.h"
#include "elapsed.h"

/* State machine states */
#define US_STATE_TRIGGER        0
#define US_STATE_WAIT_RISING    1
#define US_STATE_WAIT_FALLING   2

/* Static variables for state machine */
static uint8_t us_state = US_STATE_TRIGGER;        // Current state
static uint8_t us_edge_flag = 0;                   // Flag set by interrupt when edge captured
static uint32_t us_timing = 0;                     // Timing for ULTRASONIC_TRIGGER_INTERVAL ms delay
static uint32_t us_timeout = 0;                    // Timeout for waiting for edges
static uint32_t us_distance = 0;                   // Measured distance (pulse width in ticks)
static uint32_t us_capture_value = 0;              // First capture value
static uint8_t us_error = 0;                       // Error flag for measurement failures

/**
  * @brief Initialize ultrasonic module
  * @retval None
  */
void ultrasonic_init(void)
{
  us_state = US_STATE_TRIGGER;
  us_edge_flag = 0;
  us_distance = 0;
  us_capture_value = 0;
  us_timing = elapsed_set();
  us_error = 0;
}

/**
  * @brief Process ultrasonic measurement state machine
  * The purpouse of this function is to be called in the main loop to handle the ultrasonic measurement process.
  * It triggers the ultrasonic sensor, waits for the echo, captures the timing of the rising and falling edges,
  * and saves the pulse width. The process is repeated every ULTRASONIC_TRIGGER_INTERVAL milliseconds. The function relies on the timer input capture
  * interrupt to set a flag when an edge is captured.
  * Implements 3-state machine:
  *   State 0 (US_STATE_TRIGGER): Wait ULTRASONIC_TRIGGER_INTERVAL ms, send trigger pulse, go to wait_rising
  *   State 1 (US_STATE_WAIT_RISING): Wait for rising edge flag, capture value, switch to falling, go to wait_falling
  *   State 2 (US_STATE_WAIT_FALLING): Wait for falling edge flag, capture value, calculate distance, return to trigger
  * @retval None
  */
void ultrasonic_measure(void)
{
  switch (us_state)
  {
    case US_STATE_TRIGGER:
      /* Wait ULTRASONIC_TRIGGER_INTERVAL ms before sending trigger */
      if (!elapsed_check(us_timing + ULTRASONIC_TRIGGER_INTERVAL)) return;

      /* Set port as output for trigger pulse */
      GPIO_InitTypeDef GPIO_InitStruct = {0};
      GPIO_InitStruct.Pin = ULTRASONIC_PIN;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStruct);

      /* Send trigger pulse */
      HAL_GPIO_WritePin(ULTRASONIC_PORT, ULTRASONIC_PIN, GPIO_PIN_RESET);
      elapsed_delayus(2);
      HAL_GPIO_WritePin(ULTRASONIC_PORT, ULTRASONIC_PIN, GPIO_PIN_SET);
      elapsed_delayus(8);
      HAL_GPIO_WritePin(ULTRASONIC_PORT, ULTRASONIC_PIN, GPIO_PIN_RESET);

      /* Set port/pin back to input for echo reception */
      GPIO_InitStruct.Pin = ULTRASONIC_PIN;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
      HAL_GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStruct);

      /* Reset counter and state machine, enable interrupt and move to wait for rising edge */
      us_edge_flag = 0;
      us_state = US_STATE_WAIT_RISING;
      __HAL_TIM_SET_COUNTER(&ULTRASONIC_TIMER, 0);
      __HAL_TIM_SET_CAPTUREPOLARITY(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);
      HAL_TIM_IC_Start_IT(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL);

      /* Timeout for waiting for edges */
      us_timeout = elapsed_set();

      break;

    case US_STATE_WAIT_RISING:
      /* Wait for rising edge to be captured, if timeout occurs, return to trigger state */
      if (0 == us_edge_flag)
      {

        if (elapsed_check(us_timeout + 2))
        {
          us_state = US_STATE_TRIGGER;
          us_timing = elapsed_set();
          us_error = 1; // Set error flag if rising edge not captured in time
        }
        return;

      }

      /* Read and save the rising edge capture value */
      us_capture_value = HAL_TIM_ReadCapturedValue(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL);

      /* Switch to falling edge detection */
      __HAL_TIM_SET_CAPTUREPOLARITY(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL, TIM_INPUTCHANNELPOLARITY_FALLING);

      /* Move to wait for falling edge */
      us_edge_flag = 0;
      us_state = US_STATE_WAIT_FALLING;

      /* Timeout for waiting for edges */
      us_timeout = elapsed_set();

      break;

    case US_STATE_WAIT_FALLING:
      /* Wait for falling edge to be captured, if timeout occurs, return to trigger state */
      if (0 == us_edge_flag)
      {
        if (elapsed_check(us_timeout + 200))
        {
          us_state = US_STATE_TRIGGER;
          us_timing = elapsed_set();
          us_error = 1; // Set error flag if falling edge not captured in time
        }
        return;
      }

      /* Read the falling edge capture value */
      uint32_t capture_value = HAL_TIM_ReadCapturedValue(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL);

      /* Calculate pulse width (distance) */
      us_distance = capture_value - us_capture_value;

      /* Reset counter and switch back to rising edge */
      __HAL_TIM_SET_COUNTER(&ULTRASONIC_TIMER, 0);
      __HAL_TIM_SET_CAPTUREPOLARITY(&ULTRASONIC_TIMER, ULTRASONIC_TIMER_CHANNEL, TIM_INPUTCHANNELPOLARITY_RISING);

      /* Reset timing for next measurement cycle and go back to trigger state */
      us_timing = elapsed_set();
      us_edge_flag = 0;
      us_state = US_STATE_TRIGGER;

      break;
  }
}

/**
  * @brief Timer input capture callback for ultrasonic sensor
  * Sets the edge flag when a capture interrupt occurs
  * @param htim: Pointer to the timer handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    /* Set flag to indicate edge has been captured */
    us_edge_flag = 1;
  }
}

/**
  * @brief Get last measured distance
  * Converts the measured pulse width to distance in millimeters.
  * Formula: distance_mm = (pulse_width_ticks * 343) / 84000
  * Timer clock: APB1 = 42 MHz (84 MHz AHB / 2), so 1 tick = 1/42 us
  * Sound propagates out and back, hence divided by 2.
  * Speed of sound: 343 m/s = 0.343 mm/us
  * distance_mm = (pulse_width_ticks / 42 us) * (343 m/s / 2) = pulse_width_ticks * 343 / 84000
  * @retval Distance in millimeters (uint16_t)
  */
uint16_t ultrasonic_distance(void)
{
  return (uint16_t)(((us_distance * 343) / 168000));
}

/**
  * @brief Get the error flag
  * @retval 1 if measurement has failed, 0 otherwise
  */
uint8_t ultrasonic_error(void)
{
  return us_error;
}
