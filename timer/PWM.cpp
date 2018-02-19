#include "PWM.hpp"

vector<PWM> PWM_stash;

PWM::PWM(int ID_set, TIM_HandleTypeDef TIM_Handler_set, Pin pwm_pin_set, float duty_cycle_set, int freq_set)
{
    _ID = ID_set;
    _TIM_Handler = TIM_Handler_set;
    pwm_pin      = pwm_pin_set;

    duty_cycle = duty_cycle_set;
    freq       = freq_set;
    state      = false;

    pwm_pin.Set(0);
    PWM_stash.push_back(*this);
}

TIM_HandleTypeDef * PWM::Get_handler()
{
    return &_TIM_Handler;
}

int PWM_Handler(int channel)
{
    if (channel > (int) PWM_stash.size() - 1) {
        return -1;

        Pin('C', 7).Toggle();
    }


    PWM &actual_PWM = PWM_stash[channel];

    if (actual_PWM.duty_cycle >= 99) { // >99
        actual_PWM.pwm_pin.Set(1);
        PWM_Set_time(actual_PWM);
        actual_PWM.state = false;
    } else if (actual_PWM.duty_cycle <= 1) { // <1
        actual_PWM.pwm_pin.Set(0);
        PWM_Set_time(actual_PWM);
        actual_PWM.state = false;
    } else if (actual_PWM.state) { // 1 -> 0
        actual_PWM.pwm_pin.Set(0);
        PWM_Set_time(actual_PWM);
        actual_PWM.state = false;
    } else { // 0 -> 1
        actual_PWM.pwm_pin.Set(1);
        PWM_Set_time(actual_PWM);
        actual_PWM.state = true;
    }
    return 0;
}

void PWM_Init()
{
    PWM(0, htim4, Pin('C', 6), 0, 100);
    PWM(1, htim5, Pin('B', 14), 0, 100);
    PWM(2, htim6, Pin('B', 15), 0, 100);
}

int PWM_Start()
{
    int count = 0;
    for (auto &PWM : PWM_stash) {
        count++;
        PWM_Set_time(PWM);
        HAL_TIM_Base_Start_IT(PWM.Get_handler());
    }
    return count;
}

void PWM_Set_time(PWM actual_PWM)
{
    int usec;
    if (actual_PWM.duty_cycle >= 99) { // >99
        usec = 1000000.0 / actual_PWM.freq;
    } else if (actual_PWM.duty_cycle <= 1) { // <1
        usec = 1000000.0 / actual_PWM.freq;
    } else if (actual_PWM.state) { // 1 -> 0, calculation time for 0
        usec = ((100 - actual_PWM.duty_cycle) / 100.0) * (1000000.0 / actual_PWM.freq);
    } else { // 0 -> 1, calculation time for 1
        usec = (actual_PWM.duty_cycle / 100.0) * (1000000.0 / actual_PWM.freq);
    }
    __HAL_TIM_SET_COUNTER(&TASKER_TIMER, 0);
    actual_PWM.Get_handler()->Instance->ARR = usec / 10;
    // TIM_set_time(actual_PWM.Get_handler(), usec);
}
