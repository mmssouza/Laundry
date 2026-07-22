/* 
 * File:   Timers.h
 * Author: marcelo
 *
 * Created on 21 de Julho de 2026, 14:48
 */

#ifndef TIMERS_H
#define	TIMERS_H
#ifdef	__cplusplus
extern "C" {
#endif
#include <stdint.h>

#define N_TIMERS 2
    
// 1h 49min max (T_1SEC * 109 )
#define TBASE_100MS 4   
#define T_1SEC    1    
#define T_1MIN  (T_1SEC*60)    


#define TIMER1 0
#define TIMER2 1

uint8_t TmrBaseCounter;

typedef struct { 
    uint16_t counter;
    struct {
        uint8_t     expired: 1;
        uint8_t          on: 1;
        uint8_t      unused: 6;   
    } flags;
} timer_t;

timer_t Timer[N_TIMERS];

void InitTimer(void);
void TimerMgr(void);
void LoadTimer(uint8_t index, uint16_t Value);
void ClearTimer(uint8_t index);
void ResumeTimer(uint8_t index); 
void PauseTimer(uint8_t index);
uint8_t TimerExpired(uint8_t index);     

#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

