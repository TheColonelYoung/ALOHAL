#ifndef PWM_H
#define PWM_H

#ifdef STM32_F0
# include "stm32f0xx_hal.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
#endif


#include "timer/timer.hpp"
#include "gpio/pin.hpp"

#include <vector>

using namespace std;


extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;

class PWM {
private:
	int _ID;


public:
	TIM_HandleTypeDef _TIM_Handler;
	Pin pwm_pin;
	bool state = false;

	float duty_cycle;
	int freq;

	PWM(int ID_set, TIM_HandleTypeDef TIM_Handler_set, Pin pwm_pin_set, float duty_cycle_set, int freq_set);
	TIM_HandleTypeDef * Get_handler();
};

int PWM_Handler(int channel);
void PWM_Init();
int PWM_Start();
void PWM_Set_time(PWM actual_PWM);

#endif // ifndef PWM_H
