#include "test_application.hpp"

int Test_application::Init(){
    // Nothing
    return 0;
}

int Test_application::Run(vector<string> &args){
    if (device()->CLI_available()){
        device()->cli->Print("Test of example application in progress...\r\nArguments: \r\n");
        for( auto &arg : args){
            device()->cli->Print(arg + "\r\n");
        }
        device()->cli->Print("Done successfully\r\n");
        return 0;
    }
    return 1;
}
