#include "SSD1306.hpp"

SSD1306::SSD1306(uint res_x, uint res_y, I2C_master master, unsigned char address):
    I2C_device(master, address), Graphical_display(res_x, res_y)
{}

void SSD1306::Init(){
    Off();

    Transmit(vector<uint8_t>{0x00, 0xd3, 0x00}); //Set display offset

    Send_command(0xD4); // Set Display Clock Divide Ratio / OSC Frequency
    Send_command(0x80); // Display Clock Divide Ratio / OSC Frequency

    Send_command(0xA8); // Set Multiplex Ratio
    Send_command(0x3F); // Multiplex Ratio for 128x64 (64-1)

    Send_command(0xD3); // Set Display Offset
    Send_command(0x00); // Display Offset

    Send_command(0x40); // Set Display Start Line

    Send_command(0x8D); // Set Charge Pump
    Send_command(0x14); // Charge Pump (0x10 External, 0x14 Internal DC/DC)


    Send_command(0xA1); // Set Segment Re-Map
    Send_command(0xC8); // Set Com Output Scan Direction

    Send_command(0xDA); // Set COM Hardware Configuration
    Send_command(0x12); // COM Hardware Configuration

    Send_command(0xD9); // Set Pre-Charge Period
    Send_command(0xF1); // Set Pre-Charge Period (0x22 External, 0xF1 Internal)

    Send_command(0xDB); // Set VCOMH Deselect Level
    Send_command(0x40); // VCOMH Deselect Level

    Send_command(0x40); // Set Display Start Line

    Send_command(0xa4); // Pixels reflect RAM
    Send_command(0xa6); // Non-inverted display mode

    Transmit(vector<uint8_t>{0x00, 0x20, 0x00}); // Horizontal addressing mode

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

void SSD1306::Print(){
    Transmit(vector<uint8_t>{0x40, 0x00, 0x00, 0xff, 0xff});
}

void SSD1306::Clear_all(){
    Set_address(0,0);
    vector<uint8_t> clear(8*127,0);
    clear.insert(clear.begin(), 0x40);
    Transmit(clear);
}

int SSD1306::Set_address(uint8_t page, uint8_t column){
    if ((page > 7) || (column > 127)){
        return ERANGE;
    }

    Transmit(vector<uint8_t>{0x00, 0x22, page  , 0x03});
    Transmit(vector<uint8_t>{0x00, 0x21, column, 0x8f});

    return 0;

}
