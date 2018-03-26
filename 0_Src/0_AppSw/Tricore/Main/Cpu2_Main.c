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
//#include "../Appli/BACK/demo_handler.h"

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


int core2_main (void)
{
	uint32_t tmpTick;

    IfxCpu_enableInterrupts();

    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());

	printf("Saturate Airthmetic Operation Demo %u Hz\n", SYSTEM_GetCpuClock());

    //Test Add
    printf("\nTest Add\n");
    {
		int32_t t1 = INT32_MAX;
		int32_t t2 = 3;
		int32_t res_a = __add(t1, t2);
		int32_t res_as = __adds(t1, t2);
		printf("[ADD] %i + %i = %i\n", t1, t2, res_a);
		printf("[ADDS] %i + %i = %i\n", t1, t2, res_as);
    }

    //Test Sub
    printf("\nTest Substract\n");
    {
		int32_t t1 = INT32_MIN;
		int32_t t2 = 3;
		int32_t res_s = __sub(t1, t2);
		int32_t res_ss = __subs(t1, t2);
		printf("[SUB] %i - %i = %i\n", t1, t2, res_s);
		printf("[SUBS] %i - %i = %i\n", t1, t2, res_ss);
    }

    //Test Multiplication
    printf("\nTest Multiplication\n");
    {
		int32_t t1 = INT32_MAX/2;
		int32_t t2 = 3;
		int32_t res_m = __mul(t1, t2);
		int32_t res_ms = __muls(t1, t2);
		printf("[MUL] %i * %i = %i\n", t1, t2, res_m);
		printf("[MULS] %i * %i = %i\n", t1, t2, res_ms);
    }

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
