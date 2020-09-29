#include "serial_line.hpp"

string Serial_line::Read(int length){
    string output = RX_buffer.substr(0, length);
    RX_buffer.erase(0, length);
    return output;
}

string Serial_line::Read(string delimiter){
    size_t position = RX_buffer.find(delimiter, 0);
    if (position != string::npos) {
        string output = RX_buffer.substr(0, position + delimiter.length());
        RX_buffer.erase(0, position + delimiter.length());
        return output;
    } else {
        return "";
    }
}

int Serial_line::Clear_buffer(){
    unsigned int length = RX_buffer.length();
    RX_buffer = "";
    return length;
}
