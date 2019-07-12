/*
 * Author:	TCY
 * Name:	UID STM32 library
 * E-mail:	TheColonelYoung(at)gmail(dot)com
 * Date:	6.2.2018
 * Description:
 *
 * ????????????????????HELP????????????????????
 *
 * !!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!
 *
 * --------------------BUGS--------------------
 *
 * ++++++++++++++++++++TODO++++++++++++++++++++
 *
 **************************/
#ifndef UID_H
#define UID_H

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

#include <string>
#include <vector>

using namespace std;

// classes
namespace UID {
    extern string UID_string;

    void Init();
    string String(int word = -1);   //return UID as string, in default return whole UID
};

#endif
