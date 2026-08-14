/*
 * Fwile:   main.c
 * Author: marcelo
 *
 * Created on 9 de Maio de 2026, 10:38
 */

// PIC16F884 Configuration Bit Settings

// 'C' source line main.c:63:13: warning: implicit declaration of function 'Washing_Cycle_Manager' is invalid in C99 [-Wimplicit-function-declaration]config statements

// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "IO.h"
#include "Timers.h"
#include "cycle.h"



unsigned char task = 0;
unsigned char conta_250us = 0;
unsigned char conta_1ms = 0;
enum { STANDBY = 0,PAUSE, WASH} ProductState;

void Init(void) {
    InitIO();
    T2CON = 0x01;   // Timer2: Pre 1:4 Post 1:1
    PR2 = 124;    // Timer2 overflow @ 250 us
    PIE2 = 0;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    TMR2ON = 1;
    InitTimer();
    ProductState = WASH;
    WashingPhase = PRE_WASH_PHASE;
    WashingPhaseStep = FILLING;     
}

void StateManager() {
    switch (ProductState) {
        case STANDBY:
            break;
        case PAUSE:
            break;
        case WASH:
            WashingCycleManager();
            break;
    }
}
void main(void) {
    Init();
    
    while (1) 
     if (conta_1ms < 5) {
      __asm("nop\n");
     }
     else {
      conta_1ms = 0;
      //@ 5 ms   
           
      switch (task++) {
        case 0:
            //@ 20 ms
             TimerMgr();
            break;
        case 1:
            __asm("nop\n");
            //UIManager();
            break;
        case 2:
            StateManager();
            break;
        case 3:
            AgitateManager();
            task = 0;
            break;
        default:
            task = 0;
     }
    }
}

void __interrupt() ISR() {
    static uint8_t IrqState = 0;
    
    if (PIR1bits.TMR2IF) {
        PIR1bits.TMR2IF = 0;
        OutputDrv();
        // @ 250 us
        if (++conta_250us >= 4) {
            conta_250us = 0;
            conta_1ms++; 
        }
        switch (IrqState++)
        {
          case 0:
           // @ 1 ms 
           InputDebounceHandler();
           break;
          case 1:
           //UserInterfaceDrv();         
           break;
          case 2:
           __asm("nop\n");
           break;
          case 3:
           __asm("nop\n");
           IrqState = 0;
           break;
         }
      }
    }