#ifndef __APPLI_ASCLIN_H_
#define __APPLI_ASCLIN_H_

#include <stdlib.h>
#include "Cpu\Std\Ifx_Types.h"
#include "Asclin\Asc\IfxAsclin_Asc.h"
#include "Asclin\Lin\IfxAsclin_Lin.h""

#define IFX_INTPRIO_ASCLIN0_TX  4
#define IFX_INTPRIO_ASCLIN0_RX  5
#define IFX_INTPRIO_ASCLIN0_ER  6

#define ASC_TX_BUFFER_SIZE 64
#define ASC_RX_BUFFER_SIZE 64

void Appli_AsclinInit(void);
void Appli_AsclinDemoInit(void);
void Appli_AsclinDemoDeInit(void);
void Appli_AsclinCyclic(void);

#endif //end of __APPLI_ASCLIN_H_
