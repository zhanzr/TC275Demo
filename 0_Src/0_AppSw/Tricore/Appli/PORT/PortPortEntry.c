#include <Appli/PORT/PortDemoEntry.h>

static uint32 port_count = 0;

void PORT_DemoInit(uint32 module, uint32 idx)
{
	PORT_ModuleInit();
}

void PORT_DemoDeInit(uint32 module, uint32 idx)
{
	PORT_ModuleDeInit();
}

void PORT_DemoRun(uint32 module, uint32 idx)
{
	port_count++;
	if(port_count >= PORT_PRE_DIVDER)
	{
		port_count = 0;
		if(idx == PORT_DEMO_PINS)
			PINS_ToggleCyclic();
		else if(idx == PORT_DEMO_GROUP)
			PORT_GroupCyclic();
		else if(idx == PORT_DEMO_MANUAL)
			PORT_ManualCyclic();
	}
}
