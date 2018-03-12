#ifndef GLOBALS_H
# define GLOBALS_H

typedef unsigned int uint;

# include <cstdint>

void ALOHAL_init();

// SETTINGS

// #define ADC_1_EN
// NOTE In higher families MCUs has more ADC, in this purpose conditional compile must be added

// #define UART_1_EN
// #define UART_2_EN
// #define UART_3_EN
// #define UART_4_EN

# define TIM_1_EN
# define TIM_2_EN
// #define TIM_3_EN
// #define TIM_4_EN
// #define TIM_5_EN
// #define TIM_6_EN
// #define TIM_7_EN
// #define TIM_8_EN

// ADC
class AD_C;

# ifdef ADC_1_EN
#  include "adc/ADC.hpp"
extern ADC_HandleTypeDef hadc;
extern AD_C ADC_1;
# endif

// UART
class UART;

# ifdef UART_1_EN
#  include "uart/uart.hpp"
extern UART_HandleTypeDef huart1;
extern UART UART1;
# endif

# ifdef UART_2_EN
#  include "uart/uart.hpp"
extern UART_HandleTypeDef huart2;
extern UART UART2;
# endif

# ifdef UART_3_EN
#  include "uart/uart.hpp"
extern UART_HandleTypeDef huart3;
extern UART UART3;
# endif

# ifdef UART_4_EN
#  include "uart/uart.hpp"
extern UART_HandleTypeDef huart4;
extern UART UART4;
# endif

#endif // ifndef GLOBALS_H

class Timer;

#ifdef TIM_1_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim1;
extern Timer TIM_1;
#endif

#ifdef TIM_2_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim2;
extern Timer TIM_2;
#endif

#ifdef TIM_3_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim3;
extern Timer TIM_3;
#endif

#ifdef TIM_4_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim4;
extern Timer TIM_4;
#endif

#ifdef TIM_5_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim5;
extern Timer TIM_5;
#endif

#ifdef TIM_6_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim6;
extern Timer TIM_6;
#endif

#ifdef TIM_7_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim7;
extern Timer TIM_7;
#endif

#ifdef TIM_8_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim8;
extern Timer TIM_8;
#endif

#ifdef TIM_1_EN
# include "timer/timer.hpp"
extern TIM_HandleTypeDef htim1;
extern Timer TIM_1;
#endif
