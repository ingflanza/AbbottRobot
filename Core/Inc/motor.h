#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

/* Motor Identifiers */
#define MOTOR1  TIM_CHANNEL_1
#define MOTOR2  TIM_CHANNEL_2

/* PWM Value Definitions */
#define MOTOR_PWM_NEUTRAL  1500  /* Both motors stopped */
#define MOTOR_PWM_MIN      1000  /* Minimum PWM (maximum velocity) */
#define MOTOR_PWM_MAX      2000  /* Maximum PWM (maximum velocity opposite direction) */
#define MOTOR_VELOCITY_MAX 500   /* Maximum velocity delta from neutral */

/* Motor Direction Definitions */
#define MOTOR_DIRECTION_FORWARD   0
#define MOTOR_DIRECTION_BACKWARD  1
#define MOTOR_DIRECTION_STOP      2

/**
 * @brief  Run a motor with specified direction and velocity
 * @note   Motor1: forward = PWM 1500→1000, backward = PWM 1500→2000
 *         Motor2: forward = PWM 1500→2000, backward = PWM 1500→1000
 * @param  motor: Motor identifier (MOTOR1 or MOTOR2)
 * @param  direction: MOTOR_DIRECTION_FORWARD, MOTOR_DIRECTION_BACKWARD, or MOTOR_DIRECTION_STOP
 * @param  velocity: Velocity 0-500 (0=neutral, 500=maximum speed)
 * @retval None
 */
void motor_run(uint32_t motor, uint8_t direction, uint16_t velocity);

/**
 * @brief  Stop a motor (set to neutral position)
 * @param  motor: Motor identifier (MOTOR1 or MOTOR2)
 * @retval None
 */
void motor_stop(uint32_t motor);

#endif /* MOTOR_H */
