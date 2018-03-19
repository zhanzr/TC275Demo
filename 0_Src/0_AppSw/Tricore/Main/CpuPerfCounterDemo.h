/**
 * \file CpuPerfCounterDemo.h
 * \brief Cpu Performance counter Demo
 *
 * \version iLLD_Demos_1_0_1_3_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Demo_CpuPerfCounterDemo_SrcDoc_Main Demo Source
 * \ingroup IfxLld_Demo_CpuPerfCounterDemo_SrcDoc
 * \defgroup IfxLld_Demo_CpuPerfCounterDemo_SrcDoc_Main_Interrupt Interrupts
 * \ingroup IfxLld_Demo_CpuPerfCounterDemo_SrcDoc_Main
 */

#ifndef CPUPERFCOUNTERDEMO_H
#define CPUPERFCOUNTERDEMO_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include <Ifx_Types.h>

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
IFX_EXTERN void IfxCpuPerfCounterDemo_init(void);
IFX_EXTERN void IfxCpuPerfCounterDemo_run(void);

#endif
