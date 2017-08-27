#ifndef __APPLI_ADC_H_
#define __APPLI_ADC_H_
#include "Cpu\Std\Ifx_Types.h"
#include "Port\Io\IfxPort_Io.h"
#include "Vadc/Adc/IfxVadc_Adc.h"

void Appli_AdcInit(void);
void Appli_AdcDemoInit(void);
void Appli_AdcDemoDeInit(void);
void Appli_AdcCyclic(void);

#endif
