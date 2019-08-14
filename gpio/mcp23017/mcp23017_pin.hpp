/**
 * @file mcp23017_pin.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 14.08.2019
 */

#include "mcp23017.hpp"
#include "gpio/pin.hpp"

using namespace std;

/**
 * @brief GPIO on expander MCP23017 controlled via I2C bus
 * Can be used in same way as normal Pin
 */
class Pin_MCP23017: public Pin
{
private:
    MCP23017 *expander;

public:
    Pin_MCP23017() =default;

    /**
     * @brief Construct a new Pin_MCP23017 object
     *
     * @param expander    Reference to existing expander
     * @param pin_number  Number of pin of MCP23017 0-15
     */
    Pin_MCP23017(MCP23017 &expander, uint8_t pin_number);

    void Toggle() override;

    void Set(bool value) override;

    bool Read() override;





};
