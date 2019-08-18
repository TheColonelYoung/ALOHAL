#include "L6470.hpp"

void L6470::Hard_stop(){

}

int L6470::Soft_stop(){

    return 0;
}

int L6470::Move(Direction dir, uint steps, uint speed){

    return 0;
}

int L6470::Run(Direction dir, uint speed){

    return 0;
}

int L6470::Sleep(){

    return 0;
}

int L6470::Reset(){

    return 0;
}

uint16_t L6470::Status(){
    Transmit(vector<uint8_t> {0b11010000});
    vector<uint8_t> receive = Receive(2);
    return receive[1] << 8 | receive[0];
}

string L6470::Status_formated(){
    string message = "\r\nL6470 Status registr:\r\n";
    string new_line = "\r\n";
    uint16_t s = Status();
    status *stat;
    stat = (status *)&s;

    bitset<8> b1 = ((uint8_t *)&s)[0];
    bitset<8> b2 = ((uint8_t *)&s)[1];

    message += "Values: ";
    message += b1.to_string();
    message += "  ";
    message += b2.to_string();
    message += new_line;

    message += "StepClock mode: ";
    stat->SCK_MOD ? message += "ENABLED" : message += "DISABLED"; message += new_line;

    message += "Step loss B: ";
    stat->STEP_LOSS_B ? message += "OK" : message += "LOST STEP"; message += new_line;

    message += "Step loss A: ";
    stat->STEP_LOSS_A ? message += "OK" : message += "LOST STEP"; message += new_line;

    message += "Overcurrent detection: ";
    stat->OCD ? message += "INACTIVE" : message += "ACTIVE"; message += new_line;

    message += "Thermal shutdown: ";
    stat->TH_SD ? message += "INACTIVE" : message += "ACTIVE"; message += new_line;

    message += "Thermal warning: ";
    stat->TH_WRN ? message += "INACTIVE" : message += "ACTIVE"; message += new_line;

    message += "Under voltage lockout: ";
    stat->UVLO ? message += "INACTIVE" : message += "ACTIVE"; message += new_line;

    message += "Wrong command: ";
    stat->WRONG_CMD ? message += "WRONG" : message += "OK"; message += new_line; message += new_line;

    message += "Cannot perform cmd: ";
    stat->NOTPERF_CMD ? message += "CANNOT PERF" : message += "OK"; message += new_line;

    message += "Motor status: ";
    switch (stat->MOT_STATUS){
        case 0: message += "Stopped"; break;
        case 1: message += "Acceleration"; break;
        case 2: message += "Deceleration"; break;
        case 3: message += "Constant speed"; break;
    }
    message += new_line;

    message += "Motor direction: ";
    stat->DIR ? message += "Forward" : message += "Reverse"; message += new_line;

    message += "Switch event: ";
    stat->SW_EVN ? message += "ACTIVE" : message += "INACTIVE"; message += new_line;

    message += "Switch flag: ";
    stat->SW_F ? message += "CLOSE" : message += "OPEN"; message += new_line;

    message += "Busy flag: ";
    stat->BUSY ? message += "IDLE" : message += "BUSY"; message += new_line;

    message += "HiZ bridge state: ";
    stat->HIZ ? message += "ACTIVE" : message += "INACTIVE"; message += new_line;

    return message;
}
