/*
 * Author:	Petr Malanik (TCY)
 * Name:	Tasker for STM32
 * E-mail:	Malanik(dot)Petr(dot)gm(at)gmail(dot)com
 * Date:	1.7.2017
 * Description:
 *
 * ????????????????????HELP????????????????????
 *
 * In CubeMX enable timer and define handler below
 *
 * In main.c create global tasker:
 * Tasker tasker = Tasker();
 *
 * In main function start Tasker by:
 * tasker.Start();
 * -this automaticly start timer
 *
 * Into timers.c to Timer_IT_PeriodElapsedCallback insert:
 *
 * if(htim->Instance == ??TASKER_TIMER?? ){	//Tasker
 *  Tasker_timer_IT();
 * }
 *
 * !!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!
 * dont use Tasker timer for anything else
 * create only one tasker
 *
 * --------------------BUGS--------------------
 *
 * ++++++++++++++++++++TODO++++++++++++++++++++
 *
 **********************************************/
#ifndef TASKER_H
#define TASKER_H

// c++ libraries
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

// libraries needed for this library to run
#ifdef STM32_F0
# include "stm32f0xx_hal.h"
#elif STM32_F1
# include "stm32f1xx_hal.h"
#elif STM32_F3
# include "stm32f3xx_hal.h"
#elif STM32_F4
# include "stm32f4xx_hal.h"
#elif STM32_F7
# include "stm32f7xx_hal.h"
#endif

#include "timer/timers.hpp"
#include "misc/text_transform.hpp"

// libraries nedded for function of tasker events and DEBUG
#include "gpio/pin.h"
#include "uart/uart.h"

//#include "USB_VCP.h"

// define used timer handler
#define TASKER_TIMER      htim2
#define TASKER_TIMER_SIZE 16 // 16 or 32 bits

// class declaration
class Event;
class Tasker;

// extern of timer handler and tasker
extern TIM_HandleTypeDef TASKER_TIMER;
extern Tasker tasker;
extern UART_HandleTypeDef huart2;

using namespace std;

// event function prototypes
void Tasker_timer_IT();
void flip();
void flip2();
void send();
void timsend();


class Tasker {
	vector<Event> events;                      // vector of tasks
	TIM_HandleTypeDef * timer = &TASKER_TIMER; // timer which is used by tasker
	int number_of_events      = 0;             // number of actual taks in tasker

	string _actual_event = "None";

	bool tasker_state = false; // 0 - tasker stopped, 1 - tasker running
	int free_ID       = 0;     // Id of event original for all run of program
	int overflow      = 0;     // the number of timer overflow
	long delay_time   = -1;   // time delayed in actual time space



public:

	Tasker(); // constructor

	int Start();  // run tasker and timer
	int Stop();   // stop tasker and timer
	void Update(); // run time update, read value from timer, descrease remaining time of events by this time,

	int Create_event(string name, int time, string unit, int repeat, void (* function)(void), int priority = 0); // append new event
	int Remove_event(string event);                                                                              // delete event

	int Handle_event(); // handle first event and update next event's time
	int Event_count();  // return number of events

	int Get_freeID();   // get new ID for new event
	void Send_events(); // send/print all name of events

	int Pause_event(string event_name);                                   // stop existing event now
	int Resume_event(string event_name, int event_repeat, int delay_us = 0); // start existing event, can by launched with delay

	int Update_event();                        // update events parameters
	void Sync_events(string base, string sync); // synchronize events

	string Get_actual_event();	//return actual event name

private:
	int _Event_index(Event tmp_event); // return index of given event in events vector
	void _Set_IT(long time);           // set timer for next interrupt
	void _Update_time_now();           // update time for every event, usage for creating and chaging event data
};

class Event {
public:
	int id;      // event ID
	string name; // name of event

	long time;           // time between launching event (in us)
	long remaining_time; // time for next event start (in us) same as repeat

	int repeat; // the number of event repeats
	            // -1	to infinity
	            // 0	stopped
	            // +x	number of repeats

	void (* function)(void);

	int priority; // value of priority 0-10, 0 - no priority, 10 - max priority

	Event(string name, long time, int repeat, void(*function) (void), int priority); // constructor
};


#endif // ifndef TASKER_H
