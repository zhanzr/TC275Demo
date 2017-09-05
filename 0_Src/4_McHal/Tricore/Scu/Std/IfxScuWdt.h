/**
 * \file IfxScuWdt.h
 * \brief SCU  basic functionality
 * \ingroup IfxLld_Scu
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
 *
 * This file contains the APIs for SCU Watchdog and Endinit related functions.
 *
 * \defgroup IfxLld_Scu_Std_Wdt Wdt Basic Functionality
 * \ingroup IfxLld_Scu_Std
 * \defgroup IfxLld_Scu_Std_Wdt_Wdt_Configuration Watchdog Configuration functions
 * \ingroup IfxLld_Scu_Std_Wdt
 * \defgroup IfxLld_Scu_Std_Wdt_Wdt_Endinit Watchdog Endinit functions
 * \ingroup IfxLld_Scu_Std_Wdt
 * \defgroup IfxLld_Scu_Std_Wdt_Wdt_Operative Watchdog Operative functions
 * \ingroup IfxLld_Scu_Std_Wdt
 * \defgroup IfxLld_Scu_Std_Wdt_Wdt_Endinit_Usage How to use Endinit APIs?
 * \ingroup IfxLld_Scu_Std_Wdt
 */

#ifndef IFXSCUWDT_H
#define IFXSCUWDT_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxScu_cfg.h"
#include "Cpu/Std/Ifx_Types.h"
#include "IfxScuWdt.asm.h"

#include <tc_inc_path.h>

#include TC_INCLUDE(TCPATH/IfxScu_reg.h)

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \brief Configuration structure for Scu Watchdog.
 * IfxScuWdt_Config is a type describing configuration structure of CPU and
 * Safety WDT registers defined in IfxScuWdt.h file.
 */
typedef struct
{
    uint16            password;                          /**< \brief password for access to WDTxCON0 reg */
    uint16            reload;                            /**< \brief WDT reload value */
    IfxScu_WDTCON1_IR inputFrequency;                    /**< \brief input frequency of the WDT */
    boolean           disableWatchdog;                   /**< \brief Disable Request Control Bit */
    boolean           enableSmuRestriction;              /**< \brief Unlock Restriction Request Control Bit */
    boolean           enableAutomaticPasswordChange;     /**< \brief Password Auto-sequence Request Bit */
    boolean           enableTimerCheck;                  /**< \brief Counter Check Request Bit */
    boolean           enableTimerCheckTolerance;         /**< \brief Timer Check Tolerance Request */
    boolean           clrInternalResetFlag;              /**< \brief Clear Internal Reset Flag */
} IfxScuWdt_Config;

/** \addtogroup IfxLld_Scu_Std_Wdt_Wdt_Configuration
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief SCUWDT API to initialize WDT configuration structure - Constructor
 *   This API initialize the ScuWdt configuration structure to default values, these default
 *   values are for software specific, not necessarily hardware reset values.
 *   User must use this API call before IfxScuWdt_Init API call
 * \param config ScuWdt configuration structure to be initialized
 * \return None
 */
IFX_EXTERN void IfxScuWdt_initConfig(IfxScuWdt_Config *config);

/** \brief SCUWDT API to initialize an instance of WDT Driver which corresponds to CPU WDT Hardware module.
 *
 *   This API initialize the ScuWdt software driver and corresponding CPU WDT hardware module.
 *   User can configure project specific Watchdog password, Watchdog timer period and other settings
 *   with this interface.
 *   API corresponds to one instance of the hardware module.
 *   User must use this API call for each instance of the WDT hardware module used.
 * \param wdt reference to register map of CPU WDT hardware instance
 * \param config ScuWdt configuration structure
 * \return None
 */
IFX_EXTERN void IfxScuWdt_initCpuWatchdog(Ifx_SCU_WDTCPU *wdt, IfxScuWdt_Config *config);

/** \brief SCUWDT API to initialize an instance of WDT Driver which corresponds to Safety WDT Hardware module.
 *
 *   This API initialize the ScuWdt software driver and corresponding safety WDT hardware module.
 *   User can configure project specific Watchdog password, Watchdog timer period and other settings
 *   with this interface.
 *   API corresponds to one instance of the hardware module.
 *   User must use this API call for each instance of the WDT hardware module used.
 * \param wdt reference to register map of Safety WDT hardware instance
 * \param config ScuWdt configuration structure
 * \return None
 */
IFX_EXTERN void IfxScuWdt_initSafetyWatchdog(Ifx_SCU_WDTS *wdt, IfxScuWdt_Config *config);

/** \} */

/** \addtogroup IfxLld_Scu_Std_Wdt_Wdt_Endinit
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief SCUWDT Inline API to Clear ENDINIT bit provided by CPU WDT Hardware module.
 *
 * This Inline API will disable ENDINIT functionality provided by CPU WDT Hardware module.
 * This API is only meant to be used with startup routines where function call is not possible.
 * User need to use this API call before modifying any ENDINIT protected register. User must
 * always set the ENDINIT bit using other API IfxScuWdt_setCpuEndinit. The sequence clear and set
 * ENDINIT shall not be interrupted by another interrupt/call.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getCpuWatchdogPassword).
 * \param watchdog pointer to the watchdog register map of CPU WDT hardware instance
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_INLINE void IfxScuWdt_clearCpuEndinitInline(Ifx_SCU_WDTCPU *watchdog, uint16 password);

/** \brief SCUWDT Inline API to Clear ENDINIT bit provided by Safety WDT Hardware module.
 *
 * This API will disable ENDINIT functionality provided by Safety WDT Hardware module.
 * This API is only meant to be used with startup routines where function call is not possible.
 * User need to use this API call before modifying any ENDINIT protected register. User must
 * always set the ENDINIT bit using other API IfxScuWdt_setCpuEndinit. The sequence clear and set
 * ENDINIT shall not be interrupted by another interrupt/call.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getSafetyWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_INLINE void IfxScuWdt_clearSafetyEndinitInline(uint16 password);

/** \brief SCUWDT Inline API to Set ENDINIT bit provided by CPU WDT Hardware module.
 *
 * This API will enable ENDINIT functionality provided by CPU WDT Hardware module.
 * This API is only meant to be used with startup routines where function call is not possible.
 * User need to use this API call after modifying any ENDINIT protected register.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getCpuWatchdogPassword).
 * \param watchdog pointer to the watchdog register map of CPU WDT hardware instance
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_INLINE void IfxScuWdt_setCpuEndinitInline(Ifx_SCU_WDTCPU *watchdog, uint16 password);

/** \brief SCUWDT Inline API to Set ENDINIT bit provided by Safety WDT Hardware module.
 *
 * This API will enable ENDINIT functionality provided by Safety WDT Hardware module.
 * This API is only meant to be used with startup routines where function call is not possible.
 * User need to use this API call after modifying any ENDINIT protected register.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getSafetyWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_INLINE void IfxScuWdt_setSafetyEndinitInline(uint16 password);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief SCUWDT API to Clear ENDINIT bit provided by CPU WDT Hardware module.
 *
 *   This API will disable ENDINIT functionality provided by CPU WDT Hardware module.
 *   User need to use this API call before modifying any ENDINIT protected register. User must
 *   always set the ENDINIT bit using other API IfxScuWdt_setCpuEndinit. The sequence clear and set
 *   ENDINIT shall not be interrupted by another interrupt/call.
 *   User need to have the password stored locally in the caller function, (use IfxScuWdt_getCpuWatchdogPassword)
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_clearCpuEndinit(uint16 password);

/** \brief SCUWDT API to Clear ENDINIT bit provided by Safety WDT Hardware module.
 *
 * This API will disable ENDINIT functionality provided by Safety WDT Hardware module.
 * User need to use this API call before modifying any ENDINIT protected register. User must
 * always set the ENDINIT bit using other API IfxScuWdt_setCpuEndinit. The sequence clear and set
 * ENDINIT shall not be interrupted by another interrupt/call.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getSafetyWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_clearSafetyEndinit(uint16 password);

/** \brief SCUWDT API to set ENDINIT bit provided by CPU WDT Hardware module.
 *
 *   This API will enable ENDINIT functionality provided by CPU WDT Hardware module.
 *   User need to use this API call after modifying any ENDINIT protected register.
 *   User need to have the password stored locally in the caller function, (use IfxScuWdt_getCpuWatchdogPassword)
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_setCpuEndinit(uint16 password);

/** \brief SCUWDT API to Set ENDINIT bit provided by Safety WDT Hardware module.
 *
 * This API will enable ENDINIT functionality provided by Safety WDT Hardware module.
 * User need to use this API call after modifying any ENDINIT protected register.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getSafetyWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_setSafetyEndinit(uint16 password);

/** \} */

/** \addtogroup IfxLld_Scu_Std_Wdt_Wdt_Operative
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief SCUWDT Inline API to fetch current password of CPU Watchdog module.
 *
 * This API will fetch current Watchdog password for CPU WDT Hardware module. password is needed to be passed
 * with most of the WDT APIs. Normally this API can be used to store the password locally in the caller function
 * or store the password globally in a global variable at the application memory.
 * This API is only meant to be used with startup routines where function call is not possible.
 * \param watchdog pointer to the watchdog register map of CPU WDT hardware instance
 * \return password Existing (Application specific) password for the Watchdog module.
 */
IFX_INLINE uint16 IfxScuWdt_getCpuWatchdogPasswordInline(Ifx_SCU_WDTCPU *watchdog);

/** \brief SCUWDT API to fetch current endinit of Safety/System Watchdog module.
 * \return Endinit status for the Safety Watchdog module.
 */
IFX_INLINE boolean IfxScuWdt_getSafetyWatchdogEndInit(void);

/** \brief SCUWDT Inline API to fetch current password of Safety Watchdog module.
 *
 * This API will fetch current Watchdog password for Safety WDT Hardware module. password is needed to be passed
 * with most of the WDT APIs. Normally this API can be used to store the password locally in the caller function
 * or store the password globally in a global variable at the application memory.
 * This API is only meant to be used with startup routines where function call is not possible.
 * \return password Existing (Application specific) password for the Watchdog module.
 */
IFX_INLINE uint16 IfxScuWdt_getSafetyWatchdogPasswordInline(void);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief SCUWDT API to change CPU Watchdog password.
 *
 *   This API will change password to new one for the corresponding to CPU WDT Hardware module.
 *   User need to have the old password stored locally in the caller function, (use IfxScuWdt_getCpuWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \param newPassword Application specific new password to be changed for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_changeCpuWatchdogPassword(uint16 password, uint16 newPassword);

/** \brief SCUWDT API to change CPU Watchdog timer reload value.
 *
 * This API will change Watchdog timer reload value to new one for CPU WDT Hardware module.
 * The Watchdog timers will be reloaded with this value after every serice of Watchdog.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getCpuWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \param reload Reload value for the timer.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_changeCpuWatchdogReload(uint16 password, uint16 reload);

/** \brief SCUWDT API to change Safety Watchdog password.
 *
 * This API will change password to new one for the corresponding to Safety WDT Hardware module.
 * User need to have the old password stored locally in the caller function, (use IfxScuWdt_getSafetyWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \param newPassword Application specific new password to be changed for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_changeSafetyWatchdogPassword(uint16 password, uint16 newPassword);

/** \brief SCUWDT API to change Safety Watchdog timer reload value.
 *
 * This API will change Watchdog timer reload value to new one for Safety WDT Hardware module.
 * The Watchdog timers will be reloaded with this value after every serice of Watchdog.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getSafetyWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \param reload Reload value for the timer.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_changeSafetyWatchdogReload(uint16 password, uint16 reload);

/** \brief SCUWDT API to disable CPU Watchdog functionality.
 *
 * This API will disable Watchdog functionality of CPU WDT Hardware module. The Watchdog timers will stop counting
 * after this API call.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getCpuWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_disableCpuWatchdog(uint16 password);

/** \brief SCUWDT API to disable Safety Watchdog functionality.
 *
 * This API will disable Watchdog functionality of Safety WDT Hardware module. The Watchdog timers will stop counting
 * after this API call.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getSafetyWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_disableSafetyWatchdog(uint16 password);

/** \brief SCUWDT API to enable CPU Watchdog functionality.
 *
 *   This API will enable Watchdog functionality of CPU WDT Hardware module. The Watchdog timers need to be serviced
 *   periodically after this API call.
 *   User need to have the password stored locally in the caller function, (use IfxScuWdt_getCpuWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_enableCpuWatchdog(uint16 password);

/** \brief SCUWDT API to enable Safety Watchdog functionality.
 *
 * This API will enable Watchdog functionality of Safety WDT Hardware module. The Watchdog timers need to be serviced
 * periodically after this API call.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getSafetyWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_enableSafetyWatchdog(uint16 password);

/** \brief SCUWDT API to fetch current password of CPU Watchdog module.
 *
 * This API will fetch current Watchdog password for CPU WDT Hardware module. password is needed to be passed
 * with most of the WDT APIs. Normally this API can be used to store the password locally in the caller function
 * or store the password globally in a global variable at the application memory.
 * \return password Existing (Application specific) password for the Watchdog module.
 */
IFX_EXTERN uint16 IfxScuWdt_getCpuWatchdogPassword(void);

/** \brief SCUWDT API to fetch current password of Safety Watchdog module.
 *
 * This API will fetch current Watchdog password for Safety WDT Hardware module. password is needed to be passed
 * with most of the WDT APIs. Normally this API can be used to store the password locally in the caller function
 * or store the password globally in a global variable at the application memory.
 * \return password Existing (Application specific) password for the Watchdog module.
 */
IFX_EXTERN uint16 IfxScuWdt_getSafetyWatchdogPassword(void);

/** \brief SCUWDT API to service CPU Watchdog functionality.
 *
 *   This API will service Watchdog functionality corresponding to CPU WDT Hardware module.
 *   User need to use this API call periodically. This API results in reloading of the Watchdog Timer.
 *   User need to have the password stored locally in the caller function, (use IfxScuWdt_getCpuWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_serviceCpuWatchdog(uint16 password);

/** \brief SCUWDT API to service Safety Watchdog functionality.
 *
 * This API will service Watchdog functionality corresponding to Safety WDT Hardware module.
 * User need to use this API call periodically. This API results in reloading of the Watchdog Timer.
 * User need to have the password stored locally in the caller function, (use IfxScuWdt_getSafetyWatchdogPassword).
 * \param password Existing (Application specific) password for the Watchdog module.
 * \return None
 */
IFX_EXTERN void IfxScuWdt_serviceSafetyWatchdog(uint16 password);

/** \} */

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxScuWdt_clearCpuEndinitInline(Ifx_SCU_WDTCPU *watchdog, uint16 password)
{
    /* Read Config_0 register */
    Ifx_SCU_WDTCPU_CON0 wdt_con0;
    wdt_con0.U = watchdog->CON0.U;

    if (wdt_con0.B.LCK)
    {
        /* see Table 1 (Pass.word Access Bit Pattern Requirements) */
        wdt_con0.B.ENDINIT = 1;
        wdt_con0.B.LCK     = 0;
        wdt_con0.B.PW      = password;

        /* Password ready. Store it to WDT_CON0 to unprotect the register */
        watchdog->CON0.U = wdt_con0.U;
    }

    /* Clear ENDINT and set LCK bit in Config_0 register */
    wdt_con0.B.ENDINIT = 0;
    wdt_con0.B.LCK     = 1;
    watchdog->CON0.U   = wdt_con0.U;

    /* read back ENDINIT and wait until it has been cleared */
    while (watchdog->CON0.B.ENDINIT == 1)
    {}
}


IFX_INLINE void IfxScuWdt_clearSafetyEndinitInline(uint16 password)
{
    Ifx_SCU_WDTS     *watchdog = &MODULE_SCU.WDTS;
    /* Read Config_0 register */
    Ifx_SCU_WDTS_CON0 wdt_con0;
    wdt_con0.U = watchdog->CON0.U;

    if (wdt_con0.B.LCK)
    {
        /* see Table 1 (Password Access Bit Pattern Requirements) */
        wdt_con0.B.ENDINIT = 1;
        wdt_con0.B.LCK     = 0;
        wdt_con0.B.PW      = password;

        /* Password ready. Store it to WDT_CON0 to unprotect the register */
        watchdog->CON0.U = wdt_con0.U;
    }

    /* Clear ENDINT and set LCK bit in Config_0 register */
    wdt_con0.B.ENDINIT = 0;
    wdt_con0.B.LCK     = 1;
    watchdog->CON0.U   = wdt_con0.U;

    /* read back ENDINIT and wait until it has been cleared */
    while (watchdog->CON0.B.ENDINIT == 1)
    {}
}


IFX_INLINE uint16 IfxScuWdt_getCpuWatchdogPasswordInline(Ifx_SCU_WDTCPU *watchdog)
{
    uint16 password;

    /* Read Password from CON0 register
     * !!! NOTE: !!! when read bottom six bit of password are inverted so we have
     * to toggle them before returning password */
    password  = watchdog->CON0.B.PW;
    password ^= 0x003F;

    return password;
}


IFX_INLINE boolean IfxScuWdt_getSafetyWatchdogEndInit(void)
{
    return (boolean)MODULE_SCU.WDTS.CON0.B.ENDINIT;
}


IFX_INLINE uint16 IfxScuWdt_getSafetyWatchdogPasswordInline(void)
{
    uint16        password;
    Ifx_SCU_WDTS *watchdog = &MODULE_SCU.WDTS;

    /* Read Password from Safety WDT CON0 register
     * !!! NOTE: !!! when read bottom six bit of password are inverted so we have
     * to toggle them before returning password */
    password  = watchdog->CON0.B.PW;
    password ^= 0x003F;

    return password;
}


IFX_INLINE void IfxScuWdt_setCpuEndinitInline(Ifx_SCU_WDTCPU *watchdog, uint16 password)
{
    /* Read Config_0 register */
    Ifx_SCU_WDTCPU_CON0 wdt_con0;
    wdt_con0.U = watchdog->CON0.U;

    if (wdt_con0.B.LCK)
    {
        /* see Table 1 (Password Access Bit Pattern Requirements) */
        wdt_con0.B.ENDINIT = 1;
        wdt_con0.B.LCK     = 0;
        wdt_con0.B.PW      = password;

        /* Password ready. Store it to WDT_CON0 to unprotect the register */
        watchdog->CON0.U = wdt_con0.U;
    }

    /* Set ENDINT and set LCK bit in Config_0 register */
    wdt_con0.B.ENDINIT = 1;
    wdt_con0.B.LCK     = 1;
    watchdog->CON0.U   = wdt_con0.U;

    /* read back ENDINIT and wait until it has been set */
    while (watchdog->CON0.B.ENDINIT == 0)
    {}

    // FIXME: old version: removed this line after check: watchdog->CON0.U; /* read is required */
}


IFX_INLINE void IfxScuWdt_setSafetyEndinitInline(uint16 password)
{
    Ifx_SCU_WDTS     *watchdog = &MODULE_SCU.WDTS;

    /* Read Config_0 register */
    Ifx_SCU_WDTS_CON0 wdt_con0;
    wdt_con0.U = watchdog->CON0.U;

    if (wdt_con0.B.LCK)
    {
        /* see Table 1 (Password Access Bit Pattern Requirements) */
        wdt_con0.B.ENDINIT = 1;
        wdt_con0.B.LCK     = 0;
        wdt_con0.B.PW      = password;

        /* Password ready. Store it to WDT_CON0 to unprotect the register */
        watchdog->CON0.U = wdt_con0.U;
    }

    /* Set ENDINT and set LCK bit in Config_0 register */
    wdt_con0.B.ENDINIT = 1;
    wdt_con0.B.LCK     = 1;
    watchdog->CON0.U   = wdt_con0.U;

    /* read back ENDINIT and wait until it has been set */
    while (watchdog->CON0.B.ENDINIT == 0)
    {}
}


#endif /* IFXSCUWDT_H */
