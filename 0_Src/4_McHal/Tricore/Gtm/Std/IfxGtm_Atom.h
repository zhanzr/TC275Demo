/**
 * \file IfxGtm_Atom.h
 * \brief GTM  basic functionality
 * \ingroup IfxLld_Gtm
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
 *
 * \defgroup IfxLld_Gtm_Std_Atom Atom Basic Functionality
 * \ingroup IfxLld_Gtm_Std
 * \defgroup IfxLld_Gtm_Std_Atom_Enumerations ATOM Enumerations
 * \ingroup IfxLld_Gtm_Std_Atom
 * \defgroup IfxLld_Gtm_Std_Atom_Channel_Functions ATOM Channel Functions
 * \ingroup IfxLld_Gtm_Std_Atom
 * \defgroup IfxLld_Gtm_Std_Atom_AGC_Functions ATOM AGC Functions
 * \ingroup IfxLld_Gtm_Std_Atom
 */

#ifndef IFXGTM_ATOM_H
#define IFXGTM_ATOM_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxGtm_cfg.h"
#include "IfxGtm.h"
#include "IfxGtm_Tbu.h"
#include "Src/Std/IfxSrc.h"
#include "IfxGtm_Cmu.h"

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Gtm_Std_Atom_Enumerations
 * \{ */
/** \brief Reset source for channel counter CN0
 */
typedef enum
{
    IfxGtm_Atom_Ch_OutputTrigger_forward  = 0, /**< \brief Forward the trigger from the previous channel */
    IfxGtm_Atom_Ch_OutputTrigger_generate = 1  /**< \brief Generate the trigger from the current channel */
} IfxGtm_Atom_Ch_OutputTrigger;

/** \brief Reset event for channel counter CN0
 */
typedef enum
{
    IfxGtm_Atom_Ch_ResetEvent_onCm0     = 0,
    IfxGtm_Atom_Ch_ResetEvent_onTrigger = 1
} IfxGtm_Atom_Ch_ResetEvent;

/** \brief Enum for ATOM mode
 */
typedef enum
{
    IfxGtm_Atom_Mode_outputImmediate,
    IfxGtm_Atom_Mode_outputCompare,
    IfxGtm_Atom_Mode_outputPwm,
    IfxGtm_Atom_Mode_outputSerial
} IfxGtm_Atom_Mode;

/** \} */

/** \addtogroup IfxLld_Gtm_Std_Atom_Channel_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Clears the channel One notification
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_clearOneNotification(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Clears the channel Zero notification
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_clearZeroNotification(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param clock Clock source
 * \param activeState Active State
 * \param resetEvent Channel reset event
 * \param trigger Channel trigger output mode
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_configurePwmMode(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, IfxGtm_Cmu_Clk clock, Ifx_ActiveState activeState, IfxGtm_Atom_Ch_ResetEvent resetEvent, IfxGtm_Atom_Ch_OutputTrigger trigger);

/** \brief Returns the ATOM channel input clock frequency in Hz
 * \param gtm Pointer to GTM module
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return ATOM channel input clock frequency in Hz
 */
IFX_EXTERN float32 IfxGtm_Atom_Ch_getClockFrequency(Ifx_GTM *gtm, Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Returns the channel clock source
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return The clock source
 */
IFX_EXTERN IfxGtm_Cmu_Clk IfxGtm_Atom_Ch_getClockSource(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Returns the compare one value
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return compare one value
 */
IFX_EXTERN uint32 IfxGtm_Atom_Ch_getCompareOne(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Returns the compare one pointer
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return compare one pointer
 */
IFX_EXTERN volatile uint32 *IfxGtm_Atom_Ch_getCompareOnePointer(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Returns the compare zero value
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return compare zero value
 */
IFX_EXTERN uint32 IfxGtm_Atom_Ch_getCompareZero(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Returns the compare zero pointer
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return compare zero pointer
 */
IFX_EXTERN volatile uint32 *IfxGtm_Atom_Ch_getCompareZeroPointer(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Gets the ATOM output level
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return TRUE the output is high, FALSE the output is low
 */
IFX_EXTERN boolean IfxGtm_Atom_Ch_getOutputLevel(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Returns a pointer to the ATOM channel SRC
 * \param gtm Pointer to GTM module
 * \param atom Specifies the atom object
 * \param channel Channel index
 * \return Pointer to the TOM channel SRC
 */
IFX_EXTERN volatile Ifx_SRC_SRCR *IfxGtm_Atom_Ch_getSrcPointer(Ifx_GTM *gtm, IfxGtm_Atom atom, IfxGtm_Atom_Ch channel);

/** \brief Returns the Timer pointer
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return Timer pointer
 */
IFX_EXTERN volatile uint32 *IfxGtm_Atom_Ch_getTimerPointer(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Returns the status of channel One notification
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return Status of channel One notification
 */
IFX_EXTERN boolean IfxGtm_Atom_Ch_isOneNotification(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Returns the status of channel Zero notification
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return Status of channel Zero notification
 */
IFX_EXTERN boolean IfxGtm_Atom_Ch_isZeroNotification(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Raises the interrupt for Compare 1
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_raiseInterruptOne(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Raises the interrupt for Compare 0
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_raiseInterruptZero(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel);

/** \brief Sets the channel clock source
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param clock Channel clock source
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setClockSource(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, IfxGtm_Cmu_Clk clock);

/** \brief Sets the compare 0 and 1 values
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param compareZero Compare zero value
 * \param compareOne Compare one value
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setCompare(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, uint32 compareZero, uint32 compareOne);

/** \brief Sets the compare 1 value
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param compareOne Compare one value
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setCompareOne(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, uint32 compareOne);

/** \brief Sets the compare 1 shadow value
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param shadowOne Compare one shadow value
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setCompareOneShadow(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, uint32 shadowOne);

/** \brief Sets the compare 0 and 1 shadow values
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param shadowZero Compare zero shadow value
 * \param shadowOne Compare one shadow value
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setCompareShadow(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, uint32 shadowZero, uint32 shadowOne);

/** \brief Sets the compare 0 value
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param compareZero Compare zero value
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setCompareZero(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, uint32 compareZero);

/** \brief Sets the compare 0 shadow value
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param shadowZero Compare zero shadow value
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setCompareZeroShadow(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, uint32 shadowZero);

/** \brief Sets the counter value
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param value Counter value
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setCounterValue(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, uint32 value);

/** \brief Sets the channel notification
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param mode Interrupt mode
 * \param interruptOnCompareZero If TRUE, an interrupt is generated on compare 0, else no interrupt is generated
 * \param interruptOnCompareOne If TRUE, an interrupt is generated on compare 1, else no interrupt is generated
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setNotification(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, IfxGtm_IrqMode mode, boolean interruptOnCompareZero, boolean interruptOnCompareOne);

/** \brief Enable/disable the one shot mode
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param enabled If TRUE, the feature is enabled, else disabled
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setOneShotMode(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, boolean enabled);

/** \brief Sets the channel clock source either from local or from previous channel
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param event Channel reset event
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setResetSource(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, IfxGtm_Atom_Ch_ResetEvent event);

/** \brief Sets the signal level
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param activeState Signal level active state. In case the channel is reset, the output is set to not active. The signal is active between 0 and the leading edge (CM1) and inactive between the leading edge and the trailing edge (CM0).
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setSignalLevel(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, Ifx_ActiveState activeState);

/** \brief Sets the channel trigger output
 * \param atom Pointer to the ATOM object
 * \param channel Channel index
 * \param trigger Channel trigger output mode
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Ch_setTriggerOutput(Ifx_GTM_ATOM *atom, IfxGtm_Atom_Ch channel, IfxGtm_Atom_Ch_OutputTrigger trigger);

/** \} */

/** \addtogroup IfxLld_Gtm_Std_Atom_AGC_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Enable/disable one or more channels (ENDIS)
 * \param agc Pointer to the AGC object
 * \param enableMask Mask for the channel feature enable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \param disableMask Mask for the channel feature disable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \param immediate If TRUE, the action is done immediately else, the action is done on TGC trigger (CTRL_TRIG)
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Agc_enableChannels(Ifx_GTM_ATOM_AGC *agc, uint16 enableMask, uint16 disableMask, boolean immediate);

/** \brief Enable/disable one or more channels output (OUTEN)
 * \param agc Pointer to the TGC object
 * \param enableMask Mask for the channel feature enable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \param disableMask Mask for the channel feature disable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \param immediate If TRUE, the action is done immediately else, the action is done on TGC trigger (CTRL_TRIG)
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Agc_enableChannelsOutput(Ifx_GTM_ATOM_AGC *agc, uint16 enableMask, uint16 disableMask, boolean immediate);

/** \brief Enable/disable the TGC channels trigger
 * \param agc Pointer to the AGC object
 * \param enableMask Mask for the channel feature enable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \param disableMask Mask for the channel feature disable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Agc_enableChannelsTrigger(Ifx_GTM_ATOM_AGC *agc, uint16 enableMask, uint16 disableMask);

/** \brief Enable/disable one or more channels for update (UPEN)
 * \param agc Pointer to the AGC object
 * \param enableMask Mask for the channel feature enable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \param disableMask Mask for the channel feature disable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Agc_enableChannelsUpdate(Ifx_GTM_ATOM_AGC *agc, uint16 enableMask, uint16 disableMask);

/** \brief Enable/disable the time base trigger
 * \param agc Pointer to the AGC object
 * \param enabled If TRUE, the trigger is enabled else disabled
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Agc_enableTimeTrigger(Ifx_GTM_ATOM_AGC *agc, boolean enabled);

/** \brief Reset one or more channels
 * \param agc Pointer to the AGC object
 * \param resetMask Mask for the channel reset (bit 0: Channel 0, bit 1: channel 1, ...)
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Agc_resetChannels(Ifx_GTM_ATOM_AGC *agc, uint32 resetMask);

/** \brief Enable/disable one or more channels for the force update feature (FUPD)
 * \param agc Pointer to the AGC object
 * \param enableMask Mask for the channel feature enable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \param disableMask Mask for the channel feature disable (bit 0: Channel 0, bit 1: channel 1, ...)
 * \param resetEnableMask Mask for the enabled channels counter reset on force update (bit 0: Channel 0, bit 1: channel 1, ...) Channel 0, bit 1: channel 1, ...)
 * \param resetDisableMask Mask for the disabled channels with no counter reset on force update (bit 0: Channel 0, bit 1: channel 1, ...)
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Agc_setChannelsForceUpdate(Ifx_GTM_ATOM_AGC *agc, uint16 enableMask, uint16 disableMask, uint16 resetEnableMask, uint16 resetDisableMask);

/** \brief Sets the trigger time base and time base value
 * \param agc Pointer to the AGC object
 * \param base Time base used for comparison
 * \param value Compare value that raise the trigger
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Agc_setTimeTrigger(Ifx_GTM_ATOM_AGC *agc, IfxGtm_Tbu_Ts base, uint32 value);

/** \brief Raise the trigger for the channel enable/disable settings,  output enable settings, and force update event (CTRL_TRIG)
 * \param agc Pointer to the AGC object
 * \return None
 */
IFX_EXTERN void IfxGtm_Atom_Agc_trigger(Ifx_GTM_ATOM_AGC *agc);

/** \} */

#endif /* IFXGTM_ATOM_H */
