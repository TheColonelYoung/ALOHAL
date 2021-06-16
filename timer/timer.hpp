/**
 * @file timer.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.2
 * @date 01.04.2020
 */
#pragma once

#include "global_includes.hpp"
#include "globals.hpp"
#include "timer/TIM_channel.hpp"
#include "irq/irq_handler.hpp"

/**
 * @brief   Timer have specified modes in which can be used. This modes are defined more by an usage then by
 *              structure of inner hardware.
 *
 *          Counter for IRQ count from zero (or any other value in CNT-Counter register to value of
 *               ARR-Auto reload register)
 *          When ARRpreload is enabled, new value to ARR is written after IRQ happens
 *          Frequency of timer input is initialized only once at mcu start-up
 *
 *                CHANNELS
 * TIM_CHANNEL_1                      0x00000000U
 * TIM_CHANNEL_2                      0x00000004U
 * TIM_CHANNEL_3                      0x00000008U
 * TIM_CHANNEL_4                      0x0000000CU
 * TIM_CHANNEL_ALL                    0x00000018U
 */

class Timer {
public:
    friend TIM_channel;

    /**
     * @brief   IRQ handler for timer
     */
    IRQ_handler *IRQ = new IRQ_handler();

    /**
     * @brief   Channel of timer
     */
    vector<TIM_channel> channel;

    /**
     * @brief Modes in which timer can work
     *
     *  Timer       - measuring of time
     *  Timer_IRQ   - set time to next IRQ event
     *  Counter     - counting of external inputs
     */
    enum class Modes {
        Timer,
        Timer_IRQ,
        Counter
    };

private:

    /**
     * @brief   Index of timer, for TIM_1 is 1
     */
    unsigned short index;

    /**
     * @brief   Actual mode of timer
     */
    Modes mode = Modes::Timer;

    /**
     * @brief Size of timer counter in bits
     */
    uint8_t size;

    /**
     * @brief Flag if timer should optimize prescaler during every change of time period
     *
     */
    bool optimize = false;

    /**
     * @brief Frequency of timer clock input
     */
    uint frequency;

    /**
     * @brief   Timer of ticks (timer input) per usecond
     *          Initialized only at startup as frequency
     */
    double uticks;

    /**
     * @brief Handle to HAl timer structure of timer
     */
    TIM_HandleTypeDef *handler;

public:

    /**
     * @brief   Construct a new Timer object
     *          Initialize timer with channels then call rest of initialization
     *
     * @param handler       Handle to HAl timer structure of timer
     * @param size          Size if counter register
     * @param channels      Number of timer channels
     */
    Timer(unsigned short index, TIM_HandleTypeDef *handler, int size, int channels);

    /*  Function below are separated into section based on usability in different modes
        Function have effect only if are used in their mode, otherwise do nothing
    */

    const TIM_channel &operator[](int i) const{
        return channel[i-1];
    }

    /*  ALL MODES */

    /**
     * @brief           Set mode of timer
     *
     * @param mode      New mode of timer
     */
    inline void Mode(Modes mode) {this->mode = mode;};

    /**
     * @brief           Return actual mode of timer
     *
     * @return Modes    Actual mode of timer
     */
    inline Modes Mode(){ return mode; };

    /* Function below manipulates with register of timer, probably should be private, will be? */

    /**
     * @brief           Set counter of timer to given value
     *
     * @param counter   New value for counter
     */
    inline void Counter(uint32_t counter){ handler->Instance->CNT = counter; };

    /**
     * @brief           Return actual value of counter
     *
     * @return uint32_t Counter register value
     */
    inline uint32_t Counter(){ return handler->Instance->CNT; };

    /**
     * @brief           Set prescaler of timer to given value
     *
     * @param prescaler New value for prescaler
     */
    inline void Prescaler(std::uint16_t prescaler){ handler->Instance->PSC = prescaler; };

    /**
     * @brief           Return actual value of prescaler
     *
     * @return uint16_t Prescaler register value
     */
    inline std::uint16_t Prescaler() const { return handler->Instance->PSC; };

    /**
     * @brief               Set autoreload of timer to given value
     *
     * @param autoreload    New value for autoreload
     */
    inline void Autoreload(uint32_t autoreload){ handler->Instance->ARR = autoreload; };

    /**
     * @brief           Return actual value of autoreload
     *
     * @return uint32_t Autoreload register value
     */
    inline uint32_t Autoreload() const { return handler->Instance->ARR; };

    /**
     * @brief       Set global optimize flag for timer
     *              When true, during every change of timer period prescaler will be optimized to best value
     *              Can be override by local flag when setting time
     *
     * @param flag  New state of optimize flag
     */
    inline void Optimize(bool flag){ optimize = flag; };

    /**
     * @brief           Return optimizer flag
     *
     * @return true     Timer is globally optimizing prescaller for every new period
     * @return false    Timer is not globally optimizing prescaller for every new period
     */
    inline bool Optimize() const { return optimize; };

    /**
     * @brief   Set time after which IRQ will activated
     *          Repeated periodically until stopped
     *
     * @param useconds  Time to next IRQ
     * @param optimize  Local flag for prescaler optimization
     */
    void Time_set(double useconds, bool optimize = false);

    /**
     * @brief   Set frequency in which timer will execute IRQ
     *
     * @param frequency Frequency in Hz
     * @param optimize  Local flag for prescaler optimization
     */
    void Frequency_set(double frequency, bool optimize = false);

    /**
     * @brief   Enable input for counter, clock input or external input
     */
    void Start();

    /**
     * @brief   Disable input for counter, clock input or external input
     */
    void Stop();

    /**
     * @brief       Size of timer counter
     *
     * @return uint8_t Number of bits of timer counter register, return only values: 16 or 32
     */
    inline uint8_t Size() const { return size; };

    /**
     * @brief   Number of channel of counter
     *
     * @return int  Number of channel of counter
     */
    inline int Channel_count() const { return channel.size(); }

    /**
     * @brief       Return frequency of clock source
     *
     * @return uint Frequency in Hz
     */
    inline uint Input_frequency() const { return frequency; };

    /**
     * @brief Returns index of timer
     *
     * @return unsigned short Index of timer
     */
    unsigned short Index() {return index;};

private:

    /**
     * @brief   Return handler, used by friend TIM_channel for access to configuration structure of HAL
     *
     * @return TIM_HandleTypeDef*   Pointer to configuration structure of HAL
     */
    inline TIM_HandleTypeDef *Handler() const {return handler;};

    /**
     * @brief   Optimize prescaler for given time
     *          Compute best prescaler value
     *          Best value if bigest counter value when overflow, with lower prescaler
     *
     * @param time_us   Time to optimize in usecond
     */
    void Optimize_for(double time_us); // optimize prescaler for given time
};

#ifdef TIMER_USED_AS_TIMEBASE
void ALOHAL_Timer_IRQ_Callback(TIM_HandleTypeDef *htim);
#else
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#endif


