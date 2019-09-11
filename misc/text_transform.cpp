#include "text_transform.hpp"

namespace Colored_text{
    map<Text_colors, string> Color_sequence{
        make_pair(Text_colors::reset,   "\u001b[0m"),
        make_pair(Text_colors::black,   "\u001b[30m"),
        make_pair(Text_colors::red,     "\u001b[31m"),
        make_pair(Text_colors::green,   "\u001b[32m"),
        make_pair(Text_colors::yellow,  "\u001b[33m"),
        make_pair(Text_colors::blue,    "\u001b[34m"),
        make_pair(Text_colors::magenta, "\u001b[35m"),
        make_pair(Text_colors::cyan,    "\u001b[36m"),
        make_pair(Text_colors::white,   "\u001b[37m"),
    };

}

string color(string text, Colored_text::Text_colors color){
    return Colored_text::Color_sequence[color] + text + Colored_text::Color_sequence[Colored_text::Text_colors::reset];
}
