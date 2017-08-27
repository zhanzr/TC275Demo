#include <Appli/ASCLIN/AsclinDemoEntry.h>

// used globally
static IfxAsclin_Asc asc;

static uint8 ascTxBuffer[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 ascRxBuffer[ASC_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

// pin configuration
const IfxAsclin_Asc_Pins uart_pins = {
		NULL,                       IfxPort_InputMode_pullUp,    // CTS pin not used
		&IfxAsclin0_RXB_P15_3_IN,   IfxPort_InputMode_pullUp,    // Rx pin
		NULL,                       IfxPort_OutputMode_pushPull, // RTS pin not used
		&IfxAsclin0_TX_P15_2_OUT,   IfxPort_OutputMode_pushPull, // Tx pin
		IfxPort_PadDriver_cmosAutomotiveSpeed4
};

void UART_Init(void)
{
	// create module config
	IfxAsclin_Asc_Config ascConfig;

	IfxAsclin_Asc_initModuleConfig(&ascConfig, &DEMO_ASCLIN);
	// set the desired baudrate
	ascConfig.baudrate.prescaler = 1;
	ascConfig.baudrate.baudrate = 115200; // FDR values will be calculated in initModule

	ascConfig.frame.frameMode  = IfxAsclin_FrameMode_asc;
	ascConfig.frame.dataLength = IfxAsclin_DataLength_8;
	ascConfig.frame.stopBit = IfxAsclin_StopBit_1;
	ascConfig.frame.shiftDir = IfxAsclin_ShiftDirection_lsbFirst;
	ascConfig.frame.parityBit = FALSE;

	// ISR priorities and interrupt target
#ifndef IFX_INTPRIO_ASCLIN0_TX
#define IFX_INTPRIO_ASCLIN0_TX  4
#endif
	ascConfig.interrupt.txPriority = IFX_INTPRIO_ASCLIN0_TX;

#ifndef IFX_INTPRIO_ASCLIN0_RX
#define IFX_INTPRIO_ASCLIN0_RX  5
#endif
	ascConfig.interrupt.rxPriority = IFX_INTPRIO_ASCLIN0_RX;

#ifndef IFX_INTPRIO_ASCLIN0_ER
#define IFX_INTPRIO_ASCLIN0_ER  6
#endif
	ascConfig.interrupt.erPriority = IFX_INTPRIO_ASCLIN0_ER;
	ascConfig.interrupt.typeOfService =   IfxSrc_Tos_cpu0;

	// FIFO configuration
	ascConfig.txBuffer = &ascTxBuffer;
	ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
	ascConfig.rxBuffer = &ascRxBuffer;
	ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

	ascConfig.pins = &uart_pins;
	// initialize module
	//IfxAsclin_Asc asc; // defined globally
	IfxAsclin_Asc_initModule(&asc, &ascConfig);

	asclin_inited = TRUE;
}

IfxAsclin_Asc* UART_GetHandle(void)
{
	return &asc;
}

void UART_WriteByte(uint8 data)
{
	IfxAsclin_Asc_blockingWrite(&asc, data);
}

void UART_Write(uint8 *p_data, Ifx_SizeT count)
{
	uint8 *p_send = p_data;

	if(count > 0)
		IfxAsclin_Asc_write(&asc, p_data, &count, TIME_INFINITE);
	else
	{
		while(*p_send != (uint8)'\0')
			UART_WriteByte(*p_send++);
	}
}

void UART_Cyclic(void)
{
	static uint32 uart_count = 0;

	uart_count++;
	if(uart_count >= ASCLIN_DIVIDER_UART)
	{
		uart_count = 0;
		UART_Write("::UART Running!\r\n", 0);
		//UART_WriteByte(0x55);
	}
}


