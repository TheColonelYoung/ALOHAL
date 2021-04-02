#include "eyrina.hpp"

Eyrina::Eyrina() :
    Application("eyrina")
{ }

int Eyrina::Run(vector<string> &args){
    Log(Log_levels::Debug, "Eyrina Run");
    if (args.size() > 0) {
        args.erase(args.begin());
    }
    Parse(args);
    return 0;
}

int Eyrina::Init(){
    Log(Log_levels::Debug, "Eyrina Init");
    Init_motors();
    return 0;
}

int Eyrina::Input_load(string input){
    command_buffer.push(input);
    return command_buffer.size();
}

int Eyrina::Load_command(){
    if (blocked_queue) {
        return -1;
    }

    string command = command_buffer.front();

    size_t position;
    vector<string> gcode;

    while ((position = command.find(" ")) != string::npos) {
        string cmd = command.substr(0, position);
        if (cmd.find_first_not_of(' ') != std::string::npos) {
            gcode.emplace_back(cmd);
        }
        command.erase(0, position + 1);
    }
    gcode.emplace_back(command);

    Parse(gcode);

    command_buffer.pop();
    return command_buffer.size();
}

int Eyrina::Parse(vector<string> &gcode){
    string command = "";

    for (auto &section:gcode) {
        command += section + " ";
    }
    Log(Log_levels::Debug, "Command: " + command);

    // Save command name and removes it from arguments
    string command_tag = gcode[0];
    if (gcode.size() > 0) {
        gcode.erase(gcode.begin());
    }

    // Create parameters and flags from command arguments
    vector<char> flags;
    map<char, double> params;

    for (auto &param:gcode) {
        if (param.length() == 1) {
            // Flags F
            flags.emplace_back(param[0]);
            Log(Log_levels::Debug, param.substr(0, 1));
        } else {
            // Params X-1.2
            double param_value = stod(param.substr(1, param.length() - 1));
            params.emplace(param[0], param_value);
            Log(Log_levels::Debug, param.substr(0, 1) + ":" + to_string(stod(param.substr(1, param.length() - 1))));
        }
    }

    // Execute validation and receive pointer to g-code execution method
    const gcode_settings *g_code_method = nullptr;

    int ret_code = Validation(command_tag, params, flags, g_code_method);

    if (ret_code) {
        // Invalid command
        return ret_code;
    } else {
        // Execute method of g-code from command settings
        return (*this.*(*g_code_method).method)(params, flags);
    }
}  // Eyrina::Parse

int Eyrina::Validation(string &command, map<char, double> &params, vector<char> &flags, const gcode_settings *&g_code_method){
    // Checks if commands tag exists in defined commands
    auto iterator = find_if(g_code_commands.begin(), g_code_commands.end(),
        [&command] (const pair<string, gcode_settings> &g_code_command){
        return g_code_command.first == command;
    }
      );

    if (iterator == g_code_commands.end()) {
        // Command does not exists
        Log(Log_levels::Debug, "Invalid command");
        return -1;
    }

    const gcode_settings gcode_structure = (*iterator).second;

    // Check if command has valid parameters
    for (auto &param:params) {
        auto position = find(gcode_structure.params.begin(), gcode_structure.params.end(), param.first);
        if (position == gcode_structure.params.end()) {
            // Param does not exist
            Log(Log_levels::Debug, "Invalid parameter");
            return -2;
        }
    }

    // Check if command has valid flags
    for (auto &flag:flags) {
        auto position = find(gcode_structure.flags.begin(), gcode_structure.flags.end(), flag);
        if (position == gcode_structure.flags.end()) {
            // Flag does not exist
            Log(Log_levels::Debug, "Invalid flag");
            return -3;
        }
    }

    Log(Log_levels::Debug, "Command is valid");

    g_code_method = &gcode_structure;

    return 0;
}  // Eyrina::Validation

int Eyrina::Add_axis(char axis_name, Motion_axis *new_axis){
    if (axis.count(axis_name)) {
        Log(Log_levels::Error, "Axis with this identifier already exists");
        return -1;
    }
    axis.insert({ axis_name, new_axis });
    return 0;
}

int Eyrina::G_code_G0(map<char, double> &params, vector<char> &flags){
    int moving_axis = 0;
    for (auto &axis_command:params) {
        // First is tag of axis, second is distance to move
        if (axis.find(axis_command.first) != axis.end()) {
            Log(Log_levels::Notice, string("Moving axis ") + axis_command.first);
            axis[axis_command.first]->Move(axis_command.second);
            moving_axis += 1;
        }
    }
    return moving_axis;
}

int Eyrina::G_code_G1(map<char, double> &params, vector<char> &flags){
    int moving_axis = 0;
    for (auto &axis_command:params) {
        // First is tag of axis, second is distance to move
        if (axis.find(axis_command.first) != axis.end()) {
            Log(Log_levels::Notice, string("Shifting axis ") + axis_command.first);
            axis[axis_command.first]->Move(axis_command.second);
            moving_axis += 1;
        }
    }
    return moving_axis;
}

int Eyrina::G_code_G28(map<char, double> &params, vector<char> &flags){
    if (flags.size() == 0) {
        Log(Log_levels::Notice, "Homing all axis");
        for (auto &elem:axis) {
            elem.second->GoHome();
        }
        return axis.size();
    } else {
        int moving_axis = 0;
        for (auto &axis_label:flags) {
            if (axis.find(axis_label) != axis.end()) {
                Log(Log_levels::Notice, "Homing axis " + axis_label);
                axis[axis_label]->GoHome();
                moving_axis += 1;
            }
        }
        return moving_axis;
    }
}

int Eyrina::G_code_M0(map<char, double> &params, vector<char> &flags){
    if (flags.size() == 0) {
        Log(Log_levels::Notice, "Soft stoping all axis");
        for (auto &elem:axis) {
            elem.second->GoHome();
        }
        return axis.size();
    } else {
        int moving_axis = 0;
        for (auto &axis_label:flags) {
            if (axis.find(axis_label) != axis.end()) {
                Log(Log_levels::Notice, string("Soft stoping axis ") + axis_label);
                axis[axis_label]->Sleep();
                moving_axis += 1;
            }
        }
        return moving_axis;
    }
}

int Eyrina::G_code_M10(map<char, double> &params, vector<char> &flags){
    if (flags.size() == 0) {
        Log(Log_levels::Notice, "Disabling all axis");
        for (auto &elem:axis) {
            elem.second->GoHome();
        }
        return axis.size();
    } else {
        int moving_axis = 0;
        for (auto &axis_label:flags) {
            if (axis.find(axis_label) != axis.end()) {
                Log(Log_levels::Notice, string("Disabling axis ") + axis_label);
                axis[axis_label]->Sleep();
                moving_axis += 1;
            }
        }
        return moving_axis;
    }
}

int Eyrina::G_code_E1(map<char, double> &params, vector<char> &flags){
    if ((params.at('C') > 3) | (params.at('C') < 0)) {
        return -1;
    }

    if ((params.at('I') > 100) | (params.at('I') < 0)) {
        return -1;
    }

    Log(Log_levels::Notice, "Setting LED channel " + to_string(params.at('C')) + " to intensity " + to_string(params.at('I')) );

    led_channels[static_cast<int>(params.at('C')) - 1]->Power(params.at('I'));
    return 0;
}

int Eyrina::G_code_E2(map<char, double> &params, vector<char> &flags){
    if (display_status) {
        display_status = false;
        display->Off();
    } else {
        display_status = true;
        display->On();
    }
    return 0;
}

int Eyrina::G_code_E3(map<char, double> &params, vector<char> &flags){
    if ((params.at('X') > 5) | (params.at('X') < 1)) {
        return -1;
    }

    if ((params.at('Y') > 5) | (params.at('Y') < 1)) {
        return -1;
    }

    const int base_position_x = 33;
    const int base_position_y = 1;

    const int step = 15;

    int position_x = params.at('X') - 1 * step + base_position_x;
    int position_y = params.at('Y') - 1 * step + base_position_y;

    display->Put(position_x, position_y);
    display->Put(position_x, position_y + 1);
    display->Put(position_x + 1, position_y);
    display->Put(position_x + 1, position_y + 1);

    return params.at('Y') * 5 + params.at('X');
}

int Eyrina::G_code_R0(map<char, double> &params, vector<char> &flags){
    Log(Log_levels::Notice, "Emergency stop");
    for (auto &elem:axis) {
        elem.second->Hard_stop();
    }
    return axis.size();
}

int Eyrina::G_code_R10(map<char, double> &params, vector<char> &flags){
    blocked_queue = false;
    return 0;
}

void Eyrina::Init_light(){
    led_channels[0] = new NCL30160(&(device()->mcu->TIM_2->channel[0]), 50000, 50000);
    led_channels[1] = new NCL30160(&(device()->mcu->TIM_2->channel[2]), 50000, 50000);
    led_channels[2] = new NCL30160(&(device()->mcu->TIM_2->channel[1]), 50000, 50000);

    SSD1306 *oled = new SSD1306(128, 64, *(device()->mcu->I2C_1), static_cast<unsigned char>(0b01111000));

    oled->Init();
    oled->Off();

    oled->Set_contrast(0xff);
    oled->Clear_all();
}

void Eyrina::Init_motors(){
    Log(Log_levels::Debug, "Eyrina motor initialization");

    // Define I2C GPIO expanders
    MCP23017 *mcp_A = new MCP23017(*(device()->mcu->I2C_1), 0b01000000);
    MCP23017 *mcp_B = new MCP23017(*(device()->mcu->I2C_1), 0b01001000);

    // Init GPIO expanders
    mcp_A->Init();
    mcp_B->Init();

    // Configure pins determinated as Chip select as output
    mcp_A->Direction(12, 0);
    mcp_A->Direction(8, 0);
    mcp_A->Direction(4, 0);
    mcp_A->Direction(0, 0);

    mcp_B->Direction(12, 0);
    mcp_B->Direction(8, 0);
    mcp_B->Direction(4, 0);
    mcp_B->Direction(0, 0);

    // Set Chip select pins to High logic state
    mcp_A->Set(12, 1);
    mcp_A->Set(8, 1);
    mcp_A->Set(4, 1);
    mcp_A->Set(0, 1);

    mcp_B->Set(12, 1);
    mcp_B->Set(8, 1);
    mcp_B->Set(4, 1);
    mcp_B->Set(0, 1);

    // Configuration of pins on GPIO expander to fit SPI chip select definition
    Pin_MCP23017 *mpc_A1 = new Pin_MCP23017(*mcp_A, 12);
    Pin_MCP23017 *mpc_A2 = new Pin_MCP23017(*mcp_A, 8);
    Pin_MCP23017 *mpc_A3 = new Pin_MCP23017(*mcp_A, 4);
    Pin_MCP23017 *mpc_A4 = new Pin_MCP23017(*mcp_A, 0);

    Pin_MCP23017 *mpc_B1 = new Pin_MCP23017(*mcp_B, 12);
    Pin_MCP23017 *mpc_B2 = new Pin_MCP23017(*mcp_B, 8);
    Pin_MCP23017 *mpc_B3 = new Pin_MCP23017(*mcp_B, 4);
    Pin_MCP23017 *mpc_B4 = new Pin_MCP23017(*mcp_B, 0);

    // Define stepper motor drivers, SPI and Chip select pin from GPIO expander
    stepper_drivers.emplace_back(new L6470(*(device()->mcu->SPI_2), mpc_A1));
    stepper_drivers.emplace_back(new L6470(*(device()->mcu->SPI_2), mpc_A2));
    stepper_drivers.emplace_back(new L6470(*(device()->mcu->SPI_2), mpc_A3));
    stepper_drivers.emplace_back(new L6470(*(device()->mcu->SPI_2), mpc_A4));

    stepper_drivers.emplace_back(new L6470(*(device()->mcu->SPI_2), mpc_B1));
    stepper_drivers.emplace_back(new L6470(*(device()->mcu->SPI_2), mpc_B2));
    stepper_drivers.emplace_back(new L6470(*(device()->mcu->SPI_2), mpc_B3));
    stepper_drivers.emplace_back(new L6470(*(device()->mcu->SPI_2), mpc_B4));

    for(auto &motor:stepper_drivers){
        motor->Status();
        motor->Reset();
    }
    for(auto &motor:stepper_drivers){
        HAL_Delay(10);
        motor->Status();
    }
    for(auto &motor:stepper_drivers){
        motor->Init();
    }


    // MOTOR Z - 1

    int M1_uSteps = 32;
    stepper_drivers[0]->Set_microsteps(M1_uSteps);
    stepper_drivers[0]->Set_acceleration(1000);
    stepper_drivers[0]->Set_deceleration(1200);
    stepper_drivers[0]->Set_max_speed(2400);
    stepper_drivers[0]->Set_min_speed(200);

    uint8_t M1_KVAL_VALUE = 0x4;
    stepper_drivers[0]->Set_param(L6470::register_map::KVAL_RUN, M1_KVAL_VALUE, 8);
    stepper_drivers[0]->Set_param(L6470::register_map::KVAL_ACC, M1_KVAL_VALUE, 8);
    stepper_drivers[0]->Set_param(L6470::register_map::KVAL_DEC, M1_KVAL_VALUE, 8);
    stepper_drivers[0]->Set_param(L6470::register_map::KVAL_HOLD, 0x01, 8);

    Motion_axis Z = Motion_axis(stepper_drivers[0], 0.00125 / M1_uSteps);
    Z.Reverse(true);

    Add_axis('Z', &Z);


    // MOTOR T - 2

    int M2_uSteps = 32;
    stepper_drivers[1]->Set_microsteps(M2_uSteps);
    stepper_drivers[1]->Set_acceleration(800);
    stepper_drivers[1]->Set_deceleration(1000);
    stepper_drivers[1]->Set_max_speed(2200);
    stepper_drivers[1]->Set_min_speed(400);

    uint8_t M2_KVAL_VALUE = 0x37;
    stepper_drivers[1]->Set_param(L6470::register_map::KVAL_RUN, M2_KVAL_VALUE, 8);
    stepper_drivers[1]->Set_param(L6470::register_map::KVAL_ACC, M2_KVAL_VALUE, 8);
    stepper_drivers[1]->Set_param(L6470::register_map::KVAL_DEC, M2_KVAL_VALUE, 8);
    stepper_drivers[1]->Set_param(L6470::register_map::KVAL_HOLD, 0x01, 8);

    Motion_axis T = Motion_axis(stepper_drivers[1], 0.003 / M2_uSteps);
    T.Reverse(true);

    Add_axis('T', &T);


    // MOTOR X - 3

    int M3_uSteps = 32;
    stepper_drivers[2]->Set_microsteps(M3_uSteps);
    stepper_drivers[2]->Set_acceleration(1200);
    stepper_drivers[2]->Set_deceleration(1500);
    stepper_drivers[2]->Set_max_speed(2400);
    stepper_drivers[2]->Set_min_speed(400);

    uint8_t M3_KVAL_VALUE = 0x40;
    stepper_drivers[2]->Set_param(L6470::register_map::KVAL_RUN, M3_KVAL_VALUE, 8);
    stepper_drivers[2]->Set_param(L6470::register_map::KVAL_ACC, M3_KVAL_VALUE, 8);
    stepper_drivers[2]->Set_param(L6470::register_map::KVAL_DEC, M3_KVAL_VALUE, 8);
    stepper_drivers[2]->Set_param(L6470::register_map::KVAL_HOLD, 0x01, 8);

    stepper_drivers[2]->Set_param(L6470::register_map::FS_SPD, 0x3ff, 10);
    stepper_drivers[2]->Set_param(L6470::register_map::ST_SLP, 0x00, 8);
    stepper_drivers[2]->Set_param(L6470::register_map::FN_SLP_ACC, 0x00, 8);
    stepper_drivers[2]->Set_param(L6470::register_map::FN_SLP_DEC, 0x00, 8);

    Motion_axis *X = new Motion_axis(stepper_drivers[2], 0.0025 / M3_uSteps);
    X->Reverse(true);

    Add_axis('X', X);

    // MOTOR Y

    int M4_uSteps = 32;
    stepper_drivers[3]->Set_microsteps(M4_uSteps);
    stepper_drivers[3]->Set_acceleration(1200);
    stepper_drivers[3]->Set_deceleration(1500);
    stepper_drivers[3]->Set_max_speed(2600);
    stepper_drivers[3]->Set_min_speed(400);

    uint8_t M4_KVAL_VALUE = 0x40;
    stepper_drivers[3]->Set_param(L6470::register_map::KVAL_RUN, M4_KVAL_VALUE, 8);
    stepper_drivers[3]->Set_param(L6470::register_map::KVAL_ACC, M4_KVAL_VALUE, 8);
    stepper_drivers[3]->Set_param(L6470::register_map::KVAL_DEC, M4_KVAL_VALUE, 8);
    stepper_drivers[3]->Set_param(L6470::register_map::KVAL_HOLD, 0x01, 8);

    stepper_drivers[3]->Set_param(L6470::register_map::FS_SPD, 0x3ff, 10);
    stepper_drivers[3]->Set_param(L6470::register_map::ST_SLP, 0x00, 8);
    stepper_drivers[3]->Set_param(L6470::register_map::FN_SLP_ACC, 0x00, 8);
    stepper_drivers[3]->Set_param(L6470::register_map::FN_SLP_DEC, 0x00, 8);

    Motion_axis Y = Motion_axis(stepper_drivers[3], 0.0025 / M4_uSteps);

    Add_axis('Y', &Y);

    // MOTOR R - 5

    int M5_uSteps = 32;
    stepper_drivers[4]->Set_microsteps(M5_uSteps);
    stepper_drivers[4]->Set_acceleration(1200);
    stepper_drivers[4]->Set_deceleration(2000);
    stepper_drivers[4]->Set_max_speed(2500);
    stepper_drivers[4]->Set_min_speed(800);

    uint8_t M5_KVAL_VALUE = 0x39;
    stepper_drivers[4]->Set_param(L6470::register_map::KVAL_RUN, M5_KVAL_VALUE, 8);
    stepper_drivers[4]->Set_param(L6470::register_map::KVAL_ACC, M5_KVAL_VALUE, 8);
    stepper_drivers[4]->Set_param(L6470::register_map::KVAL_DEC, M5_KVAL_VALUE, 8);
    stepper_drivers[4]->Set_param(L6470::register_map::KVAL_HOLD, 0x01, 8);

    stepper_drivers[4]->Set_param(L6470::register_map::FS_SPD, 0x3ff, 10);
    stepper_drivers[4]->Set_param(L6470::register_map::ST_SLP, 0x00, 8);
    stepper_drivers[4]->Set_param(L6470::register_map::FN_SLP_ACC, 0x00, 8);
    stepper_drivers[4]->Set_param(L6470::register_map::FN_SLP_DEC, 0x00, 8);

    Motion_axis R = Motion_axis(stepper_drivers[4], 0.01 / M5_uSteps);

    Add_axis('R', &R);


    // MOTOR F - 6

    int M6_uSteps = 32;
    stepper_drivers[5]->Set_microsteps(M6_uSteps);
    stepper_drivers[5]->Set_acceleration(1000);
    stepper_drivers[5]->Set_deceleration(2000);
    stepper_drivers[5]->Set_max_speed(800);
    stepper_drivers[5]->Set_min_speed(20);

    uint8_t M6_KVAL_VALUE = 0x70;
    stepper_drivers[5]->Set_param(L6470::register_map::KVAL_RUN, M6_KVAL_VALUE, 8);
    stepper_drivers[5]->Set_param(L6470::register_map::KVAL_ACC, M6_KVAL_VALUE, 8);
    stepper_drivers[5]->Set_param(L6470::register_map::KVAL_DEC, M6_KVAL_VALUE, 8);
    stepper_drivers[5]->Set_param(L6470::register_map::KVAL_HOLD, 0x01, 8);

    Motion_axis F = Motion_axis(stepper_drivers[5], 0.01 / M6_uSteps);

    Add_axis('F', &F);
} // Eyrina::Init_motors
