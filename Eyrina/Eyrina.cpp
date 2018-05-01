#include "Eyrina.hpp"

Eyrina::Eyrina(){
    DBG_LED_1 = Pin('C', 8);
    DBG_LED_2 = Pin('C', 7);

    rails.reserve(EYRINA_LR_COUNT);
}

void Eyrina::Init_rails(){
    SM_DRV8825 *SM_1 = new SM_DRV8825(Pin('B', 8), Pin('B', 9), EYRINA_SM_STEP_SIZE, EYRINA_SM_SPEED, EYRINA_SM_ACCELERATION);

    TIM_1.Prescaler_set(48);
    TIM_1.Optimize(true);
    SM_1->Select_timer(TIM_1);
    auto LR_1 = Linear_rail(SM_1, EYRINA_LR_SHIFT_RATIO);
    rails.insert(rails.begin(), LR_1);
}

void Eyrina::Init_protocol(){
    protocol = E_protocol(&UART_2);
    UART_2.IRQ.Register(protocol,&E_protocol::Initial_byte);

}


int Eyrina::Move_axis(Eyrina::Axis axis, long distance){
    rails[axis].Move(distance);
    return axis;
}
