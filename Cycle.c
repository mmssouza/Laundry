#include "cycle.h"
#include "Timers.h"
#include "IO.h"

void PreWash(void){
    
    switch (WashingStep) {
        case FILLING:
            WaterValve_ON();
            if (cycle_init_state) {
                LoadTimer(FILL_DRAIN_TIMER,T_1MIN*5);
                cycle_init_state = 0;
                return;
            }
                      
            if (PR_LO == ON)  {
                // Nível do pressostado indica fim de enchimento
                WaterValve_OFF();
                LoadTimer(WASH_TIMER,T_1SEC*15);
                WashingStep = WASHING;
                cycle_init_state = 1;
            } else if (TimerExpired(FILL_DRAIN_TIMER)) {
                // Timeout de enchimento
                  __asm("nop\n");
            }            
            break;
            
        case WASHING:  
            if (cycle_init_state == 1)
            { 
                ResumeTimer(WASH_TIMER);
                StrokeOn(280,320);
                cycle_init_state = 0;
                return;
            }
            
            if (TimerExpired(WASH_TIMER)) {
                StrokeOff();
                WashingStep = FILLING;
                WashingPhase = WASH_PHASE;
                cycle_init_state = 1;
            }
            break;
        default:
         __asm("nop\n");    
        
    }    
};

void Wash(void){
  switch (WashingStep) {
        case FILLING:
            WaterValve_ON();
            if (cycle_init_state) {
                LoadTimer(FILL_DRAIN_TIMER,T_1MIN*5);
                cycle_init_state = 0;
                return;
            }
                      
            if (PR_LO == ON)  {
                // Nível do pressostado indica fim de enchimento
                WaterValve_OFF();
                LoadTimer(WASH_TIMER,T_1SEC*15);
                WashingStep = WASHING;
                cycle_init_state = 1;
            } else if (TimerExpired(FILL_DRAIN_TIMER)) {
                // Timeout de enchimento
                  __asm("nop\n");
            }            
            break;
            
        case WASHING:  
            if (cycle_init_state == 1)
            { 
                ResumeTimer(WASH_TIMER);
                StrokeOn(280,320);
                cycle_init_state = 0;
                return;
            }
            
            if (TimerExpired(WASH_TIMER)) {
                StrokeOff();
                WashingStep = DRAINING;
            }
            break;
            
        case DRAINING:
            DrainPump_ON();
            if (cycle_init_state) {
                LoadTimer(FILL_DRAIN_TIMER,T_1MIN*5);
                cycle_init_state = 0;
                return;
            }             
            
            if (PR_LO == OFF)  {
             LoadTimer(WASH_TIMER,T_1SEC*15);
             WashingStep = SPINNING;
             cycle_init_state = 1;
            } else if (TimerExpired(FILL_DRAIN_TIMER)) {
              // Timeout de drenagem
                __asm("nop\n");             
            }
            break;
            
        case SPINNING:
            DrainPump_ON();
            
            if (cycle_init_state) {
                if (TimerExpired(WASH_TIMER)) {
                 MtCW_ON();
                 LoadTimer(WASH_TIMER,T_1SEC*10);
                 cycle_init_state = 0;     
             }
             return;
            }
                
            MtCW_ON();
            
            if (TimerExpired(WASH_TIMER)) {
                Mt_OFF();
                LoadTimer(WASH_TIMER,T_1SEC*30);
                WashingStep = END_STEP;        
             }
                       
            break;
            
        case END_STEP:
            DrainPump_ON();
            Mt_OFF();
            if (TimerExpired(WASH_TIMER)) {
                 DrainPump_OFF();
                 WashingStep = FILLING;
                 WashingPhase = RINSE_PHASE;
                 cycle_init_state = 1;
            } 
            break;
            
        default:
         __asm("nop\n");    
        
    }     
};

void Rinse(void){
   switch (WashingStep) {
        case FILLING:
            WaterValve_ON();
            if (cycle_init_state) {
                LoadTimer(FILL_DRAIN_TIMER,T_1MIN*5);
                cycle_init_state = 0;
                return;
            }
                      
            if (PR_LO == ON)  {
                // Nível do pressostado indica fim de enchimento
                WaterValve_OFF();
                LoadTimer(WASH_TIMER,T_1SEC*15);
                WashingStep = WASHING;
                cycle_init_state = 1;
            } else if (TimerExpired(FILL_DRAIN_TIMER)) {
                // Timeout de enchimento
                  __asm("nop\n");
            }            
            break;
            
        case WASHING:  
            if (cycle_init_state == 1)
            { 
                ResumeTimer(WASH_TIMER);
                StrokeOn(280,320);
                cycle_init_state = 0;
                return;
            }
            
            if (TimerExpired(WASH_TIMER)) {
                StrokeOff();
                WashingStep = DRAINING;
            }
            break;
            
        case DRAINING:
            DrainPump_ON();
            if (cycle_init_state) {
                LoadTimer(FILL_DRAIN_TIMER,T_1MIN*5);
                cycle_init_state = 0;
                return;
            }             
            
            if (PR_LO == OFF)  {
               WashingPhase = SPIN_PHASE; 
               WashingStep = SPINNING;
               cycle_init_state = 1;
            } else if (TimerExpired(FILL_DRAIN_TIMER)) {
              // Timeout de drenagem
                __asm("nop\n");             
            }
            break;
            
        default:
         __asm("nop\n");    
        
    }     
};

void Spin(void){
    switch (WashingStep) {
                          
        case SPINNING:
            DrainPump_ON();
            
            if (cycle_init_state) {
                if (TimerExpired(WASH_TIMER)) {
                 MtCW_ON();
                 LoadTimer(WASH_TIMER,T_1SEC*10);
                 cycle_init_state = 0;     
             }
             return;
            }
                
            MtCW_ON();
            
            if (TimerExpired(WASH_TIMER)) {
                Mt_OFF();
                LoadTimer(WASH_TIMER,T_1SEC*30);
                WashingStep = END_STEP;        
             }
                       
            break;
            
        case END_STEP:
            DrainPump_ON();
            Mt_OFF();
            if (TimerExpired(WASH_TIMER)) {
                 DrainPump_OFF();
                 WashingPhase = DONE;
            } 
            break;
            
        default:
         __asm("nop\n");    
        
    }    
};

washing_phases_t WashingCycleManager(void) {
    WashingPhases[WashingPhase]();
    
    if (WashingStep == WASHING)
     AgitateManager();
    
    return WashingPhase;
};