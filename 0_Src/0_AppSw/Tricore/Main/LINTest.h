#ifndef __LINTEST_H__
#define __LINTEST_H__

#include <Ifx_Types.h>
#include "Configuration.h"

#include <Asclin/Lin/IfxAsclin_Lin.h>

typedef struct
{
    struct
    {
        IfxAsclin_Lin lin;                      /**< \brief ASC interface */
    }      drivers;

    uint8  txData[8];
    uint8  rxData[8];
    uint32 size;
} App_AsclinLin;

IFX_EXTERN App_AsclinLin g_AsclinLin;

//#define	LIN_MASTER	1
#define	LIN_SLAVE	1

IFX_EXTERN void LIN_Init(void);
IFX_EXTERN void LIN_TxdRcv(void);

#endif /* __LINTEST_H__ */
