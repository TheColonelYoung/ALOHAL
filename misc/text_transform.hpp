/**
 * @file text_transform.hpp
 * @author Petr Malaník (TheColonelYoung(at)gmail(dot)com)
 * @version 0.1
 * @date 9.03.2016
 */
#pragma once

#include <string>
#include <map>

using namespace std;

namespace Colored_text{
    enum class Text_colors{
        reset,
        black,
        red,
        green,
        yellow,
        blue,
        magenta,
        cyan,
        white
    };

    extern map<Text_colors, string> Color_sequence;
}

string color(string text, Colored_text::Text_colors color);
