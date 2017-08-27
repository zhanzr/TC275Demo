#ifndef DEMO_MULTICAN
#define DEMO_MULTICAN

#include <Multican/Can/IfxMultican_Can.h>
#include "Appli/Back/demo_handler.h"

#define DEMO_START_KIT 1

#define CAN_PRE_DIVDER	 (1)
#define CAN_DIVIDER_SINGLE  (50)
#define CAN_DIVIDER_FIFO	(100)
#define CAN_DIVIDER_CANFD	(50)

#define CAN_DEMO_SINGLE	 (0)
#define CAN_DEMO_FIFO	 (1)
#define CAN_DEMO_CANFD	 (2)

//add the following defines to your code globally
#define CAN_FIFO_SIZE 16

// CAN handle
extern IfxMultican_Can can_module;

// Nodes handles
extern IfxMultican_Can_Node canSrcNode;
#ifdef DEMO_START_KIT
extern IfxMultican_Can_Node canDstNode;
#endif
// Message Object handles
extern IfxMultican_Can_MsgObj canSrcMsgObj;
#ifdef DEMO_START_KIT
extern IfxMultican_Can_MsgObj canDstMsgObj;
#endif
extern const uint32 msg_id;
extern uint32 txData[16];

void CAN_ModuleInit(void);
void CAN_NodesInit(void);
void CANFD_NodesInit(void);
void CAN_Init(void);
void CAN_DeInit(void);

void CAN_MessagesInit(void);
void CAN_SendSingle(uint32 id, uint32 high, uint32 low);
void CAN_CanSendCyclic(void);

void CAN_MessagesFifoInit(void);
void CAN_SendFifo(void);
void CAN_FifoCyclic(void);

void CANFD_MessagesInit(void);
void CANFD_Receive(uint32 id, uint32 *p_data);
void CANFD_Send(void);
void CANFD_Cyclic(void);

void CAN_PrintMessage(uint32 id, uint32 *p_data, uint8 length);
void CAN_DemoInit(uint32 module, uint32 idx);
void CAN_DemoDeInit(uint32 module, uint32 idx);
void CAN_DemoRun(uint32 module, uint32 idx);

#endif

