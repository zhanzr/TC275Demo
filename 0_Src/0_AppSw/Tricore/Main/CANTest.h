#ifndef __CANTEST_H__
#define __CANTEST_H__

#define	CAN_DEMO_BAUD	1000000u

#define NODE_0	1
//#define	NODE_1	1

void CAN_Init(void);
void CAN_TxdRcv(void);

#endif /* __CANTEST_H__ */
