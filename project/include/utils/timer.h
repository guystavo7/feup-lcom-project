#ifndef TIMER
#define TIMER

#pragma once
typedef struct Timer Timer;

#include <stdlib.h>
#include <stdbool.h>

Timer* newTimer(long long int time); //initialTime=time = initial value of the counter
void freeTimer(Timer* timer);

long long int timerGetInitialTime(Timer* timer); 
void timerSetInitialTime(Timer* timer,long long int initialTime);

void timerStart(Timer* timer); // enable the timer counter if it was stopped (by timerStop)
void timerRestart(Timer* timer); // counter gets its initial time and timer is running

void timerStop(Timer* timer);
bool timerIsRunning(Timer* timer); // timer is running if (it is not stopped) and (counter did not reach 0) 

void timerUpdate(Timer* timer,long long int deltaTime); //decreases the timer

#endif 
