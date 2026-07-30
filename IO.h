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

typedef struct {
    uint8_t byte;
    union {
            uint8_t Pr_Lo:  1;
            uint8_t Pr_Hi:  1;
            uint8_t Pr_Mid: 1;
            uint8_t Zc:  1;
            uint8_t Lid: 1;
            uint8_t unused: 3;
    } bit;
} input_t;

input_t Input;

struct {
    uint8_t byte;
    union {
            uint8_t WaterValve: 1;
            uint8_t DrainPump:  1;
            uint8_t Cw:         1;
            uint8_t Ccw:        1;
            uint8_t unused:     4;
    } bit;
} Output;

void InitIO(void);
void AllOutputsOff(void);
void InputDebounceMgr(void);

#ifdef	__cplusplus
}
#endif

#endif	/* IO_H */

