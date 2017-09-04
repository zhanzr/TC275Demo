#pragma GCC optimize "-O0"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <machine/cint.h>

#include "Cpu/Std/Ifx_Types.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"
#include "Scu/Std/IfxScuWdt.h"
#include "Port/Std/IfxPort.h"
#include <Asclin/Asc/IfxAsclin_Asc.h>
#include <Stm/Std/IfxStm.h>

#include "main.h"

int core1_main (void)
{
	uint32_t tmpTick;

	IfxCpu_enableInterrupts();
    /*
     * !!WATCHDOG1 IS DISABLED HERE!!
     * Enable the watchdog in the demo if it is required and also service the watchdog periodically
     * */
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
    while (1)
    {
/*
    	if ( demo_idx == CORE_DEMO_INT) 	Core_DemoRun(0, 1);
    	else if ( demo_idx == CORE_DEMO_NO_INT) Core_DemoRun(0, 0);
*/
    	 /* Turn LED Off */
    	IfxPort_setPinState(&MODULE_P33, 8u, IfxPort_State_high);
    	IfxPort_setPinState(&MODULE_P33, 9u, IfxPort_State_high);
//    	IfxPort_setPinState(&MODULE_P33, 10u, IfxPort_State_high);
//    	IfxPort_setPinState(&MODULE_P33, 11u, IfxPort_State_high);
    	/* Click speaker */
//    	IfxPort_setPinState(&MODULE_P33, 0u, IfxPort_State_high);

//    	printf("Simple 3 Core Test \nCpu:%u Hz, Sys:%u Hz, Stm:%u Hz, Core:%04X,  %u\n",
//    			SYSTEM_GetCpuClock(),
//				SYSTEM_GetSysClock(),
//				SYSTEM_GetStmClock(),
//				__TRICORE_CORE__,
//				HAL_GetTick()
//    	);
    	/* test delay */
    	tmpTick = HAL_GetTick();
    	while((tmpTick+TEST_DELAY_MS) > HAL_GetTick())
    	{
    		_nop();
    	}

    	/* Turn LED On */
    	IfxPort_setPinState(&MODULE_P33, 8u, IfxPort_State_low);
    	IfxPort_setPinState(&MODULE_P33, 9u, IfxPort_State_low);
//    	IfxPort_setPinState(&MODULE_P33, 10u, IfxPort_State_low);
//    	IfxPort_setPinState(&MODULE_P33, 11u, IfxPort_State_low);
    	/* Click speaker */
//    	IfxPort_setPinState(&MODULE_P33, 0u, IfxPort_State_low);

    	/* test delay */
    	tmpTick = HAL_GetTick();
    	while((tmpTick+TEST_DELAY_MS) > HAL_GetTick())
    	{
    		_nop();
    	}
    }
    return (1);
}
