#ifndef USB_VCP_H
#define USB_VCP_H

#include <string>
#include <vector>

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

#include "gpio/pin.hpp"
#include "usbd_cdc_if.hpp"
#include "misc/text_transform.hpp"

#define USB_BUFFER_SIZE 512

extern PCD_HandleTypeDef hpcd_USB_FS;
extern USBD_HandleTypeDef hUsbDeviceFS;

extern char USB_VCP_TX_buffer[USB_BUFFER_SIZE];
extern char USB_VCP_RX_buffer[USB_BUFFER_SIZE];

using namespace std;

extern vector<string> USB_VCP_rec;

void USB_VCP_send(char *data);
void USB_VCP_send(int number);
void USB_VCP_send(long number);
void USB_VCP_send(double number);
void USB_VCP_send(std::string text);
string USB_VCP_read();

void USB_VCP_receive();
void _Error_Handler();

#endif

/*
TO usbd_cdc_if.c

Insert to begin of file
#include "./lib/USB_VCP.h"

Increase size of buffer
#define APP_RX_DATA_SIZE  64
#define APP_TX_DATA_SIZE  64

Insert to CDC_Receive_FS Line:+-275
strncpy((char *)USB_VCP_RX_buffer,(char *)Buf,*Len );
USB_VCP_receive();


TO usbd_conf.c

Insert
void _Error_Handler(char * file, int line)
{
	while (1) { }
}

TO usbd_desc.c
Insert
(uint8_t*)
In DEFINE part before every string
#define USBD_MANUFACTURER_STRING     (uint8_t*)"STMicroelectronics"

*/
