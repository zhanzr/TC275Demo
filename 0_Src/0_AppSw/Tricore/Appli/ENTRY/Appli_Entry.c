#include "Appli_Entry.h"

// used globally
static IfxAsclin_Asc console;
/*static uint8 consoleTxBuffer[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 consoleRxBuffer[ASC_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];*/

IFX_INTERRUPT(console0TxISR, 0, IFX_INTPRIO_COM0_TX)
{
	IfxAsclin_Asc_isrTransmit(&console);
}

IFX_INTERRUPT(console0RxISR, 0, IFX_INTPRIO_COM0_RX)
{
	IfxAsclin_Asc_isrReceive(&console);
}

IFX_INTERRUPT(console0ErISR, 0, IFX_INTPRIO_COM0_ER)
{
	IfxAsclin_Asc_isrError(&console);
}

int main_count = 0;
int menu_item = 0;
int demo_item = 0;
int demo_input = 0;
typeDemoFunc *pDemoCyclic;
typeDemoFunc *pDemoDeInit;
typeDemoFunc *pDemoInit;

boolean Appli_SetCoreMode(Ifx_CPU *cpu, IfxCpu_CoreMode mode)
{
    uint8              reqslp;
    boolean            retValue;
    IfxCpu_ResourceCpu index = IfxCpu_getIndex(cpu);

    /*Modes such as HALT, SLEEP and STBY are not handled at CPU level */
    retValue = TRUE;

    reqslp = (mode == IfxCpu_CoreMode_idle) ? IfxScu_PMCSR_REQSLP_Idle : IfxScu_PMCSR_REQSLP_Run;

    if (retValue == TRUE)
    {
        /*Check if the same core is requesting to change the core run mode */
        if (IfxCpu_getCoreId() != index)
        {                       /*Request is for the other core */
            /*To access PMCSR of other CPUs handle the safety EndInit protection */
            uint16 safetyWdtPw = IfxScuWdt_getSafetyWatchdogPassword();
            IfxScuWdt_clearSafetyEndinit(safetyWdtPw);
            MODULE_SCU.PMCSR[(uint32)IfxCpu_getIndex(cpu)].B.REQSLP = reqslp;
            IfxScuWdt_setSafetyEndinit(safetyWdtPw);

            cpu->DBGSR.B.HALT = 2; /*reset the HALT bit, if it is already done it is no harm in writing again */
        }
        else
        {                          /*Request is for self, this request normally only for halt, otherwise the core is already running anyway! */
            /*To access PMCSR of self handle the cpu EndInit protection */
            uint16 cpuWdtPw = IfxScuWdt_getCpuWatchdogPassword();
            IfxScuWdt_clearCpuEndinit(cpuWdtPw);
            MODULE_SCU.PMCSR[(uint32)index].B.REQSLP = reqslp;
            IfxScuWdt_setCpuEndinit(cpuWdtPw);
        }
    }

    return retValue;
}

void Appli_ConsoleInit(void)
{
	// create module config
	IfxAsclin_Asc_Config consoleConfig;
	IfxAsclin_Asc_initModuleConfig(&consoleConfig, &MODULE_ASCLIN0);

	// set the desired baudrate
	consoleConfig.baudrate.prescaler = 1;
	consoleConfig.baudrate.baudrate = 19200; // FDR values will be calculated in initModule

	// ISR priorities and interrupt target
	consoleConfig.interrupt.txPriority = IFX_INTPRIO_COM0_TX;
	consoleConfig.interrupt.rxPriority = IFX_INTPRIO_COM0_RX;
	consoleConfig.interrupt.erPriority = IFX_INTPRIO_COM0_ER;
	consoleConfig.interrupt.typeOfService = (IfxSrc_Tos)IfxCpu_getCoreId();

/*	// FIFO configuration
	consoleConfig.txBuffer = &consoleTxBuffer;
	consoleConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
	consoleConfig.rxBuffer = &consoleRxBuffer;
	consoleConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;*/

	// pin configuration
	const IfxAsclin_Asc_Pins pins = {
			NULL,                       IfxPort_InputMode_pullUp,        // CTS pin not used
			&IfxAsclin0_RXA_P14_1_IN,   IfxPort_InputMode_pullUp,        // Rx pin
			NULL,           			IfxPort_OutputMode_pushPull,     // RTS pin not used
			&IfxAsclin0_TX_P14_0_OUT,   IfxPort_OutputMode_pushPull,     // Tx pin
			IfxPort_PadDriver_cmosAutomotiveSpeed1
	};

	consoleConfig.pins = &pins;

	// initialize module
	//IfxAsclin_Asc console;
	// defined globally
	IfxAsclin_Asc_initModule(&console, &consoleConfig);

	// install interrupt handlers
	IfxCpu_Irq_installInterruptHandler(&console0TxISR, IFX_INTPRIO_COM0_TX);
	IfxCpu_Irq_installInterruptHandler(&console0RxISR, IFX_INTPRIO_COM0_RX);
	IfxCpu_Irq_installInterruptHandler(&console0ErISR, IFX_INTPRIO_COM0_ER);
	IfxCpu_enableInterrupts();
}

char Appli_ConsoleRead(IfxAsclin_Asc *asclin)
{
	char pchar;
	Ifx_SizeT count = 1;
	boolean result = FALSE;

	result = IfxAsclin_Asc_read(asclin, (void *)&pchar, &count, 50);
	if(result == TRUE)
		return pchar;
	else
		return '\0';
}

void Appli_ConsolePrint(IfxAsclin_Asc *asclin, const char *p_str)
{
	char *pstr = p_str;

	while(*(pstr) != '\0')
		IfxAsclin_Asc_blockingWrite(asclin, (uint8)*pstr++);
}

char ConsoleRead(void)
{
	return Appli_ConsoleRead(&console);
}

void ConsolePrint(const char *p_str)
{
	Appli_ConsolePrint(&console, p_str);
}

void Appli_Initlize(void)
{
	Appli_ConsoleInit();

	//Demo Initialize
    Appli_StmInit();
    Appli_GpioInit();
    Appli_Ccu6Init();

    Appli_DispMenu(menu_item, demo_item);
}

void Appli_ChangeMenu(int *p_menu, int *p_demo, char in)
{
//	if(*p_menu == MENU_ITEM_MAIN)
//	{
//		if(in == '1')
//		{
//			*p_menu = MENU_ITEM_GPIO;
//			*p_demo = 0;
//			pDemoInit = Appli_GpioDemoInit;
//			pDemoCyclic = Appli_GpioCyclic;
//		}
//		else if(in == '2')
//		{
//			*p_menu = MENU_ITEM_STM;
//			*p_demo = 0;
//			pDemoInit = (typeDemoFunc *)0;
//			pDemoCyclic = Appli_StmCyclic;
//		}
//		else if(in == '3')
//		{
//			*p_menu = MENU_ITEM_ASCLIN;
//			*p_demo = 0;
///*			pDemoInit = Appli_AsclinDemoInit;
//			pDemoCyclic = Appli_AsclinCyclic;*/
//		}
//		else if(in == '4')
//		{
//			*p_menu = MENU_ITEM_CCU6;
//			*p_demo = 0;
//			pDemoInit = Appli_Ccu6DemoInit;
//			pDemoCyclic = Appli_Ccu6Cyclic;
//		}
//		else if(in == '5')
//		{
//			*p_menu = MENU_ITEM_GPT;
//			*p_demo = 0;
//			pDemoInit = (typeDemoFunc *)0;;
//			pDemoCyclic = (typeDemoFunc *)0;;
//		}
//		else if(in == '6')
//		{
//			*p_menu = MENU_ITEM_ADC;
//			*p_demo = 0;
//			pDemoInit = Appli_AdcInit;
//			pDemoCyclic = Appli_AdcCyclic;
//		}
//		else if(in == '7')
//		{
//			*p_menu = MENU_ITEM_QSPI;
//			*p_demo = 0;
///*			pDemoInit = Appli_QspiInit;
//			pDemoCyclic = Appli_QspiCyclic;*/
//		}
//		else if(in == '8')
//		{
//			*p_menu = MENU_ITEM_CAN;
//			*p_demo = 0;
///*			pDemoInit = Appli_CanInit;
//			pDemoCyclic = Appli_CanCyclic;*/
//		}
//		else if(in == 'X')
//		{
//			*p_menu = MENU_ITEM_MAIN;
//			*p_demo = 0;
//			pDemoInit = (typeDemoFunc *)0;
//		}
//	}
//	else if(*p_menu > MENU_ITEM_MAIN && *p_menu < MENU_ITEM_NUM)
//	{
//		if(in >= '1' && in <= '9' && *p_demo == 0)
//		{
//			*p_demo = in - '0';
//			if(pDemoInit != (typeDemoFunc *)0)
//				pDemoInit();
//		}
//
//		if(in >= '0' && in <= '9' && *p_demo != 0)
//		{
//			demo_input = in - '0';
//		}
//		else if(in >= 'a' && in <= 'f' && *p_demo != 0)
//		{
//			demo_input = in - 'a' + 10;
//		}
//		else if(in >= 'A' && in <= 'F' && *p_demo != 0)
//		{
//			demo_input = in - 'a' + 10;
//		}
//		else if((in == 'X' || in == 'x') && *p_demo != 0)
//		{
//			if(*p_menu == MENU_ITEM_GPIO)
///*				pDemoDeInit = Appli_GpioDemoDeInit;
//			else if(*p_menu == MENU_ITEM_ASCLIN)
//				pDemoDeInit = Appli_AsclinDemoDeInit;
//			else if(*p_menu == MENU_ITEM_STM)
//				pDemoDeInit = Appli_StmDemoDeInit;
//			else if(*p_menu == MENU_ITEM_CCU6)
//					pDemoDeInit = Appli_Ccu6DemoDeInit;
//
//			if(pDemoDeInit != (typeDemoFunc *)0)
//				pDemoDeInit();*/
//			*p_demo = 0;
//		}
//		else if((in == 'X' || in == 'x') && *p_demo == 0)
//		{
//			*p_demo = 0;
//			*p_menu = MENU_ITEM_MAIN;
//		}
//
//	}
//	else
//	{
//	}
}

void Appli_DispMenu(int menu, int demo)
{
	if(menu == MENU_ITEM_MAIN)
	{
		Appli_ConsolePrint(&console, "Please select the MODULE:\r\n");
		Appli_ConsolePrint(&console, "[1] GPIO\r\n");
		Appli_ConsolePrint(&console, "[2] STM\r\n");
		Appli_ConsolePrint(&console, "[3] ASCLIN\r\n");
		Appli_ConsolePrint(&console, "[4] CCU6\r\n");
		Appli_ConsolePrint(&console, "[5] GPT\r\n");
		Appli_ConsolePrint(&console, "[6] ADC\r\n");
		Appli_ConsolePrint(&console, "[7] QSPI\r\n");
		Appli_ConsolePrint(&console, "[8] MULTI CAN\r\n");
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
	else if(menu == MENU_ITEM_GPIO && demo == 0)
	{
		Appli_ConsolePrint(&console, "Please select the demo of GPIO:\r\n");
		Appli_ConsolePrint(&console, "[1] GPIO general\r\n");
		Appli_ConsolePrint(&console, "[2] GPIO multi-core\r\n");
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
	else if(menu == MENU_ITEM_STM && demo == 0)
	{
		Appli_ConsolePrint(&console, "Please select the demo of STM:\r\n");
		Appli_ConsolePrint(&console, "[1] STM read system timer\r\n");
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
	else if(menu == MENU_ITEM_ASCLIN && demo == 0)
	{
		Appli_ConsolePrint(&console, "Please select the demo of ASCLIN:\r\n");
		Appli_ConsolePrint(&console, "[1] ASC demo\r\n");
		Appli_ConsolePrint(&console, "[2] LIN demo\r\n");
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
	else if(menu == MENU_ITEM_CCU6 && demo == 0)
	{
		Appli_ConsolePrint(&console, "Please select the demo of CCU6:\r\n");
		Appli_ConsolePrint(&console, "[1] CCU6 PWM\r\n");
		Appli_ConsolePrint(&console, "[2] CCU6 ICU\r\n");
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
	else if(menu == MENU_ITEM_GPT && demo == 0)
	{
		Appli_ConsolePrint(&console, "Please select the demo of GPT:\r\n");
		Appli_ConsolePrint(&console, "[1] GPT Not Available\r\n");
#if 0
		Appli_ConsolePrint(&console, "[1] GPT Timer\r\n");
		Appli_ConsolePrint(&console, "[2] GPT Counter\r\n");
#endif
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
	else if(menu == MENU_ITEM_ADC && demo == 0)
	{
		Appli_ConsolePrint(&console, "Please select the demo of ADC:\r\n");
		Appli_ConsolePrint(&console, "[1] ADC Queued\r\n");
		Appli_ConsolePrint(&console, "[2] ADC Scan\r\n");
		Appli_ConsolePrint(&console, "[3] ADC Background\r\n");
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
	else if(menu == MENU_ITEM_QSPI && demo == 0)
	{
		Appli_ConsolePrint(&console, "Please select the demo of QSPI:\r\n");
		Appli_ConsolePrint(&console, "[1] QSPI on board EEPROM\r\n");
		Appli_ConsolePrint(&console, "[2] QSPI DMA\r\n");
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
	else if(menu == MENU_ITEM_CAN && demo == 0)
	{
		Appli_ConsolePrint(&console, "Please select the demo of MULTI-CAN:\r\n");
		Appli_ConsolePrint(&console, "[1] CAN on board\r\n");
		Appli_ConsolePrint(&console, "[2] CAN two boards\r\n");
		Appli_ConsolePrint(&console, "[3] CAN via FIFO\r\n");
		Appli_ConsolePrint(&console, "[4] CAN FD\r\n");
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
	else
	{
		if(menu > MENU_ITEM_MAIN &&  menu < MENU_ITEM_NUM)
		{
			if(menu == MENU_ITEM_GPT)
				Appli_ConsolePrint(&console, "DEMO is not ready\r\n");
			else
				Appli_ConsolePrint(&console, "DEMO is running\r\n");
		}
		else
			Appli_ConsolePrint(&console, "Demo out of selection\r\n");
		Appli_ConsolePrint(&console, "[X] EXIT\r\n");
		Appli_ConsolePrint(&console, "\r\n");
	}
}

void Appli_MenuCyclic(void)
{
	static uint8 ch = (uint8)'\0';

	while(TRUE)
	{
		ch = Appli_ConsoleRead(&console);
		if(ch != '\0')
		{
			Appli_ChangeMenu(&menu_item, &demo_item, ch);
			Appli_DispMenu(menu_item, demo_item);

		}

		break;
	}
}

void Appli_TaskCyclic(void)
{
	main_count++;

	if(main_count % 10 == 5)
	{
		if(menu_item != 0 && demo_item != 0 && pDemoCyclic != (typeDemoFunc *)0)
			pDemoCyclic();
		Appli_MenuCyclic();
	}

	if(main_count > 10000)
	{
		main_count = 0;
	}
}

