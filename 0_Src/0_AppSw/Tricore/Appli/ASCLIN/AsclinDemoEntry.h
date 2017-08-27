#ifndef DEMO_UART
#define DEMO_UART

#include <Asclin/Asc/IfxAsclin_Asc.h>
#include <Asclin/Lin/IfxAsclin_Lin.h>
#include <Asclin/Spi/IfxAsclin_Spi.h>
#include "Appli/Back/demo_handler.h"

#define ASCLIN_PRE_DIVDER	 (1)
#define ASCLIN_DIVIDER_UART	 (50)
#define ASCLIN_DIVIDER_SPI	 (20)
#define ASCLIN_DIVIDER_LIN	 (10)
#define DEMO_ASCLIN          MODULE_ASCLIN0

#define ASCLIN_DEMO_UART	 (0)
#define ASCLIN_DEMO_SPI		 (1)
#define ASCLIN_DEMO_LIN		 (2)

//#define ASCLIN_LIN_SLAVE_RESP

// priorities are normally defined in Ifx_IntPrioDef.h
//#define IFX_INTPRIO_ASCLIN1_TX  77
//#define IFX_INTPRIO_ASCLIN1_RX  78
//#define IFX_INTPRIO_ASCLIN1_ER  79

#define ASC_TX_BUFFER_SIZE 64
#define ASC_RX_BUFFER_SIZE 64

#define SPI_BUFFER_SIZE 8

typedef struct{
	uint8 id;
	uint8 *p_data;
	uint8 size;
	boolean master_resp;
}LinMessage;

extern boolean asclin_inited;
extern IfxAsclin_Spi spi;

void UART_Init(void);
IfxAsclin_Asc* UART_GetHandle(void);
void UART_WriteByte(uint8 data);
void UART_Write(uint8 *p_data, Ifx_SizeT count);
void UART_Cyclic(void);

void LIN_Init(void);
IfxAsclin_Lin* LIN_GetHandle(void);
void LIN_SendHeader(uint8 idx);
void LIN_Response(uint8 idx);
void LIN_Cyclic(void);

void SPI_Init(void);
void SPI_Exchange(uint8 *p_data, uint8 size);
void SPI_Send(uint8 *p_data, uint8 size);
boolean SPI_WaitFor(uint8 *p_data, uint8 size);
void SPI_Cyclic(void);

void ASCLIN_Init(void);
void ASCLIN_DeInit(uint32 idx);
void ASCLIN_DemoInit(uint32 module, uint32 idx);
void ASCLIN_DemoDeInit(uint32 module, uint32 idx);
void ASCLIN_DemoRun(uint32 module, uint32 idx);

#endif

