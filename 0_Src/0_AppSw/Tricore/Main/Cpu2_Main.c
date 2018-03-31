/**
 * \file Cpu2_Main.c
 * \brief Main function definition for Cpu core 2 .
 *
 * \copyright Copyright (c) 2015 Infineon Technologies AG. All rights reserved.
 *
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 */

#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "Scu/Std/IfxScuWdt.h"
#include "Port\Std\IfxPort.h"
#include "main.h"
#include "machine/circ.h"

extern IfxCpu_mutexLock g_Asc0_Lock;

extern volatile int32_t g_share_i32;

IFX_INLINE sint32 __add(sint32 a, sint32 b)
{
    sint32 res;
    __asm__ volatile ("add %0, %1, %2": "=d" (res) : "d" (a), "d" (b));
    return res;
}

IFX_INLINE sint32 __sub(sint32 a, sint32 b)
{
    sint32 res;
    __asm__ volatile ("sub %0, %1, %2": "=d" (res) : "d" (a), "d" (b));
    return res;
}

/** multiplication signed without saturation
 */
IFX_INLINE uint32 __mul(sint32 a, sint32 b)
{
    sint32 res;
    __asm__ volatile ("mul %0, %1, %2": "=d" (res) : "d" (a), "d" (b));
    return res;
}

/** multiplication signed with saturation
 */
IFX_INLINE sint32 __muls(sint32 a, sint32 b)
{
	sint32 res;
    __asm__ volatile ("muls %0, %1, %2": "=d" (res) : "d" (a), "d" (b));
    return res;
}

//This example shows how to create a buﬀer using circular addressing. The buﬀer buf contains
//20 items which are controlled by ctrl . The buﬀer is initialised with the sequence 20, 19, 18,
//... 2, 1. Afterwards the content of the buﬀer is read. Note that the variable i is never used
//to address the current item in the ring buﬀer; this is all done by the hardware
#define TEST_LEN	16
#pragma section ".zdata"
long buf[TEST_LEN] __attribute__ ((aligned(8)));
#pragma section

int core2_main (void)
{
	uint32_t tmpTick;

	circ_t ctrl;
	long ll;

    IfxCpu_enableInterrupts();

    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());

	printf("Hardware Circular Buffer Operation Demo %u Hz\n", SYSTEM_GetCpuClock());

	ctrl = init_circ_long(ctrl, buf, TEST_LEN*sizeof(long), 0);

	for (uint32_t i = 0; i < TEST_LEN; i++)
	{
		ctrl = put_circ_long(ctrl, TEST_LEN-i);
	}

	//Debug
	for (uint32_t i = 0; i < TEST_LEN; i++)
	{
		ctrl = get_circ_long(ctrl, &ll);
		//do something with ll
		printf("%i, ", ll);
	}
	printf("\n");

	for (uint32_t i = 0; i < TEST_LEN/2; i++)
	{
		ctrl = put_circ_long(ctrl, i+100);
	}

	//Debug
	for (uint32_t i = 0; i < TEST_LEN; i++)
	{
		ctrl = get_circ_long(ctrl, &ll);
		//do something with ll
		printf("%i, ", ll);
	}
	printf("\n");

	for (uint32_t i = 0; i < TEST_LEN/2; i++)
	{
		ctrl = put_circ_long(ctrl, i+200);
	}

	//Debug
	for (uint32_t i = 0; i < TEST_LEN; i++)
	{
		ctrl = get_circ_long(ctrl, &ll);
		//do something with ll
		printf("%i, ", ll);
	}
	printf("\n");

    while (1)
    {
    	IfxPort_togglePin(&MODULE_P33, 10u);
     	IfxPort_togglePin(&MODULE_P33, 11u);

    	tmpTick = schd_GetTick();
    	while((tmpTick+TEST_DELAY_MS*2) > schd_GetTick())
    	{
    		_nop();
    	}

//    	printf("Cpu%d:%u Hz, Sys:%u Hz, Stm:%u Hz, Core:%04X,  Tick:%u\nshare_value:%d\n",
//    			IfxCpu_getCoreId(),
//				SYSTEM_GetCpuClock(),
//				SYSTEM_GetSysClock(),
//				SYSTEM_GetStmClock(),
//				__TRICORE_CORE__,
//				schd_GetTick(),
//				g_share_i32
//    	);

    }
    return (1);
}
