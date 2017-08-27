#include "Appli_Ccu6.h"
#include <string.h>
#include "Tricore\Appli\Entry\Appli_Entry.h"



// used for PWM demo
static IfxCcu6_TPwm tPwm;

// used for ICU
static IfxCcu6_Icu icu;
static IfxCcu6_Icu_Channel icuChannel;
// assumed to be declared globally   //
uint32 timeStamp[100];

static uint8 icu_cnt = 0;
uint8 pwm_duty = 1;
extern int demo_input;
int captured_count=0;
IFX_INTERRUPT(ccu60ISR_Icu, 0, IFX_INTPRIO_CCU6)
{
	//user code
	// store the time stamp in an array
	timeStamp[icu_cnt++] = IfxCcu6_Icu_getTimeStamp(&icuChannel);
	if(icu_cnt >= 100)
		icu_cnt = 0;
}

IFX_INTERRUPT(ccu60ISR_TPwm, 0, IFX_INTPRIO_CCU6)
{
	//user code
}

void CCU6_TPwm_Initialize(uint8 duty)
{
	// create configuration
	IfxCcu6_TPwm_Config tPwmConfig;
	IfxCcu6_TPwm_initModuleConfig(&tPwmConfig, &MODULE_CCU60);

	// configure the frequency of the timer in case of internal start
	// this frequency will be set for the timer block selected later
	tPwmConfig.base.frequency = 100000;  //PWM period 100K,compareValue factor is 10
										//PWM period 80K,compareValue factor is 12.5
										//PWM period 10K,compareValue factor is 100
										//PWM period 8K,compareValue factor is 125
										//PWM period 1K,compareValue factor is 125
	// configure the period of the timer
	tPwmConfig.base.period = 100;   //frequence resolution ,not the period

	// configure the waiting time in case of delayed T13 start in sync with T12
	tPwmConfig.base.waitingTime = 20;

	// select the active state of the output
	tPwmConfig.base.activeState = Ifx_ActiveState_high;

	// select the timer through which PWM is to be generated
	tPwmConfig.timer = IfxCcu6_TimerId_t13;

	// configure the clock for internal mode
	tPwmConfig.clock.t13ExtClockEnabled   = FALSE;
	tPwmConfig.clock.t13ExtClockInput     = NULL_PTR;
	tPwmConfig.clock.t13countingInputMode = IfxCcu6_CountingInputMode_internal;

	// configure the selcted timer block
	tPwmConfig.timer13.counterValue     = 0;
	tPwmConfig.timer13.compareValue     = 50 * 10 ;   //50%

	// configure the syncronisation, in case of sync start with T12
	tPwmConfig.timer13.t12SyncEvent     = IfxCcu6_T13TriggerEvent_onCC60RCompare;
	tPwmConfig.timer13.t12SyncDirection = IfxCcu6_T13TriggerDirection_onT12CountingUp;

	//select the channel out for modulation
	tPwmConfig.channelOut = IfxCcu6_ChannelOut_cout3;

	// pin configuration
	const IfxCcu6_TPwm_Pins pins = {
			NULL,   // CC60Out pin not used
			NULL,   // CC61Out pin not used
			NULL,   // CC62Out pin not used
			NULL,   // COUT60 pin not used
			NULL,   // COUT61 pin not used
			NULL,   // COUT62 pin not used
			&IfxCcu60_COUT63_P00_0_OUT,  // COUT63 pin
			IfxPort_OutputMode_pushPull,
			IfxPort_PadDriver_cmosAutomotiveSpeed1
	};

	tPwmConfig.pins = &pins;

	// configure the interrupts
	tPwmConfig.interrupt1.source = IfxCcu6_InterruptSource_t13CompareMatch;
	tPwmConfig.interrupt1.serviceRequest  = IfxCcu6_ServiceRequest_2;
	tPwmConfig.interrupt1.priority        = IFX_INTPRIO_CCU6;
	tPwmConfig.interrupt1.typeOfService   = IfxSrc_Tos_cpu0;

	// configure input and output triggers
	tPwmConfig.trigger.t12ExtInputTrigger   = &IfxCcu60_T12HRB_P00_7_IN;
	tPwmConfig.trigger.t13ExtInputTrigger   = NULL_PTR;
	tPwmConfig.trigger.extInputTriggerMode  = IfxCcu6_ExternalTriggerMode_risingEdge;
	tPwmConfig.trigger.t13InSyncWithT12     = TRUE;
	tPwmConfig.trigger.outputTriggerEnabled = TRUE;
	tPwmConfig.trigger.outputLine           = IfxCcu6_TrigOut_0;
	tPwmConfig.trigger.outputTrigger      = IfxCcu6_TrigSel_cout63;

	// initialize the module
	IfxCcu6_TPwm_initModule(&tPwm, &tPwmConfig);

    // install interrupt handlers
    IfxCpu_Irq_installInterruptHandler(&ccu60ISR_TPwm, IFX_INTPRIO_CCU6);
    IfxCpu_enableInterrupts();
}

void CCU6_Icu_Initialize(void)
{
	// ---- when not using inconjunction with Timer driver ---- //
	// create configuration
	IfxCcu6_Icu_Config icuConfig;
	IfxCcu6_Icu_initModuleConfig(&icuConfig, &MODULE_CCU60);

	// configure the frequency of the timer in case of internal start
	icuConfig.frequency = 400000;

	// configure the period of the timer
	icuConfig.period = 100;

	// configure the clock for internal mode
	icuConfig.clock.extClockEnabled   = FALSE;
	icuConfig.clock.extClockInput     = NULL_PTR;
	icuConfig.clock.countingInputMode = IfxCcu6_CountingInputMode_internal;

	// configure count operation
	icuConfig.countMode     = IfxCcu6_T12CountMode_edgeAligned;
	icuConfig.counterValue  = 0;

	// initialize the module
	IfxCcu6_Icu_initModule(&icu, &icuConfig);

	// -------------------------------------------------------- //
	// ---- when not using inconjunction with Timer driver ---- //
	// after initialising the timer
	IfxCcu6_Timer timer;
	IfxCcu6_Timer_Config timerConfig;

	// -------------------------------------------------------- //
	// create Icu channel config
	IfxCcu6_Icu_ChannelConfig icuChannelConfig;
	IfxCcu6_Icu_initChannelConfig(&icuChannelConfig, &MODULE_CCU60);

	// ---- when using inconjunction with Timer driver ---- //
	icuChannelConfig.timer = timer;
	// ---------------------------------------------------- //

	// configure the channel
	icuChannelConfig.channelId       = IfxCcu6_T12Channel_0;
	icuChannelConfig.channelMode     = IfxCcu6_T12ChannelMode_doubleRegisterCaptureRising;

	// configure the interrupts
	icuChannelConfig.interrupt1.source = IfxCcu6_InterruptSource_cc60RisingEdge;
	icuChannelConfig.interrupt1.serviceRequest  = IfxCcu6_ServiceRequest_2;
	icuChannelConfig.interrupt1.priority        = IFX_INTPRIO_CCU6;
	icuChannelConfig.interrupt1.typeOfService   = IfxSrc_Tos_cpu0;

	// ---- when not using inconjunction with Timer driver ---- //
	// configure input and output triggers
	icuChannelConfig.trigger.extInputTrigger   = &IfxCcu60_T12HRB_P00_7_IN;
	icuChannelConfig.trigger.extInputTriggerMode  = IfxCcu6_ExternalTriggerMode_risingEdge;

	// -------------------------------------------------------- //
	// pin configuration, in case of multi input capture mode select the respective CC6xIn and CCPOSxIn pins of the selected channel
	const IfxCcu6_Icu_Pins pins = {
			&IfxCcu60_CC60INA_P02_0_IN,  // CC60In pin
			NULL,   // CC61In pin not used
			NULL,   // CC62In pin not used
			NULL,   // CCPOS0In pin not used
			NULL,   // CCPOS1In pin not used
			NULL,   // CCPOS2In pin not used
			IfxPort_InputMode_pullUp,
			IfxPort_InputMode_pullUp
	};

	icuChannelConfig.pins = &pins;

	// configure multi input capture mode
	icuChannelConfig.multiInputCaptureEnabled    = FALSE;

	// initialize the channel
	IfxCcu6_Icu_initChannel(&icuChannel, &icuChannelConfig);

	// install interrupt handlers
	IfxCpu_Irq_installInterruptHandler(&ccu60ISR_Icu, IFX_INTPRIO_CCU6);
	IfxCpu_enableInterrupts();
}

extern int demo_item;
void Appli_Ccu6Init(void)
{
}

void Appli_Ccu6DemoInit(void)
{
	Appli_Ccu6Init();

	if(demo_item == 1)
	{
		CCU6_TPwm_Initialize(pwm_duty);
		IfxCcu6_TPwm_start(&tPwm);
	}
	else if(demo_item == 2)
	{
		CCU6_Icu_Initialize();
		IfxCcu6_Icu_startCapture(&icuChannel);
	}
}

void Appli_Ccu6DemoDeInit(void)
{
	if(demo_item == 1)
		IfxCcu6_TPwm_stop(&tPwm);
	else if(demo_item == 2)
		IfxCcu6_Icu_stopCapture(&icuChannel);
}

void Appli_Ccu6Demo_PWM(void)
{
	if(demo_input != pwm_duty)
	{
		pwm_duty = demo_input;
		if(pwm_duty > 15)
			pwm_duty = 15;
		IfxCcu6_TPwm_stop(&tPwm);
		CCU6_TPwm_Initialize(pwm_duty);
		IfxCcu6_TPwm_start(&tPwm);
	}
}

void Appli_Ccu6Demo_ICU(void)
{

	char str_adc[7];
	uint32 captured_value ;

	CCU6_Icu_Initialize();
	IfxCcu6_Icu_startCapture(&icuChannel);
	captured_value=IfxCcu6_getCaptureRegisterValue(icuChannel.ccu6, icuChannel.channelId);
	ConsolePrint("captured_value :");
	ShortToAscii(captured_value, str_adc);
	str_adc[6]='\0';
	ConsolePrint(str_adc);
	ConsolePrint("\r\n");
}


void Appli_Ccu6Cyclic(void)
{
	captured_count++;
	if(captured_count>500)
	{
		captured_count=0;
	if(demo_item == 1)
		Appli_Ccu6Demo_PWM();
	else if(demo_item == 2)
		Appli_Ccu6Demo_ICU();
	}
}

