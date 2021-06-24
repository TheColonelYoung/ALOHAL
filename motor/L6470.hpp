/**
 * @file L6470.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 18.08.2019
 */

#pragma once

#include <string>
#include <vector>
#include <bitset>
#include <cmath>
#include <map>

#include "stepper_motor.hpp"
#include "spi/spi_device.hpp"
#include "device/component.hpp"
#include "modifiers/loggable.hpp"

# define M_PI   3.14159265358979323846

using namespace std;

/**
 * @brief Datasheet https://www.st.com/resource/en/datasheet/l6470.pdf
 *
 * SPI settings: Format Motorola, DataSize 8 bit,  FirstBit MSB, CPOL HIGH, CPHA 2edge
 * Input Pin MISO should have pull-up resistor activated
 */
class L6470: public Stepper_motor, public SPI_device, public Component, public Loggable{
private:

public:
    struct __attribute__((packed)) __attribute__((__may_alias__)) status{
        //Order is reversed against datasheet because of structure packaging
        // MS byte, bits 8 -> 15
        uint8_t WRONG_CMD   : 1; // Wrong command  (active - HIGH)
        uint8_t UVLO        : 1; // Under voltage lockout  (active - LOW)
        uint8_t TH_WRN      : 1; // Thermal warning event  (active - LOW)
        uint8_t TH_SD       : 1; // Thermal shutdown event (active - LOW)
        uint8_t OCD         : 1; // Overcurrent detection (active - LOW)
        uint8_t STEP_LOSS_A : 1; // Lost step on bridge A (active - LOW)
        uint8_t STEP_LOSS_B : 1; // Lost step on bridge B (active - LOW)
        uint8_t SCK_MOD     : 1; // Step_clock mode
        // L byte, bits 0 -> 7
        uint8_t HIZ         : 1; // Bridge high impedance state (active - HIGH)
        uint8_t BUSY        : 1; // Reflets BUSY pin status (idle - HIGH)
        uint8_t SW_F        : 1; // Switch input status  (LOW - open, HIGH - close)
        uint8_t SW_EVN      : 1; // Switch tur-on event (active - HIGH)
        Stepper_motor::Direction DIR    : 1; // Direction of motor
        Stepper_motor::Status MOT_STATUS: 2; // Motor status
        uint8_t NOTPERF_CMD : 1; // Command cannot be performed  (active - HIGH)
    };

    enum class register_map: uint8_t{
        ABS_POS     = 0x01,
        EL_POS      = 0x02,
        MARK        = 0x03,
        SPEED       = 0x04,
        ACC         = 0x05,
        DEC         = 0x06,
        MAX_SPD     = 0x07,
        MIN_SPD     = 0x08,
        FS_SPD      = 0x15,
        KVAL_HOLD   = 0x09,
        KVAL_RUN    = 0x0a,
        KVAL_ACC    = 0x0b,
        KVAL_DEC    = 0x0c,
        INT_SPD     = 0x0d,
        ST_SLP      = 0x0e,
        FN_SLP_ACC  = 0x0f,
        FN_SLP_DEC  = 0x10,
        K_THERM     = 0x11,
        ADC_OUT     = 0x12,
        OCD_TH      = 0x13,
        STALL_TH    = 0x14,
        STEP_MODE   = 0x16,
        ALARM_EN    = 0x17,
        CONFIG      = 0x18,
        STATUS      = 0x19
    };

    enum class command: uint8_t{
        NOP         = 0b00000000,
        SetParam    = 0b00000000,
        GetParam    = 0b00100000,
        Run         = 0b01010000,
        StepClock   = 0b01011000,
        Move        = 0b01000000,
        GoTo        = 0b01100000,
        GoToDIR     = 0b01101000,
        GoUntil     = 0b10000010,
        ReleaseSW   = 0b10010010,
        GoHome      = 0b01110000,
        GoMark      = 0b01111000,
        ResetPos    = 0b11011000,
        ResetDevice = 0b11000000,
        SoftStop    = 0b10110000,
        HardStop    = 0b10111000,
        SoftHiZ     = 0b10100000,
        HardHiz     = 0b10101000,
        GetStatus   = 0b11010000
    };

    enum class error{
        Communication,
        Lost_step,
        Overcurrent,
        Thermal_shutdown,
        Thermal_warning,
        Undervoltage_lockout,
        Wrong_command,
        Cannot_perform_command
    };

    const map<int, uint> microstepping_config {
        std::make_pair (1,   0b000),
        std::make_pair (2,   0b001),
        std::make_pair (4,   0b010),
        std::make_pair (8,   0b011),
        std::make_pair (16,  0b100),
        std::make_pair (32,  0b101),
        std::make_pair (64,  0b110),
        std::make_pair (128, 0b111),
    };

private:
    /**
     * @brief   Initial configuration loaded to driver during initialization
     */
    const uint16_t standard_configuration = 0b0001111010000000;

    /**
     * @brief   Start up configuration of driver
     *          This value has different byte order then value in datasheet 2e88 -> 882e
     */
    const uint16_t power_up_configuration = 0x2e88;

    /**
     * @brief   Pin of MCU to which is connected FLAG output of driver
     */
    Pin * flag_pin = nullptr;

    /**
     * @brief   Pin of MCU to which is connected BUSY output of driver
     */
    Pin * busy_pin = nullptr;

    /**
     * @brief   Low speed optimization enable flag (datasheet LSPD_OPT)
     */
    bool low_speed_optimization = false;

    /**
     * @brief Number of microsteps per fullstep
     */
    uint microsteps = 1;

public:
    L6470(SPI_master &master, Pin *chip_select, bool cs_active = false, Pin *flag_pin = nullptr, Pin *busy_pin = nullptr);

/***** Motor Control *****/

    /**
     * @brief       Motor will makes number of steps in defined direction
     *
     * @param dir   Direction for move
     * @param steps Number of steps to do
     * @param speed Ignored
     * @return int  0 if is it possible, -1 if not (due to max or min speed)
     */
    virtual int Move(Direction dir, unsigned int steps, unsigned int speed = 0) final override;

    /**
     * @brief       Motor will make unlimited number of steps in defined direction
     *              Program must ensure stop conditions
     *
     * @param dir   Direction for move
     * @param speed Speed of steps, if is set to 0 is used default speed of motor, parameter is used only for this command
     *              Does not rewrite class variable, respects max and min speed
     * @return int  0 is is it possible, -1 if not (due to max or min speed)
     */
    int Run(Direction dir, unsigned int speed = 0) override;

        /**
     * @brief   Run motor in given direction until hits switch
     *
     * @param dir   Direction where enstop is located
     */
    void GoHome(Direction dir);

    /**
     * @brief   Sets current pocition as Home position (ABS_POS = 0)
     */
    void SetHome();

    /**
     * @brief   Run motor with minimal speed in given direction until switch is released
     *
     *
     * @param dir Direction to move
     */
    void ReleaseSW(Direction dir);

        /**
     * @brief Stop stepper motor immediately
     */
    void Hard_stop() final override;

    /**
     * @brief   Stop motor with a deceleration phase
     *          Leave motor engaged
     *
     * @return int 0 if is it possible, -1 if not
     */
    int Soft_stop() final override;

    /**
     * @brief   Decelerate motor from current speed, then set bridges to high impedance
     *          Similar to Release()
     */
    void Soft_HiZ();

    /**
     * @brief       Set motor into disengaged state, High impedance state of MOSFETs
     *              Similar to Soft_HiZ()
     *
     * @return int  0 if is it possible, -1 if not (disabled sleep state)
     */
    int Release() override;

    /**
     * @brief       Reset target stepper motor driver via SPI command
     *
     * @return int  0 if is it possible, -1 if not (target did not supports reset)
     */
    int Reset() override;

/***** Diagnostic *****/

    /**
     * @brief           Read status register from L6470
     *
     * @return uint16_t Constent of register, format of L6470::status
     */
    L6470::status Status();

    /**
     * @brief           Read status from L6470 and format it into string
     *
     * @return string   String repressentation of status register
     */
    string Status_formated();

    /**
     * @brief   Determinate status of driver
     *          Reads value from register if busy pin is not available
     *
     * @return true     Driver is in busy state
     * @return false    Driver is available
     */
    bool Busy();

    void Flag_IRQ();

    void Busy_IRQ();

    /**
     * @brief Calculate current speed of motor based on SPEED register
     *
     * @return unsigned int     Speed in steps
     */
    double Speed();

    /**
     * @brief Determinate current position based on ABS_POS register
     *
     * @return unsigned int     Current position in steps/microsteps
     */
    double Position();

    /**
     * @brief   Determinates if endstop switch is active or inactive
     *
     * @return true     Endstop switch is triggered
     * @return false    Endstop switch is open
     */
    bool Switch_status();

    /**
     * @brief   Determinates if endstop event is active
     *          Event flag is activated when switch is closed (falling edge of signal)
     *          After status readout event flag is cleared
     *
     * @return true     Switch event flag is active
     * @return false    Switch event flag is inactive
     */
    bool Switch_event();

    /**
     * @brief Enable switch event to execute HardStop at switch hit
     */
    void Switch_enable();

    /**
     * @brief   Disable switch event to execute HardStop at switch hit
     */
    void Switch_disable();

/***** Configuration *****/

public:
    /**
     * @brief initialize driver CONFIG register, configuration
     * 00011110
     * 10000000
     * - Hard stop at switch hit
     * - Stop at overcurrent detection
     */

    void Init();

    /**
     * @brief   Set parametr MAX_SPEED in driver
     *          The register value is calculated according to the formula in datasheet page 43.
     *
     * @param max_speed     New maximal speed of motor in steps/microsteps per second
     */
    void Max_speed(unsigned int max_speed);

    /**
     * @brief   Set parametr MIN_SPEED in driver
     *          The register value is calculated according to the formula in datasheet page 43.
     *
     * @param min_speed New minimal speed of motor in steps/microsteps per second
     */
    void Min_speed(unsigned int min_speed);

    /**
     * @brief               Set parametr MIN_SPEED in driver and also change low speed optimization status
     *                      Low speed optimization is immediately applied with this speed change command
     *
     * @param min_speed         New minimal speed of motor in steps/microsteps per second
     * @param low_speed_optimization_status New status of optimization (enabled/ disabled)
     */
    void Min_speed(unsigned int min_speed, bool low_speed_optimization_status);

    /**
     * @brief           Set or clear low speed optimization flag, optimization will apply after
     *                       next change of minimal speed
     *                  When set MIN_SPEED value serves as motor speed threshold under which
     *                       low speed optimization is applied
     *
     * @param status    New status of optimization (enabled/ disabled)
     */
    void Low_speed_optimization(bool optimization_status) {low_speed_optimization = optimization_status;};

    /**
     * @brief   Sets speed at which microstepping is disabled, after it motor use full step
     *          Full step driving increase torque but increase noise and amount of vibrations
     *
     * @param speed     Speed at which microstepping is disabled in steps per second
     *                  If this parameter is set to zero full step optimization ius disabled and
     *                      driver always use microstepping
     *                      minimal value is 8 steps/s, maximal 15625, resolution 15.25
     */
    void Full_step_optimization(unsigned int speed);

    /**
     * @brief   Set parametr ACC in driver
     *          The register value is calculated according to the formula in datasheet page 42.
     *
     * @param acceleration  New acceleration of motor in steps per second^2
     */
    void Acceleration(unsigned int acceleration);

    /**
     * @brief   Set parametr DEC in driver
     *          The register value is calculated according to the formula in datasheet page 43.
     *
     * @param acceleration  New acceleration of motor in steps/microsteps per second^2
     */
    void Deceleration(unsigned int deceleration);

    /**
     * @brief Set microsteping of driver
     *
     * @param microsteps    Number of microsteps, allowed values are:
     *                      1(full-step), 2(half-step), 4, 8, 16, 32, 64, 128
     *
     * @return int  validity of action, -1 if given microstepps cannot be set
     */
    int Microsteps(unsigned int microsteps);

    /**
     * @brief Return current number of microsteps per full step
     *
     * @return uint Current number of microsteps per full step
     */
    uint Microsteps() {return microsteps;};

    /**
     * @brief   Sets overcurrent protection, read status of this protection
     *
     * @param overcurrent_threshold Amount of current in mA which will trip overcurrent protection
     *                              From 375 mA to 6 A, with steps of 375 mA
     *                              If passed value is not in steps then is rounded to nearest lower value
     *                              Without parameter or with parametr zero returns status of overcurrent protection
     * @return true     Overcurrent protections is active
     * @return false    Overcurrent protections is inactive
     */
    bool Overcurrent(double overcurrent_threshold = 0);

    /**
     * @brief   Sets overcurrent protection, read status of this protection
     *
     * @param overcurrent_threshold Amount of current in mA which will trigger lost step detection
     *                              From 31.25 mA to 4 A, with steps of 31.25 mA
     *                              If passed value is not in steps then is rounded to nearest lower value
     *                              Without parameter or with parametr zero returns status of overcurrent protection
     * @return true     Step loss on bridge A or B
     * @return false    No step loss
     */
    bool Stall(double stall_threshold = 0);

    /**
     * @brief   Calculates preliminary set of parameters for motor control
     *          If calculations are correct loads them into registers
     *          Values are only preliminary and require fine tunning for best performance
     *          Detailed process of this calculation is described in application note: AN4144
     *
     *          Computes voltage which will be applied to motor coils and BEMF compensation parameters
     *          If correct will write values to registers:
     *              KVAL_HOLD, KVAL_RUN, KVAL_ACC, KVAL_DEC, INT_SPD, ST_SLP, FN_SLP_ACC, FN_SLP_DEC
     *
     *          How to determinate Motor electric constant (detailed description is in AN4144)
     *              - Connect motor phase to oscilloscope or logic analyzer with analog input (Saleae)
     *              - Slowly turn motor shaft by hand in constatnt speed (start at 90° per second)
     *              - Observe if amplitude on oscilloscope is stable (speed of rotation needs to be constant)
     *              - Measure peek to peek voltage and frequency of several stable periods (avarage them)
     *              - Constant is ration between voltage and frequency [V/Hz]
     *          For example for motor SX17-1003LQCEF this constant is 0.068 V/Hz
     *          If shaft is rotated too fast generated voltage can damage measuring device (start slow)
     *
     * @param motor_voltage             Nominal voltage, which will be used for motor driving in Volts (VSA, VSB voltage)
     * @param target_current            Maximal current through motor coil in Ampers(from motor datasheet)
     * @param phase_resistance          Resistance of phase in motor in Ohms
     * @param phase_inductance          Resistance of phase in motor in mH
     * @param motor_electric_constant   Coefficient describing how relates motor speed and BEMF amplitude
     * @return true     Tunning is possible and values are written to correspoding registers
     * @return false    With input parameters valid settings cannot be reached
     */
    bool Autotune(double motor_voltage, double target_current, double phase_resistance, double phase_inductance, double motor_electric_constant);

/***** Communication *****/

private:
    /**
     * @brief   L6470 need to have every byte in transmittion divided by re-enabling
     *          chip select signal, this method will make transmition for every byte of data
     *
     * @param data  Data to send to L6470
     * @return int  Validity of action
     */
    int Send(const vector<uint8_t> &data) const;

    /**
     * @brief   Send one byte of data to driver
     *
     * @param data  Data to send to L6470
     * @return int  Validity of action
     */
    int Send(const uint8_t data) const ;

    /**
     * @brief Set given register of L6470 to value
     *
     * @param param Parameter to set
     * @param value New value for parameter
     * @param size  Size of target register in bits, value will be cropped to that size
     * @return int  Validity of action
     */
    int Set_param(register_map param, const uint32_t value, uint size);

    /**
     * @brief   Return value of parametr from L6470
     *
     * @param param     Parameter to read
     * @param size      Size of target parameter
     * @return vector<uint8_t>  Value of parameter
     */
    vector<uint8_t> Get_param(register_map param, uint size);

};
