/**
	******************************************************************************
	* @file           : obstacle.h
	* @brief          : Obstacle avoidance module header
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
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdint.h>
#include <stdio.h>
#include "motor.h"
#include "elapsed.h"
#include "ultrasonic.h"

/* Obstacle avoidance states */
#define OBSAVOID_WAIT                     0
#define OBSAVOID_STATE_INIT               1
#define OBSAVOID_STATE_IDLE               2
#define OBSAVOID_STATE_OBSTACLE_DETECTED  3
#define OBSAVOID_STATE_TURN               4

/**
 * @brief  Initialize the obstacle avoidance module
 * @retval None
 */
void obsavoid_init(void);

/**
 * @brief  Process the obstacle avoidance logic
 * @retval None
 */
void obsavoid_process(void);

#endif /* OBSTACLE_H */