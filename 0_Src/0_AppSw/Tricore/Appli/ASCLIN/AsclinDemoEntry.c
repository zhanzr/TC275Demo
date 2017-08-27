#include <Appli/ASCLIN/AsclinDemoEntry.h>

//boolean asc_rx = FALSE;
boolean asclin_inited = FALSE;
static uint32 asclin_count = 0;

IfxAsclin_Spi spi;

IfxAsclin_Spi* SPI_GetHandle(void)
{
	return &spi;
}

IFX_INTERRUPT(asclin0TxISR, 0, IFX_INTPRIO_ASCLIN0_TX)
{
	if(demo_idx_curr == ASCLIN_DEMO_UART)
		IfxAsclin_Asc_isrTransmit(UART_GetHandle());
	else if(demo_idx_curr == ASCLIN_DEMO_SPI)
		IfxAsclin_Spi_isrTransmit(SPI_GetHandle());
	else if(demo_idx_curr == ASCLIN_DEMO_LIN)
		IfxAsclin_Asc_isrTransmit(LIN_GetHandle());
}

IFX_INTERRUPT(asclin0RxISR, 0, IFX_INTPRIO_ASCLIN0_RX)
{
	if(demo_idx_curr == ASCLIN_DEMO_UART)
		IfxAsclin_Asc_isrReceive(UART_GetHandle());
	else if(demo_idx_curr == ASCLIN_DEMO_SPI)
		IfxAsclin_Spi_isrReceive(SPI_GetHandle());
	//asc_rx = TRUE;
}

IFX_INTERRUPT(asclin0ErISR, 0, IFX_INTPRIO_ASCLIN0_ER)
{
	if(demo_idx_curr == ASCLIN_DEMO_UART)
		IfxAsclin_Asc_isrError(UART_GetHandle());
	else if(demo_idx_curr == ASCLIN_DEMO_SPI)
		IfxAsclin_Spi_isrError(SPI_GetHandle());
}

void ASCLIN_DeInit(uint32 idx)
{
	if(asclin_inited == TRUE)
	{
		if(idx == ASCLIN_DEMO_UART)
			IfxAsclin_Asc_disableModule(UART_GetHandle());
		else if(idx == ASCLIN_DEMO_SPI)
			IfxAsclin_Asc_disableModule(SPI_GetHandle());
		else if(idx == ASCLIN_DEMO_LIN)
			IfxAsclin_Asc_disableModule(LIN_GetHandle());

		asclin_inited = FALSE;
	}
}

void ASCLIN_DemoInit(uint32 module, uint32 idx)
{
//	if(idx == ASCLIN_DEMO_UART) // Uart demo
//		{
		UART_Init();
		// install interrupt handlers
#ifndef IFX_INTPRIO_ASCLIN0_TX
#define IFX_INTPRIO_ASCLIN0_TX  4
#endif
		IfxCpu_Irq_installInterruptHandler(&asclin0TxISR, IFX_INTPRIO_ASCLIN0_TX);

#ifndef IFX_INTPRIO_ASCLIN0_RX
#define IFX_INTPRIO_ASCLIN0_RX  5
#endif
		IfxCpu_Irq_installInterruptHandler(&asclin0RxISR, IFX_INTPRIO_ASCLIN0_RX);

#ifndef IFX_INTPRIO_ASCLIN0_ER
#define IFX_INTPRIO_ASCLIN0_ER  6
#endif
		IfxCpu_Irq_installInterruptHandler(&asclin0ErISR, IFX_INTPRIO_ASCLIN0_ER);
		IfxCpu_enableInterrupts();
//		}
//	else if(idx == ASCLIN_DEMO_LIN) // Lin Master demo
//		{
//		LIN_Init();
//		// install interrupt handlers
//		IfxCpu_Irq_installInterruptHandler(&asclin1TxISR, IFX_INTPRIO_ASCLIN1_TX);
//		IfxCpu_enableInterrupts();
//		}
//	else if(idx == ASCLIN_DEMO_SPI) // Spi demo
//		{
//		SPI_Init();
//		IfxCpu_Irq_installInterruptHandler(&asclin1TxISR, IFX_INTPRIO_ASCLIN1_TX);
//		IfxCpu_Irq_installInterruptHandler(&asclin1RxISR, IFX_INTPRIO_ASCLIN1_RX);
//		IfxCpu_Irq_installInterruptHandler(&asclin1ErISR, IFX_INTPRIO_ASCLIN1_ER);
//		IfxCpu_enableInterrupts();
//		}
}

void ASCLIN_DemoDeInit(uint32 module, uint32 idx)
{
	ASCLIN_DeInit(idx);
}

void ASCLIN_DemoRun(uint32 module, uint32 idx)
{
	asclin_count++;
	if(asclin_count >= ASCLIN_PRE_DIVDER)
	{
		asclin_count = 0;

		UART_Cyclic();
//		if(idx == ASCLIN_DEMO_UART)
//			UART_Cyclic();
//		else if(idx == ASCLIN_DEMO_SPI)
//			SPI_Cyclic();
//		else if(idx == ASCLIN_DEMO_LIN)
//			LIN_Cyclic();
	}
}

