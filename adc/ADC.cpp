#include "ADC.hpp"

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_EOC)){

		float V = (float)HAL_ADC_GetValue(hadc) * ADC_REF_VOLTAGE/4096;
		USB_VCP_send(V);
		}
}
