#include <Appli/ASCLIN/AsclinDemoEntry.h>

// used globally
static IfxAsclin_Lin lin;

// pin configuration
const IfxAsclin_Lin_Pins lin_pins = {
		&IfxAsclin1_RXB_P15_5_IN,   IfxPort_InputMode_pullUp,    // Rx pin
		&IfxAsclin1_TX_P15_4_OUT,   IfxPort_OutputMode_pushPull, // Tx pin
		IfxPort_PadDriver_cmosAutomotiveSpeed1
};

// data for lin demo
// prepare transmit bytes in case of sending response
uint8 lin_txData[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
// prepare receive buffer in case of receiving response
uint8 lin_rxData[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

LinMessage lin_msg[] = {
		{0x80, &lin_txData[0], 8, TRUE}
		// need slave response, please uncomment it
#ifdef ASCLIN_LIN_SLAVE_RESP
		,{0xC1, &lin_rxData[0], 8, FALSE}
#endif
};

void LIN_Init(void)
{
	// create module config
	IfxAsclin_Lin_Config linConfig;
	IfxAsclin_Lin_initModuleConfig(&linConfig, &DEMO_ASCLIN);

	// set the lin mode of operation
	linConfig.linMode = IfxAsclin_LinMode_master;

	// set the desired baudrate
	linConfig.btc.prescaler = 4;
	linConfig.brg.baudrate = 19200; // FDR values will be calculated in initModule

	linConfig.frameMode  = IfxAsclin_FrameMode_lin;
	linConfig.frame.stopBit = IfxAsclin_StopBit_1;
	linConfig.frame.shiftDir = IfxAsclin_ShiftDirection_lsbFirst;
	linConfig.frame.parityEnable = FALSE;
	linConfig.frame.idleDelay   = IfxAsclin_IdleDelay_2; /* idle dalay 2 bits */
	linConfig.frame.leadDelay   = IfxAsclin_LeadDelay_1; /*lead delay in master 1 bit */
    /* Default Values for Lin Control */
	linConfig.lin.breakLength   = 13;                    /* break generation length 13 */

	linConfig.pins = &lin_pins;

	// initialize module
	//IfxAsclin_Lin lin; // defined globally
	IfxAsclin_Lin_initModule(&lin, &linConfig);

	asclin_inited = TRUE;
}

IfxAsclin_Lin* LIN_GetHandle(void)
{
	return &lin;
}

void LIN_SendHeader(uint8 idx)
{
	LinMessage *p_msg = &lin_msg[idx];
	//send response case
	IfxAsclin_Lin_sendHeader(&lin, &p_msg->id);
}

void LIN_Response(uint8 idx)
{
	LinMessage *p_msg = &lin_msg[idx];

	if (lin.acknowledgmentFlags.txHeaderEnd == 1) // if the header is been transmitted succesfully
	{
		if(p_msg->master_resp == TRUE)
			IfxAsclin_Lin_sendResponse(&lin, p_msg->p_data, p_msg->size);
#ifdef ASCLIN_LIN_SLAVE_RESP
		else if(p_msg->master_resp == FALSE)
			IfxAsclin_Lin_receiveResponse(&lin, p_msg->p_data, p_msg->size);
#endif
	}
}

void LIN_Cyclic(void)
{
	static uint32 lin_count = 0;
	static uint32 msg_idx = 0;

	lin_count++;
	if(lin_count >= ASCLIN_DIVIDER_LIN)
	{
		lin_count = 0;
		msg_idx++;
		if(msg_idx >= sizeof(lin_msg)/sizeof(LinMessage))
			msg_idx = 0;

		LIN_SendHeader(msg_idx);
		LIN_Response(msg_idx);
	}
}

