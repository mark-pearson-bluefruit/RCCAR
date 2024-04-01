/*
 * process_raw_data.c
 *
 *  Created on: Mar 29, 2024
 *      Author: m1pea
 */
#include "process_raw_data.h"

#define MAX_UINT_12           (4095)

#define MAX_FORWARD_THROTTLE  (100)
#define MIN_BACKWARD_THROTTLE (-100)

int8_t processRawThrottle(const uint16_t raw_throttle)
{
  // clamp to range 0 to 4095
  if (raw_throttle > MAX_UINT_12)
  {
    return MAX_FORWARD_THROTTLE;
  }
  const uint32_t steps = raw_throttle * (MAX_FORWARD_THROTTLE - MIN_BACKWARD_THROTTLE);
  return MIN_BACKWARD_THROTTLE + steps / MAX_UINT_12;
}
