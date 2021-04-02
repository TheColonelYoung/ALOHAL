#include "L6470.hpp"

L6470::L6470(SPI_master master, Pin *chip_select, bool cs_active) :
    SPI_device(master, chip_select, cs_active),
    Component("L6470")
{
    if(device()->Filesystem_available()){
        Create_virtual_file("status", this, &L6470::Status_formated);
    }
}

void L6470::Init(){
    // Config
    Set_param(register_map::CONFIG, 0b0001111010000000, 16);

    // Overcurrent
    Set_param(register_map::OCD_TH, 0b00001010, 4);

    // Stall TH
    Set_param(register_map::STALL_TH, 0x7f, 7);
}

void L6470::Set_max_speed(uint max_speed){
    uint value = round((max_speed * (250 * pow(10, -9))) / pow(2, -18));
    Set_param(register_map::MAX_SPD, value, 10);
}

void L6470::Set_min_speed(uint min_speed){
    uint value = round((min_speed * (250 * pow(10, -9))) / pow(2, -24));
    Set_param(register_map::MIN_SPD, value, 12);
}

void L6470::Set_acceleration(uint acceleration){
    uint value = round((acceleration * pow(250 * pow(10, -9), 2)) / pow(2, -40));
    Set_param(register_map::ACC, value, 12);
}

void L6470::Set_deceleration(uint deceleration){
    uint value = round((deceleration * pow(250 * pow(10, -9), 2)) / pow(2, -40));
    Set_param(register_map::DEC, value, 12);
}

uint L6470::Calculate_speed(uint speed){
    return round((speed * (250 * pow(10, -9))) / pow(2, -28));
}

int L6470::Set_microsteps(uint microsteps){
    if (microstepping_config.find(microsteps) == microstepping_config.end() ) {
        return -1;
    } else {
        Set_param(register_map::STEP_MODE, microstepping_config.at(microsteps), 8);
    }
    return microstepping_config.at(microsteps);
}

int L6470::Move(Direction dir, uint steps, uint speed){
    if (speed != 0) {
        Set_max_speed(speed);
    }

    vector<uint8_t> data(4);
    data[0]  = static_cast<uint8_t>(command::Move);

    data[0] |= dir;

    data[1]  = 0x3f & (steps >> 16);
    data[2]  = 0xff & (steps >> 8);
    data[3]  = 0xff & (steps);
    Send(data);
    return 0;
}

int L6470::Run(Direction dir, uint speed){
    speed = Calculate_speed(speed);
    vector<uint8_t> data(4);
    data[0]  = static_cast<uint8_t>(command::Run);
    data[0] |= dir;
    data[1]  = 0x0f & (speed >> 16);
    data[2]  = 0xff & (speed >> 8);
    data[3]  = 0xff & (speed);
    Send(data);
    return 0;
}

void L6470::GoHome(Direction dir){
    Run(dir);
}

void L6470::ReleaseSW(Direction dir){
    Send(static_cast<uint8_t>(command::ReleaseSW));
}

void L6470::Hard_stop(){
    Send(static_cast<uint8_t>(command::HardStop));
}

int L6470::Soft_stop(){
    Send(static_cast<uint8_t>(command::SoftStop));
    return 0;
}

int L6470::Sleep(){
    Soft_HiZ();
    return 0;
}

void L6470::Soft_HiZ(){
    Send(static_cast<uint8_t>(command::SoftHiZ));
}

int L6470::Reset(){
    Send(static_cast<uint8_t>(command::ResetDevice));
    return 0;
}

int L6470::Send(uint8_t data){
    Transmit(vector<uint8_t> { data });
    return 0;
}

int L6470::Send(vector<uint8_t> data){
    for (auto &byte:data) {
        Transmit(vector<uint8_t> { byte });
    }
    return 0;
}

int L6470::Set_param(register_map param, const uint32_t value, uint size){
    vector<uint8_t> data(1 + ceil(size / 8.0));
    data[0] = static_cast<uint8_t>(param);
    if (size <= 8) {
        data[1] = value & 0xff;
    } else if (size <= 16) {
        data[1] = (value >> 8) & 0xff;
        data[2] = value & 0xff;
    } else if (size <= 24) {
        data[1] = (value >> 16) & 0xff;
        data[2] = (value >> 8) & 0xff;
        data[3] = value & 0xff;
    }
    Send(data);
    return data.size();
}

vector<uint8_t> L6470::Get_param(register_map param, uint size){
    uint bytes      = ceil(size / 8.0);
    uint8_t command = static_cast<uint8_t>(command::GetParam) | static_cast<uint8_t>(param);
    Send(vector<uint8_t> { command });
    vector<uint8_t> received_bytes(bytes);
    for (uint i = 0; i < bytes; i++) {
        received_bytes[i] = Receive(1)[0];
    }
    return received_bytes;
}

uint16_t L6470::Status(){
    Transmit(vector<uint8_t> { 0b11010000 });
    vector<uint8_t> receive1 = Receive(1);
    vector<uint8_t> receive2 = Receive(1);
    return receive2[0] << 8 | receive1[0];
}

string L6470::Status_formated(){
    string message  = "L6470 Status registr:\r\n";
    string new_line = "\r\n";
    uint16_t s      = Status();
    status *stat;
    stat = (status *) &s;

    bitset<8> b1 = ((uint8_t *) &s)[0];
    bitset<8> b2 = ((uint8_t *) &s)[1];

    message += "Values: ";
    message += b1.to_string();
    message += "  ";
    message += b2.to_string();
    message += new_line;

    message += "StepClock mode: ";
    stat->SCK_MOD ? message += "ENABLED" : message += "DISABLED";
    message += new_line;

    message += "Step loss B: ";
    stat->STEP_LOSS_B ? message += "OK" : message += "LOST STEP";
    message += new_line;

    message += "Step loss A: ";
    stat->STEP_LOSS_A ? message += "OK" : message += "LOST STEP";
    message += new_line;

    message += "Overcurrent detection: ";
    stat->OCD ? message += "INACTIVE" : message += "ACTIVE";
    message += new_line;

    message += "Thermal shutdown: ";
    stat->TH_SD ? message += "INACTIVE" : message += "ACTIVE";
    message += new_line;

    message += "Thermal warning: ";
    stat->TH_WRN ? message += "INACTIVE" : message += "ACTIVE";
    message += new_line;

    message += "Under voltage lockout: ";
    stat->UVLO ? message += "INACTIVE" : message += "ACTIVE";
    message += new_line;

    message += "Wrong command: ";
    stat->WRONG_CMD ? message += "WRONG" : message += "OK";
    message += new_line;
    message += new_line;

    message += "Cannot perform cmd: ";
    stat->NOTPERF_CMD ? message += "CANNOT PERF" : message += "OK";
    message += new_line;

    message += "Motor status: ";
    switch (stat->MOT_STATUS) {
        case 0: message += "Stopped";
            break;
        case 1: message += "Acceleration";
            break;
        case 2: message += "Deceleration";
            break;
        case 3: message += "Constant speed";
            break;
    }
    message += new_line;

    message += "Motor direction: ";
    stat->DIR ? message += "Forward" : message += "Reverse";
    message += new_line;

    message += "Switch event: ";
    stat->SW_EVN ? message += "ACTIVE" : message += "INACTIVE";
    message += new_line;

    message += "Switch flag: ";
    stat->SW_F ? message += "CLOSE" : message += "OPEN";
    message += new_line;

    message += "Busy flag: ";
    stat->BUSY ? message += "IDLE" : message += "BUSY";
    message += new_line;

    message += "HiZ bridge state: ";
    stat->HIZ ? message += "ACTIVE" : message += "INACTIVE";
    message += new_line;

    message += "===========================" + new_line;
    return message;
} // L6470::Status_formated
