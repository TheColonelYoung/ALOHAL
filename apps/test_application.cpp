#include "test_application.hpp"

int Test_application::Init(){
    // None
    return 0;
}

int Test_application::Run(vector<string> args){
    if (device()->CLI_available()){
        device()->cli->Print("Test of example application in progress...\r\nDone successfully\r\n");
        return 0;
    }
    return 1;
}
