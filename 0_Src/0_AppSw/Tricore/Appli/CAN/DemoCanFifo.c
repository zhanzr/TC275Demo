#include <Appli/CAN/MCanDemoEntry.h>

static uint32 can_send_fifo_count = 0;
uint32  can_snd_fifo[CAN_FIFO_SIZE][2];
IfxMultican_Message rxMsg_fifo[CAN_FIFO_SIZE];

void CAN_MessagesFifoInit(void)
{
	// create message object config
	IfxMultican_Can_MsgObjConfig canMsgObjConfig;
	// IfxMultican_Can_MsgObj canSrcMsgObj; // defined globally
	{
		memset(&canMsgObjConfig, 0 ,sizeof(IfxMultican_Can_MsgObjConfig));
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
		// FIFO MsgObj allocation:
		canMsgObjConfig.msgObjId = 0; // will allocate MsgObj 0
		canMsgObjConfig.msgObjCount = CAN_FIFO_SIZE/2;
		canMsgObjConfig.firstSlaveObjId = 1;
		canMsgObjConfig.messageId = msg_id; // 'id' defined gloabally
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
		memset(&canMsgObjConfig, 0 ,sizeof(IfxMultican_Can_MsgObjConfig));
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);

		// FIFO MsgObj allocation:
		canMsgObjConfig.msgObjId = CAN_FIFO_SIZE + 1; // avoid clashing with transmit FIFO message objects
		canMsgObjConfig.msgObjCount = CAN_FIFO_SIZE;
		canMsgObjConfig.firstSlaveObjId = CAN_FIFO_SIZE + 2;
		canMsgObjConfig.messageId = msg_id; // 'id' defined gloabally
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

void CAN_ReceiveFifo(void)
{
	IfxMultican_Message rxMsg;
	uint32 wait_count = 1000;
	uint32 i;

	for (i = 0; i < CAN_FIFO_SIZE; ++i)
	{
		// Receiving Data, read the data from the destination receive Fifo
		//    // wait until MCAN received the frame
		// wait until Multican received a new message
		wait_count = 1000;
		while( !IfxMultican_Can_MsgObj_isRxPending(&canDstMsgObj) && wait_count-->0);
		if(wait_count == 0)
			continue;

		IfxMultican_Message_init(&rxMsg, 0xdead, 0xdeadbeef, 0xdeadbeef, IfxMultican_DataLengthCode_8); // start with invalid values
		IfxMultican_Status readStatus = IfxMultican_Can_MsgObj_readMessage(&canDstMsgObj, &rxMsg);

		// if new data is been received report an error
		if(readStatus & IfxMultican_Status_newData)
		{
			rxMsg_fifo[i].id = rxMsg.id;
			rxMsg_fifo[i].lengthCode = rxMsg.lengthCode;
			rxMsg_fifo[i].data[0] = rxMsg.data[0];
			rxMsg_fifo[i].data[1] = rxMsg.data[1];
		}
	}
}

void CAN_SendFifo(void)
{
	IfxMultican_Message txMsg;
	uint32 i;

	for (i = 0; i < CAN_FIFO_SIZE; ++i)
	{
		// Transmit Data from the source message object //
		can_snd_fifo[i][0] = (i<<24) | (i<<16) | (i<<8) |(i<<0);
		can_snd_fifo[i][1] = ~can_snd_fifo[i][0];
		IfxMultican_Message_init(&txMsg, msg_id, can_snd_fifo[i][0], can_snd_fifo[i][1], IfxMultican_DataLengthCode_8);
		while( IfxMultican_Can_MsgObj_sendMessage(&canSrcMsgObj, &txMsg) == IfxMultican_Status_notSentBusy );
	}
}

void CAN_FifoCyclic(void)
{
	can_send_fifo_count++;
	if(can_send_fifo_count >= CAN_DIVIDER_FIFO)
	{
		can_send_fifo_count = 0;

		memset(&rxMsg_fifo[0], 0 ,sizeof(IfxMultican_Message) * CAN_FIFO_SIZE);
		CAN_SendFifo();
		CAN_ReceiveFifo();
		uint32 i;
		for (i= 0; i < CAN_FIFO_SIZE; ++i)
			CAN_PrintMessage(rxMsg_fifo[i].id, &rxMsg_fifo[i].data[0], 2);
	}
}
