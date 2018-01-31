#ifndef __CANTEST_H__
#define __CANTEST_H__

#define	CAN_DEMO_BAUD	1000000u

#define	TX_TMO_CNT	20
#define	RX_TMO_CNT	8000000

void CAN_Init(void);
void CAN_TxdRcv(void);

#endif /* __CANTEST_H__ */
