#include "DAC.hpp"

int DAC_Start(int channel)
{
	if ((channel > NUM_OF_DAC) || (channel <= 0) ) {
		return -1; // Invalid channel
	}

	if (channel == 1) {
		__HAL_DAC_ENABLE(&hdac, DAC_CHANNEL_1);
		HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	} else if (channel == 2) {
		__HAL_DAC_ENABLE(&hdac, DAC_CHANNEL_2);
		HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	}
	return channel;
}

int DAC_Stop(int channel)
{
	if ((channel > NUM_OF_DAC) || (channel <= 0) ) {
		return -1; // Invalid channel
	}
	if (channel == 1) {
		HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
	} else if (channel == 2) {
		HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
	}

	return channel;
}

int DAC_set_value(int channel, float voltage)
{
	if ((channel > NUM_OF_DAC) || (channel <= 0) ) {
		return -1; // Invalid channel
	}
	if (voltage > DAC_REF_VOLTAGE) {
		return -1; // voltage is higher as can be
	} else if (voltage < 0) {
		return -1; // voltage is negative
	}
	int value = (float(4096.0 * voltage) / float(DAC_REF_VOLTAGE));
	if (channel == 1) {
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, value);
	} else if (channel == 2) {
		HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, value);
	}
	return value;
}
