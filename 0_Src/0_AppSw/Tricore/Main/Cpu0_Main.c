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
#include <Asclin/Asc/IfxAsclin_Asc.h>
#include <Stm/Std/IfxStm.h>

#define TEST_DELAY_MS	2000U

#define ASC_TX_BUFFER_SIZE 512
static uint8 ascTxBuffer[ASC_TX_BUFFER_SIZE+ sizeof(Ifx_Fifo) + 8];
#define ASC_RX_BUFFER_SIZE 512
static uint8 ascRxBuffer[ASC_RX_BUFFER_SIZE+ sizeof(Ifx_Fifo) + 8];

#define IFX_INTPRIO_ASCLIN0_TX 1
#define IFX_INTPRIO_ASCLIN0_RX 2
#define IFX_INTPRIO_ASCLIN0_ER 3

/* Image of a port pin state */
uint8 Port10_1_State;
static IfxAsclin_Asc asc;

//extern unsigned long SYSTEM_GetSysClock(void);
//extern unsigned long SYSTEM_GetCpuClock(void);

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


Ifx_STM *schdstmSfr;
IfxStm_CompareConfig schdstmConfig;
uint32 system_tick = 0;

IFX_INTERRUPT(schdSr0ISR, 0, IFX_INTPRIO_SCHD_STM0_SR0)
{
	IfxStm_clearCompareFlag(schdstmSfr, schdstmConfig.comparator);
	IfxStm_increaseCompare(schdstmSfr, schdstmConfig.comparator, schdstmConfig.ticks);

	system_tick++;
}

void schd_init(void)
{
#ifndef	IFX_INTPRIO_SCHD_STM0_SR0
	#define IFX_INTPRIO_SCHD_STM0_SR0  110
#endif
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

//inline
void schd_SetTick(uint32 tick)
{
	system_tick = tick;
}
//
///* Send character CHR via the serial line */
//void _out_uart(const unsigned char chr)
//{
////	/* wait until space is available in the FIFO */
////	while (!TX_READY(UARTBASE))
////		;
////
////	TX_CLEAR(UARTBASE);
////
////	/* send the character */
////	PUT_CHAR(UARTBASE, chr);
//	IfxAsclin_Asc_blockingWrite(&asc, chr);
//}
//
///* Receive (and wait for) a character from the serial line */
//unsigned char _in_uart(void)
//{
//	unsigned char ch;
////
////	/* wait for a new character */
////	while (_poll_uart(&ch) == 0)
////		;
////
////	return ch;
//	return IfxAsclin_Asc_blockingRead(&asc);
//}
//
///* Check the serial line if a character has been received.
//   returns 1 and the character in *chr if there is one
//   else 0
// */
//int _poll_uart(unsigned char *chr)
//{
//	unsigned char ret;
//	int res = 0;
//
////	if (RX_READY(UARTBASE))
////	{
////		ret = (unsigned char)GET_CHAR(UARTBASE);
////		/* acknowledge receive */
////		RX_CLEAR(UARTBASE);
////		/* check for error condition */
////		if (GET_ERROR_STATUS(UARTBASE))
////		{
////			/* reset error flags */
////			RESET_ERROR(UARTBASE);
////			/* ignore this character */
////		}
////		else
////		{
////			/* this is a valid character */
////			*chr = ret;
////			res = 1;
////		}
////	}
//
//	return res;
//}
//
///* POSIX read function */
///* read characters from file descriptor fd into given buffer, at most count bytes */
///* returns number of characters in buffer */
//size_t read(int fd, void *buffer, size_t count)
//{
//	size_t index = 0;
//
//	if (fileno(stdin) == fd)
//	{
//#if (NON_BLOCKING_SERIALIO > 0)
//		char *ptr = (char *)buffer;
//		do
//		{
//			if (1 == _uart_getchar(ptr))
//			{
//				++ptr;
//				++index;
//			}
//			else
//			{
//				/* wait at least for 1 character */
//				if (index >= 1)
//				{
//					break;
//				}
//			}
//		} while (index < count);
//#else
////		unsigned char *ptr = (unsigned char *)buffer;
////		do
////		{
////			if (1 == _poll_uart(ptr))
////			{
////				++ptr;
////				++index;
////			}
////			else
////			{
////				/* wait at least for 1 character */
////				if (index >= 1)
////				{
////					break;
////				}
////			}
////		} while (index < count);
//		index = IfxAsclin_Asc_read(&asc, buffer, &count, TIME_INFINITE);
//#endif /* NON_BLOCKING_SERIALIO */
//	}
//
//	return index;
//}
//
///* POSIX write function */
///* write content of buffer to file descriptor fd */
///* returns number of characters that have been written */
//size_t write(int fd, const void *buffer, size_t count)
//{
//	size_t index = 0;
//
//	if ((fileno(stdout) == fd) || (fileno(stderr) == fd))
//	{
//#if (NON_BLOCKING_SERIALIO > 0)
//		int ret = _uart_send((const char *)buffer, (int)count);
//		if (ret)
//		{
//			index = count;
//		}
//#else
////		const unsigned char *ptr = (const unsigned char *)buffer;
////		while (index < count)
////		{
////			_out_uart(*ptr++);
////			++index;
////		}
//		IfxAsclin_Asc_write(&asc, buffer, &count, TIME_INFINITE);
//#endif /* NON_BLOCKING_SERIALIO */
//	}
//
//	return index;
//}

/* Main Program */
int core0_main (void)
{
	uint32 endinitPw;
	uint32_t tmpTick;

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
	&IfxAsclin0_RXB_P15_3_IN, IfxPort_InputMode_pullUp, /* Rx pin */
	NULL, IfxPort_OutputMode_pushPull, /* RTS pin not used */
	&IfxAsclin0_TX_P15_2_OUT, IfxPort_OutputMode_pushPull, /* Tx pin */
	IfxPort_PadDriver_cmosAutomotiveSpeed1
	};
	ascConfig.pins = &pins;
	/* Manually set pad driver to speed grade 1 */
	/*(otherwise 3v3 is not seen as a '1' ) */
	IfxPort_setPinPadDriver(&MODULE_P15,3,
	IfxPort_PadDriver_cmosAutomotiveSpeed1) ;
	IfxPort_setPinPadDriver(&MODULE_P15,2,
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
    IfxPort_setPinMode(&MODULE_P33, 8u, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinPadDriver(&MODULE_P33, 8u, IfxPort_PadDriver_cmosAutomotiveSpeed1) ;
    IfxPort_setPinMode(&MODULE_P33, 9u, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinPadDriver(&MODULE_P33, 9u, IfxPort_PadDriver_cmosAutomotiveSpeed1) ;
    IfxPort_setPinMode(&MODULE_P33, 10u, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinPadDriver(&MODULE_P33, 10u, IfxPort_PadDriver_cmosAutomotiveSpeed1) ;
    IfxPort_setPinMode(&MODULE_P33, 11u, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinPadDriver(&MODULE_P33, 11u, IfxPort_PadDriver_cmosAutomotiveSpeed1) ;

    /* Make the speaker click */
    /* Configure P33.0 as an output (speaker) */
    IfxPort_setPinMode(&MODULE_P33, 0u, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinPadDriver(&MODULE_P33, 0u, IfxPort_PadDriver_cmosAutomotiveSpeed1) ;

    endinitPw = IfxScuWdt_getCpuWatchdogPassword ();
    IfxScuWdt_clearCpuEndinit (endinitPw);

    /* Set Emergency Stop Function For P33.0 */
    IfxPort_setESR(&MODULE_P33, 0u);

    // Need to test this
    SCU_PMSWCR0.B.TRISTEN = 1u;   /* Allow write to SCU_PMSWCR0.B.TRISTREQ */
    SCU_PMSWCR0.B.TRISTREQ = 1u;  /* Emergency stop pins go tristate */
    SCU_PMSWCR0.B.TRISTEN = 0u;   /* Disable write to SCU_PMSWCR0.B.TRISTREQ */

    SCU_EMSR.B.POL = 1u;  /* Active low on P33.8 */
    SCU_EMSR.B.MODE = 1u;
    SCU_EMSR.B.PSEL = 0u; /* Use P33.8 as emergency stop trigger pin */

    IfxScuWdt_setCpuEndinit (endinitPw);

    IfxCpu_Irq_installInterruptHandler(&asclin0TxISR, IFX_INTPRIO_ASCLIN0_TX);
    IfxCpu_Irq_installInterruptHandler(&asclin0RxISR, IFX_INTPRIO_ASCLIN0_RX);
    IfxCpu_Irq_installInterruptHandler(&asclin0ErISR, IFX_INTPRIO_ASCLIN0_ER);
    IfxCpu_enableInterrupts();

    /* Endless loop */
    while (1u)
    {
        /* Turn LED Off */
    	IfxPort_setPinState(&MODULE_P33, 8u, IfxPort_State_high);
    	IfxPort_setPinState(&MODULE_P33, 9u, IfxPort_State_high);
    	IfxPort_setPinState(&MODULE_P33, 10u, IfxPort_State_high);
    	IfxPort_setPinState(&MODULE_P33, 11u, IfxPort_State_high);
    	/* Click speaker */
    	IfxPort_setPinState(&MODULE_P33, 0u, IfxPort_State_high);

    	Ifx_SizeT tmpSize16 = 5;
    	IfxAsclin_Asc_write(&asc, "tes1\n", &tmpSize16, 40);

//    	printf("First iLLD Program on TC275C Sys:%u Hz, Cpu:%u Hz, Core:%04X, %u\n",
//    			SYSTEM_GetSysClock(),
//				SYSTEM_GetCpuClock(),
//				__TRICORE_CORE__,
//				schd_GetTick());
    	/* test delay */
    	tmpTick = schd_GetTick();
    	while((tmpTick+TEST_DELAY_MS) > schd_GetTick())
    	{
    		_nop();
    	}

        /* Turn LED On */
    	IfxPort_setPinState(&MODULE_P33, 8u, IfxPort_State_low);
    	IfxPort_setPinState(&MODULE_P33, 9u, IfxPort_State_low);
    	IfxPort_setPinState(&MODULE_P33, 10u, IfxPort_State_low);
    	IfxPort_setPinState(&MODULE_P33, 11u, IfxPort_State_low);
    	/* Click speaker */
    	IfxPort_setPinState(&MODULE_P33, 0u, IfxPort_State_low);

    	tmpSize16 = 5;
    	IfxAsclin_Asc_write(&asc, "tes0\n", &tmpSize16, 40);

    	/* test delay */
    	tmpTick = schd_GetTick();
    	while((tmpTick+TEST_DELAY_MS) > schd_GetTick())
    	{
    		_nop();
    	}

    	/* Read Port 10.2 into a variable */
    	Port10_1_State = IfxPort_getPinState(&MODULE_P10, 10u);

    }
    return (1u);
}

