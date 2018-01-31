#include <stdint.h>
#include <Multican/Can/IfxMultican_Can.h>

#include "CANTest.h"

IfxMultican_Message txMsg;
IfxMultican_Message rxMsg;

//CAN handle
IfxMultican_Can can;

extern float32 g_DieTemp;

#ifdef NODE_0
IfxMultican_Can_Node canSrcNode;
IfxMultican_Can_MsgObj canSrcMsgObj;
#endif

#ifdef NODE_1
IfxMultican_Can_Node canDstNode;
IfxMultican_Can_MsgObj canDstMsgObj;
#endif

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
	//Source NodeÔ´½Úµã
#ifdef NODE_0
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
#endif

	//Destination Node
#ifdef NODE_1
	{
		canNodeConfig.nodeId = IfxMultican_NodeId_1;
		canNodeConfig.rxPin = &IfxMultican_RXD0B_P20_7_IN;//&IfxMultican_RXD1B_P14_1_IN;
		canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
		canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT;//&IfxMultican_TXD1_P14_0_OUT;
		canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;
		//initialise the node
		IfxMultican_Can_Node_init(&canDstNode, &canNodeConfig);
	}

	//IfxMultican_Can_MsgObj canDstMsgObj;
	{
		//	create message object config
		IfxMultican_Can_MsgObjConfig canMsgObjConfig;
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);
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
#endif
}

void CAN_TxdRcv(void)
{
	uint32_t timeout_cnt = 0;
#ifdef NODE_0
	uint32_t tempU32 = (uint32_t)(g_DieTemp * 1000);
	uint32_t tmpTick = schd_GetTick();

	// Initialise the message strcture
	extern IfxMultican_Message txMsg;
	IfxMultican_Message_init(&txMsg, g_SrcId, tempU32, tmpTick, IfxMultican_DataLengthCode_8);

	//Transmit Data
	while (IfxMultican_Can_MsgObj_sendMessage(&canSrcMsgObj, &txMsg)==IfxMultican_Status_notSentBusy)
	{
		printf(">\t");
		timeout_cnt++;
		if(timeout_cnt>=10)
		{
			printf("TxTO\n");
			break;
		}
	}
#endif

#ifdef NODE_1
	printf("R1\n");
	// Fill dummy id and data, which will be replaced by the received data
	IfxMultican_Message_init(&rxMsg, 0xdead, 0xdeadbeef, 0xdeadbeef, IfxMultican_DataLengthCode_8);
	printf("R2\n");

	timeout_cnt = 0;
	while(!IfxMultican_Can_MsgObj_isRxPending(&canDstMsgObj))
	{
		printf("<\t");
		timeout_cnt++;
		if(timeout_cnt>=1000000)
		{
			printf("RxTO\n");
			break;
		}
	}
	printf("R3\n");

	IfxMultican_Status readStatus = IfxMultican_Can_MsgObj_readMessage(&canDstMsgObj, &rxMsg);
    printf("Rcv:\nid:%08X\tLen:%u\tdata:%08X,%08X\tfast:%u\n",
    		rxMsg.id,
			rxMsg.lengthCode,
			rxMsg.data[0], rxMsg.data[1],
			rxMsg.fastBitRate);
#endif
}


