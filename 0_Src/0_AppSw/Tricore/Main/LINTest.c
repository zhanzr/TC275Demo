#include <stdint.h>
#include <stdio.h>
#include <Asclin/Lin/IfxAsclin_Lin.h>

#include "main.h"
#include "LINTest.h"

extern float32 g_DieTemp;

/**< \brief Demo information */
App_AsclinLin g_AsclinLin;

void LIN_Init(void)
{
	/* disable interrupts */
	boolean              interruptState = IfxCpu_disableInterrupts();

	/* create module config */
	IfxAsclin_Lin_Config linConfig;
	IfxAsclin_Lin_initModuleConfig(&linConfig, &MODULE_ASCLIN1);

	/* set the desired baudrate */
	linConfig.brg.baudrate     = 19200;      /* FDR values will be calculated in initModule */

	linConfig.btc.oversampling = IfxAsclin_OversamplingFactor_4;

	/* pin configuration */
	const IfxAsclin_Lin_Pins pins = {
			&IfxAsclin1_RXA_P15_1_IN, IfxPort_InputMode_pullUp,      /* Rx pin */
			&IfxAsclin1_TX_P15_0_OUT, IfxPort_OutputMode_pushPull,   /* Tx pin */
			IfxPort_PadDriver_cmosAutomotiveSpeed1
	};
	linConfig.pins = &pins;

	/* initialize module */
	IfxAsclin_Lin_initModule(&g_AsclinLin.drivers.lin, &linConfig);

	/* enable interrupts again */
	IfxCpu_restoreInterrupts(interruptState);

	printf("Asclin Lin is initialised\n");
}

void LIN_TxdRcv(void)
{
	/* set the id byte */
	uint8 txId = IFXASCLIN_LIN_SEND_RESPONSE;      /* for sending respose after header */
	uint8 rxId = IFXASCLIN_LIN_RECEIVE_RESPONSE;      /* for receiving respose after header */

	/* prepare data to transmit and receive */
	g_AsclinLin.size = 8;

	uint32 i;
#ifdef LIN_MASTER
	for (i = 0; i < g_AsclinLin.size; ++i)
	{
		/* prepare the data to be transfered */
		g_AsclinLin.txData[i] = i + 1;    /* {1, 2, 3 ,4 ,5, 6, 7, 8} */
		/* prepare an empty buffer for received data */
		g_AsclinLin.rxData[i] = 0;
	}

	/* master mode */

	/* TxH */
	IfxAsclin_Lin_sendHeader(&g_AsclinLin.drivers.lin, &txId);                                             /* send header */

	if (g_AsclinLin.drivers.lin.acknowledgmentFlags.txHeaderEnd == 1)                                      /* if the header is been transmitted succesfully */
	{
		//TxR
		IfxAsclin_Lin_sendResponse(&g_AsclinLin.drivers.lin, g_AsclinLin.txData, g_AsclinLin.size);        /* send response */
	}

	if (g_AsclinLin.drivers.lin.acknowledgmentFlags.txResponseEnd == 1)                                    /* if the response is been transmitted succesfully */
	{
		/* TxH */
		IfxAsclin_Lin_sendHeader(&g_AsclinLin.drivers.lin, &rxId);                                         /* send header */

		if (g_AsclinLin.drivers.lin.acknowledgmentFlags.txHeaderEnd == 1)                                  /* if the header is been transmitted succesfully */
		{
			//RxR
			IfxAsclin_Lin_receiveResponse(&g_AsclinLin.drivers.lin, g_AsclinLin.rxData, g_AsclinLin.size); /* receive response */
		}
	}

	printf("\nTxData:\n");
	for (i = 0; i < g_AsclinLin.size; ++i)
	{
		printf("%02X,", g_AsclinLin.txData[i]);
	}
	printf("\nRxData:\n");
	for (i = 0; i < g_AsclinLin.size; ++i)
	{
		printf("%02X,", g_AsclinLin.rxData[i]);
	}
	printf("Asclin Lin Master transactions are finished\n");
#endif

#ifdef LIN_SLAVE
	for (i = 0; i < g_AsclinLin.size; ++i)
	{
		/* prepare the data to be transfered */
		g_AsclinLin.txData[i] = i + 11;    /* {1, 2, 3 ,4 ,5, 6, 7, 8} */
		/* prepare an empty buffer for received data */
		g_AsclinLin.rxData[i] = 0;
	}

	/* Slave mode */

	/* RxH */
	IfxAsclin_Lin_receiveHeader(&g_AsclinLin.drivers.lin, &rxId);

	if (g_AsclinLin.drivers.lin.acknowledgmentFlags.rxHeaderEnd == 1)
	{
		//RxR
		IfxAsclin_Lin_receiveResponse(&g_AsclinLin.drivers.lin, g_AsclinLin.rxData, g_AsclinLin.size);
	}

	if (g_AsclinLin.drivers.lin.acknowledgmentFlags.rxResponseEnd == 1)
	{
		/* RxH */
		IfxAsclin_Lin_receiveHeader(&g_AsclinLin.drivers.lin, &txId);

		if (g_AsclinLin.drivers.lin.acknowledgmentFlags.rxHeaderEnd == 1)
		{
			//TxR
			IfxAsclin_Lin_sendResponse(&g_AsclinLin.drivers.lin, g_AsclinLin.txData, g_AsclinLin.size);
		}
	}

	printf("\nTxData:\n");
	for (i = 0; i < g_AsclinLin.size; ++i)
	{
		printf("%02X,", g_AsclinLin.txData[i]);
	}
	printf("\nRxData:\n");
	for (i = 0; i < g_AsclinLin.size; ++i)
	{
		printf("%02X,", g_AsclinLin.rxData[i]);
	}
	printf("Asclin Lin Slave transactions are finished\n");
#endif
}


