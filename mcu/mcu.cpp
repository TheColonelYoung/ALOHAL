#include "mcu.hpp"

#include "device/device.hpp"

void MCU::Init(){
    Init_peripherals();
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
    ALOHAL_CREATE_TIMER(TIM_1, htim1, 16, 4)
    #endif
    #ifdef TIM_2_EN
    ALOHAL_CREATE_TIMER(TIM_2, htim2, 32, 4)
    #endif
    #ifdef TIM_3_EN
    ALOHAL_CREATE_TIMER(TIM_3, htim3, 16, 4)
    #endif
    #ifdef TIM_4_EN
    ALOHAL_CREATE_TIMER(TIM_4, htim4, 16, 4)
    #endif
    #ifdef TIM_5_EN
    ALOHAL_CREATE_TIMER(TIM_5, htim5, 32, 4)
    #endif
    #ifdef TIM_6_EN
    ALOHAL_CREATE_TIMER(TIM_6, htim6, 16, 0)
    #endif
    #ifdef TIM_7_EN
    ALOHAL_CREATE_TIMER(TIM_7, htim7, 16, 0)
    #endif
    #ifdef TIM_8_EN
    ALOHAL_CREATE_TIMER(TIM_8, htim8, 16, 4)
    #endif
    #ifdef TIM_9_EN
    ALOHAL_CREATE_TIMER(TIM_9, htim9, 16, 2)
    #endif
    #ifdef TIM_10_EN
    ALOHAL_CREATE_TIMER(TIM_10, htim10, 16, 1)
    #endif
    #ifdef TIM_11_EN
    ALOHAL_CREATE_TIMER(TIM_11, htim11, 16, 1)
    #endif
    #ifdef TIM_12_EN
    ALOHAL_CREATE_TIMER(TIM_12, htim12, 16, 2)
    #endif
    #ifdef TIM_13_EN
    ALOHAL_CREATE_TIMER(TIM_13, htim13, 16, 1)
    #endif
    #ifdef TIM_14_EN
    ALOHAL_CREATE_TIMER(TIM_14, htim14, 16, 1)
    #endif
    #ifdef TIM_15_EN
    ALOHAL_CREATE_TIMER(TIM_15, htim15, 16, 4)
    #endif
    #ifdef TIM_16_EN
    ALOHAL_CREATE_TIMER(TIM_16, htim16, 16, 1)
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
