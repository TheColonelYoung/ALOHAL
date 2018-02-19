#include "IP_packet.hpp"

IP_packet::IP_packet(string IP_address, int port, string data)
    : IP_address(IP_address), port(port), data(data){
    length = data.length();
}

string IP_packet::Print(){
    return "";
}
