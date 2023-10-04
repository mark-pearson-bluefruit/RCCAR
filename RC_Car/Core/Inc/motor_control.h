/*
 * motor_control.h
 *
 *  Created on: Jun 28, 2023
 *      Author: MarkP
 */

#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_
#include <stdint.h>
#include "stm32l4xx_hal.h"

void setupMotorControl(TIM_HandleTypeDef* htim);
void wheelThrottle(int8_t throttle);

#endif /* INC_MOTOR_CONTROL_H_ */
