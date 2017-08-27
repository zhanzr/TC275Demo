/**
 * \file Cpu_Main.h
 * \brief System initialization and main program implementation.
 *
 * \version V0.1
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
 */

#ifndef CPU_MAIN_H
#define CPU_MAIN_H

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/
#include "Configuration.h"
#include <Cpu/Std/Ifx_Types.h>
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Type Definitions------------------------------*/
/******************************************************************************/
typedef struct
{
    float32 sysFreq;                /**< \brief Actual SPB frequency */
    float32 cpuFreq;                /**< \brief Actual CPU frequency */
    float32 pllFreq;                /**< \brief Actual PLL frequency */
    float32 stmFreq;                /**< \brief Actual STM frequency */
} AppInfo;

/** \brief Application information */
typedef struct
{
    AppInfo info;                               /**< \brief Info object */
    uint32 TickCount_1ms;                       /**< \brief TickCount 1ms */
} App_Cpu;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/
IFX_EXTERN App_Cpu g_AppCpu0;

#if defined(__DCC__)
#pragma section DATA ".data_cpu1" ".bss_cpu1" far-absolute RW
#endif

IFX_EXTERN App_Cpu g_AppCpu1;

#if defined(__DCC__)
#pragma section DATA ".data_cpu2" ".bss_cpu2" far-absolute RW
#endif

IFX_EXTERN App_Cpu g_AppCpu2;

#endif
