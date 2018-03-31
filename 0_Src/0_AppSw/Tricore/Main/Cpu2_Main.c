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
#include <machine/cint.h>

#include "lcd2004.h"

extern IfxCpu_mutexLock g_Asc0_Lock;

extern volatile int32_t g_share_i32;

int core2_main (void)
{
	uint32_t tmpTick;

    IfxCpu_enableInterrupts();

    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());

	printf("Trap Demo %u Hz\n", SYSTEM_GetCpuClock());

	_install_trap_handler (0, (void (*) (int)) class0_tsr);
	_install_trap_handler (1, (void (*) (int)) class1_tsr);
	_install_trap_handler (2, (void (*) (int)) class2_tsr);
	_install_trap_handler (3, (void (*) (int)) class3_tsr);
	_install_trap_handler (4, (void (*) (int)) class4_tsr);
	_install_trap_handler (5, (void (*) (int)) class5_tsr);
	_install_trap_handler (6, (void (*) (int)) class6_tsr);
	_install_trap_handler (7, (void (*) (int)) class7_tsr);

	//Test Syscall
	_syscall(4);
	_syscall(7);

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
