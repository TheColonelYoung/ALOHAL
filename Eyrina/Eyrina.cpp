#include "Eyrina.hpp"

Eyrina::Eyrina(){
    DBG_LED_1 = Pin('C', 8);
    DBG_LED_2 = Pin('C', 7);
}

void Eyrina::Init(){
    UART_2.Send("\r\nEyrina: Init start\r\n");
    Init_rails();
    Init_protocol();
    Init_PWM();
    Init_LED_drivers();
    UART_2.Send("Eyrina: Init done\r\n");
}

void Eyrina::Init_rails(){

    rails.reserve(EYRINA_LR_COUNT);

    SM_DRV8825 *SM_1 = new SM_DRV8825(Pin('C', 10), Pin('A', 15), EYRINA_SM_STEP_SIZE, EYRINA_SM_SPEED, EYRINA_SM_ACCELERATION);
    TIM_1.Prescaler_set(48);
    TIM_1.Optimize(true);
    SM_1->Select_timer(TIM_1);
    auto LR_1 = Linear_rail(SM_1, EYRINA_LR_SHIFT_RATIO);
    rails[0] = LR_1;

    SM_DRV8825 *SM_2 = new SM_DRV8825(Pin('B', 4), Pin('B', 3), EYRINA_SM_STEP_SIZE, EYRINA_SM_SPEED, EYRINA_SM_ACCELERATION);
    TIM_2.Prescaler_set(48);
    TIM_2.Optimize(true);
    SM_1->Select_timer(TIM_2);
    auto LR_2 = Linear_rail(SM_2, EYRINA_LR_SHIFT_RATIO);
    rails[1] = LR_2;

    SM_DRV8825 *SM_3 = new SM_DRV8825(Pin('B', 8), Pin('B', 9), EYRINA_SM_STEP_SIZE, EYRINA_SM_SPEED, EYRINA_SM_ACCELERATION);
    TIM_4.Prescaler_set(48);
    TIM_4.Optimize(true);
    SM_3->Select_timer(TIM_4);
    auto LR_3 = Linear_rail(SM_3, EYRINA_LR_SHIFT_RATIO);
    rails[2] = LR_3;

    SM_DRV8825 *SM_4 = new SM_DRV8825(Pin('B', 10), Pin('B', 11), EYRINA_SM_STEP_SIZE, EYRINA_SM_SPEED, EYRINA_SM_ACCELERATION);
    TIM_5.Prescaler_set(48);
    TIM_5.Optimize(true);
    SM_4->Select_timer(TIM_5);
    auto LR_4 = Linear_rail(SM_4, EYRINA_LR_SHIFT_RATIO);
    rails.insert(rails.begin()+3, LR_4);
}

void Eyrina::Init_protocol(){
    protocol = E_protocol(&UART_2);
    UART_2.IRQ.Register(protocol,&E_protocol::Initial_byte);
}

void Eyrina::Init_PWM(){
    PWM_out.reserve(EYRINA_PWM_COUNT);

    PWM_limits[0] = 60.0;

    TIM_3.Optimize(true);
    TIM_3.Frequency_set(10000);

    PWM_out[0] = TIM_3.channel[0];
}

void Eyrina::Init_LED_drivers(){
    LED_drivers.reserve(EYRINA_LED_DRIVER);

    LED_drivers[0] = Current_supply(&DAC_1.channel[0],10);
    LED_drivers[1] = Current_supply(&DAC_1.channel[1],10);

}


int Eyrina::Move_axis(Eyrina::Axis axis, long distance){
    rails[axis].Move(distance);
    return axis;
}

void Eyrina::Stop_axis(Axis axis){
    rails[axis].Stop();
}

void Eyrina::Stop_all(){
    for(auto &rail:rails){
        rail.Stop();
    }
    for(auto &PWM_o:PWM_out){
        PWM_o.PWM_Stop();
    }
}

int Eyrina::PWM(int index, float duty){

    if ((duty > 100) | (duty < 0)) {
        return -1;
    } else if(duty > PWM_limits[index]){
        return -2;
    }

    PWM_out[index].PWM_Generate(duty);
    PWM_out[index].PWM_Start();

    return static_cast<int>(duty*100);
}

int Eyrina::PWM_limit(int index, float duty_limit){
    if ((duty_limit > 100) | (duty_limit < 0)) {
        return -1;
    }
    PWM_limits[index] = duty_limit;
    return static_cast<int>(duty_limit*100);
}

int Eyrina::LED_driver_current(int index,int current_ua){
    if((index > EYRINA_LED_DRIVER) || (index< 0)){
        return -1;
    }
    LED_drivers[index].Set_current(current_ua);
    return 0;
}
