/**
 * @file mcu.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 14.09.2019
 */

#pragma once

#include <string>
#include <map>
#include <vector>

#include "globals.hpp"
#include "global_includes.hpp"

typedef unsigned int uint;


#if   defined(MCU_FAMILY_STM32_F0)
# include "stm32f0xx_hal_adc.h"
#elif defined(MCU_FAMILY_STM32_F1)
# include "stm32f1xx_hal_adc.h"
#elif defined(MCU_FAMILY_STM32_F3)
# include "stm32f3xx_hal_adc.h"
#elif defined(MCU_FAMILY_STM32_F4)
# include "stm32f4xx_hal_adc.h"
#elif defined(MCU_FAMILY_STM32_F7)
# include "stm32f7xx_hal_adc.h"
#elif defined(MCU_FAMILY_STM32_L0)
# include "stm32l0xx_hal_adc.h"
#elif defined(MCU_FAMILY_STM32_L1)
# include "stm32l1xx_hal_adc.h"
#elif defined(MCU_FAMILY_STM32_L4)
# include "stm32l4xx_hal_adc.h"
#elif defined(MCU_FAMILY_STM32_G0)
# include "stm32g0xx_hal_adc.h"
#endif // if   defined(MCU_FAMILY_STM32_F0)

typedef unsigned int uint;
using namespace std;

/**
 * @brief   Represents analog to digital convertor. Have multiplexed channel, only one can be measured at same time
 *          Also ADC1 contains channel for measuring internal reference for obtaining supply voltage and internal
 *              temperature sensor for measuring core temperature.
 *          Measuring can be performed in polling and IRQ mode, for IRQ is contained IRQ multi handler, key of
 *              multihandler is number describing channel.
 *          Continuous measurement can be performed on one channel, after every conversion will be notified registred
 *              IRQ observer asssigned to channel.
 *          All measurements are against reference supply voltage value, which is measured after device power up,
 *              if result are not valid, another calibration is required
 *          As channel number internal voltage reference have assigned channel number 32 and temperature sensor number 17
 *          Default setting are 12-bit resolution, Right alignment
 *          Initial settings in CubeMX are unchanged
 *
 *          For single ADC channel exists class which encapsulated it: ADC_channel
 */
class AD_C {
public:
    enum class Resolution {
        _6_bit = 6,
        _8_bit = 8,
        _10_bit = 10,
        _12_bit = 12
    };

    enum class Sampling_rate {
        _2C5,
        _6C5,
        _12C5,
        _24C5,
        _47C5,
        _92C5,
        _247C5,
        _640C5
    };

    enum class Internal_channel {
        VREF,
        VBAT,
        ITEMP
    };

    /**
     * @brief reference to ADC handler structure of HAL
     */
    ADC_HandleTypeDef *handler;

    /**
     * @brief Actual selected channel
     */
    int actual_channel = -1;

    /**
     * @brief Supply voltage measured during calibration
     */
    double supply_voltage;

    /**
     * @brief Value measured during last conversion, at 12bit resolution is 0-4095
     */
    uint16_t ADC_value;

    /**
     * @brief Resoution of ADC, at default is set to 12bits
     */
    Resolution resolution = Resolution::_12_bit;

private:
    map<Sampling_rate, uint32_t> sample_rate_map{
        make_pair(Sampling_rate::_2C5, ADC_SAMPLETIME_2CYCLES_5),
        make_pair(Sampling_rate::_6C5, ADC_SAMPLETIME_6CYCLES_5),
        make_pair(Sampling_rate::_12C5, ADC_SAMPLETIME_12CYCLES_5),
        make_pair(Sampling_rate::_24C5, ADC_SAMPLETIME_24CYCLES_5),
        make_pair(Sampling_rate::_47C5, ADC_SAMPLETIME_47CYCLES_5),
        make_pair(Sampling_rate::_92C5, ADC_SAMPLETIME_92CYCLES_5),
        make_pair(Sampling_rate::_247C5, ADC_SAMPLETIME_247CYCLES_5),
        make_pair(Sampling_rate::_640C5, ADC_SAMPLETIME_640CYCLES_5)
    };

    map<Resolution, uint32_t> resolution_map{
        make_pair(Resolution::_6_bit, ADC_RESOLUTION_6B),
        make_pair(Resolution::_8_bit, ADC_RESOLUTION_8B),
        make_pair(Resolution::_10_bit, ADC_RESOLUTION_10B),
        make_pair(Resolution::_12_bit, ADC_RESOLUTION_12B)
    };

    vector<uint32_t> channel_map_common = {
        ADC_CHANNEL_0,
        ADC_CHANNEL_1,
        ADC_CHANNEL_2,
        ADC_CHANNEL_3,
        ADC_CHANNEL_4,
        ADC_CHANNEL_5,
        ADC_CHANNEL_6,
        ADC_CHANNEL_7,
        ADC_CHANNEL_8,
        ADC_CHANNEL_9,
    };

    map<Internal_channel, uint32_t> channel_map_internal{
        #ifdef ADC_CHANNEL_VREFINT
        make_pair(Internal_channel::VREF, ADC_CHANNEL_VREFINT),
        #endif
        #ifdef  ADC_CHANNEL_VBAT
        make_pair(Internal_channel::VBAT, ADC_CHANNEL_VBAT),
        #endif
        #ifdef ADC_CHANNEL_TEMPSENSOR
        make_pair(Internal_channel::ITEMP, ADC_CHANNEL_TEMPSENSOR)
        #endif
    };

    /**
     * @brief Bool which defined if adc have internal channels VREF, VBAT, TEMP
     */
    bool has_vref;
    bool has_itemp;
    bool has_vbat;

    /**
     * @brief   Configuration for channel
     *          This configuration should alway be same as which was used
     *              for channel configuration
     */

    #ifdef MCU_FAMILY_STM32_G0
    ADC_ChannelConfTypeDef channel_config = {
        .Channel      = ADC_CHANNEL_0,
        .Rank         = ADC_REGULAR_RANK_1,
        .SamplingTime = ADC_SAMPLINGTIME_COMMON_1,
    };
    #else
    ADC_ChannelConfTypeDef channel_config = {
        .Channel      = ADC_CHANNEL_0,
        .Rank         = ADC_REGULAR_RANK_1,
        .SamplingTime = ADC_SAMPLETIME_2CYCLES_5,
        .SingleDiff   = ADC_SINGLE_ENDED,
        .OffsetNumber = ADC_OFFSET_NONE,
    };
    #endif

public:

    AD_C() = default;
    AD_C(ADC_HandleTypeDef *handler, bool vrefint = false, bool inttemp = false, bool vbat = false);

    int Set_resolution(int resolution);


    uint16_t Measure(); // Rin one pulse measuring ! polling => blocking
    void Start_IT();    // Start one measuring with interrupt


    /**
     * @brief Perform calibration of ADC via HAL function and measure supply voltage voltage
     *
     * @return int Error_code
     */
    int Calibration();

    /**
     * @brief   Setup continuous measurement of given ADC channel, this method supports only IRQ notification not pool
     *          Continuous measurements can be paused for performing polling measurements
     *
     * @param channel   Channel on which will be continuous measurement performed, cannot be performed on VREF or ITEMP
     * @return int      Error number
     */
    int Continuous_measurement(int channel);

    /**
     * @brief Disable continuous measurement, did not unregister IRQ observer
     *
     * @return int Error number
     */
    int Continuous_stop();

    /**
     * @brief   Perform one or more conversion in IRQ mode, requester must be registered to IRQ handler
     *          Request for this type of conversion are queued
     *          Also more then one measurement can be performed, maximal number of conversions is 16
     *
     * @param channel           Channel for performing conversion
     * @param conversion_count  Number of conversion which will be performed
     *
     * @return int      Error number
     */
    int Measure_IRQ(int channel, unsigned int conversion_count);

    /**
     * @brief   Perform one measurement in polling mode, this mode blocking
     *          Have 10ms timeout, after timeout return negative value as result of measurement
     *
     * @param channel   Channel designated for measuring
     * @return double   Measured voltage
     */
    double Measure_poll(int channel);

    /**
     * @brief   Readout function for IRQ measurement, return value of last performed measurement
     *
     * @return double Last measured voltage
     */
    double Read();

    /**
     * @brief   Set member variable after measuring,
     *
     * @param value     New value for ADC_value
     */
    void Set_value(uint16_t value);

    #ifdef ADC_CHANNEL_VREFINT

    #if !defined(INTERNAL_VOLTAGE_REFERENCE_ADDRESS)
        #error "Address of voltage reference data is not defined"
    #endif

    /**
     * @brief   Perform measuring on internal reference channel
     *          Use polling measurement mode
     *
     * @return double Supply voltage if internal channel exists
     */
    double Supply_voltage();
    #endif

    /**
     * @brief   Set supplyvoltage which can be used in mcu does not have vref channel
     *              for measuring supply voltage
     *
     * @param supply_voltage
     * @return int
     */
    int Set_supply_voltage(double supply_voltage);

    #ifdef ADC_CHANNEL_TEMPSENSOR

    #if !defined(INTERNAL_TEMPERATURE_CALIBRATION_1_ADDRESS) || !defined(INTERNAL_TEMPERATURE_CALIBRATION_2_ADDRESS)
        #error "Address of calibration adata is not defined"
    #endif

    #if !defined(INTERNAL_TEMPERATURE_REFERENCE_VOLTAGE)
        #error "Reference voltage of temperature calibration is not defined"
    #endif

    /**
     * @brief   Perform measuring on internal temperature sensor and after it calculate internal temperature
     *          Use polling measurement mode
     *
     * @return double   Internal temperature in °C
     */
    double Core_temperature();
    #endif

    #ifdef  ADC_CHANNEL_VBAT

    /**
     * @brief   Perform one conversion on battery voltage channel
     *
     * @return double   Battery voltage
     */
    double Battery_voltage();
    #endif

    /**
     * @brief Set resolution of ADC
     *
     * @param resolution    Resolution allowed values are from enum Resolution
     * @return int          Error number
     */
    int Set_resolution(Resolution resolution);

    /**
     * @brief   Set sampling rate for designated channel
     *          Sample rate cannot be set for internal channel like VREF
     *
     * @param sampling_rate New sampling rate from enum Sampling_rate
     * @return int
     */
    int Set_channel_sampling_rate(Sampling_rate sampling_rate);
private:

    /**
     * @brief Select ADC channel for measuring
     *
     * @param chan  Number or enum of channel
     * @return int  Error number
     */
    int Set_channel(int chan);
    int Set_channel(Internal_channel chan);

    /**
     * @brief Use internal structure channel_config to configure channel of ADC
     *
     *  @return int  Error number
     */
    int Configure_channel();

    /**
     * @brief   Perform measuring of internal channel
     *          Before it save configuration and after it restore it
     *
     * @return int  Conversion result, 0 if some problem occurs
     */
    uint16_t Internal_measurement(Internal_channel internal_channel);
};

/**
 * @brief IRQ function which will be called when IRQ on ADC occurs
 *
 * @param hadc Handler of adc which trigger IRQ
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc);
