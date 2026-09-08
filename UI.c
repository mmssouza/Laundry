
#include <xc.h>
#include "main.h"
#include "IO.h"
#include "cycle.h"
#include "UI.h"
#include "Timers.h"

void UIInit(void) {
  WPUB = 0x00; // PORTB pull-up disable  
  ANSELH = 0x00; // PORTB digital IO
  TRISB = 0xFF;
  PORTD = 0xFF;
  TRISD = 0X00; 
  KeysBuffer.byte = 0X00;
  LedsAllOFF();
}

key_t UIKeyCode(void) {
    
 key_t keycode;
    
 if (KeysBuffer.bit.ON_OFF == 1) {
   KeysBuffer.bit.ON_OFF = 0;
   return ON_OFF;
 } else if (KeysBuffer.bit.START == 1) {
   KeysBuffer.bit.START = 0;
   return START;
 };
 
 return NONE;
}

void UIStateTransition(key_t keycode) {
  
  switch (keycode) {          
   case START:  
    if (ProductState == PAUSE) {
     cycle_init_state = 1;
     if (PrevProductState == STANDBY) {   
        WashingPhase = PRE_WASH_PHASE;
        WashingStep = FILLING; 
     }
     
     PrevProductState = ProductState;
     ProductState = WASH;             
    }            
    break;
                 
   case ON_OFF:
    if (ProductState == STANDBY) {   
      PrevProductState = ProductState;
      ProductState = PAUSE;
    } else if ((ProductState == PAUSE) || (ProductState == WASH)) {  
      PrevProductState = ProductState;
      ProductState = STANDBY;   
    }         
    break;  
 }
}

void UILedsUpdate(void) {
    
 static uint8_t blink_tmr = 0;
 
 if (ProductState == STANDBY) {
    LedsAllOFF();
    blink_tmr = 0;
    return;
 } 

 if (ProductState == PAUSE) {
  if (++blink_tmr > 50) {
   blink_tmr = 0;
   LedsBuffer.bit.Start ^= 1;
  }
 } else LedStartON(); 
 
 switch (WashingPhase) {
  case PRE_WASH_PHASE:
       LedPreWashON();
       LedWashOFF();
       LedRinseOFF();
       LedSpinOFF();
       break;  
  case WASH_PHASE:
       LedPreWashOFF();
       LedWashON();
       LedRinseOFF();
       LedSpinOFF();
       break;
 case RINSE_PHASE:
       LedPreWashOFF();
       LedWashOFF();
       LedRinseON();
       LedSpinOFF();
       break;  
 case SPIN_PHASE:
       LedPreWashOFF();
       LedWashOFF();
       LedRinseOFF();
       LedSpinON();
       break;  
 }
}
  
void UIManager(void) {
  
 key_t keycode;
   
 keycode = UIKeyCode();
 
 if (Input.bit.Lid == CLOSE) 
  UIStateTransition(keycode);

 UILedsUpdate(); 
}

void UIDrv(void) {
  static keys_t Previous; 
  static uint16_t Tmr;
  static uint8_t Init = 1;
  keys_t current;
  
  PORTD = LedsBuffer.byte;
  current.byte = PORTB;
  
  if (Init) {    
        Tmr = 500;
        Init = 0;
  } else {
       Tmr--;
       if (current.byte != Previous.byte) {
        Tmr = 500;
       } else { 
           if (!Tmr) {
            Init = 1;
            KeysBuffer.byte = current.byte;
           }
       }
  }
  
  Previous.byte = current.byte;
};
