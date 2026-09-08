/* 
 * File:   UI.h
 * Author: marcelo
 *
 * Created on 26 de Agosto de 2026, 09:10
 */

#ifndef UI_H
#define	UI_H

#include <xc.h>
#include <stdint.h>


#ifdef	__cplusplus
extern "C" {
#endif


typedef union {
    uint8_t byte;
    struct {
        uint8_t Start: 1;
        uint8_t PreWash: 1;
        uint8_t Wash: 1;
        uint8_t Rinse: 1;
        uint8_t Spin: 1;
        uint8_t PrHigh: 1;
        uint8_t PrMid :1;
        uint8_t PrLow : 1;
    } bit;
} leds_t;


typedef union {
    uint8_t byte;
    struct {
       uint8_t ON_OFF: 1;
       uint8_t START:  1;
       uint8_t unused: 6;
    } bit;
} keys_t;    

typedef enum {NONE, ON_OFF, START} key_t;

keys_t KeysBuffer;
leds_t LedsBuffer;

#define LedsAllOFF() {LedsBuffer.byte = 0xFF;}
#define LedsAllON() {LedsBuffer.byte = 0x00;}

#define LedStartON() {LedsBuffer.bit.Start = 0;} 
#define LedStartOFF() {LedsBuffer.bit.Start = 1;}

#define LedPreWashON() {LedsBuffer.bit.PreWash = 0;}
#define LedPreWashOFF() {LedsBuffer.bit.PreWash = 1;}

#define LedWashON() {LedsBuffer.bit.Wash = 0;}
#define LedWashOFF() {LedsBuffer.bit.Wash = 1;}

#define LedRinseON() {LedsBuffer.bit.Rinse = 0;}
#define LedRinseOFF() {LedsBuffer.bit.Rinse = 1;}

#define LedSpinON() {LedsBuffer.bit.Spin = 0;}
#define LedSpinOFF() {LedsBuffer.bit.Spin = 1;}


void UIInit(void);
key_t UIKeyCode(void);
void UIStateTransition();
void UIManager(void);
void UIDrv(void);

#ifdef	__cplusplus
}
#endif

#endif	/* UI_H */

