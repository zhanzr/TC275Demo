#include <Appli/CAN/MCanDemoEntry.h>

static uint32 can_send_count = 0;

void CAN_MessagesInit(void)
{
	IfxMultican_Can_MsgObjConfig canMsgObjConfig;
	// IfxMultican_Can_MsgObj canSrcMsgObj; // defined globally
	{
		// create message object config
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);

		// assigned message object:
		canMsgObjConfig.msgObjId = 0;
		canMsgObjConfig.messageId = msg_id; // 'id' is defined globally
		canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
		canMsgObjConfig.frame = IfxMultican_Frame_transmit;
		canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
		canMsgObjConfig.control.extendedFrame = FALSE;
		canMsgObjConfig.control.matchingId = TRUE;

		// initialize message object
		IfxMultican_Can_MsgObj_init(&canSrcMsgObj, &canMsgObjConfig);
	}

#ifdef DEMO_START_KIT
	// IfxMultican_Can_MsgObj canDstMsgObj; // defined globally
	{
		// create message object config
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);

		// assigned message object:
		canMsgObjConfig.msgObjId = 1;
		canMsgObjConfig.messageId = msg_id;
		canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
		canMsgObjConfig.frame = IfxMultican_Frame_receive;
		canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_8;
		canMsgObjConfig.control.extendedFrame = FALSE;
		canMsgObjConfig.control.matchingId = TRUE;

		// initialize message object
		IfxMultican_Can_MsgObj_init(&canDstMsgObj, &canMsgObjConfig);
	}
#endif
}

void CAN_ReceiveSingle(uint32 id, uint32 *p_high, uint32 *p_low)
{
#ifdef DEMO_START_KIT
	uint32 wait_count = 1000;
	// Receiving Data
	// Initialise the message structure with dummy values, will be replaced by the received values
	IfxMultican_Message rxMsg;
	IfxMultican_Message_init(&rxMsg, 0xdead, 0xdeadbeef, 0xdeadbeef, IfxMultican_DataLengthCode_8); // start with invalid values

	// wait until Multican received a new message
	while( !IfxMultican_Can_MsgObj_isRxPending(&canDstMsgObj) && wait_count-->0);
	if(wait_count == 0)
		return;
	// read message
	IfxMultican_Status readStatus = IfxMultican_Can_MsgObj_readMessage(&canDstMsgObj, &rxMsg);
	// if new data is been received report an error
	if((readStatus & IfxMultican_Status_newData) && (rxMsg.id == id))
	{
		*p_high = rxMsg.data[0];
		*p_low = rxMsg.data[1];
	}
#endif
}

void CAN_SendSingle(uint32 id, uint32 high, uint32 low)
{
	// Initialise the message strcture
	IfxMultican_Message txMsg;
	IfxMultican_Message_init(&txMsg, id, low, high, IfxMultican_DataLengthCode_8);

	uint32_t timeout_cnt = 0;
	// Transmit Data
	printf("P1\t");
	while( IfxMultican_Can_MsgObj_sendMessage(&canSrcMsgObj, &txMsg) == IfxMultican_Status_notSentBusy )
	{
		printf("P2\t");
		printf("Busy\t");
		timeout_cnt++;
		if(timeout_cnt>=10)
		{
			printf("Timeout\n");
			break;
		}
	}
	printf("P3\t");
}

void CAN_CanSendCyclic(void)
{
	uint32 data[2] = {0,0};

	can_send_count++;
	if(can_send_count >= CAN_DIVIDER_SINGLE)
	{
		can_send_count = 0;
		CAN_SendSingle(msg_id, 0xC0CAC01A, 0xBA5EBA11);
		CAN_ReceiveSingle(msg_id, &data[0], &data[1]);
		CAN_PrintMessage(msg_id, &data[0], 2);
	}
}


