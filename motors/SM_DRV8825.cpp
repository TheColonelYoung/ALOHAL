#include "SM_DRV8825.hpp"

// globals
vector<SM_DRV8825> SM_DRV8825_stash;

SM_DRV8825::SM_DRV8825(int speed_set, bool direction_set, int steps_set, Pin step_pin_set, Pin direction_pin_set, Pin reset_pin_set, Pin sleep_pin_set, Pin fault_pin_set)
{
	_id       = SM_Get_freeID();
	speed     = speed_set;
	direction = direction_set;
	steps     = steps_set;

	_step_pin      = step_pin_set;
	_direction_pin = direction_pin_set;

	_reset_pin = reset_pin_set;
	_sleep_pin = sleep_pin_set;
	_fault_pin = fault_pin_set;

	_reset_pin.Set(1);
    _direction_pin.Set(direction_set);

	if (steps == 0) {
		Sleep();
	} else  {
		Wake_up();
	}


	SM_DRV8825_stash.push_back(*this);
}

void SM_DRV8825::Step()
{
	_step_pin.Toggle();
}

void SM_DRV8825::Sleep()
{
	_sleep_pin.Set(0);
}

void SM_DRV8825::Wake_up()
{
	_sleep_pin.Set(1);
}

void SM_DRV8825::Set_direction(bool dir)
{
	if (dir != direction) {
		direction = dir;
		_direction_pin.Set(dir);
	}
}

void SM_DRV8825_Finder()
{
	for (auto &DRV : SM_DRV8825_stash) {
		// id motor isnt stopped
		if (DRV.steps != 0) {
			DRV.Step();
			// steps are limited
			if (DRV.steps > 0) {
				DRV.steps -= 1;
				// steps decresed to 0 => set driver to sleep
				if (DRV.steps == 0) {
					DRV.Sleep();
				}
			}
		}
	}
}
