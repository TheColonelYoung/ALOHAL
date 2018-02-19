#include "timers.hpp"

void Timer_IT_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
	if (htim->Instance == TIM7) { // SM_Handler
		SM_DRV8825_Finder();
	} else if (htim->Instance == TIM2)   { // Tasker
		Tasker_timer_IT();
	}else if (htim->Instance == TIM4)   { // PWM1
		PWM_Handler(0);
	}else if (htim->Instance == TIM5)   { // PWM2
		PWM_Handler(1);
	}else if (htim->Instance == TIM6)   { // PWM3
		PWM_Handler(2);
	}
}

int TIM_set_time(TIM_HandleTypeDef * htim, float useconds)
{
	int prescaler = htim->Instance->PSC + 1;
	float tick    = 1.0 / (TIM_FREQ / prescaler );

	htim->Instance->ARR = useconds / (tick * 1000000);

	return 1;
}

int TIM_get_prescaler(TIM_HandleTypeDef * htim)
{
	return htim->Instance->PSC;
}

int TIM_get_time(TIM_HandleTypeDef * htim)
{
	return __HAL_TIM_GET_COUNTER(htim);
}
