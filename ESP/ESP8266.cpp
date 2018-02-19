#include "ESP8266.hpp"

// TODO add Disconnect from AP function

ESP8266::ESP8266(UART *ESP_UART_set)
    : ESP_UART(ESP_UART_set)
{ }

int ESP8266::Init(){
    Reset();
    Verbose(false);
    Auto_connect(false);
    Remote_IP(true);
    Set_mode(1);

    return 0;
}

int ESP8266::Reset(){
    Send_command("AT+RST");
    return 0;
}

int ESP8266::Test(){
    Send_command("AT");
    return 0;
}

int ESP8266::Verbose(bool level){
    Send_command("ATE" + itoa(int(level)));
    return 0;
}

int ESP8266::Auto_connect(bool state){
    Send_command("AT+CWAUTOCONN=" + itoa(int(state)));
    return 0;
}

int ESP8266::Remote_IP(bool state){
    Send_command("AT+CIPDINFO=" + itoa(int(state)));
    return 0;
}

int ESP8266::Set_mode(int mode_id){
    if (mode_id < 1 || mode_id > 3) {
        return -1;
    }
    Send_command("AT+CWMODE_CUR=" + itoa(mode_id));
    return 0;
}

int ESP8266::Connect_to_AP(){
    Send_command("AT+CWJAP_CUR=\"" + SSID + "\",\"" + password + "\"");
    IP_info();
    return 0;
}

void ESP8266::IP_info(){
    Send_command("AT+CIPSTA_CUR?");
}

void ESP8266::Set_rec_callback(void (*function)(void)){
    rec_callback = function;
}

int ESP8266::Send_command(string AT_command){
    if (AT_command.length() < 2) {
        return -1; // non existing command
    }
    command_buffer.push_back(AT_command);

    if (busy) {
        return command_buffer.size();
    } else {
        busy        = true;
        AT_command += "\r\n";
        ESP_UART->Send(AT_command);
    }
    return 0;
}

int ESP8266::Process_command(){
    if (command_buffer.front() == "AT+RST") { // Reset
        UART_1.Send("\r\nBoot DONE\r\n");
    } else if (command_buffer.front().find("ATE") == 0) { // Verbose
        UART_1.Send("ATE-OK\r\n");
    } else if (command_buffer.front() == "AT") { // Test
        UART_1.Send("AT-OK\r\n");
    } else if (command_buffer.front().find("AT+CWAUTOCONN=") == 0) { // Auto_connect
        UART_1.Send("AT+CWAUTOCONN=OK\r\n");
    } else if (command_buffer.front().find("AT+CIPDINFO=") == 0) { // Showing of remote IP
        UART_1.Send("AT+CIPDINFO=OK\r\n");
    } else if (command_buffer.front().find("AT+CWMODE_CUR=") == 0) { // Set_mode
        UART_1.Send("AT+CWMODE_CUR=OK\r\n");
    } else if (command_buffer.front().find("AT+CWJAP_CUR=") == 0) { // Connect_to_AP
        UART_1.Send("AT+CWJAP_CUR=OK\r\n");
    } else if (command_buffer.front().find("AT+CIPSTA_CUR?") == 0) { // IP_info
        Parse_IP_info(ESP_UART->UART_buffer);
    } else if (command_buffer.front().find("AT+CIPSTART=") == 0) { // Estabilish connection
        UART_1.Send("AT+CIPSTART=OK\r\n");
    } else if (command_buffer.front().find("AT+CIPCLOSE") == 0) { // Close connetion
        UART_1.Send("AT+CIPCLOSE=OK\r\n");
    } else if (command_buffer.front().find("AT+CIPSEND=") == 0) { // Send data
        command_buffer.insert(command_buffer.begin() + 1, "AT+CIPSEND>");
        UART_1.Send("AT+CIPSEND=OK\r\n");             // NOTE - Here shut be problem, wait for > from ESP8266
        ESP_UART->Clear_buffer();                     // clear receive buffer
        command_buffer.erase(command_buffer.begin()); // remove actual command
        ESP_UART->Send(data_send.front());            // send transmition data
        return 1;
    } else if (command_buffer.front().find("AT+CIPSEND>") == 0) { // Data transmited
        UART_1.Send("AT+CIPSEND>OK\r\n");
        data_send.erase(data_send.begin());
    }

    return 0;
} /* Process_command */

int ESP8266::Avaible(){
    ESP_UART->Clear_buffer();
    command_buffer.erase(command_buffer.begin());
    if (command_buffer.empty()) { // no command in buffer -> waiting for new commands
        busy = false;
        return 0;
    } else { // send command from buffer
        string command = command_buffer.front();
        ESP_UART->Send(command + "\r\n");
        busy = true;
    }
    return command_buffer.size();
}

void ESP8266::Receive(){
    if (boot) { // Boot
        int len = ESP_UART->UART_buffer.length();
        if (len < BOOT_MESSAGE_LENGHT - 5) { // booting time
            return;
        }
        string tail = ESP_UART->UART_buffer.substr(len - 7, 7);

        if (tail == "ready\r\n") {
            boot = false;
            Process_command();
            Avaible();
        }
    } else if (receiving) {
        Packet_receive();
    } else { // standart communication
        int len = ESP_UART->UART_buffer.length();
        if (len >= 4) {
            string tail = ESP_UART->UART_buffer.substr(len - 4, 4);

            if (tail == "OK\r\n") { // OK
                if (Process_command() == 0) {
                    Avaible();
                }
                return;
            }

            if (len >= 7) {
                if (ESP_UART->UART_buffer.substr(0, 7) == "\r\n+IPD,") { // Receiving
                    received_buffer.push_back(IP_packet());              // create new IP packet
                    receiving   = true;                                  // start filling packet
                    packet_part = 1;
                    return;
                }

                tail = ESP_UART->UART_buffer.substr(len - 7, 7);
                if (tail == "ERROR\r\n") { // ERROR
                    Error();
                    return;
                }
            }

            if (len >= 8) { // CLOSE
                tail = ESP_UART->UART_buffer.substr(len - 8, 8);
                if (tail == "CLOSED\r\n") {   // CLOSED
                    connection_estab = false; // connection was closed by server
                    //Pin('C', 13).Set(1);
                }
            }
        }
    }
} /* Receive */

void ESP8266::Error(){
    // Pin('C', 13).Set(1);
    UART_1.Send("ERROR\r\n");
}

void ESP8266::Set_SSID(string SSID){
    this->SSID = SSID;
}

void ESP8266::Set_password(string password){
    this->password = password;
}

int ESP8266::Packet_receive(){
    char last_char = ESP_UART->UART_buffer[ESP_UART->UART_buffer.length() - 1];

    switch (packet_part) {
        case 1: // Parse data length
            if (last_char != ',') {
                packet_temp += last_char;
            } else {
                packet_part       = 2;
                message_remaining = stoi(packet_temp);
                packet_temp       = "";
                received_buffer.back().length = message_remaining;
            }
            break;
        case 2: // Parse src IP address
            if (last_char != ',') {
                packet_temp += last_char;
            } else {
                received_buffer.back().IP_address = packet_temp;
                packet_part = 3;
                packet_temp = "";
            }
            break;
        case 3: // Parse src port
            if (last_char != ':') {
                packet_temp += last_char;
            } else {
                received_buffer.back().port = stoi(packet_temp);
                packet_part = 4;
                packet_temp = "";
            }
            break;
        case 4: // Parsing data
            message_remaining--;
            packet_temp += last_char;
            if (message_remaining == 0) {   //message complete
                received_buffer.back().data = packet_temp;
                packet_temp = "";
                packet_part = 0;
                receiving   = false;
                ESP_UART->Clear_buffer();
                if (rec_callback) { //if is receive callback defined call it
                    (*rec_callback)();
                }
            }
            break;
    }
    return 0;
} /* Packet_receive */

string ESP8266::Info(){
    string message;
    message += ip_address + "\r\n";
    message += gateway + "\r\n";
    message += mask + "\r\n";
    message += broadcast + "\r\n";

    return message;
}

int ESP8266::Parse_IP_info(string answer){
    string value;
    bool read = false;
    int param = 0;
    for (auto c : answer) {
        if (c == '"') {
            read = !read;
            if (read) {
                param++;
                value = "";
            } else {
                switch (param) {
                    case 1:
                        ip_address = value;
                        break;
                    case 2:
                        gateway = value;
                        break;
                    case 3:
                        mask = value;
                        break;
                }
            }
            continue;
        }
        if (read) {
            value += c;
        }
    }
    UART_1.Send(Info());
    return 0;
} /* Parse_IP_info */

int ESP8266::Estab_TCP(string IP, int port){
    if (connection_estab) {
        return -1;
    }
    Send_command("AT+CIPSTART=\"TCP\",\"" + IP + "\"," + itoa(port));
    connection_estab = true;
    return 0;
}

int ESP8266::Estab_UDP(string IP, int port){
    if (connection_estab) {
        return -1;
    }
    Send_command("AT+CIPSTART=\"UDP\",\"" + IP + "\"," + itoa(port));
    connection_estab = true;
    return 0;
}

int ESP8266::Send(string data){
    if (!connection_estab) {
        return -1;
    }
    data_send.push_back(data);
    Send_command("AT+CIPSEND=" + itoa(data.length()));
    return data_send.size();
}

int ESP8266::Close_connection(){
    if (!connection_estab) {
        return -1;
    }
    Send_command("AT+CIPCLOSE");
    connection_estab = false;
    return 0;
}
