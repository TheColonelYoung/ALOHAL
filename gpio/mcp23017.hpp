/**
 * @file mcp23017.cpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 08.08.2019
 */

#pragma once

class MCP23017
{
private:
    uint8_t address = 0;

public:
    MCP23017() =default;
    MCP23017(uint8_t addr);

    Set(char port, uint8_t pin);
};

