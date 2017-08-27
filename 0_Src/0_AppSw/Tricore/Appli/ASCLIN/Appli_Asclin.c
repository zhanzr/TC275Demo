#include "Appli_Asclin.h"
#include "Tricore\Appli\Entry\Appli_Entry.h"

// used globally
int asclin_count = 0;

// used globally
static IfxAsclin_Asc asc;
static IfxAsclin_Lin lin;

// set the id bytes
uint8 txId = 0x26; // for sending respose after header
uint8 rxId = 0x80; // for receiving respose after header

// prepare transmit bytes incase of sending response
uint8 txData[8] = { 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80 };//{ 1, 2, 3, 4, 5, 6, 7, 8 };
// prepare receive buffer incase of receiving response
uint8 rxData[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
uint32 size = 8;

static uint8 ascTxBuffer[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 ascRxBuffer[ASC_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

extern int demo_item;

//IFX_INTERRUPT(asclin0TxISR, 0, IFX_INTPRIO_ASCLIN0_TX)
//{
//	if(demo_item == 1)
//		IfxAsclin_Asc_isrTransmit(&asc);
//}
//
//IFX_INTERRUPT(asclin0RxISR, 0, IFX_INTPRIO_ASCLIN0_RX)
//{
//	if(demo_item == 1)
//		IfxAsclin_Asc_isrReceive(&asc);
//}
//
//IFX_INTERRUPT(asclin0ErISR, 0, IFX_INTPRIO_ASCLIN0_ER)
//{
//	IfxAsclin_Asc_isrError(&asc);
//}

void Appli_AscInit(void)
{
	// create module config
	IfxAsclin_Asc_Config ascConfig;
	IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN0);

	// set the desired baudrate
	ascConfig.baudrate.prescaler = 1;
	ascConfig.baudrate.baudrate = 115200; // FDR values will be calculated in initModule

	// ISR priorities and interrupt target
	ascConfig.interrupt.txPriority = IFX_INTPRIO_ASCLIN0_TX;
	ascConfig.interrupt.rxPriority = IFX_INTPRIO_ASCLIN0_RX;
	ascConfig.interrupt.erPriority = IFX_INTPRIO_ASCLIN0_ER;
	ascConfig.interrupt.typeOfService = (IfxSrc_Tos)IfxCpu_getCoreId();

	// FIFO configuration
	ascConfig.txBuffer = &ascTxBuffer;
	ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
	ascConfig.rxBuffer = &ascRxBuffer;
	ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

	// pin configuration
	const IfxAsclin_Asc_Pins pins = {
			NULL,                       IfxPort_InputMode_pullUp,        // CTS pin not used
			&IfxAsclin0_RXB_P15_3_IN,   IfxPort_InputMode_pullUp,        // Rx pin
			NULL,           			IfxPort_OutputMode_pushPull,     // RTS pin not used
			&IfxAsclin0_TX_P15_2_OUT,   IfxPort_OutputMode_pushPull,     // Tx pin
			IfxPort_PadDriver_cmosAutomotiveSpeed1
	};

	ascConfig.pins = &pins;

	// initialize module
	//IfxAsclin_Asc asc;
	// defined globally
	IfxAsclin_Asc_initModule(&asc, &ascConfig);

	// install interrupt handlers
//	IfxCpu_Irq_installInterruptHandler(&asclin0TxISR, IFX_INTPRIO_ASCLIN0_TX);
//	IfxCpu_Irq_installInterruptHandler(&asclin0RxISR, IFX_INTPRIO_ASCLIN0_RX);
//	IfxCpu_Irq_installInterruptHandler(&asclin0ErISR, IFX_INTPRIO_ASCLIN0_ER);
	IfxCpu_enableInterrupts();
}

void Appli_LinInit(void)
{
	// create module configure
	IfxAsclin_Lin_Config linConfig;
	IfxAsclin_Lin_initModuleConfig(&linConfig, &MODULE_ASCLIN1);

	// set the lin mode of operation
	linConfig.linMode = IfxAsclin_LinMode_master;

	// set the desired baudrate
	linConfig.btc.prescaler = 4;
	linConfig.brg.baudrate = 19200; // FDR values will be calculated in initModule

	// pin configuration
	const IfxAsclin_Lin_Pins pins = {
			&IfxAsclin1_RXA_P15_1_IN,   IfxPort_InputMode_pullUp,    // Rx pin
			&IfxAsclin1_TX_P15_0_OUT,   IfxPort_OutputMode_pushPull, // Tx pin
			IfxPort_PadDriver_cmosAutomotiveSpeed1
	};
	linConfig.pins = &pins;

	// initialize module
	//IfxAsclin_Lin lin; // defined globally
	IfxAsclin_Lin_initModule(&lin, &linConfig);
}

void Appli_Lin_MasterResp(void)
{
	IfxAsclin_Lin_sendHeader(&lin, &txId);

	if (lin.acknowledgmentFlags.txHeaderEnd == 1) // if the header is been transmitted successfully
	{
		IfxAsclin_Lin_sendResponse(&lin, txData, size);
	}
}

void Appli_Lin_SlaveResp(void)
{
	//receive response case
	IfxAsclin_Lin_sendHeader(&lin, &rxId);

	if (lin.acknowledgmentFlags.txHeaderEnd == 1) // if the header is been transmitted successfully
	{
		IfxAsclin_Lin_receiveResponse(&lin, rxData, size);
	}
}

void Appli_AsclinDemoInit(void)
{
	if(demo_item == 1)
		Appli_AscInit();
	else if(demo_item == 2)
		Appli_LinInit();
}

void Appli_AsclinDemoDeInit(void)
{
}

void Appli_AscDemo_ASC(void)
{
	static char ch = 'a';
	char p_out[2];

	asclin_count++;
	if(asclin_count > 100)
	{
		asclin_count = 0;
		ch++;
		if(ch > 'z' || ch < 'a')
		{
			ch = 'a';
			ConsolePrint("\r\n");
		}
		p_out[0] = ch; p_out[1] = '\0';
		ConsolePrint(p_out);
	}
}

void Appli_LinDemo_LIN(void)
{
	static boolean resp = TRUE;
	char str_lin[5];
	int i = 0;

	str_lin[4] = ',';
	asclin_count++;
	if(asclin_count > 200)
	{
		asclin_count = 0;
		if(resp == TRUE)
		{
			Appli_Lin_MasterResp();
			{
#if 0
				ConsolePrint("TxID : ");
				ByteToAscii(txId, &str_lin[0]);
				ConsolePrint(&str_lin[0]);

				ConsolePrint("\tDATA : ");
				for(i = 0; i < 8; i++)
				{
					ByteToAscii(txData[i], &str_lin[0]);
					ConsolePrint(&str_lin[0]);
				}
				ConsolePrint("\r\n");
#endif
			}
			resp = FALSE;
		}
		else if(resp == FALSE)
		{
			Appli_Lin_SlaveResp();
			{
				ConsolePrint("RxID : ");
				ByteToAscii(rxId, &str_lin[0]);
				ConsolePrint(&str_lin[0]);

				ConsolePrint("\tDATA : ");
				for(i = 0; i < 8; i++)
				{
					ByteToAscii(rxData[i], &str_lin[0]);
					ConsolePrint(&str_lin[0]);
				}
				ConsolePrint("\r\n");
			}
			resp = TRUE;
		}
	}
}

void Appli_AsclinCyclic(void)
{
	if(demo_item == 1)
		Appli_AscDemo_ASC();
	else if(demo_item == 2)
		Appli_LinDemo_LIN();
}
