#include "endstop.hpp"

bool Endstop::Read(){
    return (Pin::Read() ^ inverted);
}

bool Endstop::Invert(bool flag){
    inverted = flag;
    return inverted;
}
