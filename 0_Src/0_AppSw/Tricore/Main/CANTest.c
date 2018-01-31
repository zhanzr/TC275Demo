#include <stdint.h>
#include <stdio.h>
#include <Multican/Can/IfxMultican_Can.h>

#include "main.h"
#include "CANTest.h"

IfxMultican_Message txMsg;
IfxMultican_Message rxMsg;

//CAN handle
IfxMultican_Can can;

extern float32 g_DieTemp;

IfxMultican_Can_Node canSrcNode;
IfxMultican_Can_MsgObj canSrcMsgObj;

IfxMultican_Can_MsgObj canDstMsgObj;

const uint32_t g_SrcId = 0x0cf00203;
const uint32_t g_DstId = 0x0cf00204;

void CAN_Init(void)
{
	//Module Initialisation
	IfxMultican_Can_Config canConfig;
	IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);
	IfxMultican_Can_initModule(&can, &canConfig);

	//Node Initialisation
	IfxMultican_Can_NodeConfig canNodeConfig;
	IfxMultican_Can_Node_initConfig(&canNodeConfig, &can);

	canNodeConfig.baudrate = CAN_DEMO_BAUD;

	{
		canNodeConfig.nodeId = IfxMultican_NodeId_0;
		canNodeConfig.rxPin =&IfxMultican_RXD0B_P20_7_IN;
		canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
		canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT;
		canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

		IfxMultican_Can_Node_init(&canSrcNode,&canNodeConfig);
	}

	//Message Object Initialisation
	{
		//	create message object config
		IfxMultican_Can_MsgObjConfig canMsgObjConfig;
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);

		canMsgObjConfig.msgObjId = 0;
		canMsgObjConfig.messageId = g_SrcId;
		canMsgObjConfig.acceptanceMask =0x7FFFFFFFUL;
		canMsgObjConfig.frame = IfxMultican_Frame_transmit;
		canMsgObjConfig.control.messageLen=IfxMultican_DataLengthCode_8;
		canMsgObjConfig.control.extendedFrame = TRUE;
		canMsgObjConfig.control.matchingId = FALSE;
		//	initialize message object
		IfxMultican_Can_MsgObj_init(&canSrcMsgObj, &canMsgObjConfig);
	}

	{
		//	create message object config
		IfxMultican_Can_MsgObjConfig canMsgObjConfig;
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
		// assigned message object:
		canMsgObjConfig.msgObjId = 1;
		canMsgObjConfig.messageId = g_DstId;
		canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
		canMsgObjConfig.frame = IfxMultican_Frame_receive;
		canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
		canMsgObjConfig.control.extendedFrame = TRUE;
		canMsgObjConfig.control.matchingId = FALSE;
		// initialize message object
		IfxMultican_Can_MsgObj_init(&canDstMsgObj, &canMsgObjConfig);
	}
}

void CAN_TxdRcv(void)
{
	uint32_t timeout_cnt = 0;

	uint32_t tempU32 = (uint32_t)(g_DieTemp * 1000);
	uint32_t tmpTick = schd_GetTick();

	// Initialise the message strcture
	extern IfxMultican_Message txMsg;
	IfxMultican_Message_init(&txMsg, g_SrcId, tempU32, tmpTick, IfxMultican_DataLengthCode_8);

	//Transmit Data
	while (IfxMultican_Can_MsgObj_sendMessage(&canSrcMsgObj, &txMsg)==IfxMultican_Status_notSentBusy)
	{
		timeout_cnt++;
		if(timeout_cnt >= TX_TMO_CNT)
		{
			printf("TxTO\n");
			break;
		}
	}

	// Fill dummy id and data, which will be replaced by the received data
	IfxMultican_Message_init(&rxMsg, 0xdead, 0xdeadbeef, 0xdeadbeef, IfxMultican_DataLengthCode_8);

	timeout_cnt = 0;
	while(!IfxMultican_Can_MsgObj_isRxPending(&canDstMsgObj))
	{
		timeout_cnt++;
		if(timeout_cnt >= RX_TMO_CNT)
		{
			printf("RxTO\n");
			break;
		}
	}

	if(timeout_cnt < RX_TMO_CNT)
	{
		IfxMultican_Status readStatus = IfxMultican_Can_MsgObj_readMessage(&canDstMsgObj, &rxMsg);
		printf("Rcv:\nid:%08X\tLen:%u\tdata:%08X,%08X\tfast:%u\n",
				rxMsg.id,
				rxMsg.lengthCode,
				rxMsg.data[0], rxMsg.data[1],
				rxMsg.fastBitRate);
	}
}


