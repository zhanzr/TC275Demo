#ifndef SYSTEM_MAIN
#define SYSTEM_MAIN

#define SuspendInt()	IfxCpu_disableInterrupts()
#define ResumeInt()		IfxCpu_enableInterrupts()

#include <machine/intrinsics.h>
#include <machine/wdtcon.h>
#include "interrupts.h"

#include <system_tc2x.h>

#include <tc_inc_path.h>

#include TC_INCLUDE(TCPATH/IfxScu_reg.h)
#include TC_INCLUDE(TCPATH/IfxScu_bf.h)
#include TC_INCLUDE(TCPATH/IfxCpu_reg.h)
#include TC_INCLUDE(TCPATH/IfxCpu_bf.h)
#include TC_INCLUDE(TCPATH/IfxStm_reg.h)
#include TC_INCLUDE(TCPATH/IfxStm_bf.h)

#if (APPKIT_TC2X7 > 0)
/* Workaround for TLF35584 A-Step Bug on AppKit-TC2x7 */
# define USE_DISABLE_EXT_WDT	1
#else
# define USE_DISABLE_EXT_WDT	0
#endif /* APPKIT_TC2X7 > 0 */

#if (USE_DISABLE_EXT_WDT == 1)
# include TC_INCLUDE(TCPATH/IfxPort_reg.h)
# include TC_INCLUDE(TCPATH/IfxQspi_reg.h)
#endif /* USE_DISABLE_EXT_WDT */

//#include "Appli/back/schd_tim.h"
//#include "Appli/back/back_uart.h"

#include "../Appli/PORT/PortDemoEntry.h"
//#include "../App/VADC/VadcDemoEntry.h"

#define TEST_DELAY_MS	2000U

typedef struct
{
	char *p_name;
	uint32 base_add;
	uint8  table_idx;
}tModuleStruct;

typedef struct
{
	char *p_name;
	uint32 offset_add;
}tRegStruct;

typedef struct
{
	uint32 size;
	tRegStruct *p_table;
}tRegTable;

extern const tModuleStruct mcu_modules[];
extern const tRegTable register_map[];

void back_init(void);
void back_cyclic(void);
void demo_cyclic(void);

uint32 mcu_get_address(char *p_module, char *p_reg);

extern unsigned long SYSTEM_GetCpuClock(void);
extern unsigned long SYSTEM_GetSysClock(void);
//extern unsigned long SYSTEM_GetStmClock(void);

#endif
