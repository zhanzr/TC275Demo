#include "Appli_Gpt.h"
#include "Port\Io\IfxPort_Io.h"
#include "src\Std\IfxSrc.h"

int gpt_count = 0;
Ifx_GPT12 *p_gpt;
extern int demo_item;

IFX_INTERRUPT(gpt120ISR_Timer, 0, IFX_INTPRIO_GPT120)
{
	//user code
	// store the time stamp in an array
	gpt_count++;
	if(gpt_count > 500)
	{
		gpt_count = 0;
		//if(demo_item == 1)
			//IfxPort_togglePin(&MODULE_P33, 6);
	}
}

void Appli_GptInit(void)
{
	p_gpt = &MODULE_GPT120;

	if(demo_item == 1)
	{

		IfxCpu_disableInterrupts();

		//p_gpt->T3CON.B.T3M = 0;		//Timer Mode
		p_gpt->T3CON.B.T3UD = 0;	//Timer count up
		p_gpt->T3CON.B.T3I = 3;		//prescaler 64
		p_gpt->T3.U = 0xFB1E;
#if 0
		p_gpt->T2CON.B.T2M = 4;		//Reload T3 with T2 register
		p_gpt->T2CON.B.T2I = 7;
		p_gpt->T2.U = 0xFB1E;
		p_gpt->T3CON.B.T3R = 0;		//T3 run


	    volatile Ifx_SRC_SRCR *src;
	    src = &(MODULE_SRC.GPT12.GPT12[0].T3);

        IfxSrc_init(src, IfxSrc_Tos_cpu0, IFX_INTPRIO_GPT120);
        IfxSrc_enable(src);

	    // install interrupt handlers
	    IfxCpu_Irq_installInterruptHandler(&gpt120ISR_Timer, IFX_INTPRIO_GPT120);
#endif
	    IfxCpu_enableInterrupts();

	}
	else if(demo_item == 2)
	{
		p_gpt->T3CON.B.T3M = 1;		//count Mode
		p_gpt->T3CON.B.T3UD = 0;	//Timer count up
		p_gpt->T3CON.B.T3I = 1;		//rising edge
		p_gpt->T3.U = 0;
		p_gpt->T3CON.B.T3R = 1;		//T3 run
	}
}

void Appli_GptDemoInit(void)
{
}

void Appli_GptDemoDeInit(void)
{
	if(demo_item == 1)
	{
		p_gpt->T3CON.B.T3R = 0;		//T3 stopped
	}
	else if(demo_item == 2)
	{
		p_gpt->T3CON.B.T3M = 0;		//Timer Mode
		p_gpt->T3CON.B.T3UD = 0;	//Timer count up
		p_gpt->T3CON.B.T3I = 3;		//prescaler 64
		p_gpt->T3.U = 0xFB1E;
		p_gpt->T2CON.B.T2M = 4;		//Reload T3 with T2 register
		p_gpt->T2CON.B.T2I = 7;
		p_gpt->T2.U = 0xFB1E;
		p_gpt->T3CON.B.T3R = 0;		//T3 run
	}
}

void Appli_GptDemo_1(void)
{
}

void Appli_GptDemo_2(void)
{
}

void Appli_GptCyclic(void)
{
	if(demo_item == 1)
		Appli_GptDemo_1();
	else if(demo_item == 2)
		Appli_GptDemo_2();
}

