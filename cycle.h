/* 
 * File:   cycle.h
 * Author: marcelo
 *
 * Created on 30 de Julho de 2026, 09:53
 */
#ifndef CYCLE_H
#define	CYCLE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include <stdint.h>
    
typedef void (*ActionFunc)(void);

// 2. Create the standalone functions
void PreWash(void);
void Wash(void);
void Rinse(void);
void Spin(void);

ActionFunc const WashingPhases[] = {
        PreWash, // Index 0
        Wash, // Index 1
        Rinse,   // Index 2
        Spin    };

 typedef enum { PRE_WASH_PHASE = 0,WASH_PHASE,RINSE_PHASE,SPIN_PHASE } washing_phases_t;    // 4. Call a function dynamically via an index
 
 typedef enum {FILLING = 0,WASHING,DRAINING,SPINNING,END_STEP} washing_phases_step_t; 
 
 washing_phases_t WashingPhase; 
 washing_phases_step_t WashingPhaseStep;
 

    // Execute the target function directly
 void WashingCycleManager(void);   


#ifdef	__cplusplus
}
#endif

#endif	/* CYCLE_H */

