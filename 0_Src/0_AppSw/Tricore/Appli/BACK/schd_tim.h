#ifndef SCHD_TIM
#define SCHD_TIM

#include <Stm/Std/IfxStm.h>
#include "Appli/Back/demo_handler.h"
// priorities are normally defined in Ifx_IntPrioDef.h
//#define IFX_INTPRIO_SCHD_STM0_SR0  110

extern uint32 system_tick;

//inline
extern uint32 schd_GetTick(void);

//inline
extern void schd_SetTick(uint32 tick);

extern void schd_init(void);

#endif
