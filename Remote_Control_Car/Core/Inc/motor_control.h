/*
 * motor_control.h
 *
 *  Created on: Jun 28, 2023
 *      Author: MarkP
 */

#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_
#include <stdint.h>
#include "stm32g4xx_hal.h"

void wheelThrottle(const int8_t throttle);

struct MC_HandleTypeDef {
	TIM_HandleTypeDef* htim;
	uint32_t channel;
	GPIO_TypeDef* port; uint16_t pin;
} typedef MC_HandleTypeDef;

void setupMotorControl(const MC_HandleTypeDef _hforward, const MC_HandleTypeDef _hbackward);

#endif /* INC_MOTOR_CONTROL_H_ */
