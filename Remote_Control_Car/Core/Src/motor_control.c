/*
 * motor_control.c
 *
 *  Created on: Jun 28, 2023
 *      Author: MarkP
 */

#include "motor_control.h"
#include "main.h"

enum
{
  MAX_PULSE = 200
};

static MC_HandleTypeDef hforward;
static MC_HandleTypeDef hbackward;

static void stopDirection(const MC_HandleTypeDef hdirection);
static void goDirection(const MC_HandleTypeDef hdirection, const uint32_t duty_cycle);

static void stopDirection(const MC_HandleTypeDef hdirection)
{
  HAL_GPIO_WritePin(hdirection.port, hdirection.pin, GPIO_PIN_RESET);
  __HAL_TIM_SET_COMPARE(hdirection.htim, hdirection.channel, 0);
}

static void goDirection(const MC_HandleTypeDef hdirection, const uint32_t duty_cycle)
{
  HAL_GPIO_WritePin(hdirection.port, hdirection.pin, GPIO_PIN_SET);
  __HAL_TIM_SET_COMPARE(hdirection.htim, hdirection.channel, duty_cycle);
}

void setupMotorControl(const MC_HandleTypeDef _hforward, const MC_HandleTypeDef _hbackward)
{
  hforward = _hforward;
  hbackward = _hbackward;

  stopDirection(hforward);
  stopDirection(hbackward);

  // Start PWM timers
  HAL_TIM_PWM_Start(hforward.htim, hforward.channel);
  HAL_TIM_PWM_Start(hbackward.htim, hbackward.channel);
}

void wheelThrottle(const int8_t throttle)
{
  uint32_t throttleMult = MAX_PULSE / 100;
  if (throttle > 100 || throttle < -100)
  {
    return;
  }
  if (-25 <= throttle && throttle <= 25)
  {
    // Stop the car
    stopDirection(hforward);
    stopDirection(hbackward);
    return;
  }

  if (throttle > 0)
  {
    // Forward
    stopDirection(hbackward);
    goDirection(hforward, throttleMult * throttle);
  }
  else if (throttle < 0)
  {
    // Backward
    stopDirection(hforward);
    goDirection(hbackward, throttleMult * (-throttle));
  }
  return;
}
