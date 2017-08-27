#ifndef DEMO_IO
#define DEMO_IO

#include <Port/Io/IfxPort_Io.h>
#include "_PinMap/IfxPort_PinMap.h"

#define PORT_PRE_DIVDER		 (5)
#define PORT_DIVIDER_1		 (10)
#define PORT_DIVIDER_2		 (1)

#define PORT_DEMO_PINS		 (0)
#define PORT_DEMO_GROUP		 (1)
#define PORT_DEMO_MANUAL	 (2)

extern const IfxPort_Io_ConfigPin configPin[];
extern const IfxPort_Io_Config conf;

void PORT_ModuleInit(void);
void PORT_ModuleDeInit(void);

void PINS_ToggleCyclic(void);
void PORT_GroupCyclic(void);
void PORT_ManualCyclic(void);

void PORT_DemoInit(uint32 module, uint32 idx);
void PORT_DemoDeInit(uint32 module, uint32 idx);
void PORT_DemoRun(uint32 module, uint32 idx);

#endif

