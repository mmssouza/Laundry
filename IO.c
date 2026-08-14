
#include <xc.h>
#include "IO.h"

#define DURATION 10; // 50ms

void InitIO(void) {    
    AllOutputsOff();
    TRISC = 0b11110000;
    Input.byte = OFF;    
}

void AllOutputsOff(void) {
    Water_Valve = OFF;
    Drain_Pump = OFF;
    Motor_CW = OFF;
    Motor_CCW = OFF;
    Output.byte = 0;
}

void InputDebounceHandler(void) {
    
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

void OutputDrv(void) {    
    // if (!Input.Zc) return;
    
    if ((Output.bit.CW == ON) && (Output.bit.CCW == ON)) {
      Motor_CW  = OFF;
      Motor_CCW = OFF;
    } else {
      Motor_CW =  Output.bit.CW;
      Motor_CCW = Output.bit.CCW;
    }
   
    Water_Valve = Output.bit.WaterValve;
    Drain_Pump =  Output.bit.DrainPump;
    
}    

void StrokeOn(uint16_t t_on,uint16_t t_off) {
   Stroke.Ton = t_on;
   Stroke.Toff = t_off;
   AgStep = 1;
}

void StrokeOff(void) {
    Mt_OFF();
    AgStep = 0;
}

void AgitateManager(void) {
   // @ 20ms
  static uint8_t tmr;
  static uint8_t flag;
  uint8_t toff = Stroke.Toff/20;
  uint8_t ton = Stroke.Ton/20;
  
  if (tmr) tmr--;
  
  switch (AgStep) {
      case 0:
        tmr = 0;
        flag = 0;
        break;
      case 1:
         if (!tmr) {
          Mt_OFF();
          tmr = toff;
          flag ^= 1;
          AgStep++;
         }
        break;
      case 2: 
        if (!tmr) {
          tmr = ton;
          if (flag) {
           MtCW_ON();
          }
          else {
           MtCCW_ON();
          }
          AgStep--;
        } 
    }
}