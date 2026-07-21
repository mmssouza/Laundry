/*
 * Fwile:   main.c
 * Author: marcelo
 *
 * Created on 9 de Maio de 2026, 10:38
 */

// PIC16F884 Configuration Bit Settings

// 'C' source line config statements

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

unsigned char task = 0;
unsigned char conta_250us = 0;
unsigned char conta_5ms = 0;

void init(void) {
    TRISC = 0;
    PORTC = 0;
    T2CON = 0x01;   // Timer2: Pre 1:4 Post 1:1
    PR2 = 124;    // Timer2 overflow @ 250 us
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    TMR2ON = 1;
}

void main(void) {
    init();
    while (1) 
     if (conta_5ms < 5) {
      __asm("nop\n");
     }
     else {
      //@ 25 ms
      conta_5ms = 0;
      
      switch (task++) {
        case 0:
            PORTCbits.RC0 ^= 1;
           __asm("nop\n");
            break;
        case 1:
            PORTCbits.RC1 ^= 1;
            __asm("nop\n");
            break;
        case 2:
            PORTCbits.RC2 ^= 1;
            __asm("nop\n");
            break;
        case 3:
            PORTCbits.RC3 ^= 1;
            __asm("nop\n");
            task = 0;
            break;
        default:
            task = 0;
     }
    }
}

void __interrupt() ISR() {
    if (PIR1bits.TMR2IF) {
        PIR1bits.TMR2IF = 0;
        // @ 250 us
        if (++conta_250us >= 20) {
            // @ 5 ms
            conta_250us = 0;
            conta_5ms++;
        }   
    }
}