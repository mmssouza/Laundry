#include "cycle.h"
#include "Timers.h"
#include "IO.h"

void PreWash(void){
    static uint8_t init_state = 1;
    switch (WashingPhaseStep) {
        case FILLING:
            WaterValve_ON();
            if (init_state) {
                LoadTimer(FILL_DRAIN_TIMER,T_1MIN*5);
                init_state = 0;
            } else {
                if (TimerExpired(FILL_DRAIN_TIMER)) {
                  __asm("nop\n");
                }
                if (PR_LO == ON)  {
                 WaterValve_OFF();
                 LoadTimer(WASH_TIMER,T_1SEC*15);
                 StrokeOn(280,320);
                 WashingPhaseStep = WASHING;
                 init_state = 1;
             }
            }
            break;
            
        case WASHING:            
            if (TimerExpired(WASH_TIMER)) {
                StrokeOff();
                WashingPhaseStep = DRAINING;
            }
            break;
        case DRAINING:
            DrainPump_ON();
            if (init_state) {
                LoadTimer(FILL_DRAIN_TIMER,T_1MIN*5);
                init_state = 0;
            } else {
                if (TimerExpired(FILL_DRAIN_TIMER)) {
                  __asm("nop\n");
                }
                if (PR_LO == OFF)  {
                 LoadTimer(WASH_TIMER,T_1SEC*15);
                 WashingPhaseStep = SPINNING;
                 init_state = 1;
                }
             }
            break;
        case SPINNING:
            DrainPump_ON();
            if (init_state) {
                if (TimerExpired(WASH_TIMER)) {
                 MtCW_ON();
                 LoadTimer(WASH_TIMER,T_1SEC*10);
                 init_state = 0;
                }   
            } else {
                MtCW_ON();
                if (TimerExpired(WASH_TIMER)) {
                 Mt_OFF();
                 LoadTimer(WASH_TIMER,T_1SEC*30);
                 WashingPhaseStep = END_STEP;        
                }
            }           
            break;
            case END_STEP:
             DrainPump_ON();
             Mt_OFF();
             if (TimerExpired(WASH_TIMER)) {
                 DrainPump_OFF();
                 WashingPhaseStep = FILLING; 
                 //WashingPhase = WASH_PHASE;
                } 
             break;
    }
    
};

void Wash(void){
    switch (WashingPhaseStep) {
        case FILLING:
            break;
        case WASHING:
            break;
        case DRAINING:
            break;
        case SPINNING:
            break;
    }
    return;
};

void Rinse(void){
    switch (WashingPhaseStep) {
        case FILLING:
            break;
        case WASHING:
            break;
        case DRAINING:
            break;
        case SPINNING:
            break;
    }
     return;
};

void Spin(void){
    switch (WashingPhaseStep) {
        case DRAINING:
            break;
        case SPINNING:
            break;
    }
    return;
};

void WashingCycleManager(void) {
    WashingPhases[WashingPhase]();
};