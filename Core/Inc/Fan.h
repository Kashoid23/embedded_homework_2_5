#ifndef FAN_H
#define FAN_H

#include <stdbool.h>
#include <stdint.h>

#define FAN_PERIOD_MS (60u * 60u * 1000u) // fan turns on every hour
#define FAN_ON_MS (15u * 60u * 1000u) // and runs for 15 minutes

void fanStart(TIM_HandleTypeDef *timer);
void onFanTurnedOn(void);
void onFanTurnedOff(void);

#endif /* FAN_H */
