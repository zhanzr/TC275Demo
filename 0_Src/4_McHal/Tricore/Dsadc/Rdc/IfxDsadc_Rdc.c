/*MCMETILLD-748*/
/**
 * \file IfxDsadc_Rdc.c
 * \brief DSADC-based resolver-to-digital converter (RDC) support function
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
 */

#include "IfxDsadc_Rdc.h"
#include "string.h"
#if IFXDSADC_HW_INIT
#include "Gtm/Trig/IfxGtm_Trig.h"


const Ifx_GTM_TIM_CH_CTRL_Bits IfxDsadc_Rdc_GtmTimCfg = {
    .TIM_EN   = 1,
    .TIM_MODE = IfxGtm_Tim_Mode_inputEvent,
    .GPR0_SEL = IfxGtm_Tim_GprSel_tbuTs0, /* FIXME make this configurable */
    .GPR1_SEL = IfxGtm_Tim_GprSel_tbuTs0, /* FIXME make this configurable */
    .CNTS_SEL = IfxGtm_Tim_CntsSel_cntReg,
    /*.EGPR0_SEL= 0, //not available in A-step */
    .DSL      = 0, /* 0 = falling or low */
    .ISL      = 0, /* 1 = both edges, ignore DSL */
};


IFX_INLINE void  IfxDsadc_Rdc_updateTimestamp(IfxDsadc_Rdc *handle)
{
    IfxDsadc_Rdc_Ts *timestamp = &handle->timestamp;
    IfxDsadc_Rdc_Hw *hwHandle = &handle->hardware;

    uint32 tsPwm = hwHandle->pwmTimCh->GPR0.B.GPR0;
    uint32 clockTicks = (tsPwm - timestamp->rdc) % timestamp->maxTicks;
#    if IFX_CFG_DSADC_RDC_DEBUG
    timestamp->inTicks = clockTicks;
    timestamp->pwm = tsPwm;
#    endif
    timestamp->inSeconds = timestamp->clockPeriod * (float32)clockTicks;
}


static
Ifx_GTM_TIM_CH *IfxGtm_Tim_getChannelPtr(Ifx_GTM *gtm, IfxGtm_Tim tim, IfxGtm_Tim_Ch timCh)
{
    uint32 incr = ((uint32)&gtm->TIM[0].CH1 - (uint32)(uint32)&gtm->TIM[0].CH0);
    return (Ifx_GTM_TIM_CH*) ((uint32)&gtm->TIM[tim].CH0 + (incr * timCh));
}


Ifx_GTM_TIM_CH *IfxDsadc_Rdc_initGtmTim(Ifx_GTM *gtm, IfxGtm_Tim tim, IfxGtm_Tim_Ch timCh, boolean risingEdge)
{
    Ifx_GTM_TIM_CH     *pTimCh = IfxGtm_Tim_getChannelPtr(gtm, tim, timCh);
    Ifx_GTM_TIM_CH_CTRL ctrl;
    ctrl.B         = IfxDsadc_Rdc_GtmTimCfg;
    ctrl.B.DSL     = (risingEdge != FALSE) ? 1 : 0;
    pTimCh->CTRL.U = ctrl.U;
    return pTimCh;
}


/** Return the update period in [seconds], i.e. the period of new result (and interrupt)*/
float32 IfxDsadc_Rdc_getUpdatePeriod(IfxDsadc_Rdc *handle)
{
    IfxDsadc_Rdc_Hw *hwHandle = &(handle->hardware);
    return 1.0F / IfxDsadc_getIntegratorOutFreq(hwHandle->inputSin.module, hwHandle->inputSin.channelId);
}

float32 IfxDsadc_Rdc_calculateTimestampPeriod(const IfxDsadc_Rdc_Config *config)
{
    Ifx_GTM *gtm = config->hardware.gtmTimestamp.gtm;
    return (1.0F / IfxGtm_Tbu_getClockFrequency(gtm, IfxGtm_Tbu_Ts_0));
}


float32 IfxDsadc_Rdc_calculateGroupDelay(IfxDsadc_Rdc *handle)
{
    Ifx_DSADC *dsadc = handle->hardware.inputSin.module;
    return IfxDsadc_getMainGroupDelay(dsadc, handle->hardware.inputSin.channelId);
}


#else
float32 IfxDsadc_Rdc_calculateGroupDelay(IfxDsadc_Rdc *handle);
float32 IfxDsadc_Rdc_calculateTimestampPeriod(const IfxDsadc_Rdc_Config *config);
float32 IfxDsadc_Rdc_getUpdatePeriod(IfxDsadc_Rdc *handle);
void    IfxDsadc_Rdc_updateTimestamp(IfxDsadc_Rdc *handle);

#endif

void IfxDsadc_Rdc_initHwTimestamp(IfxDsadc_Rdc *handle, const IfxDsadc_Rdc_Config *config)
{
#if IFXDSADC_HW_INIT
    IfxDsadc_Rdc_Hw *hwHandle = &(handle->hardware);
    const IfxDsadc_Rdc_ConfigHw *configHw = &config->hardware;
    const IfxDsadc_GtmTimestamp *tsConfig = &(configHw->gtmTimestamp);
    Ifx_GTM                     *gtm      = tsConfig->gtm;

    /** - Initialize TIM channel which is triggered by application's PWM */
    {
        boolean risingEdge = (configHw->inputConfig.demodulator.timestampTrigger == IfxDsadc_TimestampTrigger_risingEdge);
        hwHandle->pwmTimCh = IfxDsadc_Rdc_initGtmTim(gtm, tsConfig->pwmTim->tim, tsConfig->pwmTim->channel, risingEdge);

        /* connect into TIM timer input: */
        IfxGtm_PinMap_setTimTin(tsConfig->pwmTim, IfxPort_InputMode_undefined);
    }
    /** - Initialize TIM channel which is triggered by DSADC sample event */
    {
        hwHandle->rdcTimCh = IfxDsadc_Rdc_initGtmTim(gtm, tsConfig->rdcTim, tsConfig->rdcTimChannel, TRUE);

        /** - Connect DSADC trigger to TIM mux */
        IfxGtm_Trig_fromDsadc(gtm, configHw->inputSinP->channel, tsConfig->rdcTim, tsConfig->rdcTimChannel);

        /** - Connect into TIM input */
        uint32 shift = tsConfig->rdcTimChannel * 4;
        __ldmst_c(&(gtm->INOUTSEL.TIM[tsConfig->rdcTim].INSEL.U), (0xFU << shift), (tsConfig->rdcTimMuxValue << shift));
    }
    IfxGtm_Tbu_enableChannel(gtm, IfxGtm_Tbu_Ts_0);
#endif
}


boolean IfxDsadc_Rdc_initHwChannels(IfxDsadc_Rdc *handle, const IfxDsadc_Rdc_Config *config)
{
#if IFXDSADC_HW_INIT
    IfxDsadc_Rdc_Hw *hwHandle = &(handle->hardware);
    const IfxDsadc_Rdc_ConfigHw *configHw = &config->hardware;
    Ifx_DSADC *module = configHw->inputConfig.module;
    boolean    result = (module != NULL_PTR) ? TRUE : FALSE;

    if (result != FALSE)
    {

        /* Initialise input channels */
        {
        	IfxDsadc_Dsadc_ChannelPins pins;
            IfxDsadc_Dsadc_ChannelConfig channelConfig = configHw->inputConfig;
            channelConfig.channelId       = configHw->inputCosP->channel;
            channelConfig.modulator.inputPin = configHw->inputCosP->select;
            channelConfig.channelPins = &pins;
            pins = (IfxDsadc_Dsadc_ChannelPins)
			{
                .cin = NULL_PTR,
                .cinMode = IfxPort_InputMode_noPullDevice,
                .din = NULL_PTR,
                .dinMode = IfxPort_InputMode_noPullDevice,
                .itr = NULL_PTR,
                .itrMode = IfxPort_InputMode_noPullDevice,
                .dsn = configHw->inputCosN,
                .dsnMode = IfxPort_InputMode_noPullDevice,
                .dsp = configHw->inputCosP,
                .dspMode = IfxPort_InputMode_noPullDevice
			};
            IfxDsadc_Dsadc_initChannel(&hwHandle->inputCos, &channelConfig);

            channelConfig.channelId       = configHw->inputSinP->channel;
            channelConfig.modulator.inputPin = configHw->inputSinP->select;
            pins = (IfxDsadc_Dsadc_ChannelPins)
			{
                .cin = NULL_PTR,
                .cinMode = IfxPort_InputMode_noPullDevice,
                .din = NULL_PTR,
                .dinMode = IfxPort_InputMode_noPullDevice,
                .itr = NULL_PTR,
                .itrMode = IfxPort_InputMode_noPullDevice,
                .dsn = configHw->inputSinN,
                .dsnMode = IfxPort_InputMode_noPullDevice,
                .dsp = configHw->inputSinP,
                .dspMode = IfxPort_InputMode_noPullDevice
			};
            IfxDsadc_Dsadc_initChannel(&hwHandle->inputSin, &channelConfig);

            if (configHw->servReqPriority != 0)
            {
                IfxDsadc_ChannelId ch = channelConfig.channelId;
                volatile Ifx_SRC_SRCR *srcr = &MODULE_SRC.DSADC.DSADC[ch].SRM;
                IfxSrc_init(srcr, configHw->servReqProvider, configHw->servReqPriority);
                IfxSrc_enable(srcr);
            }
        }

        /* Initialise carrier generator, if necessary */
        if ((configHw->carrierGen.pinNeg != NULL_PTR) || (configHw->carrierGen.pinPos != NULL_PTR))
        {
            IfxDsadc_Dsadc_initCarrierGen(config->dsadc, &configHw->carrierGen);
        }

        /* Start conversion, if necessary */
        if (configHw->startScan != FALSE)
        {
            IfxDsadc_Rdc_startConversion(handle);
        }

        /* Initialise modulator clock output, if necessary */
        if (configHw->outputClock != NULL_PTR)
        {
            const IfxPort_Pin *clkPin = &(configHw->outputClock->pin);
            IfxPort_setPinModeOutput(clkPin->port, clkPin->pinIndex, configHw->carrierGen.pinMode, configHw->outputClock->select);
            IfxPort_setPinPadDriver(clkPin->port, clkPin->pinIndex, IfxPort_PadDriver_cmosAutomotiveSpeed1);
        }
    }

    return result;
#else
    return TRUE;
#endif
}


/**
 * Initialise the DSADC RDC driver by initialising:
 * - Two DSADC hardware channel resources
 * - Carrier generation hardware resources
 * - Time-stamp hardware resources
 * - Driver's run-time data (RAM): tracking observer, fault-detection, interface layer etc.
 *
 * \param handle Driver's handle, i.e. pointer to \ref IfxDsadc_Rdc RAM location
 * \param config Pointer to the driver's configuration
 *
 * \return TRUE in case of success else FALSE
 */
boolean IfxDsadc_Rdc_init(IfxDsadc_Rdc *handle, const IfxDsadc_Rdc_Config *config)
{
	boolean result = TRUE;
    /* Initialise the DSADC hardware channels */
    result &= IfxDsadc_Rdc_initHwChannels(handle, config);

    /* Initialise the GTM timestamp hardware resources */
    IfxDsadc_Rdc_initHwTimestamp(handle, config);

    /* Initialise the software resources */
    {
        handle->updatePeriod          = IfxDsadc_Rdc_getUpdatePeriod(handle);
        handle->groupDelay            = IfxDsadc_Rdc_calculateGroupDelay(handle);
        handle->timestamp.enabled     = TRUE;
        handle->timestamp.clockPeriod = IfxDsadc_Rdc_calculateTimestampPeriod(config);
        handle->timestamp.maxTicks    = (uint32)(handle->updatePeriod / handle->timestamp.clockPeriod);
    }

    /* Initialise angle-tracking observer */
    {
        Ifx_AngleTrkF32_Config atoConfig;
        Ifx_AngleTrkF32_initConfig(&atoConfig, &(handle->sinIn), &(handle->cosIn));

        atoConfig.offset            = config->offset;
        atoConfig.speedLpfFc        = config->speedLpfFc;
        atoConfig.reversed          = config->reversed;
        atoConfig.errorThreshold    = config->errorThreshold;
        atoConfig.sqrAmplMax        = config->sqrAmplMax;
        atoConfig.sqrAmplMin        = config->sqrAmplMin;
        atoConfig.periodPerRotation = config->periodPerRotation;

        atoConfig.kp                = config->kp;
        atoConfig.ki                = config->ki;
        atoConfig.kd                = config->kd;
        atoConfig.resolution        = config->resolution;

#if IFX_CFG_DSADC_RDC_PRE_OBSERVER_CORRECTION
        Ifx_AngleTrkF32_init(&(handle->angleTrk), &atoConfig, config->userTs);
#else
        Ifx_AngleTrkF32_init(&(handle->angleTrk), &atoConfig, handle->updatePeriod);
#endif
    }

    /* Optional calibration init */
    {
    }
    return result;
}

void IfxDsadc_Rdc_iniConfig(IfxDsadc_Rdc_Config *config)
{
    config->kp = 0; /* Force to used default from Ifx_AngleTrkF32 */
    config->ki = 0;
    config->kd = 0;
    config->speedLpfFc = 100;
    config->errorThreshold = 5.0f/180*IFX_PI;
    config->userTs = 0;
    config->sqrAmplMax = (sint32)((1.01f * 1.01f) * 2048);
    config->sqrAmplMin = (sint32)((0.99f * 0.99f) * 2048);
    config->periodPerRotation = 1;
    config->reversed = FALSE;
    config->resolution = (1UL << 12)    ;/** \brief 12-bit default resolution */
    config->offset = 0;
    IfxDsadc_Dsadc_initCarrierGenConfig(&config->hardware.carrierGen);
    config->hardware.gtmTimestamp.gtm = &MODULE_GTM;
    config->hardware.gtmTimestamp.pwmTim = NULL_PTR;
    config->hardware.gtmTimestamp.rdcTim = IfxGtm_Tim_0;
    config->hardware.gtmTimestamp.rdcTimChannel = IfxGtm_Tim_Ch_0;
    config->hardware.gtmTimestamp.rdcTimMuxValue = 0;
    IfxDsadc_Dsadc_initChannelConfig(&config->hardware.inputConfig, NULL_PTR);
    config->hardware.inputCosP = NULL_PTR;
    config->hardware.inputCosN = NULL_PTR;
    config->hardware.inputSinP = NULL_PTR;
    config->hardware.inputSinN = NULL_PTR;
    config->hardware.outputClock = NULL_PTR;
    config->hardware.servReqPriority = 0;
    config->hardware.servReqProvider = IfxSrc_Tos_cpu0;
    config->hardware.startScan = FALSE;

}


/** Function to be executed on DSADC channel interrupt.
 * \param handle Driver's handle, i.e. pointer to \ref IfxDsadc_Rdc RAM location
 */
void IfxDsadc_Rdc_onEventA(IfxDsadc_Rdc *handle)
{
#if IFXDSADC_HW_INIT
    handle->timestamp.rdc = handle->hardware.rdcTimCh->GPR0.B.GPR0;

    Ifx_DSADC *dsadc = handle->hardware.inputSin.module;
    handle->sinIn = IfxDsadc_getMainResult(dsadc, handle->hardware.inputSin.channelId);
    handle->cosIn = IfxDsadc_getMainResult(dsadc, handle->hardware.inputCos.channelId);
    /*
    */
#endif

#if IFX_CFG_DSADC_RDC_PRE_OBSERVER_CORRECTION == 0
    {
        sint16 sinIn = handle->sinIn;
        sint16 cosIn = handle->cosIn;

        /* tracking observer (note: atan2 lookup function is available inside) */
        Ifx_AngleTrkF32_step(&(handle->angleTrk), sinIn, cosIn, 0);
        Ifx_AngleTrkF32_updateStatus(&(handle->angleTrk), sinIn, cosIn);
    }
#endif
}


/** Function to be executed at user's application interrupt or task, e.g. motor control
 * task.
 * \param handle Driver's handle, i.e. pointer to \ref IfxDsadc_Rdc RAM location
 * */
void IfxDsadc_Rdc_update(IfxDsadc_Rdc *handle)
{
    float32 groupDelayAngle;
    float32 timeStampAngle;
    float32 angleOut;
    float32 speedEst;
    float32 angleCorrection;

    /* previous estimated speed */
    speedEst = Ifx_AngleTrkF32_getLoopSpeed(&(handle->angleTrk));

    /* angular component due to group delay */
    groupDelayAngle = handle->groupDelay * speedEst;

    /* angular component due to time-stamp, if time-stamping is enabled */
    if (handle->timestamp.enabled != FALSE)
    {
        IfxDsadc_Rdc_updateTimestamp(handle);
        timeStampAngle = IfxDsadc_Rdc_getTimestamp(handle) * speedEst;
    }
    else
    {
        timeStampAngle = 0;
    }

    /* angle correction value */
    angleCorrection = groupDelayAngle + timeStampAngle;

#if IFX_CFG_DSADC_RDC_PRE_OBSERVER_CORRECTION != 0
    /*
    handle->sinIn = (sint16)handle->hardware.inputSin.channel->TSTMP.B.RESULT;
    handle->cosIn = (sint16)handle->hardware.inputCos.channel->TSTMP.B.RESULT;
    */

    {
        sint16 sinIn = handle->sinIn;
        sint16 cosIn = handle->cosIn;

        /* tracking observer (note: atan2 lookup function is available inside) */
        Ifx_AngleTrkF32_step(&(handle->angleTrk), sinIn, cosIn, angleCorrection);
        Ifx_AngleTrkF32_updateStatus(&(handle->angleTrk), sinIn, cosIn);
    }
    angleOut = handle->angleTrk.angleEst;
#else
    angleOut = handle->angleTrk.angleEst + angleCorrection;
#endif

    /* final output estimation */
    {
        Ifx_AngleTrkF32_PosIf  *base = &(handle->angleTrk).base;
        IfxStdIf_Pos_RawAngle newPosition = (IfxStdIf_Pos_RawAngle)(angleOut * (base->resolution / 2) / IFX_PI);
        newPosition    = (newPosition + base->offset) & (base->resolution - 1);
        base->rawPosition = newPosition;
    }
}


/** Synchronously start of both DSADC H/W channels.
 * \param handle Driver's handle, i.e. pointer to \ref IfxDsadc_Rdc RAM location
 */
void IfxDsadc_Rdc_startConversion(IfxDsadc_Rdc *handle)
{
#if IFXDSADC_HW_INIT
    Ifx_DSADC *module = handle->hardware.inputSin.module;
    uint32     mask   =
        (1U << handle->hardware.inputCos.channelId) |
        (1U << handle->hardware.inputSin.channelId);
    IfxDsadc_startScan(module, mask, mask);
#endif
}


float32 IfxDsadc_Rdc_getAbsolutePosition(IfxDsadc_Rdc *driver)
{
    return Ifx_AngleTrkF32_getAbsolutePosition(&driver->angleTrk);
}

IfxStdIf_Pos_Dir IfxDsadc_Rdc_getDirection(IfxDsadc_Rdc *driver)
{
    return Ifx_AngleTrkF32_getDirection(&driver->angleTrk);
}

sint32 IfxDsadc_Rdc_getOffset(IfxDsadc_Rdc *driver)
{
    return Ifx_AngleTrkF32_getOffset(&driver->angleTrk);
}

float32 IfxDsadc_Rdc_getPosition(IfxDsadc_Rdc *driver)
{
    return Ifx_AngleTrkF32_getPosition(&driver->angleTrk);
}


float32 IfxDsadc_Rdc_getRefreshPeriod(IfxDsadc_Rdc *driver)
{
    return Ifx_AngleTrkF32_getRefreshPeriod(&driver->angleTrk);
}

sint32 IfxDsadc_Rdc_getResolution(IfxDsadc_Rdc *driver)
{
    return Ifx_AngleTrkF32_getResolution(&driver->angleTrk);
}

IfxStdIf_Pos_SensorType IfxDsadc_Rdc_getSensorType(IfxDsadc_Rdc *driver)
{
    return IfxStdIf_Pos_SensorType_resolver;
}

sint32 IfxDsadc_Rdc_getTurn(IfxDsadc_Rdc *driver)
{
    return Ifx_AngleTrkF32_getTurn(&driver->angleTrk);
}

void IfxDsadc_Rdc_resetFaults(IfxDsadc_Rdc *driver)
{
    Ifx_AngleTrkF32_resetFaults(&driver->angleTrk);
}


void IfxDsadc_Rdc_setOffset(IfxDsadc_Rdc *driver, sint32 offset)
{
    Ifx_AngleTrkF32_setOffset(&driver->angleTrk, offset);
}


void IfxDsadc_Rdc_setRefreshPeriod(IfxDsadc_Rdc *driver, float32 updatePeriod)
{
    Ifx_AngleTrkF32_setRefreshPeriod(&driver->angleTrk, updatePeriod);
}

void IfxDsadc_Rdc_reset(IfxDsadc_Rdc *driver)
{
	Ifx_AngleTrkF32_reset(&driver->angleTrk);
}


boolean IfxDsadc_Rdc_stdIfPosInit(IfxStdIf_Pos *stdif, IfxDsadc_Rdc *driver)
{
    /* Ensure the stdif is reset to zeros */
    memset(stdif, 0, sizeof(IfxStdIf_Pos));

    /* Set the driver */
    stdif->driver = driver;

    /* *INDENT-OFF* Note: this file was indented manually by the author. */
    /* Set the API link */
	stdif->onZeroIrq          =(IfxStdIf_Pos_OnZeroIrq               )NULL_PTR;
	stdif->getAbsolutePosition=(IfxStdIf_Pos_GetAbsolutePosition     )&IfxDsadc_Rdc_getAbsolutePosition;
	stdif->getDirection		  =(IfxStdIf_Pos_GetDirection   	     )&IfxDsadc_Rdc_getDirection;
	stdif->getFault  		  =(IfxStdIf_Pos_GetFault       	     )&IfxDsadc_Rdc_getFault;
	stdif->getOffset		  =(IfxStdIf_Pos_GetOffset			     )&IfxDsadc_Rdc_getOffset;
	stdif->getPeriodPerRotation  =(IfxStdIf_Pos_GetPeriodPerRotation )&IfxDsadc_Rdc_getPeriodPerRotation;
	stdif->getPosition		  =(IfxStdIf_Pos_GetPosition			 )&IfxDsadc_Rdc_getPosition;
	stdif->getRawPosition	  =(IfxStdIf_Pos_GetRawPosition          )&IfxDsadc_Rdc_getRawPosition;
	stdif->getRefreshPeriod   =(IfxStdIf_Pos_GetRefreshPeriod        )&IfxDsadc_Rdc_getRefreshPeriod;
	stdif->getResolution      =(IfxStdIf_Pos_GetResolution           )&IfxDsadc_Rdc_getResolution;
    stdif->getSensorType      =(IfxStdIf_Pos_GetSensorType           )&IfxDsadc_Rdc_getSensorType;
	stdif->reset			  =(IfxStdIf_Pos_Reset				     )&IfxDsadc_Rdc_reset;
	stdif->resetFaults		  =(IfxStdIf_Pos_ResetFaults			 )&IfxDsadc_Rdc_resetFaults;
	stdif->getSpeed           =(IfxStdIf_Pos_GetSpeed                )&IfxDsadc_Rdc_getSpeed;
	stdif->update			  =(IfxStdIf_Pos_Update				     )&IfxDsadc_Rdc_update;
	stdif->setPosition		  =(IfxStdIf_Pos_SetPosition			 )NULL_PTR;
	stdif->setRawPosition	  =(IfxStdIf_Pos_SetRawPosition			 )NULL_PTR;
	stdif->setSpeed		      =(IfxStdIf_Pos_SetSpeed			     )NULL_PTR;
	stdif->setOffset		  =(IfxStdIf_Pos_SetOffset			     )&IfxDsadc_Rdc_setOffset;
	stdif->setRefreshPeriod   =(IfxStdIf_Pos_SetRefreshPeriod        )&IfxDsadc_Rdc_setRefreshPeriod;
	stdif->getTurn            =(IfxStdIf_Pos_GetTurn                 )&IfxDsadc_Rdc_getTurn;
	stdif->onEventA            =(IfxStdIf_Pos_OnEventA               )&IfxDsadc_Rdc_onEventA;
    /* *INDENT-ON* */

    return TRUE;
}

