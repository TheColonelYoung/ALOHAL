#include "SSD1306.hpp"

SSD1306::SSD1306(uint res_x, uint res_y, I2C_master master, unsigned char address):
    I2C_device(master, address), Graphical_display(res_x, res_y)
{}

void SSD1306::Init(){
    Off();

    Transmit(vector<uint8_t>{0x00, 0xd3, 0x00}); //Set display offset

    Send_command(0x40); //Set Display Start Line

    Send_command(0xa4); // Pixels reflect RAM
    Send_command(0xa6); // Non-inverted display mode

    Set_contrast(127);

}

void SSD1306::On(){
    Send_command(0xaf);
}

void SSD1306::Off(){
    Send_command(0xae);
}

int SSD1306::Put(uint x, uint y){

}

int SSD1306::Clear(uint x, uint y){
}

void SSD1306::Send_command(uint8_t cmd){
    Transmit(vector<uint8_t>{0x00, cmd});
}

void SSD1306::All_on(){
    Send_command(0xa5);
}

int SSD1306::Set_contrast(uint8_t contrast){
    return Transmit(vector<uint8_t>{0x00, 0x81, contrast});
}
