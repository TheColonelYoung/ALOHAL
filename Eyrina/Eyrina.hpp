#ifndef EYRINA_HPP
#define EYRINA_HPP

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

#include <vector>

#include "globals.hpp"
#include "construction/linear_rail.hpp"
#include "motor/SM_DRV8825.hpp"
#include "gpio/pin.hpp"
#include "Eyrina/protocol.hpp"
#include "dac/current_supply.hpp"

using namespace std;

#define EYRINA_SM_SPEED          5000
#define EYRINA_SM_ACCELERATION   4000
#define EYRINA_SM_STEP_SIZE      1.8

#define EYRINA_LR_COUNT          4
#define EYRINA_LR_SHIFT_RATIO    1.389

#define EYRINA_PWM_COUNT         3
#define EYRINA_PWM_LIMIT         100.0

#define EYRINA_LED_DRIVER        2
#define EYRINA_LED_CURRENT_LIMIT 100.0

class Eyrina {
    vector<Linear_rail> rails;

public:
    enum Axis { X = 0, Y, Z, F }; // F is Focus

    vector<TIM_channel> PWM_out; // PWM outputs

    vector<float> PWM_limits { vector<float>(EYRINA_PWM_COUNT, EYRINA_PWM_LIMIT) }; // Limits for PWM outputs

    vector<Current_supply> LED_drivers; // PWM outputs

    E_protocol protocol;

    Pin DBG_LED_1; // Yellow
    Pin DBG_LED_2; // Green

    Eyrina();
    void Init(); // Runs subinitializations
    void Init_rails();
    void Init_protocol();
    void Init_PWM();
    void Init_LED_drivers();

    void Stop_all();                           // Stop all movement, Disable PWM and LED drivers

    int Move_axis(Axis axis, long distance);   // Shift of axis in rail vector
    int Rotate_axis(Axis axis, float degrees); // Rotation around axis
    void Stop_axis(Axis axis);                 // Stop any move on given axis

    int PWM(int index, float duty);             // Set duty cycleof PWM channel
    int PWM_limit(int index, float duty_limit); // Set limit for PWM duty cycle

    int LED_driver_current(int index,int current_ua);//Set output current on indexed LED driver


};

#endif // ifndef EYRINA_HPP
