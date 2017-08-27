#include "Appli_Stm.h"
#include "Tricore\Appli\Entry\Appli_Entry.h"

boolean StmEvent = FALSE;
static uint64 system_timer = 0;
Ifx_STM *p_stm;
int stm_count = 0;

extern int demo_item;
void Appli_StmInit(void)
{
	p_stm = &MODULE_STM0;
	StmEvent = FALSE;
}

void Appli_StmDelay10ms(void)
{
	IfxStm_waitTicks(&MODULE_STM0, 10000);
	StmEvent = TRUE;
}

void Appli_StmDemoInit(void)
{
}

void Appli_StmDemoDeInit(void)
{
}

void Appli_Demo_STMRead(void)
{
	uint32 system_lo = 0, system_hi;
	unsigned char hex, i;
	char stm_string[18];
	char *p_str;

	system_timer = IfxStm_get(p_stm);
	system_hi = system_timer >> 32;
	system_lo = (uint32)system_timer;
	//LongToAscii(system_timer, stm_string);
	ConsolePrint("Value :");
	IntToAscii(system_hi, stm_string);
	stm_string[10] = '\0';
	ConsolePrint(stm_string);
	ConsolePrint(", ");
	IntToAscii(system_lo, stm_string);
	stm_string[10] = '\0';
	ConsolePrint(stm_string);
	ConsolePrint("\r\n");
}

void Appli_StmCyclic(void)
{
	stm_count++;
	if(stm_count > 500)
	{
		stm_count = 0;
		if(demo_item == 1)
			Appli_Demo_STMRead();
	}
}
