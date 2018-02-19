#include "TSHS.hpp"

TSHS::TSHS(ESP8266 *ESP)
    : ESP(ESP){ }


int TSHS::Init(){
    ESP->Init();
    ESP->Set_rec_callback(&(this->Receive_callback));
    Connect_to_network("wifi_is_coming", "ifyouplaythegameofthronesyouwinordie");
    ESP->Estab_UDP("brodcast", 2008);
    ESP->Send("TSHS");
    return 0;
}

void TSHS::Receive_callback(){
    Pin('C', 14).Toggle();
}

void TSHS::Connect_to_network(string SSID, string password){
    ESP->Set_SSID(SSID);
    ESP->Set_password(password);
    ESP->Connect_to_AP();
}

int TSHS::Disconnect(){
    // TODO Need to be done in ESP8266.hpp
    return -95;
}
