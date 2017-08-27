#include <Appli/DSADC/DsadcDemoEntry.h>
#include "Ifx_LutAtan2f32.h"

static uint32 dsadc_count = 0;

static IfxDsadc_Dsadc dsadc;
IfxDsadc_Dsadc_Channel dsadcChannel[IFXDSADC_NUM_CHANNELS];

#if defined(DERIVATIVE_TC22x) || defined(DERIVATIVE_TC23x) || defined(DERIVATIVE_TC24x)
uint8 dsadcChannelAvailable[IFXDSADC_NUM_CHANNELS] = { 1, 0, 0, 1 };
#elif defined(DERIVATIVE_TC27x) || defined(DERIVATIVE_TC27xB) || defined(DERIVATIVE_TC27xC) || defined(DERIVATIVE_TC27xD)
uint8 dsadcChannelAvailable[IFXDSADC_NUM_CHANNELS] = { 1, 1, 1, 1, 1, 1 };
#elif defined(DERIVATIVE_TC26x) || defined(DERIVATIVE_TC26xB)
uint8 dsadcChannelAvailable[IFXDSADC_NUM_CHANNELS] = { 1, 0, 1, 1 };
#elif defined(DERIVATIVE_TC29x) || defined(DERIVATIVE_TC29xB)
uint8 dsadcChannelAvailable[IFXDSADC_NUM_CHANNELS] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
#else# error "Testcase not prepared for this derivative!"
#endif

IfxDsadc_Rdc Rdc;
IfxStdIf_Pos dsadcRdc;

float result[];
uint32 calibration[200];
uint32 calibration_25V;
uint8  Offset_calibration_flag =0;
uint8  Offset_counter = 50;
sint16 Offset_calibration[100];
sint16 Offset_Value = 0;
uint8  Gain_calibration_flag = 0;
uint8  Gain_counter = 100;
uint32 Gain_calibration[100];
uint32 Gain_Value=0;
float  Gain_Value_float;
//typedef signed short   sint16;              /*      -32768 .. +32767          */
//typedef unsigned short uint16;

#define num_in_all 50

uint16 array_deal(uint16 *a);

uint16 N[num_in_all];
uint16 P[num_in_all];
uint16 All[num_in_all];

sint16 test_result[1000];
sint16 test_result1[1000];
uint16 result_pointer=0;
float angle[1000];

#ifdef DSADC_INT
IFX_INTERRUPT(dsadc0ISR, 0, IFX_INTPRIO_DSADC_CH0_PRIO)
{
	uint32 temp,temp1;

	IfxPort_togglePin(&MODULE_P33, 6);

    if(demo_idx == DSADC_NORMAL_CONV)
    {
		if(DSADC_EVFLAG.B.RESEV0 == 1)	DSADC_EVFLAGCLR.B.RESEC0 = 1;
		temp = DSADC_RESM0.B.RESULT;

		if((Gain_calibration_flag == 1) && (Offset_calibration_flag == 1))
    	{
    		test_result[result_pointer] = Gain_Value_float* temp;
    		result_pointer++;
    		if(result_pointer>999) result_pointer = 0;
    	}
    }
    else if (demo_idx == DSADC_RESOLVER_APP )
    {
    	if(DSADC_EVFLAG.B.RESEV0 == 1)	DSADC_EVFLAGCLR.B.RESEC0 = 1;
    	test_result[result_pointer]  = (sint16)DSADC_RESM0.B.RESULT;
    	if(DSADC_EVFLAG.B.RESEV1 == 1)	DSADC_EVFLAGCLR.B.RESEC1 = 1;
    	test_result1[result_pointer] = (sint16)DSADC_RESM1.B.RESULT;
    	//angle[result_pointer]= Ifx_LutAtan2F32_fxpAngle(test_result[result_pointer], test_result1[result_pointer]);
    	result_pointer++;
    	if(result_pointer>999) result_pointer = 0;
    }
}
#endif

void DSADC_ModuleInit(void)
{
	// create module config
	IfxDsadc_Dsadc_Config dsadcConfig;
	IfxDsadc_Dsadc_initModuleConfig(&dsadcConfig, &MODULE_DSADC);
	// initialize module
	IfxDsadc_Dsadc_initModule(&dsadc, &dsadcConfig);
}

void DSADC_ChannelInit(void)
{
	// create channel config
	IfxDsadc_Dsadc_ChannelConfig dsadcChannelConfig;
	IfxDsadc_Dsadc_initChannelConfig(&dsadcChannelConfig, &dsadc);

	// modify default configuration
	dsadcChannelConfig.modulator.positiveInput      = IfxDsadc_InputConfig_commonModeVoltage;
	dsadcChannelConfig.modulator.negativeInput      = IfxDsadc_InputConfig_commonModeVoltage;
	dsadcChannelConfig.modulator.inputGain          = IfxDsadc_InputGain_factor1;
	dsadcChannelConfig.modulator.inputPin           = IfxDsadc_InputPin_a;
	dsadcChannelConfig.modulator.modulatorClockFreq = 10.0e6;
	dsadcChannelConfig.modulator.commonModeVoltage  = IfxDsadc_CommonModeVoltage_c;
	dsadcChannelConfig.combFilter.bypassed          = FALSE;
	dsadcChannelConfig.combFilter.combFilterType    = IfxDsadc_MainCombFilterType_comb3;
	dsadcChannelConfig.combFilter.combFilterShift   = IfxDsadc_MainCombFilterShift_shiftBy3;
	dsadcChannelConfig.combFilter.serviceRequest    = IfxDsadc_MainServiceRequest_everyNewResult;
	dsadcChannelConfig.combFilter.decimationFactor  = 16;
	dsadcChannelConfig.combFilter.startValue        = 16;
	dsadcChannelConfig.firFilter.fir0Enabled        = TRUE;
	dsadcChannelConfig.firFilter.fir1Enabled        = TRUE;
	dsadcChannelConfig.firFilter.offsetCompensation = FALSE;
	dsadcChannelConfig.firFilter.dataShift          = IfxDsadc_FirDataShift_shiftBy1;
	dsadcChannelConfig.firFilter.internalShift      = IfxDsadc_FirInternalShift_noShift;

	// initialize channels
	IfxDsadc_Dsadc_initChannel(&dsadcChannel[0], &dsadcChannelConfig);
}

void DSADC_DeInit(void)
{
}

void DSADC_SimpleConv(void)
{
	sint16 result = 0;
    int chn;
	// start conversions
	IfxDsadc_Dsadc_startScan(&dsadc, 0x3f, 0x3f);
	// results are now available in IFXDSADC(ds).CH[x].RESM.B.RESULT (x=0..5)

    // declared globally    //
	for(chn=0; chn<IFXDSADC_NUM_CHANNELS; ++chn)
	{
		if( dsadcChannelAvailable[chn] )
		{
			result = IfxDsadc_Dsadc_getMainResult(&dsadcChannel[chn]);
		}
	}
}

void DSADC_Normal_Conv_Init(uint32 module, uint32 idx)
{
#ifdef iLLD
	DSADC_ModuleInit();
	DSADC_ChannelInit();
#else
	uint16 endinitPw;
	uint32 t1 = 0;
	int i;

	endinitPw = IfxScuWdt_getCpuWatchdogPassword();

	IfxScuWdt_clearCpuEndinit(endinitPw);

	DSADC_CLC.B.DISR = 0;;    // enable module clock

    while (DSADC_CLC.B.DISS == 1); // wait until module is enabled

    DSADC_KRST0.B.RST = 1;
    DSADC_KRST1.B.RST = 1;

    while (DSADC_KRST0.B.RSTSTAT == 0);

    IfxScuWdt_setCpuEndinit(endinitPw);

    DSADC_GLOBCFG.B.MCSEL = 1; //select fDSD

    /* first we set the inputs to GND to measure offset error */
    DSADC_MODCFG0.U = 0x8684800A;
    /*Bit 31 | Bit 30 | Bit 29 | Bit 28 | Bit 27 & 26 | Bit 25 & 24 | Bit 23 | Bit 22 | Bit 21 | Bit 20 | Bit 19 18 17 16|
     *  MWC  |    0   |   APC  |  GCEN  |   MCFG      |    CMVS     |   DWC  |    0   |   0    |    0   |     DIVM       |
     *Bit 15 | Bit 14 | Bit 13 & 12 | Bit 11 & 10 | Bit 9 & 8 | Bit 7 & 6 & 5 & 4 | Bit 3 & 2 | Bit 1 & 0 |
     * INCWC |  INMAC |   INMODE    |    INMUX    |   INSEL   |      GAINSEL      |  INCFGN   |  INCFGP   |
     */

    /* set Main Comb Filter (DSADC_FCFGCx) */
    DSADC_FCFGC0.U = 0x000FF60F;

    /*Bit 31 & 30 & 29 & 28 & 27 & 26 & Bit 25 & 24 | Bit 23 & 22 & 21 & 20 & 19 & 18 & 17 & 16|
     *  CFMDCNTMWC                                  |                 CFMSV                    |
     *Bit 15 & 14 | Bit 13 & 12 | Bit 11 | Bit 10 | Bit 9 & 8 | Bit 7 & 6 & 5 & 4 & 3 & 2 & & 0|
     *    SRGM    |  MFSC       |   0    | CFEN   |    CFMC   |               CFMDC            |
     */

    /* set demodulator input (DSADC_DICFGx) */
    DSADC_DICFG0.U = 0x80108080;

    /*Bit 31 | Bit 30 | Bit 29 | Bit 28 | Bit 27 | Bit 26 | Bit 25 | Bit 24 | Bit 23 & 22 & 21 & 20 | Bit 19 & 18 & 17 & 16|
     * SCWC  |    0   |   0    |  0     |   0    |   0    |    0   |    0   |     STROBE            |        CSRC          |
     *Bit 15 | Bit 14 & 13 & 12 | Bit 11 & 10 | Bit 9 & 8 | Bit 7 | Bit 6  | Bit 5 | Bit 4 | Bit 3 & 2 & 1 & 0|
     * TRWC  |       TRSEL      |   TRSTMODE  |   ITRMODE |  DSWC |   0    |   0   |   0   |      DSRC        |
     */

    /* set Filter Configuration Register (DSADC_FCFGMx) */
    DSADC_FCFGM0.U = 0x0B;
    /*Bit 31 | Bit 30 | Bit 29 | Bit 28 | Bit 27 | Bit 26 | Bit 25 | Bit 24 | Bit 23 | Bit 22 | Bit 21 | Bit 20 | Bit 19 | Bit 18 | Bit 17 | Bit 16 |
     *  0    |    0   |   0    |  0     |   0    |  0     |   0    |    0   |    0   |   0    |    0   |    0   |   0    |    0   |    0   |   0    |
     *Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9  | Bit 8  | Bit 7  | Bit 6  | Bit 5  | Bit 4 & 3  | Bit 2  | Bit 1  | Bit  0 |
     * 0     |  0     |   0    |    0   |   0    |  0     |   0    |    0   |    0   |   0    |   FSH  |     DSH    |  OCEN  | FIR1EN | FIR0EN |
     */

    /* set Integrator (DSADC_IWCTRx) */
    DSADC_IWCTR0.U = 0x0;
    /*Bit 31 | Bit 30 | Bit 29 & 28 & 27 & 26 & Bit 25 & 24 | Bit 23 | Bit 22 | Bit 21 & 20 & 19 & 18 & 17 & 16|
     *  0    |    0   |          NVALINT                    |   IWS  |    0   |         NVALDIS                |
     *Bit 15 & 14 & 13 & 12 | Bit 11 & 10 & 9 & 8 | Bit 7 | Bit 6 | Bit 5 & 4 & 3 & 2 & & 0|
     *       REPVAL         |        REPCNT       | INTEN |   0   |     NVALCNT            |
     */

#endif

#ifdef DSADC_INT
    SRC_DSADCSRM0.B.TOS = 0;							// select CPU0 to service DSADC
    SRC_DSADCSRM0.B.SRPN = IFX_INTPRIO_DSADC_CH0_PRIO;  // configure the priority of DSADC
    SRC_DSADCSRM0.B.SRE = 1;                            // enable service
    IfxPort_setPinMode(&MODULE_P33, 6, IfxPort_Mode_outputPushPullGeneral);
#endif

    DSADC_GLOBRC.U = 0x10001;//restart CH0 conversion
    /*Bit 31 | Bit 30 | Bit 29 | Bit 28 | Bit 27 | Bit 26 | Bit 25 | Bit 24 | Bit 23 | Bit 22 | Bit 21 | Bit 20 | Bit 19 | Bit 18 | Bit 17 | Bit 16 |
     *  0    |    0   |   0    |  0     |   0    |  0     |   0    |    0   |    0   |   0    |  M5RUN |  M4RUN |  M3RUN |  M2RUN |  M1RUN |  MORUN |
     *Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9  | Bit 8  | Bit 7  | Bit 6  | Bit 5  | Bit 4  | Bit 3  | Bit 2  | Bit 1  | Bit  0 |
     * 0     |  0     |   0    |    0   |   0    |  0     |   0    |    0   |    0   |   0    | CH5RUN | CH4RUN | CH3RUN | CH2RUN | CH1RUN | CH0RUN |
     */

    uint16 number_count;
    IfxCpu_disableInterrupts();

    //calibration for the offset
    for(number_count = 0; number_count<50; number_count++)
    {
    	while(SRC_DSADCSRM0.B.SRR != 1){};
    	SRC_DSADCSRM0.B.CLRR = 1;
    	if(DSADC_EVFLAG.B.RESEV0 == 1)	DSADC_EVFLAGCLR.B.RESEC0 = 1;
        Offset_calibration[number_count] = DSADC_RESM0.B.RESULT;;
    }

    Offset_Value = 0;
	for(i= 10; i< 40; i ++)
	{
		Offset_Value += Offset_calibration[i];
	}
    Offset_Value = Offset_Value/30;

	DSADC_GLOBRC.U = 0x00000;				//stop CH0 conversion
	DSADC_OFFM0.B.OFFSET=Offset_Value;
	DSADC_MODCFG0.U = 0x0000800D;           //INCFGP = Varef, INCFGN = GND
	//DSADC_FCFGM0.B.OCEN = 1;                //Enable offset compensation filter
	IfxStm_waitTicks(&MODULE_STM0, 5000000000);
	DSADC_GLOBRC.U = 0x10001;				//restart CH0 conversion
	Offset_calibration_flag = 1;

	//calibration for the gain
    for(number_count = 0; number_count<100; number_count++)
    {
    	while(SRC_DSADCSRM0.B.SRR != 1){};
    	SRC_DSADCSRM0.B.CLRR = 1;
    	if(DSADC_EVFLAG.B.RESEV0 == 1)	DSADC_EVFLAGCLR.B.RESEC0 = 1;
    	Gain_calibration[number_count] = DSADC_RESM0.B.RESULT;;
    }
    Gain_Value = 0;
    for(i =20; i<80; i++)
    {
    	Gain_Value += Gain_calibration[i];
    }
    Gain_Value      = Gain_Value/60;
    DSADC_GLOBRC.U  = 0x00000;				//stop CH0 conversion
    DSADC_MODCFG0.U = 0x86848000;           //INCFGP = Input pin, INCFGN = input pin
    IfxStm_waitTicks(&MODULE_STM0, 10000000);
    DSADC_GLOBRC.U  = 0x10001;		//restart CH0 conversion

    Gain_Value_float = (float) 5000 /(float)Gain_Value;
    Gain_calibration_flag = 1;
    IfxCpu_enableInterrupts();
}
void DSADC_RDC_ModuleInit(void)
{
	DSADC_ModuleInit();
}

void DSADC_RDC_ChannelInit(void)
{

    boolean                         result = TRUE;
    //App_printStatusTitle("Initializing Resolver %d on AURIX DSADC", index);

    IfxDsadc_Rdc_Config             config;
    IfxDsadc_Rdc_ConfigHw           ConfigHw;
    IfxDsadc_Dsadc_CarrierGenConfig configCarrier;
    IfxDsadc_Dsadc_ChannelConfig    configChannel;

    /* FIXME add initConfig() */
    int signDelay = 1;                                                                            /* APPLICATION SPECIFIC */

    /** Carrier generation configuration */
    configCarrier.bitReversed         = TRUE;                                           		/* Default */
    configCarrier.carrierWaveformMode = IfxDsadc_CarrierWaveformMode_sine;              		/* Default */
    configCarrier.frequency           = 10e3;                                           		/* Default. This is only the expected frequency, real frequency may differ */
    configCarrier.inverted            = FALSE;                                          		/* Default */
    configCarrier.pinDriver           = IfxPort_PadDriver_cmosAutomotiveSpeed1;         		/* Default. recommended, but it can be also board specific. */
    configCarrier.pinMode             = IfxPort_OutputMode_pushPull;                    		/* Default. recommended, but it can be also board specific. */
    configCarrier.pinNeg              = &IfxDsadc_CGPWMN_P00_5_OUT;                     		/* APPLICATION SPECIFIC */
    configCarrier.pinPos              = &IfxDsadc_CGPWMP_P00_6_OUT;                     		/* APPLICATION SPECIFIC */

    /** Default configuration for one DSADC resolver input channel  */
    configChannel.auxFilter.bypassed             = TRUE;                                        /* Default. Don't change */
    configChannel.auxFilter.combFilterShift      = IfxDsadc_AuxCombFilterShift_noShift;         /* Don't care, unused */
    configChannel.auxFilter.combFilterType       = IfxDsadc_AuxCombFilterType_comb1;            /* Don't care, unused */
    configChannel.auxFilter.decimationFactor     = 4;                                           /* Don't care, unused */
    configChannel.auxFilter.eventGate            = IfxDsadc_AuxGate_definedByESEL;              /* Don't care, unused */
    configChannel.auxFilter.eventSelect          = IfxDsadc_AuxEvent_everyNewResult;            /* Don't care, unused */
    configChannel.auxFilter.serviceRequest       = IfxDsadc_AuxServiceRequest_never;            /* Default. Don't change */

    configChannel.channelId                      = 0;                                           /* Don't care, will be overwritten by driver */
	configChannel.channelPins                    = NULL_PTR;                                    /* Don't care, will be overwritten by driver */

    configChannel.combFilter.bypassed            = FALSE;                                       /* Default. Don't change */
    configChannel.combFilter.combFilterShift     = IfxDsadc_MainCombFilterShift_shiftBy3;       /* APPLICATION SPECIFIC */
    configChannel.combFilter.combFilterType      = IfxDsadc_MainCombFilterType_comb3;           /* APPLICATION SPECIFIC */
    configChannel.combFilter.decimationFactor    = 16;                                          /* APPLICATION SPECIFIC */
    configChannel.combFilter.serviceRequest      = IfxDsadc_MainServiceRequest_everyNewResult;  /* Default. Don't change */
    configChannel.combFilter.startValue          = 1;                                           /* Default. Don't change */

    configChannel.demodulator.inputDataSource    = IfxDsadc_InputDataSource_onChipStandAlone;   /* Default. Don't change */
    configChannel.demodulator.integrationTrigger = IfxDsadc_IntegratorTrigger_alwaysActive;     /* Default. Don't change */
    configChannel.demodulator.sampleClockSource  = IfxDsadc_SampleClockSource_internal;         /* Default. Don't change */
    configChannel.demodulator.sampleStrobe       = IfxDsadc_SampleStrobe_sampleOnRisingEdge;    /* Default. Don't change */
    configChannel.demodulator.timestampTrigger   = (INVERTER_TRIGGER_RISING_EDGE_AT_PERIOD
                                                    ? IfxDsadc_TimestampTrigger_fallingEdge
                                                    : IfxDsadc_TimestampTrigger_risingEdge);
    configChannel.demodulator.triggerInput       = IfxDsadc_TriggerInput_a;                     /* APPLICATION SPECIFIC */

    configChannel.firFilter.dataShift            = IfxDsadc_FirDataShift_shiftBy2;              /* APPLICATION SPECIFIC */
    configChannel.firFilter.fir0Enabled          = TRUE;                                        /* APPLICATION SPECIFIC */
    configChannel.firFilter.fir1Enabled          = TRUE;                                        /* APPLICATION SPECIFIC */
    configChannel.firFilter.internalShift        = IfxDsadc_FirInternalShift_noShift;           /* APPLICATION SPECIFIC */
    configChannel.firFilter.offsetCompensation   = TRUE;                                        /* Default. Don't change */

    configChannel.integrator.discardCount        = signDelay + 1;                               /* Default */
    configChannel.integrator.integrationCount    = 16;                                          /* Default */
    configChannel.integrator.integrationCycles   = 1;                                           /* Default. Don't change */
    configChannel.integrator.windowSize          = IfxDsadc_IntegrationWindowSize_internalControl;/* Default. Don't change */

    configChannel.modulator.commonModeVoltage    = IfxDsadc_CommonModeVoltage_b;                /* Default */
    configChannel.modulator.inputGain            = IfxDsadc_InputGain_factor1;                  /* Default */
    configChannel.modulator.inputPin             = IfxDsadc_InputPin_a;                         /* Don't care, will be overwritten by driver */
    configChannel.modulator.modulatorClockFreq   = 10.0e6;                                      /* Default. This is only the expected frequency, real frequency may differ */
    configChannel.modulator.negativeInput        = IfxDsadc_InputConfig_inputPin;               /* Default. Don't change */
    configChannel.modulator.positiveInput        = IfxDsadc_InputConfig_inputPin;               /* Default. Don't change */

    configChannel.module                         = &MODULE_DSADC;                               /* Default */

    configChannel.rectifier.enabled              = TRUE;                                        /* Default. Don't change */
    configChannel.rectifier.signDelay            = signDelay;                                   /* Default */
    configChannel.rectifier.signPeriod           = 16;                                          /* Default */
    configChannel.rectifier.signSource           = IfxDsadc_RectifierSignSource_onChipGenerator;/* Default */

    /** Hardware configuration for DSADC resolver interface */
    ConfigHw.carrierGen                          = configCarrier;

    /* Here DSADC event is connected into a TIM channel. In this example, TIM0 CH0 is used.
     * Then, from tc27xC_um_v2.0OM.pdf Table 26-72 or Table 26-73, MUX value of b1011 is obtained */
    ConfigHw.gtmTimestamp.gtm                    = &MODULE_GTM;
    /* IfxGtm_ATOM0_7_TOUT7_P02_7 can be connected to TIM1 CH7 with mux. value b0001 */
    ConfigHw.gtmTimestamp.pwmTim                 = &IfxGtm_TIM1_7_TIN7_P02_7_IN;
    ConfigHw.gtmTimestamp.rdcTim                 = IfxGtm_Tim_1;
    ConfigHw.gtmTimestamp.rdcTimChannel          = IfxGtm_Tim_Ch_6;
    ConfigHw.gtmTimestamp.rdcTimMuxValue         = 0xE;

    ConfigHw.inputConfig                         = configChannel;

	ConfigHw.inputCosP                           = &IfxDsadc_DS0PA_AN2_IN;
	ConfigHw.inputCosN                           = &IfxDsadc_DS0NA_AN3_IN;
	ConfigHw.inputSinP                           = &IfxDsadc_DS1PA_AN0_IN;
	ConfigHw.inputSinN                           = &IfxDsadc_DS1NA_AN1_IN;

    ConfigHw.outputClock                         = NULL_PTR;
    ConfigHw.servReqPriority                     = IFX_INTPRIO_DSADC_CH0_PRIO;
    ConfigHw.servReqProvider                     = 0;
    ConfigHw.startScan                           = FALSE;

    /** Configuration for DSADC resolver interface */
    /* set ALL gain to zero for using default values */
    config.kd                = 0;
    config.ki                = 0;
    config.kp                = 0;
    config.errorThreshold    = 1.0f / 180 * IFX_PI;                                 /* Default */
    config.hardware          = ConfigHw;
    config.offset            = 210;     											/* APPLICATION SPECIFIC */
    config.periodPerRotation = 4;       											/* APPLICATION SPECIFIC */
    config.resolution        = 4096;    											/* APPLICATION SPECIFIC */
    config.reversed          = 0;       											/* APPLICATION SPECIFIC */
    config.speedLpfFc        = 100;                                                 /* Default */
    config.sqrAmplMax        = 2700*2700;                                         	/* APPLICATION SPECIFIC */
    config.sqrAmplMin        = 2100*2100;                                         	/* APPLICATION SPECIFIC */
    config.userTs            = 0.0001; 												/* Default */
    config.dsadc             = &dsadc;                              				/* Default */

    result                  &= IfxDsadc_Rdc_init(&Rdc, &config);
    result                  &= IfxDsadc_Rdc_stdIfPosInit(&dsadcRdc, &Rdc);

}
void DSADC_RDC_Init(module, idx)
{
#ifdef iLLD
    /** - GTM clocks */
    Ifx_GTM* gtm = &MODULE_GTM;
    //g_App.info.gtmFreq = IfxGtm_Cmu_getModuleFrequency(gtm);
    IfxGtm_enable(gtm);

    /* Set the global clock frequency */
    IfxGtm_Cmu_setGclkFrequency(gtm, 100000000);
    //g_App.info.gtmGclkFreq = IfxGtm_Cmu_getGclkFrequency(gtm);

    IfxGtm_Cmu_setClkFrequency(gtm, IfxGtm_Cmu_Clk_0, 100000000);
    //g_App.info.gtmCmuClk5Freq = IfxGtm_Cmu_getClkFrequency(gtm, IfxGtm_Cmu_Clk_5, TRUE);

#ifdef LOGIC_BOARD
	P20_IOCR4.B.PC7      = 0x10; //AD2S_SEL
	P20_OUT.B.P7         = 0;

	P32_IOCR4.B.PC6      = 0x10;
	P32_OUT.B.P6         = 1;
	P32_IOCR4.B.PC7      = 0x10;
	P32_OUT.B.P7         = 1;

	P23_IOCR4.B.PC6      = 0x10;
	P23_OUT.B.P6         = 0;
	P23_IOCR4.B.PC7      = 0x10;
	P23_OUT.B.P7         = 0;

#endif

	DSADC_RDC_ModuleInit();
	DSADC_RDC_ChannelInit();
	DSADC_GLOBRC.U = 0x30003;//restart CH0 conversion

#else
	uint16 endinitPw;
	uint32 t1 = 0;
	endinitPw = IfxScuWdt_getCpuWatchdogPassword();

	IfxScuWdt_clearCpuEndinit(endinitPw);

	DSADC_CLC.B.DISR = 0;;    // enable module clock

	while (DSADC_CLC.B.DISS == 1); // wait until module is enabled

	DSADC_KRST0.B.RST = 1;
	DSADC_KRST1.B.RST = 1;

	while (DSADC_KRST0.B.RSTSTAT == 0);

	IfxScuWdt_setCpuEndinit(endinitPw);

	//configure GTM clock
    uint16 psw = IfxScuWdt_getCpuWatchdogPassword();

    IfxScuWdt_clearCpuEndinit(psw);
    GTM_CLC.B.DISR = 0;
	IfxScuWdt_setCpuEndinit(psw);

	psw = IfxScuWdt_getCpuWatchdogPassword();
	IfxScuWdt_clearCpuEndinit(psw);
	GTM_CMU_GCLK_NUM.B.GCLK_NUM = 1;   	// Numerator for global clock divider
	GTM_CMU_GCLK_NUM.B.GCLK_NUM = 1;   	// Denominator for global clock divider
	GTM_CMU_GCLK_DEN.B.GCLK_DEN = 1;
										// float32 N          = gtm->CMU.GCLK_DEN.B.GCLK_DEN;
										// float32 multiplier = N / Z;
    									// return IfxGtm_Cmu_getModuleFrequency(gtm) * multiplier;
	IfxScuWdt_setCpuEndinit(psw);
	GTM_CMU_CLK_0_CTRL.B.CLK_CNT = 0;  	// Clock count Defines count value for the clock divider of clock source CMU_CLK[x] (x:0бн5)

	DSADC_GLOBCFG.B.MCSEL = 1; //select fDSD

	//configure carrier signal
	DSADC_CGCFG.U   = 0x00000047;
	/*
	 * Bit 31 | Bit 30 | Bit 29 | Bit 28 | Bit 27 | Bit 26 | Bit 25 | Bit 24 | Bit 23 | Bit 22 | Bit 21 | Bit 20 | Bit 19 | Bit 18 | Bit 17 | Bit 16 |
	 *   0    |  SGNCG |  STEPD | STEPS  |           STEPCOUNT               |    0   |    0   |   0    |             BITCOUNT                       |
	 * Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9  | Bit 8  | Bit 7  | Bit 6  | Bit 5  | Bit 4  | Bit 3  | Bit 2  | Bit 1  | Bit  0 |
	 *   RUN  |  0     |   0    |    0   |   0    |  0     |   0    |    0   |    DIVCG                          | SIGPOL | BREV   |      CGMOD      |
	 */
	//CGMOD   : 11B    ---> Sine wave
	//BREV    : 1B     ---> Bit-reverse mode
	//SIGPOL  : 0B     ---> Normal: carrier signal begins with +1
	//DIVCG   : 0100B  ---> 4H fCG = fCLK / 10

	IfxScuWdt_clearCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());
	P00_IOCR4.B.PC5 	 = 0x12;
	P00_IOCR4.B.PC6 	 = 0x12;

	P00_PDR0.B.PL6		 = 0;
	P00_PDR0.B.PD6		 = 0;

	P00_PDR0.B.PL5		 = 0;
	P00_PDR0.B.PD5		 = 0;
	IfxScuWdt_setCpuEndinit(IfxScuWdt_getCpuWatchdogPassword());

#ifdef LOGIC_BOARD
	P20_IOCR4.B.PC7      = 0x10; //AD2S_SEL
	P20_OUT.B.P7         = 0;

	P32_IOCR4.B.PC6      = 0x10;
	P32_OUT.B.P6         = 1;
	P32_IOCR4.B.PC7      = 0x10;
	P32_OUT.B.P7         = 1;

	P23_IOCR4.B.PC6      = 0x10;
	P23_OUT.B.P6         = 0;
	P23_IOCR4.B.PC7      = 0x10;
	P23_OUT.B.P7         = 0;

#endif

#if 1
	/* set the modulator (DSADC_MODCFGx) */
	DSADC_MODCFG0.U = 0x81848000;
	DSADC_MODCFG1.U = 0x81848000;
	/*Bit 31 | Bit 30 | Bit 29 | Bit 28 | Bit 27 & 26 | Bit 25 & 24 | Bit 23 | Bit 22 | Bit 21 | Bit 20 | Bit 19 18 17 16|
	 *  MWC  |    0   |   APC  |  GCEN  |   MCFG      |    CMVS     |   DWC  |    0   |   0    |    0   |     DIVM       |
	 *Bit 15 | Bit 14 | Bit 13 & 12 | Bit 11 & 10 | Bit 9 & 8 | Bit 7 & 6 & 5 & 4 | Bit 3 & 2 | Bit 1 & 0 |
	 * INCWC |  INMAC |   INMODE    |    INMUX    |   INSEL   |      GAINSEL      |  INCFGN   |  INCFGP   |
	 */
	//INCFGP  : 00B    ---> Input pin
	//INCFGN  : 00B    ---> Input pin
	//GAINSEL : 0000B  ---> Gain factor 1
	//INSEL   : 00B    ---> Input pin position A
	//INMUX   : 00B    ---> follow INSEL
	//INMODE  : 00B    ---> Software control (INMUX follows INSEL)
	//INMAC   : 0B     ---> Preset mode (load INMUX upon a trigger)
    //INCWC   : 1B     ---> Bitfields INCFGP, INCFGN, GAINSEL, INSEL,INMODE, INMAC can be written
	//DIVM    : 0100B  ---> divide by 10
	//DWC     : 1B     ---> Bitfield DIVM can be written
    //CMVS    : 01B    ---> VCM = VAREF / 2.27
    //MCFG    : 00B    ---> High-performance mode active
	//GEEN    : 0B     ---> Normal operation
	//APC     : 0B     ---> Off: Modulator active while its associated bit MxRUN is set
	//MWC     : 1B     ---> Bitfields CMVS, MCFG, GCEN, APC can be written

	/* set Main Comb Filter (DSADC_FCFGCx) */
	DSADC_FCFGC0.U = 0x0000F60F;
	DSADC_FCFGC1.U = 0x0000F60F;
	/*Bit 31 & 30 & 29 & 28 & 27 & 26 & Bit 25 & 24 | Bit 23 & 22 & 21 & 20 & 19 & 18 & 17 & 16|
	 *  CFMDCNTMWC                                  |                 CFMSV                    |
	 *Bit 15 & 14 | Bit 13 & 12 | Bit 11 | Bit 10 | Bit 9 & 8 | Bit 7 & 6 & 5 & 4 & 3 & 2 & & 0|
	 *    SRGM    |  MFSC       |   0    | CFEN   |    CFMC   |               CFMDC            |
	 */
    //CFMDF   : 0x0F    ---> The decimation factor of the Main CIC filter is 16
	//CFMC    : 10B     ---> CIC3
	//CFEN    : 01B     ---> Enable CIC filter
	//MFSC    : 11B     ---> Shift by 3
	//SRGM    : 11B     ---> Always, for each new result value
	//CFMSV   : 0x00    ---> The decimation counter begins counting at 0


	/* set demodulator input (DSADC_DICFGx) */
    DSADC_DICFG0.U = 0x80108780;
    DSADC_DICFG1.U = 0x80108780;
	/*Bit 31 | Bit 30 | Bit 29 | Bit 28 | Bit 27 | Bit 26 | Bit 25 | Bit 24 | Bit 23 & 22 & 21 & 20 | Bit 19 & 18 & 17 & 16|
	 * SCWC  |    0   |   0    |  0     |   0    |   0    |    0   |    0   |     STROBE            |        CSRC          |
	 *Bit 15 | Bit 14 & 13 & 12 | Bit 11 & 10 | Bit 9 & 8 | Bit 7 | Bit 6  | Bit 5 | Bit 4 | Bit 3 & 2 & 1 & 0|
	 * TRWC  |       TRSEL      |   TRSTMODE  |   ITRMODE |  DSWC |   0    |   0   |   0   |      DSRC        |
	 */
    //DSRC    : 0000B  ---> On-chip modulator, standalone
    //DSWC    : 1B     ---> Bitfield DSRC can be written
    //ITRMODE : 11B    ---> No trigger, integrator active all the time
    //TRSTMODE: 01B    ---> Time Stamp Trigger event upon a falling edge
    //TRSEL   : 000B   ---> Trigger signal, channel 0, input A
    //TRWC    : 1B     ---> Bitfields TRSEL, TSTRMODE, ITRMODE can be written
    //CSRC    : 0000B  ---> Internal clock
    //STROBE  : 0001B  ---> Direct clock, a sample trigger is generated at each rising clock edge
    //SCWC    : 1B     ---> Bitfields STROBE, CSRC can be written


	/* set Filter Configuration Register (DSADC_FCFGMx) */
	DSADC_FCFGM0.U = 0x00000017;
	DSADC_FCFGM1.U = 0x00000017;
	/*Bit 31 | Bit 30 | Bit 29 | Bit 28 | Bit 27 | Bit 26 | Bit 25 | Bit 24 | Bit 23 | Bit 22 | Bit 21 | Bit 20 | Bit 19 | Bit 18 | Bit 17 | Bit 16 |
	 *  0    |    0   |   0    |  0     |   0    |  0     |   0    |    0   |    0   |   0    |    0   |    0   |   0    |    0   |    0   |   0    |
	 *Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9  | Bit 8  | Bit 7  | Bit 6  | Bit 5  | Bit 4 & 3  | Bit 2  | Bit 1  | Bit  0 |
	 * 0     |  0     |   0    |    0   |   0    |  0     |   0    |    0   |    0   |   0    |   FSH  |     DSH    |  OCEN  | FIR1EN | FIR0EN |
	 */
    //FIR0EN  : 1B     ---> Enable FIR filter 0
	//FIR1EN  : 1B     ---> Enable FIR filter 1
	//OCEN    : 1B     ---> Enable offset compensation filter
	//DSH     : 10B    ---> Shift by 2
	//FSH     : 0B     ---> No shift, use full range

	/* set Integrator (DSADC_IWCTRx) */
	DSADC_IWCTR0.U = 0x0F020000;
	DSADC_IWCTR1.U = 0x0F020000;
	/*Bit 31 | Bit 30 | Bit 29 & 28 & 27 & 26 & Bit 25 & 24 | Bit 23 | Bit 22 | Bit 21 & 20 & 19 & 18 & 17 & 16|
	 *  0    |    0   |          NVALINT                    |   IWS  |    0   |         NVALDIS                |
	 *Bit 15 & 14 & 13 & 12 | Bit 11 & 10 & 9 & 8 | Bit 7 | Bit 6 | Bit 5 & 4 & 3 & 2 & & 0|
	 *       REPVAL         |        REPCNT       | INTEN |   0   |     NVALCNT            |
	 */
    //REPVAL   : 0000B   ---> Defines the number of integration cycles to be counted by REPCNT if activated (IWS = 0).The number of cycles is <REPVAL+1>.
	//NVALDIS  : 000010B ---> Start the integration cycle after NVALDIS values
	//IWS      : 0B      ---> Integration Window Size
	//NVALINT  : 001111B ---> Stop the integration cycle after <NVALINT+1> values

	DSADC_RECTCFG0.U = 0x00000001;
	DSADC_RECTCFG1.U = 0x00000001;
	/*Bit 31 | Bit 30 | Bit 29 & 28 & 27 & 26 & Bit 25 & 24 | Bit 23 | Bit 22 | Bit 21 & 20 & 19 & 18 & 17 & 16|
	 *  SGND | SGNDCS |                                                                                        |
	 *Bit 15 & 14 & 13 & 12 & 11 & 10 & 9 & 8 & 7 & 6 | Bit 5 & 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
	 * SDCVAL|                                        |  SSRC     |                       | RFEN  |
	 */
	//RFEN     : 1B       ---> Data are rectified according to SGND
	//SSRC     : 0B       ---> On-chip carrier generator

    //DSADC_CGSYNC0.U = 0x09010000;
    //DSADC_CGSYNC1.U = 0x09010000;

    DSADC_CGSYNC0.U = 0x08000000;
    DSADC_CGSYNC1.U = 0x08000000;
	/*Bit 31 & 30 & 29 & 28 & 27 & 26 & Bit 25 & 24 | Bit 23 & 22 & 21 & 20 & 19 & 18 & 17 & 16|
	 *                   SDNEG                      |                SDPOS                     |
	 *Bit 15 & 14 & 13 & 12 & 11 & 10 & 9 & 8 | Bit 7 & 6 & 5 & 4 & 3 & 2 & & 0|
	 *                   SDCAP                |            SDCOUNT             |
	 */
    //SDNED    : 0x09     ---> Defines the content of SDCOUNT to generate a high delayed sign signal SGND
    //SDPOS    : 0X01     ---> Defines the content of SDCOUNT to generate a low delayed sign signal SGND
#endif

#ifdef DSADC_INT
	SRC_DSADCSRM0.B.TOS = 0;							// select CPU0 to service DSADC
	SRC_DSADCSRM0.B.SRPN = IFX_INTPRIO_DSADC_CH0_PRIO;  // configure the priority of DSADC
	SRC_DSADCSRM0.B.SRE = 1;                            // enable service
	IfxPort_setPinMode(&MODULE_P33, 6, IfxPort_Mode_outputPushPullGeneral);

#endif

	//configure TIM channel for the time stamp of DSADC
    GTM_TIM1_CH6_CTRL.U = 0x00002005;
	/*Bit 31 & 30 | Bit 29    | Bit 28    | Bit 27     | Bit 26 & 25 & 24 | Bit 23     | Bit 22      | Bit 21     | Bit 20     | Bit 19     | Bit 18 & 17 | Bit 16 |
	 *  TOCTRL    | EGPR1_SEL | EGPR0_SEL | FRECNT_OFL |      CLKSEL      | FLT_CTR_FE | FLT_MODE_FE | FLT_CTR_RE | FLT_MODE_RE| EXT_CAP_EN | FLT_CNT_FRQ | FLT_EN |
	 *Bit 15      | Bit 14    | Bit 13    | Bit 12     | Bit 11 & 10      | Bit 9 & 8  | Bit 7       | Bit 6      | Bit 5      | Bit 4      | Bit 3 & 2 &1| Bit  0 |
	 * ECNT_RESET | ISL       | DSL       | CNTS_SEL   | GPRS_SEL         | GPR0_SEL   | Reserved    | CICTRL     | ARU_EN     | OSM        | TIM_MODE    | TIM_EN |
	 */
    //TIM_EN   		: 1B      ---> Channel enabled
	//TIM_MODE 		: 010B    ---> Input Event Mode (TIEM)
	//OSM      		: 0B      ---> Continuous operation mode
	//ARU_EN   		: 0B 	 ---> Registers content not routed
	//CICTRL   		: 0B 	 ---> use signal TIM_IN(x) as input for channel x
	//GPR0_SEL 		: 00B 	 ---> use TBU_TS0 as input
	//GPR1_SEL 		: 00B 	 ---> use TBU_TS0 as input
	//CNTS_SEL 		: 0B 	 ---> use CNT register as input
	//DSL      		: 1B 	 ---> Measurement starts with rising edge (high level measurement)
	//ISL      		: 0B 	 ---> use DSL bit for selecting active signal level
	//ECNT_RESET	: 0B 	 ---> ECNT counter operating in wrap around mode
	//FLT_EN   		: 0B 	 ---> Filter disabled and internal states are reset
	//FLT_CNT_FRQ   : 00B 	 ---> FLT_CNT counts with CMU_CLK0
	//EXT_CAP_EN   	: 0B 	 ---> External capture disabled
	//FLT_MODE_RE 	: 0B 	 ---> Immediate edge propagation mode
	//FLT_CTR_RE   	: 0B 	 ---> Up/Down Counter
	//FLT_MODE_FE 	: 0B 	 ---> Immediate edge propagation mode
	//FLT_CTR_FE   	: 0B 	 ---> Up/Down Counter
	//CLKSEL     	: 000B 	 ---> CMU_CLK0 selected
	//FRECNT_OFL   	: 0B 	 ---> Overflow will be signalled on ECNT bit width = 8
	//ERPR0_SEL   	: 0B 	 ---> Details described in GPR0_SEL bit field.
    //ERPR1_SEL   	: 0B 	 ---> Details described in GPR1_SEL bit field.
    //TOCTRL     	: 00B 	 ---> Timeout feature disabled

    GTM_TIM1INSEL.B.CH6SEL   = 14;
    //CH6SEL        : 1110B  ---> DSADC trigger
    //GTM_TBU_CHEN.B.ENDIS_CH0 = 3;
    IfxGtm_Tbu_enableChannel(&MODULE_GTM, IfxGtm_Tbu_Ts_0);
    //ENDIS_CH0     : 11B    ---> channel enabled

	DSADC_GLOBRC.U = 0x30003;//restart CH0 conversion
	/*Bit 31 | Bit 30 | Bit 29 | Bit 28 | Bit 27 | Bit 26 | Bit 25 | Bit 24 | Bit 23 | Bit 22 | Bit 21 | Bit 20 | Bit 19 | Bit 18 | Bit 17 | Bit 16 |
	 *  0    |    0   |   0    |  0     |   0    |  0     |   0    |    0   |    0   |   0    |  M5RUN |  M4RUN |  M3RUN |  M2RUN |  M1RUN |  MORUN |
	 *Bit 15 | Bit 14 | Bit 13 | Bit 12 | Bit 11 | Bit 10 | Bit 9  | Bit 8  | Bit 7  | Bit 6  | Bit 5  | Bit 4  | Bit 3  | Bit 2  | Bit 1  | Bit  0 |
	 * 0     |  0     |   0    |    0   |   0    |  0     |   0    |    0   |    0   |   0    | CH5RUN | CH4RUN | CH3RUN | CH2RUN | CH1RUN | CH0RUN |
	 */
	IfxCpu_disableInterrupts();

    //calibration for SDPOS and SDNEG
	uint8 counter0,counter1;
    while(SRC_DSADCSRM0.B.SRR != 1){};
    SRC_DSADCSRM0.B.CLRR = 1;
    if(DSADC_EVFLAG.B.RESEV0 == 1)	DSADC_EVFLAGCLR.B.RESEC0 = 1;
    test_result[0]  = (sint16)DSADC_RESM0.B.RESULT;
    counter0 = DSADC_CGSYNC0.B.SDCAP;
    if(DSADC_EVFLAG.B.RESEV1 == 1)	DSADC_EVFLAGCLR.B.RESEC1 = 1;
    test_result1[0] = (sint16)DSADC_RESM1.B.RESULT;
    counter1 = DSADC_CGSYNC1.B.SDCAP;

    if((test_result[0]>= 0)&&(test_result1[0]>= 0))
    {
    	//SIN signal, resolver position is between 0 degrees and 90 degrees
    	DSADC_CGSYNC0.B.SDPOS = counter0;
    	DSADC_CGSYNC0.B.SDNEG = (counter0 + (DSADC_IWCTR0.B.NVALINT + 1)/2)%(DSADC_IWCTR0.B.NVALINT + 1);

    	//COS signal, resolver position is between 0 degrees and 90 degrees
    	DSADC_CGSYNC1.B.SDPOS = counter1;
    	DSADC_CGSYNC1.B.SDNEG = (counter1 + (DSADC_IWCTR1.B.NVALINT + 1)/2)%(DSADC_IWCTR1.B.NVALINT + 1);
    }
    else if((test_result[0]>= 0)&&(test_result1[0]<= 0))
    {
    	//SIN signal, resolver position is between 90 degrees and 180 degrees
    	DSADC_CGSYNC0.B.SDPOS = counter0;
    	DSADC_CGSYNC0.B.SDNEG = (counter0 + (DSADC_IWCTR0.B.NVALINT + 1)/2)%(DSADC_IWCTR0.B.NVALINT + 1);

    	//COS signal, resolver position is between 90 degrees and 180 degrees
    	DSADC_CGSYNC1.B.SDNEG = counter1;
    	DSADC_CGSYNC1.B.SDPOS = (counter1 + (DSADC_IWCTR1.B.NVALINT + 1)/2)%(DSADC_IWCTR1.B.NVALINT + 1);

    }
    else if((test_result[0]<= 0)&&(test_result1[0]<= 0))
    {
    	//SIN signal, resolver position is between 180 degrees and 270 degrees
    	DSADC_CGSYNC0.B.SDNEG = counter0;
    	DSADC_CGSYNC0.B.SDPOS = (counter0 + (DSADC_IWCTR0.B.NVALINT + 1)/2)%(DSADC_IWCTR0.B.NVALINT + 1);

    	//COS signal, resolver position is between 180 degrees and 270 degrees
    	DSADC_CGSYNC1.B.SDNEG = counter1;
    	DSADC_CGSYNC1.B.SDPOS = (counter1 + (DSADC_IWCTR1.B.NVALINT + 1)/2)%(DSADC_IWCTR1.B.NVALINT + 1);
    }
    else
    {
    	//SIN signal, resolver position is between 270 degrees and 360 degrees
    	DSADC_CGSYNC0.B.SDNEG = counter0;
    	DSADC_CGSYNC0.B.SDPOS = (counter0 + (DSADC_IWCTR0.B.NVALINT + 1)/2)%(DSADC_IWCTR0.B.NVALINT + 1);

    	//COS signal, resolver position is between 270 degrees and 3600 degrees
    	DSADC_CGSYNC1.B.SDPOS = counter1;
    	DSADC_CGSYNC1.B.SDNEG = (counter1 + (DSADC_IWCTR1.B.NVALINT + 1)/2)%(DSADC_IWCTR1.B.NVALINT + 1);
    }

    IfxCpu_enableInterrupts();
#if 0
	uint32 flag=0;
    flag = DSADC_CGCFG.B.SGNCG;
    IfxPort_setPinMode(&MODULE_P20, 10, IfxPort_Mode_outputPushPullGeneral);
    while(1)
    {
    	//while(flag != DSADC_CGCFG.B.SGNCG){};
    	//flag = DSADC_CGCFG.B.SGNCG;
    	//IfxPort_togglePin(&MODULE_P20, 10);
    	P20_OUT.B.P10 = DSADC_CGCFG.B.SGNCG;
    }
#endif

#endif
}
void DSADC_DemoInit(uint32 module, uint32 idx)
{
	if(idx == DSADC_NORMAL_CONV)
	{
		DSADC_Normal_Conv_Init(module, idx);
	}
	else if(idx == DSADC_RESOLVER_APP)
	{
		DSADC_RDC_Init(module, idx);
	}
}


void DSADC_Cyclic(void)
{
	//IfxPort_togglePin(&MODULE_P33, 6);
	if (DSADC_EVFLAG.B.RESEV0 == 1)
	{
		//DSADC_EVFLAGCLR.B.RESEC0 = 1;
		//IfxPort_togglePin(&MODULE_P33, 6);
	}
}
void DSADC_Resolver_Cyclic(void)
{

}
void DSADC_DemoDeInit(uint32 module, uint32 idx)
{
	DSADC_DeInit();
}

void DSADC_DemoRun(uint32 module, uint32 idx)
{
	dsadc_count++;

	if(dsadc_count >= DSADC_PRE_DIVDER)
	{
		dsadc_count = 0;
		if(idx == DSADC_NORMAL_CONV)
			DSADC_Cyclic();
		else if (idx == DSADC_RESOLVER_APP)
			DSADC_Resolver_Cyclic();
	}
}


uint16 uint16_neg_to_abs(uint16 a)
{
	sint16 b,c;
	uint16 result;
	//printf("%u\n",a);
	b = (sint16) a;
	//printf("%d\n",b);
	c = 0 - b;
	//printf("%d\n",c);
	result = (uint16) c;
	//printf("%u\n",result);
	return result;
}

void num_deal(uint16 *a)
{
	uint16 *b;
	uint16 temp;
	int num=0;
	int num_p=0;
	int num_n=0;
	//printf("Done");
	for(b=a;num!=num_in_all;num++)
	{
		//printf("%u\n",b[num]);
		if((b[num])<200)
		{
			P[num_p++]=b[num];
			//printf("%u\n",b[num]);
		}
		else if(b[num]>0x7fff)	//confirm is a neg
		{
			temp = uint16_neg_to_abs(b[num]);
			if(temp<2000)
			{
				N[num_n++]=temp;
				//printf("%u\n",temp);
			}
		}
	}
	N[num_n]=0xffff;
	P[num_p]=0xffff;
	num=0;
	for(num_p=0;P[num_p]!=0xffff;num_p++)
	{
		All[num++]=P[num_p];
	}
	for(num_n=0;N[num_n]!=0xffff;num_n++)
	{
		All[num++]=N[num_n];
		//printf("%u",All[num]);
	}
	All[num]=0xffff;
	/*
	num=0;
	while(All[num]!=0xffff)
	{
		printf("%u\n",All[num]);
		num++;
	}
	*/
}

uint16 average(uint16 *a)
{
	uint16 result;
	uint16 sum=0;
	uint16 num=0;
	uint16 *b;
	for(b=a;b[num]!=0xffff;num++)
	{
		sum += b[num];
		//num++;
	}
	result = sum / num;
	return result;
}

uint16 array_deal(uint16 *a)
{
	uint16 result;
	num_deal(a);
	printf("Done");
	result = average(All);
	return result;
}
