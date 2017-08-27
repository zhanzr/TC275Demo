/**
 * \file IfxMsc_cfg.h
 * \brief MSC on-chip implementation data
 * \ingroup IfxLld_Msc
 *
 * \version iLLD_1_0_0_8_0
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
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
 * \defgroup IfxLld_Msc MSC
 * \ingroup IfxLld
 * \defgroup IfxLld_Msc_Impl Implementation
 * \ingroup IfxLld_Msc
 * \defgroup IfxLld_Msc_Std Standard Driver
 * \ingroup IfxLld_Msc
 */

#ifndef IFXMSC_CFG_H
#define IFXMSC_CFG_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "Cpu/Std/Ifx_Types.h"
#include "IfxMsc_reg.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

#define IFXMSC_NUM_MODULES             2

#define IFXMSC_NUM_ENABLE_SELECT_LINES (4)

/******************************************************************************/
/*-------------------------------Enumerations---------------------------------*/
/******************************************************************************/

/** \brief List of the available MSC resources
 */
typedef enum
{
    IfxMsc_ResourceMsc_0    = 0,  /**< \brief MSC 0 */
    IfxMsc_ResourceMsc_1    = 1,  /**< \brief MSC 1 */
    IfxMsc_ResourceMsc_none = -1  /**< \brief None of the MSC */
} IfxMsc_ResourceMsc;

/******************************************************************************/
/*-------------------Global Exported Variables/Constants----------------------*/
/******************************************************************************/

IFX_EXTERN const IfxModule_IndexMap IfxMsc_cfg_indexMap[IFXMSC_NUM_MODULES];

#endif /* IFXMSC_CFG_H */
