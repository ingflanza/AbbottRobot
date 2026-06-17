/**
  ******************************************************************************
  * @file           : elapsed.h
  * @brief          : Elapsed time and delay utilities
  ******************************************************************************
  */
#ifndef __ELAPSED_H__
#define __ELAPSED_H__

#include <stdint.h>

/**
  * @brief  Delay function with microsecond precision
  * @param  us: delay in microseconds
  * @retval None
  */
void elapsed_delayus(uint32_t us);

/**
  * @brief  Get current SysTick timer value
  * @retval Current SysTick value in milliseconds
  */
uint32_t elapsed_set(void);

/**
  * @brief  Check if elapsed time has reached the target value
  * @param  value: target value to compare against
  * @retval 1 if current SysTick >= value, 0 otherwise
  */
uint8_t elapsed_check(uint32_t value);

#endif /* __ELAPSED_H__ */
