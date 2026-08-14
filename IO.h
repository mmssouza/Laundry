/* 
 * File:   IO.h
 * Author: marcelo
 *
 * Created on 22 de Julho de 2026, 11:23
 */

#ifndef IO_H
#define	IO_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <stdint.h>
    
#define OFF 0
#define ON  1
    
// Outputs 
#define Water_Valve PORTCbits.RC0 
#define Drain_Pump  PORTCbits.RC1
#define Motor_CW    PORTCbits.RC2
#define Motor_CCW   PORTCbits.RC3

// Inputs 
#define PR_LO       PORTCbits.RC6    
#define PR_MID      PORTCbits.RC5
#define PR_HI       PORTCbits.RC4 

typedef union {
    uint8_t byte;
    struct {
            uint8_t Pr_Lo:  1;
            uint8_t Pr_Hi:  1;
            uint8_t Pr_Mid: 1;
            uint8_t Zc:  1;
            uint8_t Lid: 1;
            uint8_t unused: 3;
    } bit;
} input_t;

input_t Input;

union {
    uint8_t byte;
    struct {
            uint8_t WaterValve: 1;
            uint8_t DrainPump:  1;
            uint8_t CW:         1;
            uint8_t CCW:        1;
            uint8_t unused:     4;
    } bit;
} Output;

struct {
    uint16_t Ton;
    uint16_t Toff;
} Stroke;

uint8_t AgStep = 0;

void InitIO(void);
void AllOutputsOff(void);
void InputDebounceHandler(void);
void StrokeOn(uint16_t t_on,uint16_t t_off);
void StrokeOff(void);
void AgitateManager(void);
void OutputDrv(void);

#define MtCW_ON()  {Output.bit.CW = ON;}
#define MtCCW_ON() {Output.bit.CCW = ON;}
#define Mt_OFF() {Output.bit.CW = OFF;Output.bit.CCW = OFF;}
#define WaterValve_ON()  {Output.bit.WaterValve = ON;}
#define WaterValve_OFF() {Output.bit.WaterValve = OFF;}
#define DrainPump_ON() {Output.bit.DrainPump = ON;}
#define DrainPump_OFF() {Output.bit.DrainPump = OFF;}

#ifdef	__cplusplus
}
#endif

#endif	/* IO_H */

