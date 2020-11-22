/**
 * @file color.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 06.10.2020
 */

#pragma once

#include <array>
#include <map>

using namespace std;

namespace Color {
/**
 * @brief Color which can be used for struct initialization via color_codes
 */
enum class Colors {
    White,
    Red,
    Green,
    Blue,
    Black,
    Cyan,
    Magenta,
    Yellow,
};

/**
 * @brief Mapping between Colors and their RGB representation
 */
extern map<Colors, array<uint8_t, 3> > color_codes;

/**
 * @brief Structure that hold RGb representation fo color
 *
 */
struct RGB {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    /**
     * @brief Construct a new RGB object
     *
     * @param color Color to which will be struct initialized
     */
    RGB(Colors color){
        red   = color_codes[color][0];
        green = color_codes[color][1];
        blue  = color_codes[color][2];
    }
};
}
