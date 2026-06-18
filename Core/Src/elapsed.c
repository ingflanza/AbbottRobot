/**
  ******************************************************************************
  * @file           : elapsed.c
  * @brief          : Elapsed time and delay utilities
  ******************************************************************************
  */
#include "elapsed.h"
#include "main.h"

#define SYSTICK_LOAD        (SystemCoreClock/1000000U)
#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)

/**
  * @brief  Delay function with microsecond precision
  * @param  us: delay in microseconds
  * @retval None
  */
void elapsed_delayus(uint32_t us)
{
  uint32_t start = SysTick->VAL;
  uint32_t ticks = (us * SYSTICK_LOAD) - SYSTICK_DELAY_CALIB;
  while((start - SysTick->VAL) < ticks);
}

/**
  * @brief  Get current SysTick timer value
  * @retval Current SysTick value in milliseconds
  */
uint32_t elapsed_set(void)
{
  return HAL_GetTick();
}

/**
  * @brief  Check if elapsed time has reached the target value
  * @param  value: target value to compare against
  * @retval 1 if current SysTick >= value, 0 otherwise
  */
uint8_t elapsed_check(uint32_t value)
{
  return (HAL_GetTick() >= value) ? 1 : 0;
}
