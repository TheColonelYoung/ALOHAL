#ifndef TEXT_TRANSFORM_H
#define TEXT_TRANSFORM_H

#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

#include "gpio/pin.hpp"

void ftoa(float f,char *buf);
string itoa(int a);

int stoi(string s_val);

string to_string(int number);
string to_string(uint number);
string to_string(float number);
string to_string(double number);





















#endif
