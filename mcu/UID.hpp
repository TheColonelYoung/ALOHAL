/*
 * Author:	TCY
 * Name:	UID STM32 library
 * E-mail:	TheColonelYoung(at)gmail(dot)com
 * Date:	6.2.2018
 * Description:
 *
 * ????????????????????HELP????????????????????
 *
 * !!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!
 *
 * --------------------BUGS--------------------
 *
 * ++++++++++++++++++++TODO++++++++++++++++++++
 *
 **************************/
#ifndef UID_H
#define UID_H

#include "global_includes.hpp"
#include <string>
#include <vector>

using namespace std;

// classes
namespace UID {
    extern string UID_string;

    void Init();
    string String(int word = -1);   //return UID as string, in default return whole UID
};

#endif
