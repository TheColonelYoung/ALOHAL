#include "text_transform.hpp"

void ftoa(float f, char *buf){
    int pos = 0, ix, dp, num;
    if (f < 0) {
        buf[pos++] = '-';
        f = -f;
    }
    dp = 0;
    while (f >= 10.0) {
        f = f / 10.0;
        dp++;
    }
    for (ix = 1; ix < 9; ix++) {
        num = f;
        f   = f - num;
        if (num > 9)
            buf[pos++] = '#';
        else
            buf[pos++] = '0' + num;
        if (dp == 0) buf[pos++] = '.';
        f = f * 10.0;
        dp--;
    }
}

int stoi(string s_val){
    int value = 0;
    int rank  = 1;

    for (int i = (s_val.length() - 1); i >= 0; i--) {
        value += (s_val[i] - 48) * rank;
        rank  *= 10;
    }
    return value;
}

string itoa(int a){
    char buffer[32];
    sprintf(buffer, "%d", a);
    return string(buffer);
}

string to_string(int number){
    char buffer[32];
    sprintf(buffer, "%d", number);
    return string(buffer);
}

string to_string(uint number){
    return to_string((int)number);
}

string to_string(float number){
    char buffer[10];
    int pos = 0, ix, dp, num;
    if (number < 0) {
        buffer[pos++] = '-';
        number = -number;
    }
    dp = 0;
    while (number >= 10.0) {
        number = number / 10.0;
        dp++;
    }
    for (ix = 1; ix < 9; ix++) {
        num = number;
        number   = number - num;
        if (num > 9)
            buffer[pos++] = '#';
        else
            buffer[pos++] = '0' + num;
        if (dp == 0) buffer[pos++] = '.';
        number = number * 10.0;
        dp--;
    }
    return string(buffer);
}

string to_string(double number){
    return to_string((float)number);
}
