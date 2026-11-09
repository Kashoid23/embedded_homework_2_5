#include "Fan.h"

#define TICKS_PER_MS 10u // Timer counts at 10 kHz: 100 MHz timer clock / (Prescaler 9999 + 1)

volatile bool fanIsOn = false;
volatile uint32_t fanCycles = 0;

static TIM_HandleTypeDef *fanTimer = NULL;

void fanStart(TIM_HandleTypeDef *timer)
{
	if (fanTimer != NULL) {
		return; // already running
	}
	fanTimer = timer;

	// Period = one hour, pulse = 15 minutes
	__HAL_TIM_SET_AUTORELOAD(timer, FAN_PERIOD_MS * TICKS_PER_MS - 1u);
	__HAL_TIM_SET_COMPARE(timer, TIM_CHANNEL_2, FAN_ON_MS * TICKS_PER_MS);
	__HAL_TIM_SET_COUNTER(timer, 0);
	HAL_TIM_GenerateEvent(timer, TIM_EVENTSOURCE_UPDATE);

	HAL_TIM_Base_Start_IT(timer); // new hour: fan on
	HAL_TIM_PWM_Start_IT(timer, TIM_CHANNEL_2); // 15 minutes passed: fan off
}

void onFanTurnedOn(void)
{
	fanIsOn = true;
	fanCycles++;
	HAL_GPIO_WritePin(FAN_LED_GPIO_Port, FAN_LED_Pin, GPIO_PIN_RESET);
}

void onFanTurnedOff(void)
{
	fanIsOn = false;
	HAL_GPIO_WritePin(FAN_LED_GPIO_Port, FAN_LED_Pin, GPIO_PIN_SET);
}
