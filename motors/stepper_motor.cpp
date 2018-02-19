#include "stepper_motor.hpp"

//TODO: Cumulationg of shift commands
//		Setting new microstepping while shifting
//		Add sleeping of motor

int init_motor(){

	SM_steps = 500;

	return 0;
}

int SM_step(){
	if((SM_out & 0b1111) == 0){
		SM_out = 1;
	}else{
		SM_out <<= 1;
	}
	Pin_set('A',0,SM_out & 0b0001);
	Pin_set('A',1,SM_out & 0b0010);
	Pin_set('A',2,SM_out & 0b0100);
	Pin_set('A',3,SM_out & 0b1000);
	return 1;
}
