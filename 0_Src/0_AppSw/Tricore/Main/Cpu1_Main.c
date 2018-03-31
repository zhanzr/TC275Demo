/**
 * \file Cpu1_Main.c
 * \brief Main function definition for Cpu core 1 .
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
#include <Dts/Dts/IfxDts_Dts.h>
#include "main.h"

#include "lcd2004.h"

extern IfxCpu_mutexLock g_Asc0_Lock;

extern volatile int32_t g_share_i32;

extern volatile float32 g_DieTemp;

int core1_main (void)
{
	uint32_t tmpTick;

    IfxCpu_enableInterrupts();
    /*
     * !!WATCHDOG1 IS DISABLED HERE!!
     * Enable the watchdog in the demo if it is required and also service the watchdog periodically
     * */
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());

	_install_trap_handler (0, (void (*) (int)) class0_tsr);
	_install_trap_handler (1, (void (*) (int)) class1_tsr);
	_install_trap_handler (2, (void (*) (int)) class2_tsr);
	_install_trap_handler (3, (void (*) (int)) class3_tsr);
	_install_trap_handler (4, (void (*) (int)) class4_tsr);
	_install_trap_handler (5, (void (*) (int)) class5_tsr);
	_install_trap_handler (6, (void (*) (int)) class6_tsr);
	_install_trap_handler (7, (void (*) (int)) class7_tsr);

//	//Test Unaligned access
//	uint32_t testU32 = 0x11223344;
//	printf("%04X\n", *(uint16_t*)(((uint8_t*)testU32)+1));

	//Test Syscall
	_syscall(2);
	_syscall(3);

    while (1)
    {
//        /* start Sensor */
//        IfxDts_Dts_startSensor();
//
//    	/* wait until a new result is available */
//    	while (IfxDts_Dts_isBusy())
//    	{}
//
//    	/* convert result to Celsius */
//    	g_DieTemp = IfxDts_Dts_getTemperatureCelsius();
    	while(IfxCpu_acquireMutex(&g_Asc0_Lock))
    	{
//    		printf("Cpu%d:%u Hz, Sys:%u Hz, Stm:%u Hz, Core:%04X,  %u\n"\
//    				"DTS Temperature: %3.1f'C",
//    				IfxCpu_getCoreId(),
//					SYSTEM_GetCpuClock(),
//					SYSTEM_GetSysClock(),
//					SYSTEM_GetStmClock(),
//					__TRICORE_CORE__,
//					schd_GetTick(),
//					g_DieTemp
//    		);
//    		printf("%.2f\n", g_DieTemp);
//    		g_share_i32 --;
    		g_share_i32 ++;

    		IfxCpu_releaseMutex(&g_Asc0_Lock);
    	}
    }

    return (1);
}
