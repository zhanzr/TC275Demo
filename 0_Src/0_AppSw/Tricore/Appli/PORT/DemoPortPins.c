#include <Appli/PORT/PortDemoEntry.h>

void PINS_ToggleCyclic(void)
{
	static uint32 toggle_count = 0;

	if(toggle_count++ < PORT_DIVIDER_1)
		return;
	toggle_count = 0;

	IfxPort_togglePin(&MODULE_P33, 6);
	IfxPort_togglePin(&MODULE_P33, 7);
	IfxPort_togglePin(&MODULE_P33, 8);
	IfxPort_togglePin(&MODULE_P33, 9);
	IfxPort_togglePin(&MODULE_P33, 10);
	IfxPort_togglePin(&MODULE_P33, 11);
	IfxPort_togglePin(&MODULE_P33, 12);
	IfxPort_togglePin(&MODULE_P33, 13);
}

