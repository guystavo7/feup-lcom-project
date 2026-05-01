
#include "../../include/utils/timer.h"

struct timer 
{
  long long int initialTime;
  long long int counter;
  bool stopped;
} ;

typedef struct timer Timer;


Timer* timerCreate(long long int time){
  Timer* timer= malloc(sizeof(Timer));
  timer->initialTime=time;
  timer->counter=time;
  timer->stopped=true;
}

long long int timerGetInitialTime(Timer* timer){
  return timer->initialTime;
}

void timerSetInitialTime(Timer* timer,long long int initialTime){
  timer->initialTime=initialTime;
}

void timerFree(Timer* timer){
  free(timer);
}
void timerStart(Timer* timer){
  timer->stopped=false;
}
void timerRestart(Timer* timer){
  timer->stopped=false;
  timer->counter=timer->initialTime;
}
void timerStop(Timer* timer){
  timer->stopped=true;
}
bool timerIsRunning(Timer* timer){
  return (timer->counter>0) && (!timer->stopped);
}
void timerUpdate(Timer* timer,long long int deltaTime){
  if(!timer->stopped){
    timer->counter-= deltaTime;
  }
  
}
 