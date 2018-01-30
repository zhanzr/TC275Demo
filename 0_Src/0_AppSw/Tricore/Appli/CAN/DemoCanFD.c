#include <Appli/CAN/MCanDemoEntry.h>

static uint32 canfd_send_count = 0;
uint32 canfd_txData[16];
uint32 canfd_rxData[16];

void CANFD_MessagesInit(void) {
	// create message object config
	IfxMultican_Can_MsgObjConfig canMsgObjConfig;
	// IfxMultican_Can_MsgObj canSrcMsgObj; // defined globally
	{
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canSrcNode);
		// assigned message object:
		canMsgObjConfig.msgObjId = 0;
		canMsgObjConfig.messageId = msg_id;
		canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
		canMsgObjConfig.frame = IfxMultican_Frame_transmit;
		canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_64;
		canMsgObjConfig.control.extendedFrame = FALSE;
		canMsgObjConfig.control.matchingId = TRUE;
		canMsgObjConfig.control.topMsgObjId = 250;
		canMsgObjConfig.control.bottomMsgObjId = 251;
		canMsgObjConfig.control.fastBitRate = TRUE; // fast bit rate enable/disable
		// initialize message object
		IfxMultican_Can_MsgObj_init(&canSrcMsgObj, &canMsgObjConfig);
	}
#ifdef DEMO_START_KIT
	// IfxMultican_Can_MsgObj canDstMsgObj; // defined globally
	{
		IfxMultican_Can_MsgObj_initConfig(&canMsgObjConfig, &canDstNode);
		// assigned message object:
		canMsgObjConfig.msgObjId = 1;
		canMsgObjConfig.messageId = msg_id;
		canMsgObjConfig.acceptanceMask = 0x7FFFFFFFUL;
		canMsgObjConfig.frame = IfxMultican_Frame_receive;
		canMsgObjConfig.control.messageLen = IfxMultican_DataLengthCode_64;
		canMsgObjConfig.control.extendedFrame = FALSE;
		canMsgObjConfig.control.matchingId = TRUE;
		canMsgObjConfig.control.topMsgObjId = 252;
		canMsgObjConfig.control.bottomMsgObjId = 253;
		canMsgObjConfig.control.fastBitRate = TRUE;// fast bit rate enable/disable
		// initialize message object
		IfxMultican_Can_MsgObj_init(&canDstMsgObj, &canMsgObjConfig);
	}
#endif
}

void CANFD_Receive(uint32 id, uint32 *p_data) {
#ifdef DEMO_START_KIT
	uint32 wait_count = 5000;
	// Receiving Data
	// Initialise the message strcture with dummy values, will be replaced by the received values
	IfxMultican_Message rxMsg;
	IfxMultican_Message_longFrameInit(&rxMsg, 0xdead,
			IfxMultican_DataLengthCode_64, FALSE);

	// start with invalid values// wait until Multican received a new message
	while (!IfxMultican_Can_MsgObj_isRxPending(&canDstMsgObj)
			&& wait_count-- > 0)
		;
	if (wait_count == 0)
		return;

	// read the message//uint32 rxData[16]; //defined globally
	IfxMultican_Status readStatus = IfxMultican_Can_MsgObj_readLongFrame(
			&canDstMsgObj, &rxMsg, &canfd_rxData[0]);
	// if no new data is been received report an error
	// if a new data is been received no one lost, report the status
	if (!(readStatus & IfxMultican_Status_newData)) {
	}
	if (readStatus != IfxMultican_Status_newDataButOneLost) {
	}
	IfxMultican_Can_MsgObj_clearRxPending(&canDstMsgObj);// data will be available at rxData
#endif
}

void CANFD_Send(void) {
	uint32 i;
	// Initialise the message strcture
	IfxMultican_Message txMsg;
	// load canfd_txData buffer with the data that needs to be send
	// canfd_txData is assumed to be declared globally
	for (i = 0; i < 16; ++i) {
		//  uint32 canfd_txData[16]; // defined globally
		canfd_txData[i] = (0x11110000 + 0x1111 * i);
	}

	IfxMultican_Message_longFrameInit(&txMsg, msg_id,
			IfxMultican_DataLengthCode_64, TRUE);

	// Transmit Data
	uint32_t timeout_cnt = 0;
	while (IfxMultican_Can_MsgObj_sendLongFrame(&canSrcMsgObj, &txMsg,
			canfd_txData) == IfxMultican_Status_notSentBusy)
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
}

void CANFD_Cyclic(void) {
	canfd_send_count++;

	if (canfd_send_count >= CAN_DIVIDER_CANFD) {
		canfd_send_count = 0;
		memset(&canfd_rxData[0], 0, sizeof(uint32) * 16);
		printf("T2\n");
		CANFD_Send();
		printf("T3\n");
		CANFD_Receive(msg_id, &canfd_rxData[0]);
		printf("T4\n");
		CAN_PrintMessage(msg_id, &canfd_rxData[0], 16);
	}
}
