#ifndef __APPLI_GPT_H_
#define __APPLI_GPT_H_
#include "Cpu\Std\Ifx_Types.h"
#include "Cpu\Std\IfxCpu.h"
#include "Port\Io\IfxPort_Io.h"
#include "_PinMap/IfxGpt12_PinMap.h"

#define IFX_INTPRIO_GPT120	10

void Appli_GptInit(void);
void Appli_GptDemoInit(void);
void Appli_GptDemoDeInit(void);
void Appli_GptCyclic(void);

#endif
