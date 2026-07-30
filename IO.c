
#include <xc.h>
#include "IO.h"

#define DURATION 10; // 50ms

void InitIO(void) {
    AllOutputsOff();
    Input.byte = OFF;    
}

void AllOutputsOff(void) {
    Water_Valve = OFF;
    Drain_Pump = OFF;
    Motor_CW = OFF;
    Motor_CCW = OFF;
    Output.byte = OFF;
}

void InputDebounceMgr(void) {
    
  static input_t Previous; 
  static uint8_t Tmr;
  static uint8_t Init = 1;
  input_t current;
  
  current.bit.Pr_Lo = PR_LO;
  current.bit.Pr_Hi = PR_HI; 
  current.bit.Pr_Mid = PR_MID;
  
  if (Init) {    
        Tmr = DURATION;
        Init = 0;
  } else {
       Tmr--;
       if (current.byte != Previous.byte) {
        Tmr = DURATION;
       } else { 
           if (!Tmr) {
            Init = 1;
            Input.byte = current.byte;
           }
       }
  }
  
  Previous.byte = current.byte;

}

