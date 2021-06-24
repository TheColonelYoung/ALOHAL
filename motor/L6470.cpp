#include "L6470.hpp"

L6470::L6470(SPI_master &master, Pin *chip_select, bool cs_active, Pin *flag_pin, Pin *busy_pin) :
    SPI_device(master, chip_select, cs_active),
    Component("L6470"),
    flag_pin(flag_pin),
    busy_pin(busy_pin){
    if (device()->Filesystem_available()) {
        Create_virtual_file("status", this, &L6470::Status_formated);
        Create_virtual_file("speed", this, &L6470::Speed);
        Create_virtual_file("position", this, &L6470::Position);
    }
}

void L6470::Init(){
    vector<uint8_t> config_register = Get_param(register_map::CONFIG, 16);
    uint16_t config_value = (config_register[0] << 8) | config_register[1];
    // Checks if configuration if default or last used during initialization
    if ((config_value != power_up_configuration) and (config_value != standard_configuration)) {
        Log_line(Log_levels::Debug, name + ": Non-standard default value in configuration register (0x" + dec2hex(config_value) + ")");
    }

    // Check if config can be read, if zero communication line is not working
    // Possibly due to under-voltage lockout of L6470
    if (config_value == 0x0000) {
        Log_line(Log_levels::Error, name + ": Issue with comunication via SPI, empty config (0x000)");
        Log_line(Log_levels::Fatal, name + ": Initialization aborted");
        return;
    }

    // Check status register, compare with default value
    // This also clears default raised flag for under-voltage lockout
    status status_register = Status();
    // casting to char* is due to aliasing warning (char* is able to be aliased anything)
    uint16_t status_value = *((uint16_t *) &status_register);
    if ((status_value != 0x037e) and (status_value != 0x037c)) {
        Log_line(Log_levels::Warning, name + ": Potential issue with status register (0x" + dec2hex(status_value) + ")");
    }

    // Write initial config
    Set_param(register_map::CONFIG, standard_configuration, 16);

    // Registers pin IRQ which is changed by driver flag pin
    if (flag_pin) {
        flag_pin->IRQ->Register(this, &L6470::Flag_IRQ);
    }

    // Registers pin IRQ which is changed by driver flag pin
    if (flag_pin) {
        busy_pin->IRQ->Register(this, &L6470::Busy_IRQ);
    }

    Log_line(Log_levels::Debug, name + ": Initialized")
} // L6470::Init

double L6470::Speed(){
    auto read_value = Get_param(register_map::SPEED, 20);
    uint32_t register_speed = read_value[0] << 16 |read_value[1] << 8 | read_value[2];
    double speed = register_speed * pow(2, -28) / (250 * pow(10, -9));
    return speed;
}

double L6470::Position(){
    auto register_value = Get_param(register_map::ABS_POS, 22);
    uint32_t value = (register_value[0] << 16) |(register_value[1] << 8) | register_value[2];
    int position;
    if (value & (1 << 21)){
        value |= 0xffc00000;
        value = ~value;
        value += 1;
        position = value *-1;
    } else{
        position = value;
    }

    return static_cast<double>(position);
}

void L6470::Max_speed(unsigned int max_speed){
    this->max_speed = max_speed;
    uint32_t value = round((max_speed * (250 * pow(10, -9))) / pow(2, -18));
    Set_param(register_map::MAX_SPD, value, 10);
}

void L6470::Min_speed(unsigned int min_speed){
    this->min_speed = min_speed;
    uint32_t value = round((min_speed * (250 * pow(10, -9))) / pow(2, -24));

    if (low_speed_optimization) {
        value |= 1UL << 12;
    } else {
        value &= ~(1UL << 12);
    }

    Set_param(register_map::MIN_SPD, value, 13);
}

void L6470::Min_speed(unsigned int min_speed, bool low_speed_optimization_status){
    Low_speed_optimization(low_speed_optimization_status);
    Min_speed(min_speed);
}

void L6470::Acceleration(unsigned int acceleration){
    this->acceleration = acceleration;
    uint32_t value = round((acceleration * pow(250 * pow(10, -9), 2)) / pow(2, -40));
    Set_param(register_map::ACC, value, 12);
}

void L6470::Deceleration(unsigned int deceleration){
    this->deceleration = deceleration;
    uint32_t value = round((deceleration * pow(250 * pow(10, -9), 2)) / pow(2, -40));
    Set_param(register_map::DEC, value, 12);
}

int L6470::Microsteps(unsigned int microsteps){
    if (microstepping_config.find(microsteps) == microstepping_config.end() ) {
        return -1;
    } else {
        this->microsteps = microsteps;
        Set_param(register_map::STEP_MODE, microstepping_config.at(microsteps), 8);
    }
    return microstepping_config.at(microsteps);
}

void L6470::Full_step_optimization(unsigned int optimization_speed){
    uint32_t value;
    if (optimization_speed == 0) {
        value = 0x3ff;
    } else {
        value = round(((optimization_speed * (250 * pow(10, -9))) / pow(2, -18)) - 0.5);
    }
    Set_param(register_map::FS_SPD, value, 10);
}

int L6470::Move(Direction dir, unsigned int steps, unsigned int speed){
    if (speed != 0) {
        Max_speed(speed);
    }

    vector<uint8_t> data(4);
    data[0] = static_cast<uint8_t>(command::Move);
    data[0] |= static_cast<uint8_t>(dir);
    data[1] = 0x3f & (steps >> 16);
    data[2] = 0xff & (steps >> 8);
    data[3] = 0xff & (steps);
    Send(data);
    return 0;
}

int L6470::Run(Direction dir, unsigned int speed){
    speed = round((speed * (250 * pow(10, -9))) / pow(2, -28));
    vector<uint8_t> data(4);
    data[0] = static_cast<uint8_t>(command::Run);
    if(dir == Stepper_motor::Direction::Forward){
        data[0] |= 1UL;
    }
    data[1]  = 0x0f & (speed >> 16);
    data[2]  = 0xff & (speed >> 8);
    data[3]  = 0xff & (speed);
    Send(data);
    return 0;
}

void L6470::GoHome(Direction dir){
    Run(dir);
}

void L6470::SetHome(){
    Send(static_cast<uint8_t>(command::ResetPos));
}

void L6470::ReleaseSW(Direction dir){
    uint8_t command = static_cast<uint8_t>(command::ReleaseSW);
    if (dir == Direction::Forward) {
        command |= 1;
    }
    Send(command);
}

void L6470::Hard_stop(){
    Send(static_cast<uint8_t>(command::HardStop));
}

int L6470::Soft_stop(){
    Send(static_cast<uint8_t>(command::SoftStop));
    return 0;
}

int L6470::Release(){
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

int L6470::Send(const uint8_t data) const{
    Transmit(vector<uint8_t> { data });
    return 0;
}

int L6470::Send(const vector<uint8_t> &data) const{
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

bool L6470::Overcurrent(double overcurrent_threshold){
    if (overcurrent_threshold != 0) {
        overcurrent_threshold = min(max(overcurrent_threshold, 375.0), 6000.0);
        uint8_t oc_th_steps = floor(overcurrent_threshold / 375) - 1;
        if (oc_th_steps >= 0 && oc_th_steps <= 15) {
            Set_param(register_map::OCD_TH, oc_th_steps, 4);
        }
    }

    return Status().OCD;
}

bool L6470::Stall(double stall_threshold){
    if (stall_threshold != 0) {
        stall_threshold = min(max(stall_threshold, 31.25), 4000.0);
        uint8_t st_th_steps = floor(stall_threshold / 31.25) - 1;
        if (st_th_steps >= 0 && st_th_steps <= 15) {
            Set_param(register_map::STALL_TH, st_th_steps, 7);
        }
    }

    L6470::status status_register = Status();
    return (status_register.STEP_LOSS_A | status_register.STEP_LOSS_B);
}

bool L6470::Autotune(double motor_voltage, double target_current, double phase_resistance, double phase_inductance, double motor_electric_constant){
    unsigned int K_VAL = phase_resistance * target_current / motor_voltage * pow(2, 8);
    if ((K_VAL > 255) or (K_VAL == 0)) {
        Log_line(Log_levels::Error, name + ": Autotune failed KVAL out of range (" + to_string(K_VAL) + ")");
        return false;
    }
    Set_param(register_map::KVAL_HOLD, static_cast<uint32_t>(K_VAL), 8);
    Set_param(register_map::KVAL_RUN, static_cast<uint32_t>(K_VAL), 8);
    Set_param(register_map::KVAL_ACC, static_cast<uint32_t>(K_VAL), 8);
    Set_param(register_map::KVAL_DEC, static_cast<uint32_t>(K_VAL), 8);

    // NOTE: Calculation of intersect speed are little bit confusing in AN4144 (example not corelates with formulas), needs to be verified
    unsigned int intersect_speed = (4 * phase_resistance / (2 * M_PI * (phase_inductance / 1000))) * pow(2, 26) * 250 * pow(10, -9);
    if ((intersect_speed > ((1 << 14) - 1)) or (intersect_speed == 0)) {
        Log_line(Log_levels::Error, name + ": Autotune failed Intersect speed out of range (" + to_string(intersect_speed) + ")");
        return false;
    }
    Set_param(register_map::INT_SPD, static_cast<uint32_t>(intersect_speed), 14);

    unsigned int starting_slope = ((motor_electric_constant / 4.0) / motor_voltage) * pow(2, 16);
    if ((starting_slope > 255) or (starting_slope == 0)) {
        Log_line(Log_levels::Error, name + ": Autotune failed Starting slope out of range (" + to_string(starting_slope) + ")");
        return false;
    }
    Set_param(register_map::ST_SLP, static_cast<uint32_t>(starting_slope), 8);

    unsigned int final_slope = (((2 * M_PI * (phase_inductance / 1000) * target_current + motor_electric_constant) / 4.0) / motor_voltage) * pow(2, 16);
    if ((final_slope > 255) or (final_slope == 0)) {
        Log_line(Log_levels::Error, name + ": Autotune failed Final slope out of range (" + to_string(final_slope) + ")");
        return false;
    }
    Set_param(register_map::FN_SLP_ACC, static_cast<uint32_t>(final_slope), 8);
    Set_param(register_map::FN_SLP_DEC, static_cast<uint32_t>(final_slope), 8);

    Log_line(Log_levels::Notice, name + ": Autotune successfully done")
    Log_line(Log_levels::Debug, name + ": Autotune values: "
      + "KVAL = " + to_string(K_VAL) + " (0x" + dec2hex(K_VAL) + "), "
      + "Intersect speed = " + to_string(intersect_speed) + " (0x" + dec2hex(intersect_speed) + "), "
      + "Starting slope = " + to_string(starting_slope) + " (0x" + dec2hex(starting_slope) + "), "
      + "Final slope = " + to_string(final_slope) + " (0x" + dec2hex(final_slope) + ")");
    return true;
}  // L6470::Autotune

L6470::status L6470::Status(){
    Transmit(vector<uint8_t> { 0b11010000 });
    vector<uint8_t> receive1 = Receive(1);
    vector<uint8_t> receive2 = Receive(1);
    uint16_t status_register = receive2[0] << 8 | receive1[0];
    L6470::status status;
    // write status register values to packed structure
    status = *((L6470::status *) &status_register);

    return status;
}

bool L6470::Busy(){
    if (busy_pin) {
        return !busy_pin->Read();
    } else {
        return !Status().BUSY;
    }
}

bool L6470::Switch_status(){
    return Status().SW_F;
}

bool L6470::Switch_event(){
    return Status().SW_EVN;
}

void L6470::Switch_enable(){
    Set_param(register_map::CONFIG, standard_configuration, 16);
}

void L6470::Switch_disable(){
    uint16_t config = 0b0001111010010000;
    Set_param(register_map::CONFIG, config, 16);
}

void L6470::Flag_IRQ(){
    Log_line(Log_levels::Debug, name + "Flag IRQ activated");
}

void L6470::Busy_IRQ(){
    Log_line(Log_levels::Debug, name + "Busy IRQ activated");
}

string L6470::Status_formated(){
    string message     = "L6470 Status registr:\r\n";
    string new_line    = "\r\n";
    L6470::status stat = Status();

    bitset<8> b1 = ((uint8_t *) &stat)[0];
    bitset<8> b2 = ((uint8_t *) &stat)[1];

    message += "Values: ";
    message += b1.to_string();
    message += "  ";
    message += b2.to_string();
    message += new_line;

    message += "StepClock mode: ";
    stat.SCK_MOD ? message += "ENABLED" : message += "DISABLED";
    message += new_line;

    message += "Step loss B: ";
    stat.STEP_LOSS_B ? message += "OK" : message += "LOST STEP";
    message += new_line;

    message += "Step loss A: ";
    stat.STEP_LOSS_A ? message += "OK" : message += "LOST STEP";
    message += new_line;

    message += "Overcurrent detection: ";
    stat.OCD ? message += "INACTIVE" : message += "ACTIVE";
    message += new_line;

    message += "Thermal shutdown: ";
    stat.TH_SD ? message += "INACTIVE" : message += "ACTIVE";
    message += new_line;

    message += "Thermal warning: ";
    stat.TH_WRN ? message += "INACTIVE" : message += "ACTIVE";
    message += new_line;

    message += "Under voltage lockout: ";
    stat.UVLO ? message += "INACTIVE" : message += "ACTIVE";
    message += new_line;

    message += "Wrong command: ";
    stat.WRONG_CMD ? message += "WRONG" : message += "OK";
    message += new_line;
    message += new_line;

    message += "Cannot perform cmd: ";
    stat.NOTPERF_CMD ? message += "CANNOT PERF" : message += "OK";
    message += new_line;

    message += "Motor status: ";
    switch (static_cast<uint8_t>(stat.MOT_STATUS)) {
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
    static_cast<uint8_t>(stat.DIR) ? message += "Forward" : message += "Reverse";
    message += new_line;

    message += "Switch event: ";
    stat.SW_EVN ? message += "ACTIVE" : message += "INACTIVE";
    message += new_line;

    message += "Switch flag: ";
    stat.SW_F ? message += "CLOSE" : message += "OPEN";
    message += new_line;

    message += "Busy flag: ";
    stat.BUSY ? message += "IDLE" : message += "BUSY";
    message += new_line;

    message += "HiZ bridge state: ";
    stat.HIZ ? message += "ACTIVE" : message += "INACTIVE";
    message += new_line;

    message += "===========================" + new_line;
    return message;
}  // L6470::Status_formated
