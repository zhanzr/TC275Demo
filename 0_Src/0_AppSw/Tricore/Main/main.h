#ifndef SYSTEM_MAIN
#define SYSTEM_MAIN

#define SuspendInt()	IfxCpu_disableInterrupts()
#define ResumeInt()		IfxCpu_enableInterrupts()

//#include "Appli/back/schd_tim.h"
//#include "Appli/back/back_uart.h"
#include <stdint.h>
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

uint32 schd_GetTick(void);

uint32 mcu_get_address(char *p_module, char *p_reg);

#endif
