#include "test_application_thread.hpp"

int Test_application_thread::Init(){
    // Nothing
    return 0;
}

int Test_application_thread::Thread(vector<string> &args){
    if (device()->CLI_available()) {
        string text;
        text += "Test of example application in progress...\r\nArguments: \r\n";
        for (auto &arg : args) {
            text += arg + "\r\n";
        }
        text += "Done successfully\r\n";
        device()->cli->Print(text);
        return 0;
    }
    return 1;
}

int Test_application_thread::Input_load(string input){
    // Nothing
    return 0;
}
