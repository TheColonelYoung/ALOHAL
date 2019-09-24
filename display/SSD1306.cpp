#include "SSD1306.hpp"



void SSD1306::On(){
    Transmit(vector<uint8_t>{0x00,0xaf});
    Transmit(vector<uint8_t>{0x00,0xa5});
    //Transmit(vector<uint8_t>{0x00,0xa7});
}

void SSD1306::Off(){
}

int SSD1306::Put(uint x, uint y){

}

int SSD1306::Clear(uint x, uint y){
}
