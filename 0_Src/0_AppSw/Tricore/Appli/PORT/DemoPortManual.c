#include <Appli/PORT/PortDemoEntry.h>

const IfxPort_Io_ConfigPin configPin[] = {
		{&IfxPort_P33_6,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4},  // P33.6
		{&IfxPort_P33_7,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4},  // P33.7
		{&IfxPort_P33_8,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4},  // P33.8
		{&IfxPort_P33_9,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4},  // P33.9
		{&IfxPort_P33_10,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4},  // P33.10
		{&IfxPort_P33_11,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4},  // P33.11
		{&IfxPort_P33_12,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4},  // P33.12
		{&IfxPort_P33_13,  IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4}   // P33.13
};

const IfxPort_Io_Config conf = {
		sizeof(configPin)/sizeof(IfxPort_Io_ConfigPin),
		(IfxPort_Io_ConfigPin *)configPin
};

void PORT_ModuleInit(void)
{
	IfxPort_Io_initModule(&conf);

	// configure P33.6~13 as general output
	IfxPort_setPinMode(&MODULE_P33, 6, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 7, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 8, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 9, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 10, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 11, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 12, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(&MODULE_P33, 13, IfxPort_Mode_outputPushPullGeneral);

	// configure P33.6~13 as output in general pushPull mode
	//IfxPort_setPinModeOutput(&MODULE_P33, 0, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);
	IfxPort_setPinPadDriver(&MODULE_P33, 6, IfxPort_PadDriver_cmosAutomotiveSpeed4);
	IfxPort_setPinPadDriver(&MODULE_P33, 7, IfxPort_PadDriver_cmosAutomotiveSpeed4);
	IfxPort_setPinPadDriver(&MODULE_P33, 8, IfxPort_PadDriver_cmosAutomotiveSpeed4);
	IfxPort_setPinPadDriver(&MODULE_P33, 9, IfxPort_PadDriver_cmosAutomotiveSpeed4);
	IfxPort_setPinPadDriver(&MODULE_P33, 10, IfxPort_PadDriver_cmosAutomotiveSpeed4);
	IfxPort_setPinPadDriver(&MODULE_P33, 11, IfxPort_PadDriver_cmosAutomotiveSpeed4);
	IfxPort_setPinPadDriver(&MODULE_P33, 12, IfxPort_PadDriver_cmosAutomotiveSpeed4);
	IfxPort_setPinPadDriver(&MODULE_P33, 13, IfxPort_PadDriver_cmosAutomotiveSpeed4);

	// configure P33.6~13 output as high to turn off
	IfxPort_setPinState(&MODULE_P33, 6, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 7, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 8, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 9, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 10, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 11, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 12, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 13, IfxPort_State_high);
}

void PORT_ModuleDeInit(void)
{
	// configure P33.6~13 output as as high to turn off
	IfxPort_setPinState(&MODULE_P33, 6, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 7, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 8, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 9, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 10, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 11, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 12, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P33, 13, IfxPort_State_high);

	// configure P33.6~13 as general input
	IfxPort_setPinMode(&MODULE_P33, 6, IfxPort_Mode_inputPullDown);
	IfxPort_setPinMode(&MODULE_P33, 7, IfxPort_Mode_inputPullDown);
	IfxPort_setPinMode(&MODULE_P33, 8, IfxPort_Mode_inputPullDown);
	IfxPort_setPinMode(&MODULE_P33, 9, IfxPort_Mode_inputPullDown);
	IfxPort_setPinMode(&MODULE_P33, 10, IfxPort_Mode_inputPullDown);
	IfxPort_setPinMode(&MODULE_P33, 11, IfxPort_Mode_inputPullDown);
	IfxPort_setPinMode(&MODULE_P33, 12, IfxPort_Mode_inputPullDown);
	IfxPort_setPinMode(&MODULE_P33, 13, IfxPort_Mode_inputPullDown);
}

void PORT_ManualCyclic(void)
{
}


