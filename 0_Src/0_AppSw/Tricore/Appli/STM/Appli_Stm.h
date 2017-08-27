#ifndef __APPLI_STM_H_
#define __APPLI_STM_H_

#include "Cpu\Std\Ifx_Types.h"
#include "Stm\Std\IfxStm.h"

extern boolean StmEvent;

IFX_INLINE boolean Stm_GetEvent(void);
IFX_INLINE void Stm_ResetEvent(void);

IFX_INLINE boolean Stm_GetEvent(void)
{
	return StmEvent;
}

IFX_INLINE void Stm_ResetEvent(void)
{
	StmEvent = FALSE;
}

void Appli_StmInit(void);
void Appli_StmDelay10ms(void);
void Appli_StmDemoInit(void);
void Appli_StmDemoDeInit(void);
void Appli_StmCyclic(void);

#endif //end of __APPLI_STM_H_
