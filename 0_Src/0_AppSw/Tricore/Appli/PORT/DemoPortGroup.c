#include <Appli/PORT/PortDemoEntry.h>

void PORT_GroupCyclic(void)
{
	static uint16 grp_out = 0x1;
	static uint8 direct  = 0;
	static uint32 grp_count = 0;

	if(grp_count++ < PORT_DIVIDER_2)
		return;
	grp_count = 0;

	if(direct == 0)
	{
		grp_out = (grp_out<<1) & 0xff;
		if(grp_out == 0)
		{
			grp_out = 0x80;
			direct = 1;
		}
	}
	else
	{
		grp_out = (grp_out>>1) & 0xff;
		if(grp_out == 0)
		{
			grp_out = 0x1;
			direct = 0;
		}
	}

	IfxPort_setGroupState(&MODULE_P33, 0, (uint16)0xff<<6, ~(grp_out<<6));
}


