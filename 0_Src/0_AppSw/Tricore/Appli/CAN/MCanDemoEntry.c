#include <Appli/CAN/MCanDemoEntry.h>

// CAN handle
IfxMultican_Can can_module;
// Nodes handles
IfxMultican_Can_Node canSrcNode;
#ifdef DEMO_START_KIT
IfxMultican_Can_Node canDstNode;
#endif

// Message Object handles
IfxMultican_Can_MsgObj canSrcMsgObj;
#ifdef DEMO_START_KIT
IfxMultican_Can_MsgObj canDstMsgObj;
#endif

const uint32 msg_id = 0x100;
static uint32 demo_can_count = 0;

void CAN_ModuleInit(void)
{
	// create configuration
	IfxMultican_Can_Config canConfig;
	IfxMultican_Can_initModuleConfig(&canConfig, &MODULE_CAN);

	// initialize module
	// IfxMultican_Can can_module; // defined globally
	IfxMultican_Can_initModule(&can_module, &canConfig);
}

void CAN_NodesInit(void)
{
	// create CAN node config
	IfxMultican_Can_NodeConfig canNodeConfig;
	IfxMultican_Can_Node_initConfig(&canNodeConfig, &can_module);
	canNodeConfig.baudrate = 1000000; // 1 MBaud

	// Source Node
	// IfxMultican_Can_Node canSrcNode; // defined globally
	{
		canNodeConfig.nodeId = IfxMultican_NodeId_0;
		canNodeConfig.rxPin = &IfxMultican_RXD0B_P20_7_IN;
		canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
		canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT;
		canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

		// initialise the node
		IfxMultican_Can_Node_init(&canSrcNode, &canNodeConfig);
	}

#ifdef DEMO_START_KIT
	// Destination Node
	// IfxMultican_Can_Node canDstNode; // defined globally
	{
		canNodeConfig.nodeId = IfxMultican_NodeId_1;
		canNodeConfig.rxPin = &IfxMultican_RXD1B_P14_1_IN;
		canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
		canNodeConfig.txPin = &IfxMultican_TXD1_P14_0_OUT;
		canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

		// initialise the node
		IfxMultican_Can_Node_init(&canDstNode, &canNodeConfig);
	}
#endif
}

void CANFD_NodesInit(void)
{
	// create CAN node config
	IfxMultican_Can_NodeConfig canNodeConfig;

	// Source Node// IfxMultican_Can_Node canSrcNode; // defined globally
	{
		IfxMultican_Can_Node_initConfig(&canNodeConfig, &can_module);
		canNodeConfig.baudrate = 2000000; // 2 MBaud
		canNodeConfig.nodeId = IfxMultican_NodeId_0;
		canNodeConfig.rxPin = &IfxMultican_RXD0B_P20_7_IN;
		canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
		canNodeConfig.txPin = &IfxMultican_TXD0_P20_8_OUT;
		canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

		// choose CAN FD transfer enable or disable //
		canNodeConfig.flexibleDataRate               = TRUE;

		// if CAN FD enabled choose the FD configuration //
		canNodeConfig.fdConfig.nominalBaudrate       = 500000;
		canNodeConfig.fdConfig.nominalSynchJumpWidth = 8000;
		canNodeConfig.fdConfig.nominalSamplePoint    = 7500;
		canNodeConfig.fdConfig.fastBaudrate          = 2000000;
		canNodeConfig.fdConfig.fastSynchJumpWidth    = 8000;
		canNodeConfig.fdConfig.fastSamplePoint       = 7500;
		canNodeConfig.fdConfig.loopDelayOffset       = 0;

		// initialise the node
		IfxMultican_Can_Node_init(&canSrcNode, &canNodeConfig);
	}

	// Destination Node// IfxMultican_Can_Node canDstNode; // defined globally
	{
		IfxMultican_Can_Node_initConfig(&canNodeConfig, &can_module);
		canNodeConfig.baudrate = 2000000; // 2 MBaud
		canNodeConfig.nodeId = IfxMultican_NodeId_1;
		canNodeConfig.rxPin = &IfxMultican_RXD1B_P14_1_IN;
		canNodeConfig.rxPinMode = IfxPort_InputMode_pullUp;
		canNodeConfig.txPin = &IfxMultican_TXD1_P14_0_OUT;
		canNodeConfig.txPinMode = IfxPort_OutputMode_pushPull;

		// choose CAN FD transfer enable or disable //
		canNodeConfig.flexibleDataRate               = TRUE;

		// if CAN FD enabled choose the FD configuration //
		canNodeConfig.fdConfig.nominalBaudrate       = 500000;
		canNodeConfig.fdConfig.nominalSynchJumpWidth = 8000;
		canNodeConfig.fdConfig.nominalSamplePoint    = 7500;
		canNodeConfig.fdConfig.fastBaudrate          = 2000000;
		canNodeConfig.fdConfig.fastSynchJumpWidth    = 8000;
		canNodeConfig.fdConfig.fastSamplePoint       = 7500;
		canNodeConfig.fdConfig.loopDelayOffset       = 0;

		// initialise the node
		IfxMultican_Can_Node_init(&canDstNode, &canNodeConfig);
	}
}

void CAN_DeInit(void)
{
	IfxMultican_Can_deinit(&can_module);
}

void CAN_PrintMessage(uint32 id, uint32 *p_data, uint8 length)
{
	char   cout[20];
/*

	backUart_Write("Id: ", 0);
	memset(&cout[0], 0, sizeof(cout));
	uint2string(id, &cout[0]);
	backUart_Write(cout, 0);

	backUart_Write(", Data: ", 0);
	uint8 i;
	for(i = 0; i < length; i++)
	{
		memset(&cout[0], 0, sizeof(cout));
		uint2string(*p_data++, &cout[0]);
		backUart_Write(cout, 0);
		backUart_Write(", ", 0);
	}

	backUart_Write("\r\n",0);
*/
}

void CAN_DemoInit(uint32 module, uint32 idx)
{
	if(idx == CAN_DEMO_SINGLE)
	{
		CAN_ModuleInit();
		CAN_NodesInit();
		CAN_MessagesInit();
	}
	else if(idx == CAN_DEMO_FIFO)
	{
		CAN_ModuleInit();
		CAN_NodesInit();
		CAN_MessagesFifoInit();
	}
	else if(idx == CAN_DEMO_CANFD)
	{
		CAN_ModuleInit();
		CANFD_NodesInit();
		CANFD_MessagesInit();
	}
}

void CAN_DemoDeInit(uint32 module, uint32 idx)
{
	CAN_DeInit();
}

void CAN_DemoRun(uint32 module, uint32 idx)
{
	demo_can_count++;
	if(demo_can_count >= CAN_PRE_DIVDER)
	{
		demo_can_count = 0;
		if(idx == CAN_DEMO_SINGLE)
			CAN_CanSendCyclic();
		else if(idx == CAN_DEMO_FIFO)
			CAN_FifoCyclic();
		else if(idx == CAN_DEMO_CANFD)
			CANFD_Cyclic();
	}
}

