#ifndef DEMO_DSADC
#define DEMO_DSADC

#include "IfxDsadc_Rdc.h"
//#include "Appli/Back/demo_handler.h"

#define DSADC_PRE_DIVDER	 	(1)
#define DSADC_DIVIDER_CONV	    (10)
#define DSADC_NORMAL_CONV       (1)
#define DSADC_RESOLVER_APP      (2)

#define INVERTER_FREQUENCY                  (10000)                 /**< \brief Define the PWM frequency in Hz. */
#define INVERTER_DEAD_TIME                  (2.0e-6)                /**< \brief Define the dead time between the top and bottom PWM in s. */
#define INVERTER_MIN_PULSE                  (2.5e-6)                /**< \brief Define the smallest PWM pulse in s. */
#define INVERTER_CCX_ACTIVESTATE            (Ifx_ActiveState_high)      /**< \brief Define the CCx outputs active state. Type is @ref ActiveState. */
#define INVERTER_COUTX_ACTIVESTATE          (Ifx_ActiveState_high)      /**< \brief Define the COUTx outputs active state. Type is @ref ActiveState. */
#define INVERTER_TRIGGER_RISING_EDGE_AT_PERIOD        (0)           /**< \brief Define the Trigger rising edge. */

//#define iLLD
#define DERIVATIVE_TC27xC
//#define DSADC_INT
#define LOGIC_BOARD
//#define GAIN_CALIBRATION_1
//#define GAIN_CALIBRATION_2

void DSADC_DemoInit(uint32 module, uint32 idx);
void DSADC_DemoDeInit(uint32 module, uint32 idx);
void DSADC_DemoRun(uint32 module, uint32 idx);
#if 0
CONST_CFG IfxDsadc_Dsadc_CarrierGenConfig cfg_PosIf_RdcDs_Carrier =
{
    .carrierWaveformMode    = IfxDsadc_CarrierWaveformMode_sine,
    .inverted               = FALSE,
    .bitReversed            = TRUE,
    .frequency              = 10e3,
    .pinPos                 = &IfxDsadc_CGPWMP_P00_6_OUT,
    .pinNeg                 = &IfxDsadc_CGPWMN_P00_5_OUT,
    .pinMode                = IfxPort_OutputMode_pushPull,
    .pinDriver              = IfxPort_PadDriver_cmosAutomotiveSpeed1
};

/** Default configuration for one DSADC resolver input channel  */
CONST_CFG IfxDsadc_Dsadc_ChannelConfig cfg_PosIf_RdcDs_Channel =
{
    .module                 = &MODULE_DSADC,
    .channelId              = 0, /* will be overwritten */
    .modulator = {
        .positiveInput      = IfxDsadc_InputConfig_inputPin,
        .negativeInput      = IfxDsadc_InputConfig_inputPin,
        .inputGain          = IfxDsadc_InputGain_factor1,
        .inputPin           = IfxDsadc_InputPin_a,
        .modulatorClockFreq = 10.0e6,
        .commonModeVoltage  = IfxDsadc_CommonModeVoltage_b,
    },
    .demodulator = {
        .inputDataSource    = IfxDsadc_InputDataSource_onChipStandAlone,
//        .triggerSelect      = IfxDsadc_DICFG_TRSEL_c,   /* using adc trig 0 */
        .triggerInput       = IfxDsadc_TriggerInput_a,   /* using dsadc trig 0 */
        .integrationTrigger = IfxDsadc_IntegratorTrigger_alwaysActive,
        .timestampTrigger   = (INVERTER_TRIGGER_RISING_EDGE_AT_PERIOD
                              ? IfxDsadc_TimestampTrigger_fallingEdge
                              : IfxDsadc_TimestampTrigger_risingEdge),
        .sampleClockSource  = IfxDsadc_SampleClockSource_internal,
        .sampleStrobe       = IfxDsadc_SampleStrobe_sampleOnRisingEdge,
    },
    .combFilter = {
        .bypassed           = FALSE,
        .combFilterType     = IfxDsadc_MainCombFilterType_comb3,
        .combFilterShift    = IfxDsadc_MainCombFilterShift_shiftBy3,
        .serviceRequest     = IfxDsadc_MainServiceRequest_everyNewResult,
        .decimationFactor   = 16,
        .startValue         = 1,
    },
    .firFilter = {
        .fir0Enabled        = TRUE,
        .fir1Enabled        = TRUE,
        .offsetCompensation = TRUE,
        .dataShift          = IfxDsadc_FirDataShift_shiftBy2,
        .internalShift      = IfxDsadc_FirInternalShift_noShift
    },
    .integrator = {
        .windowSize         = IfxDsadc_IntegrationWindowSize_internalControl,
        .discardCount       = 0,
        .integrationCount   = 16,
        .integrationCycles  = 1,
    },
    .rectifier = {
        .enabled            = TRUE,
        .signSource         = IfxDsadc_RectifierSignSource_onChipGenerator,
        //.signSource         = IfxDsadc_RECTCFG_SSRC_externalA,
        .signPeriod         = 16,
        .signDelay          = 1
    },
    .auxFilter = {
        .bypassed           = TRUE,
        .combFilterType     = IfxDsadc_AuxCombFilterType_comb1,
        .combFilterShift    = IfxDsadc_AuxCombFilterShift_noShift,
        .serviceRequest     = IfxDsadc_AuxServiceRequest_never,
        .eventSelect        = IfxDsadc_AuxEvent_everyNewResult,
        .eventGate          = IfxDsadc_AuxGate_definedByESEL,
        .decimationFactor   = 4,
    },
};

/** Hardware configuration for DSADC resolver interface */
CONST_CFG IfxDsadc_Rdc_ConfigHw cfg_PosIf_RdcDs_Hardware =
{
    .inputSinP              = &IfxDsadc_DS1PA_AN0_IN,
    .inputSinN              = &IfxDsadc_DS1NA_AN1_IN,
    .inputCosP              = &IfxDsadc_DS0PA_AN2_IN,
    .inputCosN              = &IfxDsadc_DS0NA_AN3_IN,
    .inputConfig            = cfg_PosIf_RdcDs_Channel,
    .carrierGen             = cfg_PosIf_RdcDs_Carrier,
    .startScan              = FALSE,
    .servReqPriority        = IFX_INTPRIO_DSADC_CH0_PRIO,
    .servReqProvider        = ISR_PROVIDER_RDC_DS,

#if IFXDSADC_RDC_TIMESTAMP_INTERNAL == 0
    .gtmTimestamp = {
         .gtm               = &MODULE_GTM,
         .rdcTim            = IfxGtm_Tim_1,
         .rdcTimChannel     = IfxGtm_Tim_Ch_0,
         .rdcTimMuxValue    = 11, // dsadc trigger

         // IfxGtm_ATOM0_7_TOUT7_P02_7 can be connected to TIM1 CH7 with mux value b0001
         .pwmTim            = &IfxGtm_TIM1_7_TIN7_P02_7_IN
    },
#endif
};

/** Configuration for DSADC resolver interface */
CONST_CFG IfxDsadc_Rdc_Config cfg_PosIf_RdcDs =
{
    .speedLpfFc             = 100,
    .errorThreshold         = 1.0/180*IFX_PI,
    .sqrAmplMax             = 2300*2300,
    .sqrAmplMin             = 2100*2100,
    .periodPerRotation      = 4,
    .reversed               = FALSE, //TRUE,
    .offset                 = 0,
    .hardware               = &cfg_PosIf_RdcDs_Hardware,
    .userTs                 = 1.0F / INVERTER_FREQUENCY,

    /* set ALL gain to zero for using default values */
    .Kp                     = 0,
    .Ki                     = 0,
    .Kd                     = 0,
};
#endif

#endif

