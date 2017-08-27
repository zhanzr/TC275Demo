/*****************************************************
 *
 * TriBoard_TC265D_framework.c
 *
 * Description : Hello World in C, ANSI-style
 *
 */
#include "demo_handler.h"

uint32 demo_module = 4;
uint32 demo_module_curr = 0xffffffff;
uint32 demo_idx = 1;
uint32 demo_idx_curr = 0xffffffff;

sDemoEntry demo_modules[] = {
//		{"SCU", (pFunc_DemoInit)0, (pFunc_DemoDeInit)0, (pFunc_DemoRun)0},
//		{"PORT", (pFunc_DemoInit)PORT_DemoInit, (pFunc_DemoDeInit)PORT_DemoDeInit, (pFunc_DemoRun)PORT_DemoRun},
//		{"CAN", (pFunc_DemoInit)CAN_DemoInit, (pFunc_DemoDeInit)CAN_DemoDeInit, (pFunc_DemoRun)CAN_DemoRun},
		{"ASCLIN", (pFunc_DemoInit)ASCLIN_DemoInit, (pFunc_DemoDeInit)ASCLIN_DemoDeInit, (pFunc_DemoRun)ASCLIN_DemoRun},
//		{"DSADC", (pFunc_DemoInit)DSADC_DemoInit, (pFunc_DemoDeInit)DSADC_DemoDeInit, (pFunc_DemoRun)DSADC_DemoRun},
/*
		{"STM", (pFunc_DemoInit)STM_DemoInit, (pFunc_DemoDeInit)STM_DemoDeInit, (pFunc_DemoRun)STM_DemoRun},
		{"QSPI", (pFunc_DemoInit)QSPI_DemoInit, (pFunc_DemoDeInit)QSPI_DemoDeInit, (pFunc_DemoRun)QSPI_DemoRun},
		{"VADC", (pFunc_DemoInit)VADC_DemoInit, (pFunc_DemoDeInit)VADC_DemoDeInit, (pFunc_DemoRun)VADC_DemoRun},
		{"CCU6", (pFunc_DemoInit)CCU6_DemoInit, (pFunc_DemoDeInit)CCU6_DemoDeInit, (pFunc_DemoRun)CCU6_DemoRun},
		{"GPT12", (pFunc_DemoInit)GPT_DemoInit, (pFunc_DemoDeInit)GPT_DemoDeInit, (pFunc_DemoRun)GPT_DemoRun},
		{"I2C", (pFunc_DemoInit)I2C_DemoInit, (pFunc_DemoDeInit)I2C_DemoDeInit, (pFunc_DemoRun)I2C_DemoRun},
		{"ERAY", (pFunc_DemoInit)ERAY_DemoInit, (pFunc_DemoDeInit)ERAY_DemoDeInit, (pFunc_DemoRun)ERAY_DemoRun},
		{"SENT", (pFunc_DemoInit)SENT_DemoInit, (pFunc_DemoDeInit)SENT_DemoDeInit, (pFunc_DemoRun)SENT_DemoRun},
		{"MSC", (pFunc_DemoInit)MSC_DemoInit, (pFunc_DemoDeInit)MSC_DemoDeInit, (pFunc_DemoRun)MSC_DemoRun},
		{"HSSL", (pFunc_DemoInit)HSSL_DemoInit, (pFunc_DemoDeInit)HSSL_DemoDeInit, (pFunc_DemoRun)HSSL_DemoRun},
		{"ETH", (pFunc_DemoInit)ETH_DemoInit, (pFunc_DemoDeInit)ETH_DemoDeInit, (pFunc_DemoRun)ETH_DemoRun},
		{"CORE", (pFunc_DemoInit)Core_DemoInit, (pFunc_DemoDeInit)Core_DemoDeInit, (pFunc_DemoRun)Core_DemoRun},
		{"GTM", (pFunc_DemoInit)GTM_DemoInit, (pFunc_DemoDeInit)GTM_DemoDeInit, (pFunc_DemoRun)GTM_DemoRun}
*/
};

extern boolean compare_name(char *src, char *tgt);

uint32 demo_getmodule(char * demo_name)
{
	uint32 i = 0xffffffff;
/*

	for(i = 0; i < sizeof(demo_modules)/sizeof(sDemoEntry); i++)
	{
		if(compare_name(demo_name, demo_modules[i].demo_name) == TRUE)
			break;
	}
*/
	return i;
}

void demo_init(uint32 module, uint32 idx)
{
	pFunc_DemoInit pfunc_init;
	char demo[2];
	demo[0] = '0' + idx;

	pfunc_init = demo_modules[module].demo_init;
	if(pfunc_init != (pFunc_DemoInit)0)
	{
		pfunc_init(module, idx);
#ifdef PRINT_DEMO_INIT
/*
		backUart_Write("Enter ", 0);
		backUart_Write(demo_modules[module].demo_name, 0);
		backUart_Write(" demo", 0);
		backUart_Write(demo, 1);
		backUart_Write("\r\n", 0);
*/
#endif
	}
}

void demo_deinit(uint32 module, uint32 idx)
{
	pFunc_DemoDeInit pfunc_deinit;
	char demo[2];
	demo[0] = '0' + idx;

	pfunc_deinit = demo_modules[module].demo_deinit;
	if(pfunc_deinit != (pFunc_DemoDeInit)0)
	{
		pfunc_deinit(module, idx);
#ifdef PRINT_DEMO_DEINIT
/*
		backUart_Write("Exit ", 0);
		backUart_Write(demo_modules[module].demo_name, 0);
		backUart_Write(" demo", 0);
		backUart_Write(demo, 1);
		backUart_Write("\r\n", 0);
*/
#endif
	}
}

void demo_run(uint32 module, uint32 idx)
{
	static demo_run_count = 0;
	pFunc_DemoRun pfunc_run;
	char demo[2];
	demo[0] = '0' + idx;

	pfunc_run = demo_modules[module].demo_run;
	if(pfunc_run != (pFunc_DemoRun)0)
		pfunc_run(module, idx);

#ifdef PRINT_DEMO_RUN
	demo_run_count++;
	if(demo_run_count > 50)
	{
		demo_run_count = 0;
		backUart_Write(demo_modules[module].demo_name, 0);
		backUart_Write(" demo", 0);
		backUart_Write(demo, 1);
		backUart_Write(" is running!\r\n", 0);
	}
#endif
}

void demo_cyclic(void)
{
	if(demo_module != demo_module_curr || demo_idx != demo_idx_curr)
	{
		if(demo_idx == 0xffffffff && demo_idx_curr != 0xffffffff && demo_module_curr != 0xffffffff && demo_module_curr < sizeof(demo_modules)/sizeof(sDemoEntry))
		{
			demo_deinit(demo_module_curr, demo_idx_curr);
			demo_module = 0xffffffff;
			demo_module_curr = 0xffffffff;
			demo_idx = 0xffffffff;
			demo_idx_curr = 0xffffffff;
			return;
		}
		if(demo_module_curr != 0xffffffff && demo_module_curr < sizeof(demo_modules)/sizeof(sDemoEntry))
		{
			demo_deinit(demo_module_curr, demo_idx_curr);
		}
		demo_module_curr = demo_module;
		demo_idx_curr = demo_idx;
		if(demo_idx == 0xffffffff)
			return;
		if(demo_module_curr != 0xffffffff && demo_module_curr < sizeof(demo_modules)/sizeof(sDemoEntry))
			demo_init(demo_module_curr, demo_idx_curr);
	}
	else
	{
		if(demo_module_curr != 0xffffffff && demo_module_curr < sizeof(demo_modules)/sizeof(sDemoEntry))
			demo_run(demo_module_curr, demo_idx_curr);
	}
}
