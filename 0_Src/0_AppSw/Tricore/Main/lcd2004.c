#include "Cpu\Std\Ifx_Types.h"
#include "Port\Io\IfxPort_Io.h"
#include "Cpu/Std/IfxCpu.h"
#include "Cpu/CStart/IfxCpu_CStart.h"

#include "Bsp.h"

#include "lcd2004.h"
#include "lcd_pwm_vo.h"

#define	RS_PORT	MODULE_P00
#define	RS_PIN	11

#define	E_PORT	MODULE_P00
#define	E_PIN	12

#define	DB4_PORT	MODULE_P00
#define	DB4_PIN	4

#define	DB5_PORT	MODULE_P00
#define	DB5_PIN	5

#define	DB6_PORT	MODULE_P00
#define	DB6_PIN	6

#define	DB7_PORT	MODULE_P00
#define	DB7_PIN	7

static inline void SimpleDelay(uint32_t d)
{
	for(uint32_t t=0; t<10; ++t)
	{
		wait(d);
	}
}

static inline void RS_H(void)
{
	IfxPort_setPinState(&RS_PORT, RS_PIN, IfxPort_State_high);
}

static inline void RS_L(void)
{
	IfxPort_setPinState(&RS_PORT, RS_PIN, IfxPort_State_low);
}

static inline void E_H(void)
{
	IfxPort_setPinState(&E_PORT, E_PIN, IfxPort_State_high);
}

static inline void E_L(void)
{
	IfxPort_setPinState(&E_PORT, E_PIN, IfxPort_State_low);
}

static inline void D4_H(void)
{
	IfxPort_setPinState(&DB4_PORT, DB4_PIN, IfxPort_State_high);
}

static inline void D4_L(void)
{
	IfxPort_setPinState(&DB4_PORT, DB4_PIN, IfxPort_State_low);
}

static inline void D5_H(void)
{
	IfxPort_setPinState(&DB5_PORT, DB5_PIN, IfxPort_State_high);
}

static inline void D5_L(void)
{
	IfxPort_setPinState(&DB5_PORT, DB5_PIN, IfxPort_State_low);
}

static inline void D6_H(void)
{
	IfxPort_setPinState(&DB6_PORT, DB6_PIN, IfxPort_State_high);
}

static inline void D6_L(void)
{
	IfxPort_setPinState(&DB6_PORT, DB6_PIN, IfxPort_State_low);
}

static inline void D7_H(void)
{
	IfxPort_setPinState(&DB7_PORT, DB7_PIN, IfxPort_State_high);
}

static inline void D7_L(void)
{
	IfxPort_setPinState(&DB7_PORT, DB7_PIN, IfxPort_State_low);
}

static inline void DB4_Wr(uint8_t dat)
{
	(0==(dat&0x08))?D7_L():D7_H();
	(0==(dat&0x04))?D6_L():D6_H();
	(0==(dat&0x02))?D5_L():D5_H();
	(0==(dat&0x01))?D4_L():D4_H();
}

void LCD_WrCmd_4 (uint8_t cmd)
{
	SimpleDelay(WAIT_AVAIL_DLY);

	RS_L(); 
	SimpleDelay(20);
	//	RW_L();
	DB4_Wr(cmd>>4);
	SimpleDelay(20);
	E_H();
	SimpleDelay(20);
	E_L();
	SimpleDelay(20);
	DB4_Wr(cmd);
	SimpleDelay(20);
	E_H();
	SimpleDelay(20);
	E_L();	
}

void LCD_WrDat_4 (uint8_t dat)
{
	SimpleDelay(WAIT_AVAIL_DLY);

	RS_H(); 
	SimpleDelay(20);
	//	RW_L();
	DB4_Wr(dat>>4);
	SimpleDelay(20);
	E_H();
	SimpleDelay(20);
	E_L();
	SimpleDelay(20);
	DB4_Wr(dat);
	SimpleDelay(20);
	E_H();
	SimpleDelay(20);
	E_L();		
}

void LCD_SetPos (uint8_t x, uint8_t y)
{	
	static const uint8_t pos_tab[] = {0x80, 0xc0, 0x94, 0xd4};

	LCD_WrCmd_4(pos_tab[x] + y);	
}

void LCD_Initialize (void) 
{
	//RS
	IfxPort_setPinMode(&RS_PORT, RS_PIN,  IfxPort_Mode_outputPushPullGeneral);
	//E
	IfxPort_setPinMode(&E_PORT, E_PIN, IfxPort_Mode_outputPushPullGeneral);

	//DB4-DB7
	IfxPort_setPinMode(&DB4_PORT, DB4_PIN,  IfxPort_Mode_outputOpenDrainGeneral);
	IfxPort_setPinMode(&DB5_PORT, DB5_PIN,  IfxPort_Mode_outputOpenDrainGeneral);
	IfxPort_setPinMode(&DB6_PORT, DB6_PIN,  IfxPort_Mode_outputOpenDrainGeneral);
	IfxPort_setPinMode(&DB7_PORT, DB7_PIN,  IfxPort_Mode_outputOpenDrainGeneral);

	IfxPort_setPinPadDriver(&RS_PORT,RS_PIN,
	IfxPort_PadDriver_cmosAutomotiveSpeed1) ;
	IfxPort_setPinPadDriver(&E_PORT,E_PIN,
	IfxPort_PadDriver_cmosAutomotiveSpeed1) ;
	IfxPort_setPinPadDriver(&DB4_PORT,DB4_PIN,
	IfxPort_PadDriver_cmosAutomotiveSpeed1) ;
	IfxPort_setPinPadDriver(&DB5_PORT,DB5_PIN,
	IfxPort_PadDriver_cmosAutomotiveSpeed1) ;
	IfxPort_setPinPadDriver(&DB6_PORT,DB6_PIN,
	IfxPort_PadDriver_cmosAutomotiveSpeed1) ;
	IfxPort_setPinPadDriver(&DB7_PORT,DB7_PIN,
	IfxPort_PadDriver_cmosAutomotiveSpeed1) ;

	E_L();

	//Change to 4 bit mode
	LCD_WrCmd_4(0x28);
	LCD_WrCmd_4(0x28);
	LCD_WrCmd_4(0x28);

	LCD_WrCmd_4(0x08);
	LCD_WrCmd_4(LCD_CLEARDISPLAY);
	LCD_WrCmd_4(0x06);
	LCD_WrCmd_4(0x0c);

	LCD_PWM_VO_Init();	
}

void LCD_displayL(uint8_t l,uint8_t hori,uint8_t *s)
{
	uint8_t pos = hori;
	LCD_SetPos(l, hori);
	while((*s) && (pos<LINE_LEN))
	{
		LCD_WrDat_4(*s);
		s++;
		pos++;
	}
}


void class0_tsr (int tin)
{
	switch (tin)
	{
	case 1:
		// serve trap with TIN 1
		break;
	case 2:
		// serve trap with TIN 2
		break;
	case 3:
		// serve trap with TIN 3
		break;

	case 4:
		//
		break;
	case 5:
		// serve trap with TIN 2
		break;
	}

	printf("%s:%i", __func__, tin);
}

void class1_tsr (int tin)
{
	switch (tin)
	{
	case 1:
		// serve trap with TIN 1
		break;
	case 2:
		// serve trap with TIN 2
		break;
	case 3:
		// serve trap with TIN 3
		break;

	case 4:
		//
		break;
	case 5:
		// serve trap with TIN 2
		break;
	}

	printf("%s:%i", __func__, tin);
}

void class2_tsr (int tin)
{
	switch (tin)
	{
	case 1:
		// serve trap with TIN 1
		break;
	case 2:
		// serve trap with TIN 2
		break;
	case 3:
		// serve trap with TIN 3
		break;

	case 4:
		//4 ALN Synch. HW Data Address Alignment.
		break;
	case 5:
		// serve trap with TIN 2
		break;
	}

	printf("%s:%i", __func__, tin);
}

void class3_tsr (int tin)
{
	switch (tin)
	{
	case 1:
		// serve trap with TIN 1
		break;
	case 2:
		// serve trap with TIN 2
		break;
	case 3:
		// serve trap with TIN 3
		break;

	case 4:
		//
		break;
	case 5:
		// serve trap with TIN 2
		break;
	}

	printf("%s:%i", __func__, tin);
}

void class4_tsr (int tin)
{
	switch (tin)
	{
	case 1:
		// serve trap with TIN 1
		break;
	case 2:
		// serve trap with TIN 2
		break;
	case 3:
		// serve trap with TIN 3
		break;
	}

	printf("%s:%i", __func__, tin);
}

void class5_tsr (int tin)
{
	switch (tin)
	{
	case 1:
		// serve trap with TIN 1
		break;
	case 2:
		// serve trap with TIN 2
		break;
	case 3:
		// serve trap with TIN 3
		break;

	case 4:
		//
		break;
	case 5:
		// serve trap with TIN 2
		break;
	}

	printf("%s:%i", __func__, tin);
}

void class6_tsr (int tin)
{
	switch (tin)
	{
	case 1:
		// serve trap with TIN 1
		break;
	case 2:
		// serve trap with TIN 2
		break;
	case 3:
		// serve trap with TIN 3
		break;

	case 4:
		//
		break;
	case 5:
		// serve trap with TIN 2
		break;
	}

	printf("%s:%i", __func__, tin);
}

void class7_tsr (int tin)
{
	switch (tin)
	{
	case 1:
		// serve trap with TIN 1
		break;
	case 2:
		// serve trap with TIN 2
		break;
	case 3:
		// serve trap with TIN 3
		break;

	case 4:
		//
		break;
	case 5:
		// serve trap with TIN 2
		break;
	}

	printf("%s:%i", __func__, tin);
}
