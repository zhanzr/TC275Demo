#ifndef __APPLI_ENTRY_H_
#define __APPLI_ENTRY_H_

#include "Cpu\Std\Ifx_Types.h"

#include "Asclin\Asc\IfxAsclin_Asc.h"
#include "Tricore\Appli\Stm\Appli_Stm.h"
#include "Tricore\Appli\Gpio\Appli_Gpio.h"
//#include "Tricore\Appli\Asclin\Appli_Asclin.h"
#include "Tricore\Appli\VAdc\Appli_Adc.h"
#include "Tricore\Appli\Ccu6\Appli_Ccu6.h"
#include "Tricore\Appli\Gpt\Appli_Gpt.h"
//#include "Tricore\Appli\MultiCAN\Appli_Can.h"
//#include "Tricore\Appli\Qspi\Appli_Qspi.h"

#define IFX_INTPRIO_COM0_TX  1
#define IFX_INTPRIO_COM0_RX  2
#define IFX_INTPRIO_COM0_ER  3

#define MENU_ITEM_MAIN		0
#define MENU_ITEM_GPIO		1
#define MENU_ITEM_STM		2
#define MENU_ITEM_ASCLIN    3
#define MENU_ITEM_CCU6		4
#define MENU_ITEM_GPT		5
#define MENU_ITEM_ADC		6
#define MENU_ITEM_QSPI		7
#define MENU_ITEM_CAN		8
#define MENU_ITEM_NUM		9

typedef void *typeDemoFunc(void);

IFX_INLINE boolean Appli_EventCheck(void);
IFX_INLINE void Appli_EventClear(void);
IFX_INLINE void LongToAscii(uint64 data, char* p_str);
IFX_INLINE void IntToAscii(uint32 data, char* p_str);
IFX_INLINE void ShortToAscii(uint16 data, char* p_str);
IFX_INLINE void ByteToAscii(uint16 data, char* p_str);

IFX_INLINE boolean Appli_EventCheck(void)
{
	return Stm_GetEvent();
}

IFX_INLINE void Appli_EventClear(void)
{
	Stm_ResetEvent();
}

IFX_INLINE void Appli_TimerDelay(void)
{
	Appli_StmDelay10ms();
}

IFX_INLINE void LongToAscii(uint64 data, char* p_str)
{
	int hex,i;

	p_str[0] = '0';
	p_str[1] = 'x';

	for(i = 0; i < 16; i++)
	{
		hex = (data >> (60 - i * 4)) & 0xf;
		if(hex < 10)
			p_str[2 + i] = '0' + hex;
		else if(hex < 16)
			p_str[2 + i] = 'A' + (hex - 10);
	}
}

IFX_INLINE void IntToAscii(uint32 data, char* p_str)
{
	int hex,i;

	p_str[0] = '0';
	p_str[1] = 'x';

	for(i = 0; i < 8; i++)
	{
		hex = (data >> (28 - i * 4)) & 0xf;
		if(hex < 10)
			p_str[2 + i] = '0' + hex;
		else if(hex < 16)
			p_str[2 + i] = 'A' + (hex - 10);
	}
}

IFX_INLINE void ShortToAscii(uint16 data, char* p_str)
{
	int hex,i;

	p_str[0] = '0';
	p_str[1] = 'x';

	for(i = 0; i < 4; i++)
	{
		hex = (data >> (12 - i * 4)) & 0xf;
		if(hex < 10)
			p_str[2 + i] = '0' + hex;
		else if(hex < 16)
			p_str[2 + i] = 'A' + (hex - 10);
	}
}

IFX_INLINE void ByteToAscii(uint16 data, char* p_str)
{
	int hex,i;

	p_str[0] = '0';
	p_str[1] = 'x';

	for(i = 0; i < 2; i++)
	{
		hex = (data >> (4 - i * 4)) & 0xf;
		if(hex < 10)
			p_str[2 + i] = '0' + hex;
		else if(hex < 16)
			p_str[2 + i] = 'A' + (hex - 10);
	}
}

static void Appli_ChangeMenu(int *p_level, int *p_item, char in);
static void Appli_DispMenu(int level, int item);

void Appli_ConsolePrint(IfxAsclin_Asc *asclin, const char *p_str);
char Appli_ConsoleRead(IfxAsclin_Asc *asclin);
void Appli_Initlize(void);
void Appli_TaskCyclic(void);

boolean Appli_SetCoreMode(Ifx_CPU *cpu, IfxCpu_CoreMode mode);
char ConsoleRead(void);
void ConsolePrint(const char *p_str);

#endif //end of __APPLI_STM_H_
