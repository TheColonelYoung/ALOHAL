#include "StepperMotor.hpp"

int SM_ID = 0;

int SM_Get_freeID(){
	SM_ID ++;
	return SM_ID;
}

int StepperMotor::Get_ID(){
	return _id;
}
