#include "lcd_pwm_vo.h"

extern IfxCcu6_TPwm tPwm;

void LCD_PWM_VO_Init(void)
{
	CCU6_TPwm_Initialize(20);
	IfxCcu6_TPwm_start(&tPwm);
}
