/**
 * \file IfxCfg.h
 * \brief Driver configuration.
 *
 * \license
 * You can use this file under the terms of the IFX License.
 *
 * This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the IFX License for more details (IFX_License.txt).
 *
 * This file may be used, copied, and distributed, with or without modification, provided
 * that all copyright notices are retained; that all modifications to this file are
 * prominently noted in the modified file; and that this paragraph is not modified.
 *
 * \copyright Copyright (C) 2012 Infineon Technologies AG
 * \author Alann Denais <Alann.Denais@infineon.com>
 *
 * $Revision: 1170 $
 * $Date: 2012-11-16 13:12:10 +0100 (Fr, 16 Nov 2012) $
 *
 * \defgroup Ifx Infineon Software
 * \defgroup Ifx_Lld Infineon Low Level Drivers' Software
 * \ingroup Ifx
 */

#ifndef IFXCFG_H
#define IFXCFG_H
/******************************************************************************/

/*______________________________________________________________________________
** Configuration for IfxScu_cfg.h
**____________________________________________________________________________*/
#define IFX_CFG_SCU_XTAL_FREQUENCY		(20000000)	/**<allowed values 16000000, 20000000 or 40000000*/
#define IFX_CFG_SCU_PLL_FREQUENCY		(200000000) /**<allowed values 80000000, 133000000, 160000000 or 200000000 */

/*______________________________________________________________________________
** Configuration for Software managed interrupt
**____________________________________________________________________________*/

//#define IFX_USE_SW_MANAGED_INT

/*______________________________________________________________________________
** Configuration for Trap Hook Functions' Extensions
**____________________________________________________________________________*/

//#define IFX_CFG_EXTEND_TRAP_HOOKS

/******************************************************************************/
/*Disable the Core1*/
#define IFX_CFG_CPU_CSTART_ENABLE_TRICORE1    (1)

/*Disable the Core2*/
#define IFX_CFG_CPU_CSTART_ENABLE_TRICORE2    (1)
#endif /* IFX_CFG_H */
