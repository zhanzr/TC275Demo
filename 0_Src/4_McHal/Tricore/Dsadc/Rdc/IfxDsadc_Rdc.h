/*MCMETILLD-748*/
/**
 * \file IfxDsadc_Rdc.h
 * \brief DSADC-based resolver-to-digital converter (RDC) support function
 * \ingroup IfxLld_Dsadc_rdc
 *
 *
 *
 * \version disabled
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
 * \defgroup IfxLld_Dsadc_rdc Resolver-to-digital converter with DSADC
 * Please refer to page \ref page_dsadc_rdc for more details.
 * This module uses the following modules:
 * - \ref IfxLld_Dsadc
 * - \ref library_srvsw_sysse_math_f32_angletrk
 * \ingroup IfxLld_Dsadc
 */

/* *INDENT-OFF* Note: this file was indented manually by the author. */

#ifndef IFX_RDC_DSADC_H
#define IFX_RDC_DSADC_H

#define IFXDSADC_HW_INIT 1

/*________________________________________________________________________________________
// INCLUDE FILES */

#include "Ifx_AngleTrkF32.h"
#include "Dsadc/Dsadc/IfxDsadc_Dsadc.h"

#include "Gtm/Std/IfxGtm_Tim.h"
#include "_PinMap/IfxGtm_PinMap.h"
#include "Ifx_Cfg.h"

/*________________________________________________________________________________________
// CONFIGURATION DEFINES */

#ifndef IFX_CFG_DSADC_RDC_PRE_OBSERVER_CORRECTION
#define IFX_CFG_DSADC_RDC_PRE_OBSERVER_CORRECTION  (1)
#endif

#ifndef IFX_CFG_DSADC_RDC_DEBUG
#define IFX_CFG_DSADC_RDC_DEBUG                    (0)
#endif

/*________________________________________________________________________________________
// DATA STRUCTURES */

/** DSADC timestamp helper using GTM */
typedef struct
{
    Ifx_GTM           *gtm;
    IfxGtm_Tim         rdcTim;
    IfxGtm_Tim_Ch      rdcTimChannel;   /**< \brief TIM channel triggered by DSADC channel */
    uint32             rdcTimMuxValue;  /**< \brief Mux value used */
    IfxGtm_Tim_TinMap *pwmTim;          /**< \brief TIM channel triggered by PWM trigger */
} IfxDsadc_GtmTimestamp;

/** DSADC RDC hardware object structure */
typedef struct
{
    IfxDsadc_Dsadc_Channel inputSin;    /**< \brief Handle to SIN input channel */
    IfxDsadc_Dsadc_Channel inputCos;    /**< \brief Handle to COS input channel */
    Ifx_GTM_TIM_CH        *pwmTimCh;    /**< \brief TIM channel for PWM timestamp */
    Ifx_GTM_TIM_CH        *rdcTimCh;    /**< \brief TIM channel for DSADC timestamp */
} IfxDsadc_Rdc_Hw;

/** DSADC RDC hardware configuration */
typedef struct
{
    /** \brief Default channel configuration for both SIN and COS input. */
    IfxDsadc_Dsadc_ChannelConfig   inputConfig;

    IfxDsadc_Dsp_In  *inputSinP;           /**< \brief DSADC  SIN input, Channel ID must match inputSinN. This will override value given in the inputConfig */
    IfxDsadc_Dsn_In  *inputSinN;           /**< \brief DSADC  SIN input, Channel ID must match inputSinP. This will override value given in the inputConfig */
    IfxDsadc_Dsp_In  *inputCosP;           /**< \brief DSADC  COS input, Channel ID must match inputCosN. This will override value given in the inputConfig */
    IfxDsadc_Dsn_In  *inputCosN;           /**< \brief DSADC  COS input, Channel ID must match inputCosP. This will override value given in the inputConfig */
    Ifx_Priority        servReqPriority;    /**< \brief Interrupt priority. NOTE: Only used by inputSin and will override value given in the inputConfig */
    IfxSrc_Tos          servReqProvider;    /**< \brief Service provider. NOTE: Only used by inputSin and will override value given in the inputConfig */
    boolean             startScan;          /**< \brief Specifies whether the conversion will be started as soon as the IfxDsadc_Rdc_initHw() is called */

    /** \brief Pointer to the carrier signal generation configuration
     * \note If NULL_PTR, it is assumed that the carrier generation has been pre-configured */
    IfxDsadc_Dsadc_CarrierGenConfig carrierGen;

    IfxDsadc_Cout_Out      *outputClock;    /**< \brief if not NULL_PTR, modulator clock output pin from inputSin channel will be configured */
    IfxDsadc_GtmTimestamp   gtmTimestamp;   /**< \brief Timestamp helper using GTM */
} IfxDsadc_Rdc_ConfigHw;

typedef struct
{
    boolean       enabled;
    float32       inSeconds;

    uint32        rdc;                /**< \brief Absolute time of DSADC result */

    uint32        pwm;                /**< \brief Absolute time of user's PWM trigger. Used only if IFX_CFG_DSADC_RDC_DEBUG is set  */
    sint32        inTicks;            /**< \brief Time-stamp of last result (in ticks). Used only if IFX_CFG_DSADC_RDC_DEBUG is set  */

    uint32        maxTicks;           /**< \brief Maximum time-stamp value (in ticks of absolute time clock) */

    float32       clockPeriod;        /**< \brief Period of absolute time clock (in second) */
} IfxDsadc_Rdc_Ts;

/** DSADC RDC data structure */
typedef struct
{
    Ifx_AngleTrkF32        angleTrk;
    sint16          sinIn;
    sint16          cosIn;
    float32         updatePeriod;
    float32         groupDelay;
    IfxDsadc_Rdc_Hw hardware;
    IfxDsadc_Rdc_Ts timestamp;
} IfxDsadc_Rdc;

/** DSADC RDC configuration structure
 * Note: if ALL kp, ki, kd are zero, then the init function will use default value. */
typedef struct
{
    float32     kp;                 /**< \brief Observer proportional gain */
    float32     ki;                 /**< \brief Observer integral gain */
    float32     kd;                 /**< \brief Observer differential gain */
    float32     speedLpfFc;         /**< \brief Cut-off frequency of speed low-pass filter (in Hertz). */
    float32     errorThreshold;     /**< \brief Threshold of error value in the tracking loop (in radian) */
    float32     userTs;             /**< \brief Sampling period of the application */
    sint32      sqrAmplMax;         /**< \brief Maximum value for square of signal amplitudes */
    sint32      sqrAmplMin;         /**< \brief Minimum value for square of signal amplitudes */
    uint16      periodPerRotation;  /**< \brief Number of electrical periods per mechanical rotation */
    boolean     reversed;           /**< \brief TRUE: reversed direction, FALSE: straight direction */
    sint32 resolution;                              /**< \brief Sensor resolution */
    IfxStdIf_Pos_RawAngle   offset;             /**< \brief Offset in ticks. [0 .. (\ref IfxDsadc_Rdc_Config.resolution - 1)] */
    IfxDsadc_Rdc_ConfigHw hardware;     /**< \brief Pointer to hardware config. */
    IfxDsadc_Dsadc *dsadc;			/**< \brief Pointer to the DSADC driver */
} IfxDsadc_Rdc_Config;

/*________________________________________________________________________________________
// FUNCTION PROTOTYPES */

/** \addtogroup IfxLld_Dsadc_rdc
 * \{ */
/** \name Main functions
 *
 * Initialisation shall be done by using IfxDsadc_Rdc_init().
 * \code
 * extern IfxDsadc_Rdc rdcHandle;
 * extern const IfxDsadc_Rdc_Config rdcConfig;
 *
 * IfxDsadc_Rdc_init(&rdcHandle, &rdcConfig);
 * IfxDsadc_Rdc_startConversion(&rdcHandle);
 *
 * \endcode
 *
 * After successful initialisation, one DSADC channel may generate interrupt request to CPU.
 * Function IfxDsadc_Rdc_onEventA() shall be called in the interrupt service routine.
 *
 * The function IfxDsadc_Rdc_update() shall be called in the user application's task
 * or interrupt context for updating the final outputs (position and speed),
 * (e.g. running inside motor PWM interrupt context)
 *
 * Prototypes:
 * \{ */
IFX_EXTERN boolean   IfxDsadc_Rdc_init              (IfxDsadc_Rdc *handle, const IfxDsadc_Rdc_Config *config);
IFX_EXTERN void      IfxDsadc_Rdc_initConfig        (IfxDsadc_Rdc_Config *config);
IFX_EXTERN void      IfxDsadc_Rdc_startConversion   (IfxDsadc_Rdc *handle);
/** \} */
/**
 * \name Getting result functions
 *
 * Example usage
 * \code
 * {  // within context of user's application interrupt or task
 *     extern IfxDsadc_Rdc rdcHandle;
 *
 *     IfxDsadc_Rdc_update(&rdcHandle); // final update
 *
 *     IfxStdIf_Pos_RawAngle position = IfxDsadc_Rdc_getRawPosition(&rdcHandle);
 *     StdReal speedRad_s = IfxDsadc_Rdc_getSpeed(&rdcHandle);
 * }
 * \endcode
 *
 * Alternatively, the application may use \ref library_srvsw_stdif_posif for accessing the actual results.
 *
 * \code
 * {   // within context of user's application interrupt or task
 *     extern IfxDsadc_Rdc rdcHandle;
 *     Ifx_AngleTrkF32_PosIf* posIf = (Ifx_AngleTrkF32_PosIf*)&rdcHandle; // cast is safe because it's located in the same address
 *
 *     PosIf_update(posIf); // final update, alias of IfxDsadc_Rdc_update()
 *
 *     elAngle = PosIf_getElAngle(posIf);
 *     elSpeed = PosIf_getElSpeed(posIf);
 *     faultOccurred = PosIf_isFault(posIf);
 * }
 * \endcode
 *
 * \note the IfxDsadc_Rdc_update() is implementing the PosIf_update(). Therefore,
 * only one of them shall be used.
 *
  */
/** \name Auxiliary functions
 * These functions are used by the main functions above
 * \{ */
IFX_INLINE float32   IfxDsadc_Rdc_getTimestamp    (IfxDsadc_Rdc *handle);
/** \} */
/** \} */

/* *INDENT-ON* */

//________________________________________________________________________________________
// INLINE FUNCTION IMPLEMENTATIONS

/** Get the actual position.
 *
 * The periods per mechanical rotation is defined in the \ref IfxDsadc_Rdc_Config at
 * initialisation (call to IfxDsadc_Rdc_init()).
 *
 * \param handle Pointer to IfxDsadc_Rdc object (RAM location)
 * \return actual position. Range is within 0 .. (\ref IfxDsadc_Rdc.base.resolution - 1)
 */
IFX_INLINE IfxStdIf_Pos_RawAngle IfxDsadc_Rdc_getRawPosition(IfxDsadc_Rdc *handle)
{
    return Ifx_AngleTrkF32_getRawPosition(&handle->angleTrk);
}

/** Get the actual mechanical speed.
 *
 * \param handle Pointer to IfxDsadc_Rdc object (RAM location)
 * \return actual mechanical speed in rad/s
 */
IFX_INLINE float32 IfxDsadc_Rdc_getSpeed(IfxDsadc_Rdc *handle)
{ /* FIXME output speed using handle->angleTrk.speedEstB is not stable (big periodical spikes )*/
    return Ifx_AngleTrkF32_getSpeed(&handle->angleTrk);
}

/** Get the number of periods per rotation as configured during initialisation */
IFX_INLINE uint16 IfxDsadc_Rdc_getPeriodPerRotation(IfxDsadc_Rdc *handle)
{
    return Ifx_AngleTrkF32_getPeriodPerRotation(&handle->angleTrk);
}

/** Return fault state. \see PosIf_Status fields for more details */
IFX_INLINE IfxStdIf_Pos_Status IfxDsadc_Rdc_getFault(IfxDsadc_Rdc *handle)
{
    return Ifx_AngleTrkF32_getFault(&handle->angleTrk);
}

/** Return timestamp in seconds */
IFX_INLINE float32 IfxDsadc_Rdc_getTimestamp(IfxDsadc_Rdc *handle)
{
    return handle->timestamp.inSeconds;
}

/** \addtogroup library_srvsw_sysse_virtualDevice_positionSensor_StdIf_Functions
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief \see IfxStdIf_Pos_GetAbsolutePosition
 * \param driver driver handle
 * \return absolute position
 */
IFX_EXTERN float32 IfxDsadc_Rdc_getAbsolutePosition(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetDirection
 * \param driver driver handle
 * \return direction
 */
IFX_EXTERN IfxStdIf_Pos_Dir IfxDsadc_Rdc_getDirection(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetFault
 * \param driver driver handle
 * \return Fault
 */
IFX_INLINE IfxStdIf_Pos_Status IfxDsadc_Rdc_getFault(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetOffset
 * \param driver driver handle
 * \return offset address
 */
IFX_EXTERN sint32 IfxDsadc_Rdc_getOffset(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetPeriodPerRotation
 * \param driver driver handle
 * \return Period per rotation
 */
IFX_INLINE uint16 IfxDsadc_Rdc_getPeriodPerRotation(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetPosition
 * \param driver driver handle
 * \return position
 */
IFX_EXTERN float32 IfxDsadc_Rdc_getPosition(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetRawPosition
 * \param driver driver handle
 * \return position in ticks
 */
IFX_INLINE IfxStdIf_Pos_RawAngle IfxDsadc_Rdc_getRawPosition(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetRefreshPeriod
 * \param driver driver handle
 * \return update period
 */
IFX_EXTERN float32 IfxDsadc_Rdc_getRefreshPeriod(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetResolution
 * \param driver driver handle
 * \return resolution
 */
IFX_EXTERN sint32 IfxDsadc_Rdc_getResolution(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetTurn
 * \param driver driver handle
 * \return Returns the number of turns
 */
IFX_EXTERN sint32 IfxDsadc_Rdc_getTurn(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetSensorType
 * \param driver driver handle
 * \return sensor type
 */
IFX_EXTERN IfxStdIf_Pos_SensorType IfxDsadc_Rdc_getSensorType(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_GetSpeed
 * \param driver driver handle
 * \return speed
 */
IFX_INLINE float32 IfxDsadc_Rdc_getSpeed(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_Reset
 * \param driver driver handle
 * \return None
 */
IFX_EXTERN void IfxDsadc_Rdc_reset(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_ResetFaults
 * \param driver driver handle
 * \return None
 */
IFX_EXTERN void IfxDsadc_Rdc_resetFaults(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_SetOffset
 * \param driver driver handle
 * \param offset offset
 * \return None
 */
IFX_EXTERN void IfxDsadc_Rdc_setOffset(IfxDsadc_Rdc *driver, sint32 offset);

/** \brief \see IfxStdIf_Pos_SetRefreshPeriod
 * \param driver driver handle
 * \param updatePeriod update period
 * \return None
 */
IFX_EXTERN void IfxDsadc_Rdc_setRefreshPeriod(IfxDsadc_Rdc *driver, float32 updatePeriod);

/** \brief \see IfxStdIf_Pos_OnEventA
 * The API handles the event A which corresponds to the DSADC interrupt event.
 * \param driver driver handle
 * \return None
 */
IFX_EXTERN void IfxDsadc_Rdc_onEventA(IfxDsadc_Rdc *driver);

/** \brief \see IfxStdIf_Pos_Update
 * \param driver driver handle
 * \return None
 * FIXME need to be renamed step(). Add step to stdif. Step should simulate a change in the sensor. Update should update the drivers values, multiple call to the update () should not change the position sensor state, whereas step does.
 */
IFX_EXTERN void IfxDsadc_Rdc_update(IfxDsadc_Rdc *driver);

/** \} */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initializes the standard interface "Pos"
 * \param stdif Standard interface position object
 * \param driver Virtual position sensor
 * \return TRUE on success else FALSE
 */
IFX_EXTERN boolean IfxDsadc_Rdc_stdIfPosInit(IfxStdIf_Pos *stdif, IfxDsadc_Rdc *driver);


#endif /* IFX_RDC_DSADC_H */

/*________________________________________________________________________________________
// DOCUMENTATION */

/* *INDENT-OFF* */

/**
\page page_dsadc_rdc Resolver-to-digital Converter with DSADC

\tableofcontents

One of the popular rotor positioning sensor used by electric motor drive is resolver.
A resolver device modulates carrier signal R1-R2 by the SINE and COSINE function of actual
angle and feeds it back into the controller unit (S1-S3, S2-S6).

AURIX family provides support for resolver-to-digital converter (RDC) by providing the
following functionalities:
- Carrier generation (DSADC hardware)
- Signal acquisition and carrier cancellation (DSADC hardware)
- Timestamp acquisition (GTM TIM hardware)

Additionally, the following functionalities are provided by software:
- Angle tracking observer function
- Fault detection

This page contains concept documentation of the driver software (\ref IfxLld_Dsadc_rdc "DSADC RDC"),
especially implemented in files: \ref IfxDsadc_Rdc.h and \ref IfxDsadc_Rdc.c

Below an overview of the resolver use case:
 \image html "TC.Dsadc.Rdc[Overview].png" "RSADC RDC use case"


\section sec_dsadc_rdc_cgen Carrier generation

DSADC hardware supports the generation of resolver excitation carrier by providing PWM
pin outputs which can be simply filtered by low-pass filter circuit to obtain a pair of
differential signal.

Common configuration of the positive and negative signals are:
- carrier signal frequency
- bit-reversed PWM (optional, but recommended)
- waveform mode: square, triangle, or sinusoidal

Those configuration are set by IfxDsadc_Dsadc_initCarrierGen() function which is called in the
context of IfxDsadc_Rdc_init() function. 
Therefore, user **doesn't need** to call IfxDsadc_Dsadc_initCarrierGen() separately.

For each pin output (positive and negative), a low-pass filter and amplifier circuitry
is required to drive a resolver device. Example of such circuitry can be seen in the
figure below.

TODO: add picture

\section sec_dsadc_rdc_acq Signal acquisition and carrier cancellation

Two differential signals are generated from a resolver device (Sin+, Sin-, Cos+, Cos- or
some write also as S1-S3, S2-S6). These signals basically can be connected directly into
the two channels of DSADC, where each DSADC channel has P and N inputs.

Single DSADC channel and carrier cancelation configuration is done by function
IfxDsadc_Dsadc_initChannel() which is called in the context of IfxDsadc_Rdc_init() function.
Therefore, user **doesn't need** to call IfxDsadc_Dsadc_initChannel() separately.

Common configuration items for each DSADC channel are:
- modulator frequency (default: 10MHz)
- CIC filter type (default: COMB3)
- CIC decimation factor (default: 32)
- FIR0 and FIR1 blocks (default: disabled)

Common configuration items of the carrier cancelation in each DSADC channel are:
- offset compensation block (default: enabled)
- integration count (default: 32)

With above default values (modulator frequency 10MHz, CIC decimation factor = 32, both FIRs
disabled and integration count = 32), the output sample rate is:
  10MHz / (32 * 32) = ~9.7kHz

In each acquired sample, interrupt service is requested by the first DSADC channel
initialised by IfxDsadc_Rdc_init(). User's application shall provide an interrupt service
routine (ISR) to handle this request. In the ISR, or an OS task which is synchronised to
the event, user shall call IfxDsadc_Rdc_onEventA() function in order to update all
necessary driver data related to this event.

\section sec_dsadc_rdc_tstmp Timestamp acquisition

Typically, user's application run at different sampling rate than DSADC output sampling
rate. For example, motor control PWM interrupt occurs with 10 kHz sampling whereas the
DSADC interrupt runs with ~9.7 kHz. This condition creates situation where the sampled
resolver position is already aged with a few timer ticks, but can be significant to the
motor control algorithm.

Therefore, timestamp which indicates the elapsed time since last DSADC channel sampling
is required for compensating or computing the missing rotor position. In addition, this
timestamp is also used for compensating the group delay which is inherent property of
DSADC.

The implementation of timestamp acquisition uses following hardware resources:
- one timebase unit or global timer (free-running), i.e. GTM TBU
- first timer capture unit, to capture the time at the DSADC sample output event, i.e.
  using GTM TIM channel
- second timer capture unit, to capture the time at the application sample point (e.g.
  PWM event), i.e. using GTM TIM channel.

Those resources are initialised by IfxDsadc_Rdc_initHwTimestamp() function which is called
in the context of IfxDsadc_Rdc_init() function. Therefore, user **doesn't need** to call
IfxDsadc_Rdc_initHwTimestamp() separately.

The user shall call IfxDsadc_Rdc_update() function in order to execute:
- timestamp and group delay compensation
- angle tracking observer
- fault-detection

\section sec_dsadc_rdc_ato Angle tracking observer

Output samples acquired by DSADC channel and carrier cancelation are the envelope of SINE
and COSINE resolver signals. In principle, ATAN2 function can be used to calculate the
rotor position (angle), but in this driver a \ref library_srvsw_sysse_math_f32_angletrk "Ifx_AngleTrkF32" module is used.

When user's application calls function IfxDsadc_Rdc_update(), function Ifx_AngleTrkF32_step()
is also called which contains implementation of:
- ATAN2,
- 3rd order angle tracking observer

The following figure shows the idea:

TODO: add picture

ATAN2 function computes the initial angle for the observer. Additional phase is computed
from the timestamp and group delay to compensate 'missing' rotor position (angle).
The combined initial angle and phase are used as reference for the first integrator for
estimating acceleration. The second integrator estimates speed. The final integrator
estimates the output angle.

The output angle is normally more 'smooth' in steady-state than the angle from ATAN2
function, albeit some settling time and overshoot which occur during transient. The dynamic
performance depends on the gain constants set in the integrators. By default, the constants
are calculated using formula from proceeding's paper
[Design of advanced resolver-to-digital converters](http://www.univ-nantes.fr/servlet/com.univ.collaboratif.utils.LectureFichiergw?CODE_FICHIER=1307605985775&ID_FICHE=1329),
by Auger, et.al, in Electrimacs 2011, CergyPontoise (France), 6-8 june 2011.

\subsection sec_dsadc_rdc_groupDelay Group delay estimation

It is shown in previous figure that group delay is required to estimate the actual position
based on the angular speed and measured position.

The group delay of itself is estimated to be the sum of group delay of components of the
DSADC channel used for acquisition such as CIC filter, FIR0, FIR1, and Integrator blocks.

The following formulas are used to estimate the group delays:

<table>
<tr><td>Input group delay:     </td><td>\f$ \tau_{IN}   = (7+1)\cdot t_{mod}               \f$</td><td>\f$t_{mod} = 1/F_{mod}\f$</td></tr>
<tr><td>CIC0..CIC3 group delay:</td><td>\f$ \tau_{CICk} = (k\cdot(N - 1) / 2)\cdot t_{mod} \f$</td><td>\f$F_{mod}\f$: modulator frequency</td></tr>
<tr><td>CICF group delay:      </td><td>\f$ \tau_{CICF} = (2\cdot N - 1)\cdot t_{mod}      \f$</td><td>\f$N\f$: CIC filter decimation factor</td></tr>
<tr><td>FIR0 group delay:      </td><td>\f$ \tau_{FIR0} = (3.5 + (3/N))\cdot t_{FIR0_{in}} \f$</td><td>\f$t_{FIR0_{in}}\f$: update period at FIR0 input (or CIC output)</td></tr>
<tr><td>FIR1 group delay:      </td><td>\f$ \tau_{FIR1} = (28  + (5/N))\cdot t_{FIR0_{in}} \f$</td><td></td></tr>
<tr><td>Integrator group delay:</td><td>\f$ \tau_{INT}  = ((N_{INT}-1)/2)\cdot t_{INT_{in}}\f$</td><td>\f$t_{INT_{in}}\f$: update period at INTegrator input.<br/>\f$N_{INT}\f$: Integrator's decimation factor</td></tr>
</table>

The function IfxDsadc_getMainGroupDelay() implements those formulas, invoked by IfxDsadc_Rdc_update().

\section sec_dsadc_rdc_fault Fault detection

When application calls function IfxDsadc_Rdc_update(), fault detection
implemented in \ref library_srvsw_sysse_math_f32_angletrk "Ifx_AngleTrkF32" module is also executed.
The fault-detection is based on simple trigonometry equation:

\f$ \sin^2(\phi) + \cos^2(\phi) = 1 \f$

With some tolerance, whenever the amplitude of one of SINE or COSINE signal is outside
the allowed band, then fault flag is set. 

- **SignalLoss** flag is set when the amplitude is below lower boundary.
- **SignalDegradation** flag is set when the amplitude is higher than upper boundary.
- **TrackingLoss**, in addition, is set when the internal error value of the angle
  tracking observer is above a threshold.

Functions IfxDsadc_Rdc_getFault() can be used for checking the actual error flags.

Configuration of the fault-detection (tolerance) boundary values and threshold are done
by user's call to IfxDsadc_Rdc_init().

*/
/* *INDENT-ON* */

//________________________________________________________________________________________
