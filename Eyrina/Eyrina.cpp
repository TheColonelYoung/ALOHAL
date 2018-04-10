#include "Eyrina.hpp"

Eyrina::Eyrina(){
    rails.reserve(EYRINA_LR_COUNT);
    Init_rails();
}

void Eyrina::Init_rails(){
    SM_DRV8825 *SM_1 = new SM_DRV8825(Pin('C', 8), Pin('C', 7), EYRINA_SM_STEP_SIZE, EYRINA_SM_SPEED);
    TIM_1.Prescaler_set(48);
    SM_1->Select_timer(TIM_1);
    auto LR_1 = Linear_rail(SM_1,EYRINA_LR_SHIFT_RATIO);
    rails.insert(rails.begin(),LR_1);
}

int Eyrina::Move_axis(Eyrina::Axis axis, long distance){
    rails[axis].Move(distance);
    return axis;
}
