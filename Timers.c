#include "Timers.h"

void InitTimer(void){
    
  TmrBaseCounter = TBASE_100MS;
          
  for(uint8_t i = 0; i < N_TIMERS; i++) {
      
   Timer[i].counter = 0;
   Timer[i].flags.on = 0;
   Timer[i].flags.expired = 1;
  }
  
}

void TimerMgr(void) {
    
   if (--TmrBaseCounter) 
     return;
    
   TmrBaseCounter = TBASE_100MS;     
   
   for(uint8_t i = 0; i < N_TIMERS; i++) {
       
     if (Timer[i].flags.on) { 
         
        if (--Timer[i].counter)
            continue;
        
        Timer[i].flags.on = 0;
        Timer[i].flags.expired = 1;   
      }
   }
   
}
 
void LoadTimer(uint8_t index,uint16_t Value) {
    
 Timer[index].counter = Value;
 Timer[index].flags.on = 1;
 Timer[index].flags.expired = 0;
 
}

void ClearTimer(uint8_t index) {
   
 Timer[index].counter = 0;
 Timer[index].flags.on = 0;
 Timer[index].flags.expired = 0;
 
}

void ResumeTimer(uint8_t index) { 
    if (TimerPaused(index)) 
     Timer[index].flags.on = 1;
}

void PauseTimer(uint8_t index){ 
    Timer[index].flags.on = 0;
}

uint8_t TimerExpired(uint8_t index) {
    return Timer[index].flags.expired;
}
