#pragma GCC optimize "-O0" /* Disable optimisation for debugging */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <machine/cint.h>

#include "Cpu\Std\Ifx_Types.h"
#include "Cpu\Std\IfxCpu_Intrinsics.h"
#include "Scu\Std\IfxScuWdt.h"
#include "main.h"
#include "Port\Std\IfxPort.h"
#include "IfxAsclin_reg.h"
#include <Asclin/Asc/IfxAsclin_Asc.h>
#include <Stm/Std/IfxStm.h>
#include <Dts/Dts/IfxDts_Dts.h>
#include "Configuration.h"

//#include "Appli/CAN/MCanDemoEntry.h"
#include "Gtm/Tom/PwmHl/IfxGtm_Tom_PwmHl.h"
#include "CANTest.h"
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/
#define IFX_INTPRIO_ASCLIN0_TX 1
#define IFX_INTPRIO_ASCLIN0_RX 2
#define IFX_INTPRIO_ASCLIN0_ER 3
#define IFX_INTPRIO_SCHD_STM0_SR0  110
#define ISR_PRIORITY_TIMER          111 /**< \brief Define the 1ms timer interrupt priority.  */

#define ISR_PROVIDER_TIMER          IfxSrc_Tos_cpu0 /**< \brief Define the 1ms timer interrupt provider.  */
#define INTERRUPT_TIMER             ISR_ASSIGN(ISR_PRIORITY_TIMER, ISR_PROVIDER_TIMER)                   /**< \brief Define the 1ms timer interrupt priority.  */

uint32 system_tick_2 = 0;

typedef enum
{
    GtmTomPwmHl_State_init = 0,
    GtmTomPwmHl_State_off,
	GtmTomPwmHl_State_duty0,
	GtmTomPwmHl_State_duty30,
	GtmTomPwmHl_State_duty31,
	GtmTomPwmHl_State_duty32,
	GtmTomPwmHl_State_duty33,
	GtmTomPwmHl_State_duty34,
	GtmTomPwmHl_State_duty35,
	GtmTomPwmHl_State_duty36,
	GtmTomPwmHl_State_duty37,
	GtmTomPwmHl_State_duty38,
	GtmTomPwmHl_State_duty39,
	GtmTomPwmHl_State_duty40,
	GtmTomPwmHl_State_duty41,
	GtmTomPwmHl_State_duty42,
	GtmTomPwmHl_State_duty43,
	GtmTomPwmHl_State_duty44,
	GtmTomPwmHl_State_duty45,
	GtmTomPwmHl_State_duty46,
	GtmTomPwmHl_State_duty47,
	GtmTomPwmHl_State_duty48,
	GtmTomPwmHl_State_duty49,
	GtmTomPwmHl_State_duty50,
	GtmTomPwmHl_State_duty51,
	GtmTomPwmHl_State_duty52,
	GtmTomPwmHl_State_duty53,
	GtmTomPwmHl_State_duty54,
	GtmTomPwmHl_State_duty55,
	GtmTomPwmHl_State_duty56,
	GtmTomPwmHl_State_duty57,
	GtmTomPwmHl_State_duty58,
	GtmTomPwmHl_State_duty59,
	GtmTomPwmHl_State_duty60,
	GtmTomPwmHl_State_duty61,
	GtmTomPwmHl_State_duty62,
	GtmTomPwmHl_State_duty63,
	GtmTomPwmHl_State_duty64,
	GtmTomPwmHl_State_duty65,
	GtmTomPwmHl_State_duty66,
	GtmTomPwmHl_State_duty67,
	GtmTomPwmHl_State_duty68,
	GtmTomPwmHl_State_duty69,
	GtmTomPwmHl_State_duty70,
    GtmTomPwmHl_State_duty100,
    GtmTomPwmHl_State_count
}GtmTomPwmHl_State;

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/** \brief Application information */
typedef struct
{
    struct
    {
        float32           gtmFreq;
        float32           gtmGclkFreq;
        float32           gtmCmuClk0Freq; /**< \brief CMU CLK 0 frequency*/
        GtmTomPwmHl_State state;
        Ifx_TickTime      stateDeadline;
    }info;
    struct
    {
        IfxGtm_Tom_Timer timer;        /**< \brief Timer driver */
        IfxGtm_Tom_PwmHl pwm;
    }drivers;
    struct
    {
        uint32 slotTimer;
    }isrCounter;
}App_GtmTomPwmHl;

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

App_GtmTomPwmHl g_GtmTomPwmHl;

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/

IFX_INTERRUPT(ISR_Timer, 0, ISR_PRIORITY_TIMER);

/** \} */

/** \} */

/** \brief Handle 1ms interrupt.
 *
 * \isrProvider \ref ISR_PROVIDER_TIMER
 * \isrPriority \ref ISR_PRIORITY_TIMER
 *
 */
void ISR_Timer(void)
{
    IfxCpu_enableInterrupts();

    IfxGtm_Tom_Timer_acknowledgeTimerIrq(&g_GtmTomPwmHl.drivers.timer);
    g_GtmTomPwmHl.isrCounter.slotTimer++;

    if (isDeadLine(g_GtmTomPwmHl.info.stateDeadline))
    {
        g_GtmTomPwmHl.info.stateDeadline = addTTime(g_GtmTomPwmHl.info.stateDeadline, TimeConst_10ns);
        g_GtmTomPwmHl.info.state++;

        if (g_GtmTomPwmHl.info.state >= GtmTomPwmHl_State_count)
        {
            g_GtmTomPwmHl.info.state = GtmTomPwmHl_State_off;
        }
    }

    system_tick_2 ++;
}

void GtmTomPwmHl_initTimer(void)
{
    {   /* GTM TOM configuration */
        IfxGtm_Tom_Timer_Config timerConfig;
        IfxGtm_Tom_PwmHl_Config pwmHlConfig;

        IfxGtm_Tom_Timer_initConfig(&timerConfig, &MODULE_GTM);
        timerConfig.base.frequency                  = 2500;
        timerConfig.base.isrPriority                = ISR_PRIORITY(INTERRUPT_TIMER);
        timerConfig.base.isrProvider                = ISR_PROVIDER(INTERRUPT_TIMER);
        timerConfig.base.minResolution              = (1.0 / timerConfig.base.frequency) / 1000;
        timerConfig.base.trigger.enabled            = FALSE;
        timerConfig.tom                             = IfxGtm_Tom_1;
        timerConfig.timerChannel                    = IfxGtm_Tom_Ch_4;
        timerConfig.clock                           = IfxGtm_Cmu_Clk_0;

        timerConfig.triggerOut                      = &IfxGtm_TOM1_0_TOUT9_P00_0_OUT;
        timerConfig.base.trigger.outputEnabled      = TRUE;
        timerConfig.base.trigger.enabled            = TRUE;
        timerConfig.base.trigger.triggerPoint       = 500;
        timerConfig.base.trigger.risingEdgeAtPeriod = TRUE;

        IfxGtm_Tom_Timer_init(&g_GtmTomPwmHl.drivers.timer, &timerConfig);

        IfxGtm_Tom_PwmHl_initConfig(&pwmHlConfig);
        IfxGtm_Tom_ToutMapP ccx[1]   = {&IfxGtm_TOM1_4_TOUT22_P33_0_OUT};
        IfxGtm_Tom_ToutMapP coutx[1] = {&IfxGtm_TOM1_4_TOUT30_P33_8_OUT};
        pwmHlConfig.timer                 = &g_GtmTomPwmHl.drivers.timer;
        pwmHlConfig.tom                   = timerConfig.tom;
        pwmHlConfig.base.deadtime         = 2e-6;
        pwmHlConfig.base.minPulse         = 1e-6;
        pwmHlConfig.base.channelCount     = 1;
        pwmHlConfig.base.emergencyEnabled = FALSE;
        pwmHlConfig.base.outputMode       = IfxPort_OutputMode_pushPull;
        pwmHlConfig.base.outputDriver     = IfxPort_PadDriver_cmosAutomotiveSpeed1;
        pwmHlConfig.base.ccxActiveState   = Ifx_ActiveState_high;
        pwmHlConfig.base.coutxActiveState = Ifx_ActiveState_high;
        pwmHlConfig.ccx                   = ccx;
        pwmHlConfig.coutx                 = coutx;

        IfxGtm_Tom_PwmHl_init(&g_GtmTomPwmHl.drivers.pwm, &pwmHlConfig);

        IfxGtm_Tom_Timer_run(&g_GtmTomPwmHl.drivers.timer);
    }

    /* Set PWM duty */
    IfxGtm_Tom_PwmHl *pwmHl = &g_GtmTomPwmHl.drivers.pwm;

    IfxGtm_Tom_Timer *timer = &g_GtmTomPwmHl.drivers.timer;
    Ifx_TimerValue    tOn[1];
    /* Set 50% duty cycle, center aligned */
    tOn[0] = IfxGtm_Tom_Timer_getPeriod(timer) / 2;
    IfxGtm_Tom_Timer_disableUpdate(timer);
    IfxGtm_Tom_PwmHl_setOnTime(pwmHl, &tOn[0]);
    IfxGtm_Tom_Timer_applyUpdate(timer);
}


/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void GtmTomPwmHlDemo_init(void)
{
    /* disable interrupts */
    boolean  interruptState = IfxCpu_disableInterrupts();

    /** - GTM clocks */
    Ifx_GTM *gtm = &MODULE_GTM;
    g_GtmTomPwmHl.info.gtmFreq = IfxGtm_Cmu_getModuleFrequency(gtm);
    IfxGtm_enable(gtm);

    /* Set the global clock frequencies */
    IfxGtm_Cmu_setGclkFrequency(gtm, g_GtmTomPwmHl.info.gtmFreq);
    g_GtmTomPwmHl.info.gtmGclkFreq = IfxGtm_Cmu_getGclkFrequency(gtm);

    IfxGtm_Cmu_setClkFrequency(gtm, IfxGtm_Cmu_Clk_0, g_GtmTomPwmHl.info.gtmGclkFreq);
    g_GtmTomPwmHl.info.gtmCmuClk0Freq = IfxGtm_Cmu_getClkFrequency(gtm, IfxGtm_Cmu_Clk_0, TRUE);
    g_GtmTomPwmHl.info.state          = GtmTomPwmHl_State_init;
    g_GtmTomPwmHl.info.stateDeadline  = now();

    /** - Initialise the GTM part */
    GtmTomPwmHl_initTimer();

//    printf("Gtm Tom PwmHl is initialised\n");

    /* enable interrupts again */
    IfxCpu_restoreInterrupts(interruptState);

    IfxGtm_Cmu_enableClocks(gtm, IFXGTM_CMU_CLKEN_FXCLK | IFXGTM_CMU_CLKEN_CLK0);
}

/* Simple timing loop */
uint32 volatile DelayLoopCounter;

/* Image of a port pin state */
uint8 Port10_1_State;
static IfxAsclin_Asc asc;

IfxCpu_mutexLock g_Asc0_Lock;

#define ASC_TX_BUFFER_SIZE 512
static uint8 ascTxBuffer[ASC_TX_BUFFER_SIZE+ sizeof(Ifx_Fifo) + 8];
#define ASC_RX_BUFFER_SIZE 512
static uint8 ascRxBuffer[ASC_RX_BUFFER_SIZE+ sizeof(Ifx_Fifo) + 8];

Ifx_STM *schdstmSfr;
IfxStm_CompareConfig schdstmConfig;
uint32 system_tick = 0;
//Die Temperature
float32 g_DieTemp;

extern int demo_item;

extern unsigned long SYSTEM_GetCpuClock(void);
extern unsigned long SYSTEM_GetSysClock(void);
extern unsigned long SYSTEM_GetStmClock(void);

IFX_INTERRUPT(asclin0TxISR, 0, IFX_INTPRIO_ASCLIN0_TX)
{
	IfxAsclin_Asc_isrTransmit(&asc);
}

IFX_INTERRUPT(asclin0RxISR, 0, IFX_INTPRIO_ASCLIN0_RX)
{
	IfxAsclin_Asc_isrReceive(&asc);
}

IFX_INTERRUPT(asclin0ErISR, 0, IFX_INTPRIO_ASCLIN0_ER)
{
	IfxAsclin_Asc_isrError(&asc);
}

IFX_INTERRUPT(schdSr0ISR, 0, IFX_INTPRIO_SCHD_STM0_SR0)
{
	IfxStm_clearCompareFlag(schdstmSfr, schdstmConfig.comparator);
	IfxStm_increaseCompare(schdstmSfr, schdstmConfig.comparator, schdstmConfig.ticks);

	system_tick++;
}

void schd_init(void)
{
	schdstmSfr = &MODULE_STM0;
	IfxStm_initCompareConfig(&schdstmConfig);

	// configure to generate interrupt every 1000 us
	sint32 ticks = IfxStm_getTicksFromMicroseconds(schdstmSfr, 1000);
	schdstmConfig.ticks = ticks;
	schdstmConfig.comparatorInterrupt  = IfxStm_ComparatorInterrupt_ir0; //User must select the interrupt output
	schdstmConfig.triggerPriority = IFX_INTPRIO_SCHD_STM0_SR0;
	schdstmConfig.typeOfService = IfxSrc_Tos_cpu0;
	IfxStm_initCompare(schdstmSfr, &schdstmConfig);

	// install interrupt handlers
	IfxCpu_Irq_installInterruptHandler(&schdSr0ISR, IFX_INTPRIO_SCHD_STM0_SR0);
	IfxCpu_enableInterrupts();
	system_tick = 0;
}

uint32 schd_GetTick(void)
{
	return system_tick;
}

void schd_SetTick(uint32 tick)
{
	system_tick = tick;
}

size_t read(int fd, void *buffer, size_t count)
{
	Ifx_SizeT tmpCnt = count;

	IfxAsclin_Asc_read(&asc, buffer, &tmpCnt, TIME_INFINITE);
	return count;
}

size_t write(int fd, const void *buffer, size_t count)
{
	Ifx_SizeT tmpCnt = count;

	IfxAsclin_Asc_write(&asc, buffer, &tmpCnt, TIME_INFINITE);

	return tmpCnt;
}

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void DtsBasicDemo_init(void)
{
    /* Get the default configuration */
    IfxDts_Dts_Config dtsConfig;
    IfxDts_Dts_initModuleConfig(&dtsConfig);

    /* adapt the default configuration if required */
    dtsConfig.lowerTemperatureLimit = -35; /* SMU alarm if g_DieTemp value is below this Celsius value */
    dtsConfig.upperTemperatureLimit = 150; /* SMU alarm if g_DieTemp value is above this Celsius value */

    /* Module initialisation */
    IfxDts_Dts_initModule(&dtsConfig);
}

/* Main Program */
int core0_main (void)
{
	uint32 endinitPw;
	uint32_t tmpTick;
	uint32 sys_count = 0, new_count = 0;
	uint32 back_count = 0;

	IfxAsclin_Asc_Config ascConfig;
	IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN0);

	/* set the desired baudrate */
	ascConfig.baudrate.prescaler = 1;
	/* FDR values will be calculated in initModule */
	ascConfig.baudrate.baudrate = 115200;
	/* ISR priorities and interrupt target */
	ascConfig.interrupt.txPriority = IFX_INTPRIO_ASCLIN0_TX;
	ascConfig.interrupt.rxPriority = IFX_INTPRIO_ASCLIN0_RX;
	ascConfig.interrupt.erPriority = IFX_INTPRIO_ASCLIN0_ER;
	ascConfig.interrupt.typeOfService = (IfxSrc_Tos)IfxCpu_getCoreId();
	/* FIFO configuration */
	ascConfig.txBuffer = &ascTxBuffer;
	ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
	ascConfig.rxBuffer = &ascRxBuffer;
	ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;
	/* pin configuration */
	const IfxAsclin_Asc_Pins pins = {
	NULL, IfxPort_InputMode_pullUp, /* CTS pin not used */
	&IfxAsclin0_RXA_P14_1_IN, IfxPort_InputMode_pullUp, /* Rx pin */
	NULL, IfxPort_OutputMode_pushPull, /* RTS pin not used */
//	&IfxAsclin0_TX_P15_2_OUT, IfxPort_OutputMode_pushPull, /* Tx pin */
	&IfxAsclin0_TX_P14_0_OUT, IfxPort_OutputMode_pushPull, /* Tx pin */
	IfxPort_PadDriver_cmosAutomotiveSpeed1
	};
	ascConfig.pins = &pins;
	/* Manually set pad driver to speed grade 1 */
	/*(otherwise 3v3 is not seen as a '1' ) */
	IfxPort_setPinPadDriver(&MODULE_P14,0,
//	IfxPort_setPinPadDriver(&MODULE_P15,3,
	IfxPort_PadDriver_cmosAutomotiveSpeed1) ;

	IfxPort_setPinPadDriver(&MODULE_P14,1,
	IfxPort_PadDriver_cmosAutomotiveSpeed1) ;

	IfxAsclin_Asc_initModule(&asc, &ascConfig);

	schd_init();

    /*
     * !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdog in the demo if it is required and also service the watchdog periodically
     * */
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());
    IfxScuWdt_disableSafetyWatchdog (IfxScuWdt_getSafetyWatchdogPassword ());

    /* Configure P33.8 P33.9 P33.10 P33.11 as an output */
    Appli_GpioInit();

    /* Make the speaker click */
    /* Configure P33.0 as an output (speaker) */
//    IfxPort_setPinMode(&MODULE_P33, 0u, IfxPort_Mode_outputPushPullGeneral);
//    IfxPort_setPinPadDriver(&MODULE_P33, 0u, IfxPort_PadDriver_cmosAutomotiveSpeed1) ;

//    endinitPw = IfxScuWdt_getCpuWatchdogPassword ();
//    IfxScuWdt_clearCpuEndinit (endinitPw);

//    /* Set Emergency Stop Function For P33.0 */
//    IfxPort_setESR(&MODULE_P33, 0u);

    // Need to test this
//    SCU_PMSWCR0.B.TRISTEN = 1u;   /* Allow write to SCU_PMSWCR0.B.TRISTREQ */
//    SCU_PMSWCR0.B.TRISTREQ = 1u;  /* Emergency stop pins go tristate */
//    SCU_PMSWCR0.B.TRISTEN = 0u;   /* Disable write to SCU_PMSWCR0.B.TRISTREQ */
//
//    SCU_EMSR.B.POL = 1u;  /* Active low on P33.8 */
//    SCU_EMSR.B.MODE = 1u;
//    SCU_EMSR.B.PSEL = 0u; /* Use P33.8 as emergency stop trigger pin */

//    IfxScuWdt_setCpuEndinit (endinitPw);

    IfxCpu_Irq_installInterruptHandler(&asclin0TxISR, IFX_INTPRIO_ASCLIN0_TX);
    IfxCpu_Irq_installInterruptHandler(&asclin0RxISR, IFX_INTPRIO_ASCLIN0_RX);
    IfxCpu_Irq_installInterruptHandler(&asclin0ErISR, IFX_INTPRIO_ASCLIN0_ER);

	IfxCpu_Irq_installInterruptHandler(&ISR_Timer, ISR_PRIORITY_TIMER);

    DtsBasicDemo_init();

    IfxCpu_enableInterrupts();

    GtmTomPwmHlDemo_init();

//    demo_item = CAN_DEMO_SINGLE;
//    demo_item = CAN_DEMO_FIFO;
//    demo_item = CAN_DEMO_CANFD;
//    Appli_AdcInit();
    CAN_Init();
	printf("CAN Demo %u\n", demo_item);

    /* Endless loop */
    while (1)
    {
        /* start Sensor */
        IfxDts_Dts_startSensor();

    	/* wait until a new result is available */
    	while (IfxDts_Dts_isBusy())
    	{}

    	/* convert result to Celsius */
    	g_DieTemp = IfxDts_Dts_getTemperatureCelsius();
    	boolean flag = IfxCpu_acquireMutex(&g_Asc0_Lock);
    	if (flag)
    	{
//    		printf("Cpu%d:%u Hz, Sys:%u Hz, Stm:%u Hz, Core:%04X,  %u\n"\
//    				"DTS Temperature: %3.1f'C",
//    				IfxCpu_getCoreId(),
//					SYSTEM_GetCpuClock(),
//					SYSTEM_GetSysClock(),
//					SYSTEM_GetStmClock(),
//					__TRICORE_CORE__,
//					schd_GetTick(),
//					g_DieTemp
//    		);

    		printf("%.2f\n", g_DieTemp);

    		IfxCpu_releaseMutex(&g_Asc0_Lock);
    	}
    	else
    	{
    		wait(1000);
    	}

//    	Appli_AdcCyclic();

    	wait(10000000);
    	CAN_TxdRcv();
    	wait(10000000);
    }

    return (1u);
}

