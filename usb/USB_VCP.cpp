#include "USB_VCP.hpp"

char USB_VCP_TX_buffer[USB_BUFFER_SIZE];
char USB_VCP_RX_buffer[USB_BUFFER_SIZE];

vector<string> USB_VCP_rec;


void USB_VCP_send(char *data){
	std::string temp_s(data);
	USB_VCP_send(temp_s);
}

void USB_VCP_send(double number){

	char buffer[64] = "0";
	ftoa(number,buffer);
	USB_VCP_send(std::string(buffer));
}

void USB_VCP_send(int number){
	char buffer[32];
	sprintf(buffer,"%d",number);
	USB_VCP_send(std::string(buffer));
}

void USB_VCP_send(long number){
	USB_VCP_send((int)number);
}

void USB_VCP_send(std::string data){
	strcpy(USB_VCP_TX_buffer,data.c_str());
	CDC_Transmit_FS(((uint8_t *)USB_VCP_TX_buffer), data.length());
}

void USB_VCP_receive(){
	/*
	if(USB_VCP_RX_buffer[0]==32){
		pin_tog_m(C,14);
	}
	*/
	USB_VCP_rec.push_back(string(USB_VCP_RX_buffer));
	//USB_VCP_send((int)USB_VCP_rec[(int)USB_VCP_rec.size()-1].length());
	/*
	if(USB_VCP_rec[(int)USB_VCP_rec.size()-1] == "HELLO STM32"){
		USB_VCP_send("HELLO PC");
	}*/

	//USB_VCP_send(USB_VCP_rec[(int)USB_VCP_rec.size()-1]);
}

string USB_VCP_read(){
	return USB_VCP_rec[(int)USB_VCP_rec.size()-1];
}
