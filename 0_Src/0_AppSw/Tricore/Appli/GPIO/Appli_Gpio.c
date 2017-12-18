#include "Appli_Gpio.h"
#include "Cpu/Std/IfxCpu.h"
#include "Cpu/CStart/IfxCpu_CStart.h"

int gpio_count = 0;
int gpio_out = 0;
/*const IfxPort_Io_ConfigPin configPin[] = {
		{&IfxPort_P33_8,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed1}, // P33.8
		{&IfxPort_P33_9, IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed1},  // P33.9
		{&IfxPort_P33_10,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed1}, // P33.10
		{&IfxPort_P33_11, IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed1},  // P33.11
};

const IfxPort_Io_Config conf = {  sizeof(configPin)/sizeof(IfxPort_Io_ConfigPin), (IfxPort_Io_ConfigPin *)configPin};
*/
extern int demo_item;

void Appli_GpioInit(void)
{
    //IfxPort_Io_initModule(&conf);
    // configure P33.8 as general output
    IfxPort_setPinMode(&MODULE_P33, 8,  IfxPort_Mode_outputOpenDrainGeneral);
    // configure P33.9 as general output
    IfxPort_setPinMode(&MODULE_P33, 9,  IfxPort_Mode_outputOpenDrainGeneral);
    // configure P33.10 as general output
    IfxPort_setPinMode(&MODULE_P33, 10,  IfxPort_Mode_outputOpenDrainGeneral);
    // configure P33.11 as general output
    IfxPort_setPinMode(&MODULE_P33, 11,  IfxPort_Mode_outputOpenDrainGeneral);

	IfxPort_setPinState(&MODULE_P33,8,IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33,9,IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33,10,IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33,11,IfxPort_State_high);
}

void Appli_GpioDemoInit(void)
{
	Appli_GpioInit();

    if(demo_item == 2)
    	IfxCpu_setCoreMode(&MODULE_CPU1, IfxCpu_CoreMode_run);
}

void Appli_GpioDemoDeInit(void)
{
	IfxPort_setPinState(&MODULE_P33,8,IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33,9,IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33,10,IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33,11,IfxPort_State_high);

    if(demo_item == 2)
    	IfxCpu_setCoreMode(&MODULE_CPU1, IfxCpu_CoreMode_idle);
}

