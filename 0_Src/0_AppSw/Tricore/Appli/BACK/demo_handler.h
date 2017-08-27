/*****************************************************
 *
 * TriBoard_TC265D_framework.c
 *
 * Description : Hello World in C, ANSI-style
 *
 */

#ifndef DEMO_HANDLER
#define DEMO_HANDLER

#include <stdlib.h>
#include "../ASCLIN/AsclinDemoEntry.h"
#include "../CAN/MCanDemoEntry.h"
//#include "../CCU6/CCU6DemoEntry.h"
//#include "../GPT/GPT12DemoEntry.h"
//#include "../I2C/I2CDemoEntry.h"
#include "../PORT/PortDemoEntry.h"
//#include "../QSPI/QSPIDemoEntry.h"
//#include "../STM/STMDemoEntry.h"
//#include "../VADC/VADCDemoEntry.h"
#include "../DSADC/DSADCDemoEntry.h"
//#include "../ERAY/ErayDemoEntry.h"
//#include "../SENT/SentDemoEntry.h"
//#include "../MSC/MscDemoEntry.h"
//#include "../HSSL/HsslDemoEntry.h"
//#include "../ETH/EthDemoEntry.h"
//#include "../GTM/GtmDemoEntry.h"
//#include "../CORE/CoreDemoEntry.h"
#include "main/main.h"
#include "../Ifx_IntPrioDef.h"

#define PRINT_DEMO_INIT
#define PRINT_DEMO_DEINIT

#define DEMO_SCU_IDX		(0)
#define DEMO_PORT_IDX		(1)
#define DEMO_STM_IDX		(2)
#define DEMO_ASCLIN_IDX		(3)
#define DEMO_QSPI_IDX		(4)
#define DEMO_VADC_IDX		(5)
#define DEMO_CCU6_IDX		(6)
#define DEMO_GPT12_IDX		(7)
#define DEMO_I2C_IDX		(8)
#define DEMO_CAN_IDX		(9)
#define DEMO_ERAY_IDX		(10)
#define DEMO_SENT_IDX       (11)
#define DEMO_MSC_IDX        (12)
#define DEMO_HSSL_IDX       (13)
#define DEMO_DSADC_IDX      (14)
#define DEMO_ETH_IDX        (15)
#define DEMO_CORE_IDX		(16)
#define DEMO_GTM_IDX    	(17)

extern uint32 demo_module;
extern uint32 demo_module_curr;
extern uint32 demo_idx;
extern uint32 demo_idx_curr;

typedef void (*pFunc_DemoInit)(uint32 module, uint32 idx);
typedef void (*pFunc_DemoDeInit)(uint32 module, uint32 idx);
typedef void (*pFunc_DemoRun)(uint32 module, uint32 idx);
typedef struct{
	char *demo_name;
	pFunc_DemoInit demo_init;
	pFunc_DemoDeInit demo_deinit;
	pFunc_DemoRun demo_run;
}sDemoEntry;

uint32 demo_getmodule(char * demo_name);
void demo_init(uint32 module, uint32 idx);
void demo_deinit(uint32 module, uint32 idx);
void demo_run(uint32 module, uint32 idx);
void demo_cyclic(void);

#endif
