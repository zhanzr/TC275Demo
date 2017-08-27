/*
 * Ifx_IntPrioDef.h
 *
 *  Created on: 2016-9-13
 *      Author: chenhan
 */

#ifndef IFX_INTPRIODEF_H_
#define IFX_INTPRIODEF_H_
#include "ConfigurationIsr.h"

#define ISR_PRIORITY_ATOM0_CH0      26
 
#define ISR_PRIORITY_TIM0_CH0       27

#define IFX_ERAY_INT0_PRIO        28
#define IFX_ERAY_INT1_PRIO        29
#define IFX_ERAY_NDAT0_PRIO       30
#define IFX_ERAY_NDAT1_PRIO       31
#define IFX_ERAY_MBSC0_PRIO       32
#define IFX_ERAY_MBSC1_PRIO       33

#define IFX_INTPRIO_ASCLIN1_TX    34
#define IFX_INTPRIO_ASCLIN1_RX    35
#define IFX_INTPRIO_ASCLIN1_ER    36

#define IFX_INTPRIO_STM1_SR0  	  37
#define IFX_INTPRIO_FCE  	 	  38
#define IFX_INTPRIO_CCU6 	      39
#define ISR_PRIORITY_INCRENC_ZERO 40
#define ISR_PRIORITY_TIMER_T3     41

#define IFX_INTPRIO_SENT_CHANNEL  42

#define IFX_INTPRIO_DMA_CH1   	  43
#define IFX_INTPRIO_DMA_CH2       44

#define IFX_INTPRIO_DSADC_CH0_PRIO  45

#define IFX_INTPRIO_QSPI0_TX  ISR_PRIORITY_QSPI0_TX
#define IFX_INTPRIO_QSPI0_RX  ISR_PRIORITY_QSPI0_RX
#define IFX_INTPRIO_QSPI0_ER  ISR_PRIORITY_QSPI0_ER

#define IFX_INTPRIO_STM0_SR1      100
#define IFX_INTPRIO_STM1_SR1      101
#define IFX_INTPRIO_STM2_SR1      102

#define IFX_INTPRIO_BACK_UART_TX  105
#define IFX_INTPRIO_BACK_UART_RX  106
#define IFX_INTPRIO_BACK_UART_ER  107

#define IFX_INTPRIO_SCHD_STM0_SR0  110

#endif /* IFX_INTPRIODEF_H_ */
