#include "mcu.hpp"

#include "device/device.hpp"

void MCU::Init(){
    Init_peripherals();
}

float MCU::Uptime(){
    return HAL_GetTick()/1000.0;
}

void MCU::Enable_CLI_control(Timer * timer){
    if (!device()->Filesystem_available()){
        return;
    }

    device()->fs->Make_directory("/mcu/TIM_" + to_string(timer->Index()));

    auto func = new function<int(vector<string>&)>(
        [timer](vector<string>& text){
            if(text.size()>=2){
                timer->Frequency_set(stof(text[1]), true);
            }
            return 0;
        }
    );

    auto iw = new Invocation_wrapper<void, int, vector<string>&>(func);

    device()->fs->Make_executable("mcu/TIM_" + to_string(timer->Index()) + "/Frequency",iw);
}

void MCU::Enable_CLI_control(TIM_channel *timer_channel){

    unsigned short timer_index = timer_channel->parent_timer->Index();
    unsigned short channel_index = timer_channel->Index();

    auto func = new function<int(vector<string>&)>(
        [timer_channel](vector<string>& text){
            if(text.size()>=2){
                timer_channel->PWM_Generate(stof(text[1]));
                timer_channel->Start();
            }
            return 0;
        }
    );

    auto iw = new Invocation_wrapper<void, int, vector<string>&>(func);

    device()->fs->Make_executable("mcu/TIM_" + to_string(timer_index) + "/CH_" + to_string(channel_index) ,iw);
}

void MCU::Filesystem_interface_initialization(){
    if (!device()->Filesystem_available()){
        return;
    }

    #ifdef ADC_1_EN
    #ifdef ADC_CHANNEL_TEMPSENSOR
    if(ADC_1 != nullptr){
        auto temperature = new File<AD_C>("core_temperature", this->ADC_1, &AD_C::Core_temperature);
        device()->fs->Add_entry("/mcu/core_temperature", temperature);
    }
    #endif

    #ifdef  ADC_CHANNEL_VBAT
    if(ADC_1 != nullptr){
        auto supply_voltage = new File<AD_C>("supply_voltage", this->ADC_1, &AD_C::Supply_voltage);
        device()->fs->Add_entry("/mcu/supply_voltage", supply_voltage);
    }
    #endif
    #endif
}

void MCU::Init_peripherals(){

    EXT_IRQ = new IRQ_multi_handler<int>();

    // ADC
    #ifdef ADC_1_EN
    ADC_1 = new AD_C(&hadc1, true, true, true);
    #endif
    #ifdef ADC_2_EN
    ADC_2 = new AD_C(&hadc2);
    #endif
    #ifdef ADC_3_EN
    ADC_3 = new AD_C(&hadc3);
    #endif

    // FLASH
    #ifdef FLASH_EN
    Flash = new Flash_mem();
    #endif

    // USB_CDC
    #ifdef USB_CDC_EN
    USB_port = new USB_CDC();
    #endif

    // UART
    #ifdef UART_1_EN
    UART_1 = new UART(&huart1);
    #endif
    #ifdef UART_2_EN
    UART_2 = new UART(&huart2);
    #endif
    #ifdef UART_3_EN
    UART_3 = new UART(&huart3);
    #endif
    #ifdef UART_4_EN
    UART_4 = new UART(&huart4);
    #endif

    // DAC
    #ifdef DAC_1_EN
    DAC_1 = new DA_C(&hdac, 2);
    #endif

    #ifdef TIM_1_EN
    ALOHAL_CREATE_TIMER(1, TIM_1, htim1, 16, 4)
    #endif
    #ifdef TIM_2_EN
    ALOHAL_CREATE_TIMER(2, TIM_2, htim2, 32, 4)
    #endif
    #ifdef TIM_3_EN
    ALOHAL_CREATE_TIMER(3, TIM_3, htim3, 16, 4)
    #endif
    #ifdef TIM_4_EN
    ALOHAL_CREATE_TIMER(4, TIM_4, htim4, 16, 4)
    #endif
    #ifdef TIM_5_EN
    ALOHAL_CREATE_TIMER(5, TIM_5, htim5, 32, 4)
    #endif
    #ifdef TIM_6_EN
    ALOHAL_CREATE_TIMER(6, TIM_6, htim6, 16, 0)
    #endif
    #ifdef TIM_7_EN
    ALOHAL_CREATE_TIMER(7, TIM_7, htim7, 16, 0)
    #endif
    #ifdef TIM_8_EN
    ALOHAL_CREATE_TIMER(8, TIM_8, htim8, 16, 4)
    #endif
    #ifdef TIM_9_EN
    ALOHAL_CREATE_TIMER(9, TIM_9, htim9, 16, 2)
    #endif
    #ifdef TIM_10_EN
    ALOHAL_CREATE_TIMER(10, TIM_10, htim10, 16, 1)
    #endif
    #ifdef TIM_11_EN
    ALOHAL_CREATE_TIMER(11, TIM_11, htim11, 16, 1)
    #endif
    #ifdef TIM_12_EN
    ALOHAL_CREATE_TIMER(12, TIM_12, htim12, 16, 2)
    #endif
    #ifdef TIM_13_EN
    ALOHAL_CREATE_TIMER(13, TIM_13, htim13, 16, 1)
    #endif
    #ifdef TIM_14_EN
    ALOHAL_CREATE_TIMER(14, TIM_14, htim14, 16, 1)
    #endif
    #ifdef TIM_15_EN
    ALOHAL_CREATE_TIMER(15, TIM_15, htim15, 16, 4)
    #endif
    #ifdef TIM_16_EN
    ALOHAL_CREATE_TIMER(16, TIM_16, htim16, 16, 1)
    #endif

    // I2C
    #ifdef I2C_1_EN
    I2C_1 = new I2C_master(&hi2c1);
    #endif
    #ifdef I2C_2_EN
    I2C_2 = new I2C_master(&hi2c2);
    #endif
    #ifdef I2C_3_EN
    I2C_3 = new I2C_master(&hi2c3);
    #endif
    #ifdef I2C_4_EN
    I2C_4 = new I2C_master(&hi2c4);
    #endif

    // SPI
    #ifdef SPI_1_EN
    SPI_1 = new SPI_master(&hspi1);
    #endif
    #ifdef SPI_2_EN
    SPI_2 = new SPI_master(&hspi2);
    #endif
    #ifdef SPI_3_EN
    SPI_3 = new SPI_master(&hspi3);
    #endif
    #ifdef SPI_4_EN
    SPI_4 = new SPI_master(&hspi4);
    #endif
}
