/*
 * motor_control.c
 *
 *  Created on: Jun 28, 2023
 *      Author: MarkP
 */
/*
#include "motor_control.h"
#include "main.h"

enum {MAX_PULSE = 20000};

static TIM_HandleTypeDef *timer;
void setupMotorControl(TIM_HandleTypeDef* htim) {
	timer = htim;
	HAL_GPIO_WritePin(BACKWARD_GPIO_Port, BACKWARD_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FORWARD_GPIO_Port, FORWARD_Pin, GPIO_PIN_RESET);
	HAL_TIM_PWM_Start(timer, TIM_CHANNEL_2); // Backward
	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, 0);
	HAL_TIM_PWM_Start(timer, TIM_CHANNEL_1); // Forward
	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1, 0);
}

void wheelThrottle(int8_t throttle) {
	uint32_t throttleMult = MAX_PULSE/100;
	if (throttle > 100 || throttle < -100) {
		return;
	}
	if (-25 <= throttle && throttle <= 25) {
		// Stop the car
		HAL_GPIO_WritePin(BACKWARD_GPIO_Port, BACKWARD_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(FORWARD_GPIO_Port, FORWARD_Pin, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1, 0);
		return;
	}

	if (throttle > 0) {
		// Forward
		// Open backward pins
		HAL_GPIO_WritePin(BACKWARD_GPIO_Port, BACKWARD_Pin, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, 0);

		// PWM Forward pin PE_3
		// close PE_6
		HAL_GPIO_WritePin(FORWARD_GPIO_Port, FORWARD_Pin, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1, throttleMult * throttle);

	} else if (throttle < 0) {
		// Backward
		// Open forward pins
		HAL_GPIO_WritePin(FORWARD_GPIO_Port, FORWARD_Pin, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_1, 0);

		// PWM Backward pin PE_4
		// close pin PB_2 (close when pin high)
		HAL_GPIO_WritePin(BACKWARD_GPIO_Port, BACKWARD_Pin, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, throttleMult * (-throttle));

	}
	return;

}
*/
