#ifndef __APPLI_CCU6_H_
#define __APPLI_CCU6_H_
#include "Cpu\Std\Ifx_Types.h"
#include "Port\Io\IfxPort_Io.h"
#include "Ccu6/Icu/IfxCcu6_Icu.h"
#include "Ccu6/TPwm/IfxCcu6_TPwm.h"

#include <stdlib.h>

#define IFX_INTPRIO_CCU6    11

void Appli_Ccu6Init(void);
void Appli_Ccu6DemoInit(void);
void Appli_Ccu6DemoDeInit(void);
void Appli_Ccu6Cyclic(void);

#endif
