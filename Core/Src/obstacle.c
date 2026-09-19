/**
	******************************************************************************
	* @file           : obstacle.c
	* @brief          : Obstacle avoidance module implementation
	* @author         : Federico Lanza
	* @date           : 2026-09-19
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

#include "obstacle.h"
#include "main.h"

static uint8_t obsavoid_state;
static uint8_t obsavoid_afterwait_state;
static uint8_t back_direction;
static uint32_t obsavoid_timer;

void obsavoid_init(void)
{
    obsavoid_state = OBSAVOID_STATE_INIT;
    obsavoid_afterwait_state = OBSAVOID_STATE_INIT;
    obsavoid_timer = elapsed_set();
    back_direction = 0;
}

void obsavoid_process(void)
{
    switch (obsavoid_state)
    {
      case OBSAVOID_WAIT:
        if (elapsed_check(obsavoid_timer))
        {
          printf("Resuming...\r\n");
          obsavoid_state = obsavoid_afterwait_state;
        }
        break;
      case OBSAVOID_STATE_INIT:
        // Initialization state, transition to idle
        printf("Moving forward...\r\n");
        motor_run(MOTOR1, MOTOR_DIRECTION_FORWARD, 50);
        motor_run(MOTOR2, MOTOR_DIRECTION_FORWARD, 50);
        obsavoid_state = OBSAVOID_STATE_IDLE;
        break;
      case OBSAVOID_STATE_IDLE:
        // Idle state, wait for obstacle detection
        if (ultrasonic_distance() < 100)
        {
          printf("Obstacle detected!\r\n");
          motor_stop_all();
          obsavoid_afterwait_state = OBSAVOID_STATE_OBSTACLE_DETECTED;
          obsavoid_state = OBSAVOID_WAIT;
          obsavoid_timer = elapsed_set() + 800;
        }
        break;
      case OBSAVOID_STATE_OBSTACLE_DETECTED:
        // Obstacle detected, move backwards
        if (back_direction == 0)
        {
          // Move backward for 1 second
          printf("Backwards 0\r\n");
          motor_run(MOTOR1, MOTOR_DIRECTION_BACKWARD, 50);
          motor_run(MOTOR2, MOTOR_DIRECTION_BACKWARD, 5);
          back_direction = 1;
        }
        else
        {
          // Move forward for 1 second
          printf("Backwards 1\r\n");
          motor_run(MOTOR1, MOTOR_DIRECTION_BACKWARD, 5);
          motor_run(MOTOR2, MOTOR_DIRECTION_BACKWARD, 50);
          back_direction = 0;
        }
        obsavoid_afterwait_state = OBSAVOID_STATE_TURN;
        obsavoid_state = OBSAVOID_WAIT;
        obsavoid_timer = elapsed_set() + 2500;
        break;
      case OBSAVOID_STATE_TURN:
        // After turn state
        printf("Turn finished\r\n");
        motor_stop_all();
        obsavoid_afterwait_state = OBSAVOID_STATE_INIT;
        obsavoid_state = OBSAVOID_WAIT;
        obsavoid_timer = elapsed_set() + 800;
        break;
    }
}
