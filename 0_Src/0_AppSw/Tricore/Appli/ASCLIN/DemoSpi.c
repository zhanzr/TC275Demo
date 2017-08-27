#include <Appli/ASCLIN/AsclinDemoEntry.h>

// used globally
//extern IfxAsclin_Spi spi;

uint8 spiTxBuffer[SPI_BUFFER_SIZE];
uint8 spiRxBuffer[SPI_BUFFER_SIZE];

// pin configuration
const IfxAsclin_Spi_Pins spi_pins = {
		&IfxAsclin1_SCLK_P15_0_OUT, IfxPort_OutputMode_pushPull, // Clock out pin
		&IfxAsclin1_RXB_P15_5_IN,   IfxPort_InputMode_pullUp,    // Rx pin
		&IfxAsclin1_TX_P15_4_OUT,   IfxPort_OutputMode_pushPull, // Tx pin
		&IfxAsclin1_SLSO_P20_8_OUT, IfxPort_OutputMode_pushPull, // Slave select pin
		IfxPort_PadDriver_cmosAutomotiveSpeed1
};

void SPI_Init(void)
{
	int i;
	// create module config
	IfxAsclin_Spi_Config spiConfig;
	IfxAsclin_Spi_initModuleConfig(&spiConfig, &MODULE_ASCLIN1);

	// set the frame word length & format
	spiConfig.dataLength = IfxAsclin_DataLength_16;
	spiConfig.frame.idleDelay = IfxAsclin_IdleDelay_0; /* no idle delay */
	spiConfig.frame.leadDelay = IfxAsclin_LeadDelay_1; /* one lead bit */
	spiConfig.frame.stopBit = IfxAsclin_StopBit_1;     /* one stop bit (trail delay) */
	spiConfig.frame.shiftDir = IfxAsclin_ShiftDirection_msbFirst;

	spiConfig.inputOutput.alti = IfxAsclin_RxInputSelect_0;         /* alternate input 0; */
	spiConfig.inputOutput.cpol = IfxAsclin_ClockPolarity_idleHigh;  /* CPOL  active high */
	spiConfig.inputOutput.spol = IfxAsclin_SlavePolarity_idlehigh;  /* SPOL  active high */
	spiConfig.inputOutput.loopBack = FALSE;                         /* no loop back */

	// set the desired baudrate
	spiConfig.baudrate.prescaler = 2;
	spiConfig.baudrate.baudrate = 100000; // FDR values will be calculated in initModule

	// ISR priorities and interrupt target
	spiConfig.interrupt.txPriority = IFX_INTPRIO_ASCLIN1_TX; // see also \ref IfxLld_Asclin
	spiConfig.interrupt.rxPriority = IFX_INTPRIO_ASCLIN1_RX; // see also \ref IfxLld_Asclin
	spiConfig.interrupt.erPriority = IFX_INTPRIO_ASCLIN1_ER; // see also \ref IfxLld_Asclin
	spiConfig.interrupt.typeOfService =  IfxSrc_Tos_cpu0;

	spiConfig.pins = &spi_pins;

	// initialize module
	//IfxAsclin_Spi spi; // defined globally
	IfxAsclin_Spi_initModule(&spi, &spiConfig);

	// load the data buffers
	for (i = 0; i < SPI_BUFFER_SIZE; i++)
	{
		spiTxBuffer[i] = i + 1;
		spiRxBuffer[i] = 0;
	}

	asclin_inited = TRUE;
}



void SPI_Exchange(uint8 *p_data, uint8 size)
{
	// wait until transfer of previous data stream is finished
	while( IfxAsclin_Spi_getStatus(&spi) == IfxAsclin_Spi_Status_busy );

	// send/receive new stream
	IfxAsclin_Spi_exchange(&spi, p_data, spiRxBuffer, size);
}

void SPI_Send(uint8 *p_data, uint8 size)
{
	uint16 to_send[16];
	uint8 i;

	for(i = 0; i < (size+1)/2; i++)
	{
		to_send[i] = *p_data;
		to_send[i] = (to_send[i]<<8) | *(p_data+1);
		p_data += 2;
	}
	// wait until transfer of previous data stream is finished
	while( IfxAsclin_Spi_getStatus(&spi) == IfxAsclin_Spi_Status_busy );

	// send new stream
	IfxAsclin_Spi_exchange(&spi, (void *)&to_send[0], NULL_PTR, (size+1)/2);
}

boolean SPI_WaitFor(uint8 *p_data, uint8 size)
{
	// wait until transfer of previous data stream is finished
	while( IfxAsclin_Spi_getStatus(&spi) == IfxAsclin_Spi_Status_busy );

	// receive new stream
	IfxAsclin_Spi_exchange(&spi, NULL_PTR, p_data, size);
}

void SPI_Cyclic(void)
{
	static uint32 spi_count = 0;

	spi_count++;
	if(spi_count >= ASCLIN_DIVIDER_SPI)
	{
		spi_count = 0;
		SPI_Send(spiTxBuffer, 8);
	}
}
