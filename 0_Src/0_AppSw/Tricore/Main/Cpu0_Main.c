#include "Cpu\Std\Ifx_Types.h"
#include "Cpu\Std\IfxCpu_Intrinsics.h"
#include "Scu\Std\IfxScuWdt.h"
#include "main.h"

//#include "Tricore\Appli\Entry\Appli_Entry.h"

int core0_main (void)
{
	uint32 sys_count = 0, new_count = 0;
	uint32 back_count = 0;

    /*
     * !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdog in the demo if it is required and also service the watchdog periodically
     * */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Enable the global interrupts of this CPU */
    IfxCpu_enableInterrupts();

#ifdef APPLICATION_KIT
    // all our ports are tristate, we enable the pull-up on some pins
    // CS to tlf35584
    IfxPort_setPinModeInput(TLF_USE_CHIPSELECT.pin.port, TLF_USE_CHIPSELECT.pin.pinIndex, IfxPort_Mode_inputPullUp);

    /* TLF init */
    IfxTLF35584_init();

    /* Disable Window Watchdog and ERR pin */
    IfxTLF35584_unprotect_register();
    IfxTLF35584_disable_window_watchdog();
    IfxTLF35584_disable_err_pin_monitor();
    IfxTLF35584_protect_register();
#endif

    schd_init();

	while(1)
	{
		sys_count = schd_GetTick();
		if(new_count != sys_count)
		{
			new_count = sys_count;

			//run demos every 10 ms
			if(new_count % 10 == 0)
			{
				demo_cyclic();
			}

			//check command from console every 50 ms
/*
			if(new_count % 10 == 5)
			{
				back_count++;
				if(back_count >= 5)
				{
					back_count = 0;
					back_cyclic();
				}
			}
*/
		}
	}
	return (1);
}

