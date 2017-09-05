/**
 * \file IfxAsclin.h
 * \brief ASCLIN  basic functionality
 * \ingroup IfxLld_Asclin
 *
 * \version iLLD_1_0_0_8_0
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Asclin_Std_Enumerations Enumerations
 * \ingroup IfxLld_Asclin_Std
 * \defgroup IfxLld_Asclin_Std_Operative Operative Functions
 * \ingroup IfxLld_Asclin_Std
 * \defgroup IfxLld_Asclin_Std_Utility Utility Functions
 * \ingroup IfxLld_Asclin_Std
 * \defgroup IfxLld_Asclin_Std_Configuration Configuration Functions
 * \ingroup IfxLld_Asclin_Std
 * \defgroup IfxLld_Asclin_Std_External External Functions
 * \ingroup IfxLld_Asclin_Std
 * \defgroup IfxLld_Asclin_Std_IO IO Pin Configuration Functions
 * \ingroup IfxLld_Asclin_Std
 */

#ifndef IFXASCLIN_H
#define IFXASCLIN_H 1

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include "_Impl/IfxAsclin_cfg.h"
#include "Scu/Std/IfxScuCcu.h"
#include "Scu/Std/IfxScuWdt.h"
#include "Cpu/Std/IfxCpu_Intrinsics.h"

#include "Src/Std/IfxSrc.h"
#include "_PinMap/IfxAsclin_PinMap.h"

#include <tc_inc_path.h>

#include TC_INCLUDE(TCPATH/IfxAsclin_reg.h)
/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/** \addtogroup IfxLld_Asclin_Std_Enumerations
 * \{ */
/** \brief Defines if the classic or the enhanced checksum will be calculated by the checksum block.\n
 * Definition in Ifx_ASCLIN.DATCON.B.CSM
 */
typedef enum
{
    IfxAsclin_Checksum_classic  = 0, /**< \brief classic checksum */
    IfxAsclin_Checksum_enhanced = 1  /**< \brief enhanced checksum */
} IfxAsclin_Checksum;

/** \brief Defines if the received checksum byte is written into the RXFifo or not.\n
 * Definition in Ifx_ASCLIN.LIN.CON.B.CSI
 */
typedef enum
{
    IfxAsclin_ChecksumInjection_notWritten = 0,  /**< \brief checksum byte is not written */
    IfxAsclin_ChecksumInjection_written    = 1   /**< \brief checksum byte is written */
} IfxAsclin_ChecksumInjection;

/** \brief CPOL defines the idle level of the clock signal in the SPI mode.\n
 * Idle level is the level outside the data transmission time intervals.\n
 * Definition in Ifx_ASCLIN.IOCR.B.CPOL
 */
typedef enum
{
    IfxAsclin_ClockPolarity_idleLow  = 0, /**< \brief idle low */
    IfxAsclin_ClockPolarity_idleHigh = 1  /**< \brief idle high */
} IfxAsclin_ClockPolarity;

/** \brief Selection of clock source\n
 * Definition in Ifx_ASCLIN.CSR.B.CLKSEL
 */
typedef enum
{
    IfxAsclin_ClockSource_noClock         = 0,  /**< \brief no clock will be supplied */
    IfxAsclin_ClockSource_kernelClock     = 1,  /**< \brief f clc will be supplied */
    IfxAsclin_ClockSource_oscillatorClock = 2,  /**< \brief XTAL oscillator clock foso0 will be supplied */
    IfxAsclin_ClockSource_flexRayClock    = 4,  /**< \brief f eray will be supplied */
    IfxAsclin_ClockSource_ascFastClock    = 8,  /**< \brief f asclinf wiil be supplied */
    IfxAsclin_ClockSource_ascSlowClock    = 16  /**< \brief f asclins will be supplied */
} IfxAsclin_ClockSource;

/** \brief CTS input pin selection\n
 * Definition in Ifx_ASCLIN.IOCR.B.CTS
 */
typedef enum
{
    IfxAsclin_CtsInputSelect_0,     /**< \brief CTS input pin 0  */
    IfxAsclin_CtsInputSelect_1,     /**< \brief CTS input pin 1  */
    IfxAsclin_CtsInputSelect_2,     /**< \brief CTS input pin 2  */
    IfxAsclin_CtsInputSelect_3      /**< \brief CTS input pin 3  */
} IfxAsclin_CtsInputSelect;

/** \brief Number of bits per transfer\n
 * Definition in Ifx_ASCLIN.FRAMECON.B.DATALEN
 */
typedef enum
{
    IfxAsclin_DataLength_1 = 0,      /**< \brief number of bits per transfer 0  */
    IfxAsclin_DataLength_2,          /**< \brief number of bits per transfer 1  */
    IfxAsclin_DataLength_3,          /**< \brief number of bits per transfer 2  */
    IfxAsclin_DataLength_4,          /**< \brief number of bits per transfer 3  */
    IfxAsclin_DataLength_5,          /**< \brief number of bits per transfer 4  */
    IfxAsclin_DataLength_6,          /**< \brief number of bits per transfer 5  */
    IfxAsclin_DataLength_7,          /**< \brief number of bits per transfer 6  */
    IfxAsclin_DataLength_8,          /**< \brief number of bits per transfer 7  */
    IfxAsclin_DataLength_9,          /**< \brief number of bits per transfer 8  */
    IfxAsclin_DataLength_10,         /**< \brief number of bits per transfer 9  */
    IfxAsclin_DataLength_11,         /**< \brief number of bits per transfer 10  */
    IfxAsclin_DataLength_12,         /**< \brief number of bits per transfer 11  */
    IfxAsclin_DataLength_13,         /**< \brief number of bits per transfer 12  */
    IfxAsclin_DataLength_14,         /**< \brief number of bits per transfer 13  */
    IfxAsclin_DataLength_15,         /**< \brief number of bits per transfer 14  */
    IfxAsclin_DataLength_16          /**< \brief number of bits per transfer 15  */
} IfxAsclin_DataLength;

/** \brief Defines the basic operating mode of the module. Changing the mode must be done by switching first to initialize mode, and then to the other mode.\n
 * Definition in Ifx_ASCLIN.FRAMECON.B.MODE
 */
typedef enum
{
    IfxAsclin_FrameMode_initialise = 0,  /**< \brief initialise mode */
    IfxAsclin_FrameMode_asc        = 1,  /**< \brief asc mode */
    IfxAsclin_FrameMode_spi        = 2,  /**< \brief spi mode */
    IfxAsclin_FrameMode_lin        = 3   /**< \brief lin mode */
} IfxAsclin_FrameMode;

/** \brief Defines if LIN frame shall consist of a header and response or of a header only.\n
 * Definition in Ifx_ASCLIN.DATCON.B.HO
 */
typedef enum
{
    IfxAsclin_HeaderResponseSelect_headerAndResponse = 0,  /**< \brief header and response expected */
    IfxAsclin_HeaderResponseSelect_headerOnly        = 1   /**< \brief header only expected */
} IfxAsclin_HeaderResponseSelect;

/** \brief Defines the duration of the IDLE delay in bit times.\n
 * SPI mode: this is the idle time between the frames.\n
 * ASC and LIN mode: this is the pause inserted between transmission of bytes.\n
 * Definition in Ifx_ASCLIN.FRAMECON.B.IDLE
 */
typedef enum
{
    IfxAsclin_IdleDelay_0,     /**< \brief idle delay in 0 bit times  */
    IfxAsclin_IdleDelay_1,     /**< \brief idle delay in 1 bit times  */
    IfxAsclin_IdleDelay_2,     /**< \brief idle delay in 2 bit times  */
    IfxAsclin_IdleDelay_3,     /**< \brief idle delay in 3 bit times  */
    IfxAsclin_IdleDelay_4,     /**< \brief idle delay in 4 bit times  */
    IfxAsclin_IdleDelay_5,     /**< \brief idle delay in 5 bit times  */
    IfxAsclin_IdleDelay_6,     /**< \brief idle delay in 6 bit times  */
    IfxAsclin_IdleDelay_7      /**< \brief idle delay in 7 bit times  */
} IfxAsclin_IdleDelay;

/** \brief Defines the leading delay in bit times in SPI mode. ASC mode: not applicable.\n
 * LIN mode: delay between the end of the break and the start of the sync character.\n
 * Definition in Ifx_ASCLIN.FRAMECON.B.LEAD
 */
typedef enum
{
    IfxAsclin_LeadDelay_0,     /**< \brief lead delay in 0 bit times  */
    IfxAsclin_LeadDelay_1,     /**< \brief lead delay in 1 bit times  */
    IfxAsclin_LeadDelay_2,     /**< \brief lead delay in 2 bit times  */
    IfxAsclin_LeadDelay_3,     /**< \brief lead delay in 3 bit times  */
    IfxAsclin_LeadDelay_4,     /**< \brief lead delay in 4 bit times  */
    IfxAsclin_LeadDelay_5,     /**< \brief lead delay in 5 bit times  */
    IfxAsclin_LeadDelay_6,     /**< \brief lead delay in 6 bit times  */
    IfxAsclin_LeadDelay_7      /**< \brief lead delay in 7 bit times  */
} IfxAsclin_LeadDelay;

/** \brief Configures if the module in LIN mode operates as master or slave\n
 * Definition in Ifx_ASCLIN.LIN.CON.B.MS
 */
typedef enum
{
    IfxAsclin_LinMode_slave  = 0, /**< \brief operates in slave mode */
    IfxAsclin_LinMode_master = 1  /**< \brief operates in master mode */
} IfxAsclin_LinMode;

/** \brief Defines the timeout threshold of RESPOSE bit is for LIN response timeout or LIN frame timeout.\n
 * Definition in Ifx_ASCLIN.DATCON.B.RM
 */
typedef enum
{
    IfxAsclin_LinResponseTimeoutMode_frameTimeout    = 0, /**< \brief timeout threshold is for frame */
    IfxAsclin_LinResponseTimeoutMode_responseTimeout = 1  /**< \brief timeout threshold is for reponse */
} IfxAsclin_LinResponseTimeoutMode;

/** \brief Defines the bit length in ticks in the range of 1 to 16. The lengths of 1 to 3 are not allowed\n
 * Definition in Ifx_ASCLIN.BITCON.B.OVERSAMPLING
 */
typedef enum
{
    IfxAsclin_OversamplingFactor_4  = 3,   /**< \brief oversampling factor 4 */
    IfxAsclin_OversamplingFactor_5  = 4,   /**< \brief oversampling factor 5 */
    IfxAsclin_OversamplingFactor_6  = 5,   /**< \brief oversampling factor 6 */
    IfxAsclin_OversamplingFactor_7  = 6,   /**< \brief oversampling factor 7 */
    IfxAsclin_OversamplingFactor_8  = 7,   /**< \brief oversampling factor 8 */
    IfxAsclin_OversamplingFactor_9  = 8,   /**< \brief oversampling factor 9 */
    IfxAsclin_OversamplingFactor_10 = 9,   /**< \brief oversampling factor 10 */
    IfxAsclin_OversamplingFactor_11 = 10,  /**< \brief oversampling factor 11 */
    IfxAsclin_OversamplingFactor_12 = 11,  /**< \brief oversampling factor 12 */
    IfxAsclin_OversamplingFactor_13 = 12,  /**< \brief oversampling factor 13 */
    IfxAsclin_OversamplingFactor_14 = 13,  /**< \brief oversampling factor 14 */
    IfxAsclin_OversamplingFactor_15 = 14,  /**< \brief oversampling factor 15 */
    IfxAsclin_OversamplingFactor_16 = 15   /**< \brief oversampling factor 16 */
} IfxAsclin_OversamplingFactor;

/** \brief Type of parity bit attached to data bits\n
 * Definition in Ifx_ASCLIN.FRAMECON.B.ODD
 */
typedef enum
{
    IfxAsclin_ParityType_even = 0,  /**< \brief even parity */
    IfxAsclin_ParityType_odd  = 1   /**< \brief odd parity */
} IfxAsclin_ParityType;

/** \brief Receive buffer mode\n
 * Definition in Ifx_ASCLIN.RXFIFOCON.B.BUF
 */
typedef enum
{
    IfxAsclin_ReceiveBufferMode_rxFifo   = 0, /**< \brief RxFIFO mode */
    IfxAsclin_ReceiveBufferMode_rxBuffer = 1  /**< \brief single stage Rx buffer */
} IfxAsclin_ReceiveBufferMode;

/** \brief Polarity of the RTS and CTS signals\n
 * Definition in Ifx_ASCLIN.IOCR.B.RCPOL
 */
typedef enum
{
    IfxAsclin_RtsCtsPolarity_activeHigh = 0,  /**< \brief ready/clear are active-high */
    IfxAsclin_RtsCtsPolarity_activeLow  = 1   /**< \brief ready/clear are active-low */
} IfxAsclin_RtsCtsPolarity;

/** \brief Defines the filling level that triggers a drain (RX) interrupt or DMA access\n
 * Definition in Ifx_ASCLIN.RXFIFOCON.B.INTLEVEL
 */
typedef enum
{
    IfxAsclin_RxFifoInterruptLevel_1,     /**< \brief RX FIFO level 1  */
    IfxAsclin_RxFifoInterruptLevel_2,     /**< \brief RX FIFO level 2  */
    IfxAsclin_RxFifoInterruptLevel_3,     /**< \brief RX FIFO level 3  */
    IfxAsclin_RxFifoInterruptLevel_4,     /**< \brief RX FIFO level 4  */
    IfxAsclin_RxFifoInterruptLevel_5,     /**< \brief RX FIFO level 5  */
    IfxAsclin_RxFifoInterruptLevel_6,     /**< \brief RX FIFO level 6  */
    IfxAsclin_RxFifoInterruptLevel_7,     /**< \brief RX FIFO level 7  */
    IfxAsclin_RxFifoInterruptLevel_8,     /**< \brief RX FIFO level 8  */
    IfxAsclin_RxFifoInterruptLevel_9,     /**< \brief RX FIFO level 9  */
    IfxAsclin_RxFifoInterruptLevel_10,    /**< \brief RX FIFO level 10  */
    IfxAsclin_RxFifoInterruptLevel_11,    /**< \brief RX FIFO level 11  */
    IfxAsclin_RxFifoInterruptLevel_12,    /**< \brief RX FIFO level 12  */
    IfxAsclin_RxFifoInterruptLevel_13,    /**< \brief RX FIFO level 13  */
    IfxAsclin_RxFifoInterruptLevel_14,    /**< \brief RX FIFO level 14  */
    IfxAsclin_RxFifoInterruptLevel_15,    /**< \brief RX FIFO level 15  */
    IfxAsclin_RxFifoInterruptLevel_16     /**< \brief RX FIFO level 16  */
} IfxAsclin_RxFifoInterruptLevel;

/** \brief Defines the number of bytes read from the Rx FIFO with one FPI bus read\n
 * Definition in Ifx_ASCLIN.RXFIFOCON.B.OUTW
 */
typedef enum
{
    IfxAsclin_RxFifoOutletWidth_0,     /**< \brief number of bytes 0  */
    IfxAsclin_RxFifoOutletWidth_1,     /**< \brief number of bytes 1  */
    IfxAsclin_RxFifoOutletWidth_2,     /**< \brief number of bytes 2  */
    IfxAsclin_RxFifoOutletWidth_3      /**< \brief number of bytes 3  */
} IfxAsclin_RxFifoOutletWidth;

/** \brief Alternate input selection for Rx signal.\n
 * Definition in Ifx_ASCLIN.IOCR.B.ALTI
 */
typedef enum
{
    IfxAsclin_RxInputSelect_0,     /**< \brief alternate input selection 0  */
    IfxAsclin_RxInputSelect_1,     /**< \brief alternate input selection 1  */
    IfxAsclin_RxInputSelect_2,     /**< \brief alternate input selection 2  */
    IfxAsclin_RxInputSelect_3,     /**< \brief alternate input selection 3  */
    IfxAsclin_RxInputSelect_4,     /**< \brief alternate input selection 4  */
    IfxAsclin_RxInputSelect_5,     /**< \brief alternate input selection 5  */
    IfxAsclin_RxInputSelect_6,     /**< \brief alternate input selection 6  */
    IfxAsclin_RxInputSelect_7      /**< \brief alternate input selection 7  */
} IfxAsclin_RxInputSelect;

/** \brief Sample point position\n
 * Definition in Ifx_ASCLIN.BITCON.B.SAMPLEPOINT
 */
typedef enum
{
    IfxAsclin_SamplePointPosition_1  = 1,   /**< \brief sample point position at 1 */
    IfxAsclin_SamplePointPosition_2  = 2,   /**< \brief sample point position at 2 */
    IfxAsclin_SamplePointPosition_3  = 3,   /**< \brief sample point position at 3 */
    IfxAsclin_SamplePointPosition_4  = 4,   /**< \brief sample point position at 4 */
    IfxAsclin_SamplePointPosition_5  = 5,   /**< \brief sample point position at 5 */
    IfxAsclin_SamplePointPosition_6  = 6,   /**< \brief sample point position at 6 */
    IfxAsclin_SamplePointPosition_7  = 7,   /**< \brief sample point position at 7 */
    IfxAsclin_SamplePointPosition_8  = 8,   /**< \brief sample point position at 8 */
    IfxAsclin_SamplePointPosition_9  = 9,   /**< \brief sample point position at 9 */
    IfxAsclin_SamplePointPosition_10 = 10,  /**< \brief sample point position at 10 */
    IfxAsclin_SamplePointPosition_11 = 11,  /**< \brief sample point position at 11 */
    IfxAsclin_SamplePointPosition_12 = 12,  /**< \brief sample point position at 12 */
    IfxAsclin_SamplePointPosition_13 = 13,  /**< \brief sample point position at 13 */
    IfxAsclin_SamplePointPosition_14 = 14,  /**< \brief sample point position at 14 */
    IfxAsclin_SamplePointPosition_15 = 15   /**< \brief sample point position at 15 */
} IfxAsclin_SamplePointPosition;

/** \brief Number of samples per bit, sample mode/medianfilter\n
 * Definition in Ifx_ASCLIN.BITCON.B.SM
 */
typedef enum
{
    IfxAsclin_SamplesPerBit_one   = 0, /**< \brief one sample per bit */
    IfxAsclin_SamplesPerBit_three = 1  /**< \brief three samples per bit */
} IfxAsclin_SamplesPerBit;

/** \brief SPI mode: defines the shift direction of the shift register.\n
 * ASC and LIN mode: should be set to 0.\n
 * Definition in Ifx_ASCLIN.FRAMECON.B.MSB
 */
typedef enum
{
    IfxAsclin_ShiftDirection_lsbFirst = 0,  /**< \brief LSB first */
    IfxAsclin_ShiftDirection_msbFirst = 1   /**< \brief MSB first */
} IfxAsclin_ShiftDirection;

/** \brief Defines the idle level of the SLSO signal, which is the level.\n
 * Outside the data transmission, leading and trailing time intervals.\n
 * Definition in Ifx_ASCLIN.IOCR.B.SPOL
 */
typedef enum
{
    IfxAsclin_SlavePolarity_idleLow  = 0, /**< \brief idle low */
    IfxAsclin_SlavePolarity_idlehigh = 1  /**< \brief idle high */
} IfxAsclin_SlavePolarity;

/** \brief Enable/disable the sensitivity of the module to sleep signal\n
 * Definition in Ifx_ASCLIN.CLC.B.EDIS
 */
typedef enum
{
    IfxAsclin_SleepMode_enable  = 0, /**< \brief enables sleep mode */
    IfxAsclin_SleepMode_disable = 1  /**< \brief disables sleep mode */
} IfxAsclin_SleepMode;

/** \brief Error status
 */
typedef enum
{
    IfxAsclin_Status_configurationError = 0,  /**< \brief Configuration error */
    IfxAsclin_Status_noError            = 1   /**< \brief No error */
} IfxAsclin_Status;

/** \brief ASC and LIN mode: number of stop bits (0 is not allowed), SPI mode: trailing delay.\n
 * Definition in Ifx_ASCLIN.FRAMECON.B.STOP
 */
typedef enum
{
    IfxAsclin_StopBit_0,     /**< \brief number of stop bits 0  */
    IfxAsclin_StopBit_1,     /**< \brief number of stop bits 1  */
    IfxAsclin_StopBit_2,     /**< \brief number of stop bits 2  */
    IfxAsclin_StopBit_3,     /**< \brief number of stop bits 3  */
    IfxAsclin_StopBit_4,     /**< \brief number of stop bits 4  */
    IfxAsclin_StopBit_5,     /**< \brief number of stop bits 5  */
    IfxAsclin_StopBit_6,     /**< \brief number of stop bits 6  */
    IfxAsclin_StopBit_7      /**< \brief number of stop bits 7  */
} IfxAsclin_StopBit;

/** \brief Defines the number of bytes written to the Tx FIFO with one FPI bus write\n
 * Definition in Ifx_ASCLIN.TXFIFOCON.B.INW
 */
typedef enum
{
    IfxAsclin_TxFifoInletWidth_0,     /**< \brief number of bytes 0  */
    IfxAsclin_TxFifoInletWidth_1,     /**< \brief number of bytes 1  */
    IfxAsclin_TxFifoInletWidth_2,     /**< \brief number of bytes 2  */
    IfxAsclin_TxFifoInletWidth_3      /**< \brief number of bytes 3  */
} IfxAsclin_TxFifoInletWidth;

/** \brief Defines the filling level that triggers a refill (TX) interrupt or DMA access\n
 * Definition in Ifx_ASCLIN.TXFIFOCON.B.INTLEVEL
 */
typedef enum
{
    IfxAsclin_TxFifoInterruptLevel_0,     /**< \brief TX FIFO level 0  */
    IfxAsclin_TxFifoInterruptLevel_1,     /**< \brief TX FIFO level 1  */
    IfxAsclin_TxFifoInterruptLevel_2,     /**< \brief TX FIFO level 2  */
    IfxAsclin_TxFifoInterruptLevel_3,     /**< \brief TX FIFO level 3  */
    IfxAsclin_TxFifoInterruptLevel_4,     /**< \brief TX FIFO level 4  */
    IfxAsclin_TxFifoInterruptLevel_5,     /**< \brief TX FIFO level 5  */
    IfxAsclin_TxFifoInterruptLevel_6,     /**< \brief TX FIFO level 6  */
    IfxAsclin_TxFifoInterruptLevel_7,     /**< \brief TX FIFO level 7  */
    IfxAsclin_TxFifoInterruptLevel_8,     /**< \brief TX FIFO level 8  */
    IfxAsclin_TxFifoInterruptLevel_9,     /**< \brief TX FIFO level 9  */
    IfxAsclin_TxFifoInterruptLevel_10,    /**< \brief TX FIFO level 10  */
    IfxAsclin_TxFifoInterruptLevel_11,    /**< \brief TX FIFO level 11  */
    IfxAsclin_TxFifoInterruptLevel_12,    /**< \brief TX FIFO level 12  */
    IfxAsclin_TxFifoInterruptLevel_13,    /**< \brief TX FIFO level 13  */
    IfxAsclin_TxFifoInterruptLevel_14,    /**< \brief TX FIFO level 14  */
    IfxAsclin_TxFifoInterruptLevel_15     /**< \brief TX FIFO level 15  */
} IfxAsclin_TxFifoInterruptLevel;

/** \} */

/** \addtogroup IfxLld_Asclin_Std_Operative
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief clears all the flags
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearAllFlags(Ifx_ASCLIN *asclin);

/** \brief Clears the break detected flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearBreakDetectedFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the collision detection error flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearCollisionDetectionErrorFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the falling edge detected flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearFallingEdgeDetectedFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the frame error flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearFrameErrorFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the header timeout flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearHeaderTimeoutFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the kernel reset status
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearKernelResetStatus(Ifx_ASCLIN *asclin);

/** \brief Clears the auto baudrate detection error flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearLinAutoBaudDetectionErrorFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the LIN checksum error flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearLinChecksumErrorFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the LIN parity error flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearLinParityErrorFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the parity error flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearParityErrorFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the raising edge detected flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearRaisingEdgeDetectedFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the response timeout flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearResponseTimeoutFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the Rx FIFO level flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearRxFifoFillLevelFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the Rx FIFO overflow flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearRxFifoOverflowFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the Rx FIFO underflow flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearRxFifoUnderflowFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the receive header end flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearRxHeaderEndFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the receive response end flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearRxResponseEndFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the transmission pending flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearTransmissionCompletedFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the Tx FIFO level flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearTxFifoFillLevelFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the Tx FIFO overflow flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearTxFifoOverflowFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the transmit header end flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearTxHeaderEndFlag(Ifx_ASCLIN *asclin);

/** \brief Clears the transmit response end flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_clearTxResponseEndFlag(Ifx_ASCLIN *asclin);

/** \brief Flushes (empties) the Rx FIFO
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_flushRxFifo(Ifx_ASCLIN *asclin);

/** \brief Flushes (empties) the Tx FIFO
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_flushTxFifo(Ifx_ASCLIN *asclin);

/** \brief Sets the baudrate detection's lower limit
 * \param asclin pointer to ASCLIN registers
 * \param limit value of the lower limit
 * \return None
 */
IFX_INLINE void IfxAsclin_setBrdLowerlimt(Ifx_ASCLIN *asclin, uint8 limit);

/** \brief Sets the baudrate detection's upper limit
 * \param asclin pointer to ASCLIN registers
 * \param limit value of the upper limit
 * \return None
 */
IFX_INLINE void IfxAsclin_setBrdUpperlimt(Ifx_ASCLIN *asclin, uint8 limit);

/** \brief Sets the checksum injection
 * \param asclin pointer to ASCLIN registers
 * \param csi checksum injection selection (not written / written)
 * \return None
 */
IFX_INLINE void IfxAsclin_setChecksumInjection(Ifx_ASCLIN *asclin, IfxAsclin_ChecksumInjection csi);

/** \brief Sets the checksum mode
 * \param asclin pointer to ASCLIN registers
 * \param mode checksum mode selection (classic / enhanced)
 * \return None
 */
IFX_INLINE void IfxAsclin_setChecksumMode(Ifx_ASCLIN *asclin, IfxAsclin_Checksum mode);

/** \brief Sets the data length (number of bits per transfer)
 * \param asclin pointer to ASCLIN registers
 * \param length data length selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setDataLength(Ifx_ASCLIN *asclin, IfxAsclin_DataLength length);

/** \brief Sets the denominator of the fractional divider
 * \param asclin pointer to ASCLIN registers
 * \param denominator value of the denominator
 * \return None
 */
IFX_INLINE void IfxAsclin_setDenominator(Ifx_ASCLIN *asclin, uint16 denominator);

/** \brief Sets the disable module request
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_setDisableModuleRequest(Ifx_ASCLIN *asclin);

/** \brief Sets the enable module request
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_setEnableModuleRequest(Ifx_ASCLIN *asclin);

/** \brief Sets digital glitch filter depth
 * \param asclin pointer to ASCLIN registers
 * \param depth digital glitch filter depth selection (1 to 63)
 * \return None
 */
IFX_INLINE void IfxAsclin_setFilterDepth(Ifx_ASCLIN *asclin, uint8 depth);

/** \brief Sets the header response selection
 * \param asclin pointer to ASCLIN registers
 * \param type type of selection (header and response or header only)
 * \return None
 */
IFX_INLINE void IfxAsclin_setHeaderResponseSelect(Ifx_ASCLIN *asclin, IfxAsclin_HeaderResponseSelect type);

/** \brief Sets the idle delay
 * \param asclin pointer to ASCLIN registers
 * \param delay idle delay selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setIdleDelay(Ifx_ASCLIN *asclin, IfxAsclin_IdleDelay delay);

/** \brief Sets the kernal reset of the KRST1
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_setKernelResetOne(Ifx_ASCLIN *asclin);

/** \brief Sets the kernel reset of the KRST0
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_setKernelResetZero(Ifx_ASCLIN *asclin);

/** \brief Sets the lead delay
 * \param asclin pointer to ASCLIN registers
 * \param delay lead delay selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setLeadDelay(Ifx_ASCLIN *asclin, IfxAsclin_LeadDelay delay);

/** \brief Sets the LIN break length
 * \param asclin pointer to ASCLIN registers
 * \param length value of the break length
 * \return None
 */
IFX_INLINE void IfxAsclin_setLinBreakLength(Ifx_ASCLIN *asclin, uint8 length);

/** \brief Sets the LIN header timeout
 * \param asclin pointer to ASCLIN registers
 * \param timeout value of the header timeout
 * \return None
 */
IFX_INLINE void IfxAsclin_setLinHeaderTimeout(Ifx_ASCLIN *asclin, uint8 timeout);

/** \brief Sets the LIN mode of operation
 * \param asclin pointer to ASCLIN registers
 * \param mode mode selection (slave / master)
 * \return None
 */
IFX_INLINE void IfxAsclin_setLinMode(Ifx_ASCLIN *asclin, IfxAsclin_LinMode mode);

/** \brief Sets the LIN response timeout mode
 * \param asclin pointer to ASCLIN registers
 * \param mode LIN response timeout mode selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setLinResponseTimeoutMode(Ifx_ASCLIN *asclin, IfxAsclin_LinResponseTimeoutMode mode);

/** \brief Sets the LIN response timeout threshold
 * \param asclin pointer to ASCLIN registers
 * \param threshold value of the tomeout threshold (1 to 256 bit times)
 * \return None
 */
IFX_INLINE void IfxAsclin_setLinResponseTimeoutThreshold(Ifx_ASCLIN *asclin, uint16 threshold);

/** \brief Sets the numerator of the fractional divider
 * \param asclin pointer to ASCLIN registers
 * \param numerator value of the numerator
 * \return None
 */
IFX_INLINE void IfxAsclin_setNumerator(Ifx_ASCLIN *asclin, uint16 numerator);

/** \brief Sets the division ratio of the baudrate post divider
 * \param asclin pointer to ASCLIN registers
 * \param ovsFactor value of oversampling factor
 * \return None
 */
IFX_INLINE void IfxAsclin_setOversampling(Ifx_ASCLIN *asclin, IfxAsclin_OversamplingFactor ovsFactor);

/** \brief Sets the parity type
 * \param asclin pointer to ASCLIN registers
 * \param type parity type selction (even / odd)
 * \return None
 */
IFX_INLINE void IfxAsclin_setParityType(Ifx_ASCLIN *asclin, IfxAsclin_ParityType type);

/** \brief Sets the division ratio of the predivider (prescaler)
 * \param asclin pointer to ASCLIN registers
 * \param prescaler value of prescaler
 * \return None
 */
IFX_INLINE void IfxAsclin_setPrescaler(Ifx_ASCLIN *asclin, uint16 prescaler);

/** \brief Sets the receive buffer mode
 * \param asclin pointer to ASCLIN registers
 * \param mode receive buffer mode selection (Rx FIFO or single stage Rx buffer)
 * \return None
 */
IFX_INLINE void IfxAsclin_setRxBufferMode(Ifx_ASCLIN *asclin, IfxAsclin_ReceiveBufferMode mode);

/** \brief Sets the Tx FIFO interrupt level
 * \param asclin pointer to ASCLIN registers
 * \param level interrupt level selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setRxFifoInterruptLevel(Ifx_ASCLIN *asclin, IfxAsclin_RxFifoInterruptLevel level);

/** \brief Sets the receive FIFO outlet width
 * \param asclin pointer to ASCLIN registers
 * \param width number of bytes read to the Rx FIFO with one FPI bus read
 * \return None
 */
IFX_INLINE void IfxAsclin_setRxFifoOutletWidth(Ifx_ASCLIN *asclin, IfxAsclin_RxFifoOutletWidth width);

/** \brief Sets the sample mode (number of samples per bit / median filter)
 * \param asclin pointer to ASCLIN registers
 * \param medianFilter value of the median filter
 * \return None
 */
IFX_INLINE void IfxAsclin_setSampleMode(Ifx_ASCLIN *asclin, IfxAsclin_SamplesPerBit medianFilter);

/** \brief Sets the sampling point position
 * \param asclin pointer to ASCLIN registers
 * \param spPosition sample point position selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setSamplePointPosition(Ifx_ASCLIN *asclin, IfxAsclin_SamplePointPosition spPosition);

/** \brief Sets the shift direction
 * \param asclin pointer to ASCLIN registers
 * \param dir shift direction selection (LSB / MSB first)
 * \return None
 */
IFX_INLINE void IfxAsclin_setShiftDirection(Ifx_ASCLIN *asclin, IfxAsclin_ShiftDirection dir);

/** \brief Sets the number of stop bits
 * \param asclin pointer to ASCLIN registers
 * \param stopBit number of stop bits selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setStopBit(Ifx_ASCLIN *asclin, IfxAsclin_StopBit stopBit);

/** \brief Sets the transmit header request flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_setTransmitHeaderRequestFlag(Ifx_ASCLIN *asclin);

/** \brief Sets the transmit response request flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_setTransmitResponseRequestFlag(Ifx_ASCLIN *asclin);

/** \brief Sets the transmission wake request flag
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_setTransmitWakeRequestFlag(Ifx_ASCLIN *asclin);

/** \brief Sets the transmit FIFO inlet width
 * \param asclin pointer to ASCLIN registers
 * \param width number of bytes written to the Tx FIFO with one FPI bus write
 * \return None
 */
IFX_INLINE void IfxAsclin_setTxFifoInletWidth(Ifx_ASCLIN *asclin, IfxAsclin_TxFifoInletWidth width);

/** \brief Sets the Tx FIFO interrupt level
 * \param asclin pointer to ASCLIN registers
 * \param level interrupt level selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setTxFifoInterruptLevel(Ifx_ASCLIN *asclin, IfxAsclin_TxFifoInterruptLevel level);

/** \} */

/** \addtogroup IfxLld_Asclin_Std_Utility
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the break detected flag status
 * \param asclin pointer to ASCLIN registers
 * \return break detected flag status
 */
IFX_INLINE boolean IfxAsclin_getBreakDetectedFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the clock source
 * \param asclin pointer to ASCLIN registers
 * \return clock source
 */
IFX_INLINE uint8 IfxAsclin_getClockSource(Ifx_ASCLIN *asclin);

/** \brief Returns the clock status
 * \param asclin pointer to ASCLIN registers
 * \return clock status (off / on)
 */
IFX_INLINE boolean IfxAsclin_getClockStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the collision detection error flag status
 * \param asclin pointer to ASCLIN registers
 * \return collision detection error flag status
 */
IFX_INLINE boolean IfxAsclin_getCollisionDetectionErrorFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns  the falling edge detected flag status
 * \param asclin pointer to ASCLIN registers
 * \return falling edge detected flag status
 */
IFX_INLINE boolean IfxAsclin_getFallingEdgeDetectedFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the frame error flag status
 * \param asclin pointer to ASCLIN registers
 * \return frame error flag status
 */
IFX_INLINE boolean IfxAsclin_getFrameErrorFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the header timeout flag status
 * \param asclin pointer to ASCLIN registers
 * \return header timeout flag status
 */
IFX_INLINE boolean IfxAsclin_getHeaderTimeoutFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the kernel reset status
 * \param asclin pointer to ASCLIN registers
 * \return kernel reset status
 */
IFX_INLINE boolean IfxAsclin_getKernelResetStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the LIN auto baudrate detection error flag status
 * \param asclin pointer to ASCLIN registers
 * \return LIN auto baudrate detection error flag status
 */
IFX_INLINE boolean IfxAsclin_getLinAutoBaudDetectionErrorFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the LIN checksum error flag status
 * \param asclin pointer to ASCLIN registers
 * \return LIN checksum error flag status
 */
IFX_INLINE boolean IfxAsclin_getLinChecksumErrorFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the LIN parity error flag status
 * \param asclin pointer to ASCLIN registers
 * \return LIN parity error flag status
 */
IFX_INLINE boolean IfxAsclin_getLinParityErrorFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the module status
 * \param asclin pointer to ASCLIN registers
 * \return module status (enabled / disabled)
 */
IFX_INLINE boolean IfxAsclin_getModuleStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the parity error flag status
 * \param asclin pointer to ASCLIN registers
 * \return parity error flag status
 */
IFX_INLINE boolean IfxAsclin_getParityErrorFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the division ratio of the predivider (prescaler)
 * \param asclin pointer to ASCLIN registers
 * \return value of prescaler
 */
IFX_INLINE uint16 IfxAsclin_getPrescaler(Ifx_ASCLIN *asclin);

/** \brief Returns the raising edge detected flag status
 * \param asclin pointer to ASCLIN registers
 * \return raising edge detected flag status
 */
IFX_INLINE boolean IfxAsclin_getRaisingEdgeDetectedFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the received signal status
 * \param asclin pointer to ASCLIN registers
 * \return received signal status (0 is low & 1 is high)
 */
IFX_INLINE boolean IfxAsclin_getReceiveSignalStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the response timeout flag status
 * \param asclin pointer to ASCLIN registers
 * \return response timeout flag status
 */
IFX_INLINE boolean IfxAsclin_getResponseTimeoutFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the current filling level of Tx FIFO
 * \param asclin pointer to ASCLIN registers
 * \return current filling level of Rx FIFO
 */
IFX_INLINE uint8 IfxAsclin_getRxFifoFillLevel(Ifx_ASCLIN *asclin);

/** \brief Returns the Rx FIFO level flag status
 * \param asclin pointer to ASCLIN registers
 * \return Rx FIFO level flag status
 */
IFX_INLINE boolean IfxAsclin_getRxFifoFillLevelFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the receive FIFO inlet width
 * \param asclin pointer to ASCLIN registers
 * \return number of bytes read to the Rx FIFO with one FPI bus read
 */
IFX_INLINE uint8 IfxAsclin_getRxFifoOutletWidth(Ifx_ASCLIN *asclin);

/** \brief Returns the Rx FIFO overflow flag status
 * \param asclin pointer to ASCLIN registers
 * \return Rx FIFO overflow flag status
 */
IFX_INLINE boolean IfxAsclin_getRxFifoOverflowFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the Rx FIFO underflow flag status
 * \param asclin pointer to ASCLIN registers
 * \return Rx FIFO underflow flag status
 */
IFX_INLINE boolean IfxAsclin_getRxFifoUnderflowFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the receive header end flag status
 * \param asclin pointer to ASCLIN registers
 * \return receive header end flag status
 */
IFX_INLINE boolean IfxAsclin_getRxHeaderEndFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the receive response end flag status
 * \param asclin pointer to ASCLIN registers
 * \return receive response end flag status
 */
IFX_INLINE boolean IfxAsclin_getRxResponseEndFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the transmission pending flag status
 * \param asclin pointer to ASCLIN registers
 * \return transmission pending flag status
 */
IFX_INLINE boolean IfxAsclin_getTransmissionCompletedFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the transmit signal status
 * \param asclin pointer to ASCLIN registers
 * \return transmit signal status (0 is low & 1 is high)
 */
IFX_INLINE boolean IfxAsclin_getTransmitSignalStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the current filling level of Tx FIFO
 * \param asclin pointer to ASCLIN registers
 * \return current filling level of Tx FIFO
 */
IFX_INLINE uint8 IfxAsclin_getTxFifoFillLevel(Ifx_ASCLIN *asclin);

/** \brief Returns the Tx FIFO level flag status
 * \param asclin pointer to ASCLIN registers
 * \return Tx FIFO level flag status
 */
IFX_INLINE boolean IfxAsclin_getTxFifoFillLevelFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the transmit FIFO inlet width
 * \param asclin pointer to ASCLIN registers
 * \return number of bytes written to the Tx FIFO with one FPI bus write
 */
IFX_INLINE uint8 IfxAsclin_getTxFifoInletWidth(Ifx_ASCLIN *asclin);

/** \brief Returns the Tx FIFO overflow flag status
 * \param asclin pointer to ASCLIN registers
 * \return Tx FIFO overflow flag status
 */
IFX_INLINE boolean IfxAsclin_getTxFifoOverflowFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the transmit header end flag status
 * \param asclin pointer to ASCLIN registers
 * \return transmit header end flag status
 */
IFX_INLINE boolean IfxAsclin_getTxHeaderEndFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Returns the transmit response end flag status
 * \param asclin pointer to ASCLIN registers
 * \return transmit response end flag status
 */
IFX_INLINE boolean IfxAsclin_getTxResponseEndFlagStatus(Ifx_ASCLIN *asclin);

/** \brief Sets the sensitivity of the module to sleep signal
 * \param asclin pointer to ASCLIN registers
 * \param mode mode selection (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_setSleepMode(Ifx_ASCLIN *asclin, IfxAsclin_SleepMode mode);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Returns the ASCLIN module FA frequency in Hz
 * \param asclin pointer to ASCLIN registers
 * \return Returns the ASCLIN module FA frequency in Hz
 */
IFX_EXTERN float32 IfxAsclin_getFaFrequency(Ifx_ASCLIN *asclin);

/** \brief Returns the OVS frequency
 * \param asclin pointer to ASCLIN registers
 * \return Returns the ASCLIN module OVS frequency in Hz
 */
IFX_EXTERN float32 IfxAsclin_getOvsFrequency(Ifx_ASCLIN *asclin);

/** \brief Returns the PD frequency
 * \param asclin pointer to ASCLIN registers
 * \return Returns the ASCLIN module PD frequency in Hz
 */
IFX_EXTERN float32 IfxAsclin_getPdFrequency(Ifx_ASCLIN *asclin);

/** \brief Returns the SHIFT frequency
 * \param asclin pointer to ASCLIN registers
 * \return Returns the ASCLIN module SHIFT frequency in Hz
 */
IFX_EXTERN float32 IfxAsclin_getShiftFrequency(Ifx_ASCLIN *asclin);

/** \brief resets Asclin kernel
 * \param asclin pointer to ASCLIN resgisters
 * \return None
 */
IFX_EXTERN void IfxAsclin_resetModule(Ifx_ASCLIN *asclin);

/** \} */

/** \addtogroup IfxLld_Asclin_Std_Configuration
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief disables all flags
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_INLINE void IfxAsclin_disableAllFlags(Ifx_ASCLIN *asclin);

/** \brief Enables/disables the auto baudrate detection
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableAutoBaudrateDetection(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables break detected flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableBreakDetectedFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables the collision detection
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable/disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableCollisionDetection(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables LIN collision detection error flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableCollisionDetectionErrorFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables CTS
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable/disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableCts(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables falling edge detected flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableFallingEdgeDetectedFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables frame error flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableFrameErrorFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables the hardware checksum
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableHardwareChecksum(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables header timeout flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableHeaderTimeoutFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables LIN auto baudrate detection error flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableLinAutoBaudDetectionErrorFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables LIN checksum error flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableLinChecksumErrorFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables LIN parity error flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableLinParityErrorFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables the loop back mode
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable/disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableLoopBackMode(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables the parity bit attachment to tha data bits
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable/disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableParity(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables parity error flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableParityErrorFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables raising edge detected flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableRaisingEdgeDetectedFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables response timeout flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableResponseTimeoutFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables Rx FIFO level flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableRxFifoFillLevelFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables the receive FIFO outlet to allow filling of Rx FIFO through shift register
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable/disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableRxFifoInlet(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables Rx FIFO overflow flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableRxFifoOverflowFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables Rx FIFO underflow flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableRxFifoUnderflowFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables receive header end flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableRxHeaderEndFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables receive response end flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableRxResponseEndFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables transmission completed flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableTransmissionCompletedFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables Tx FIFO level flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableTxFifoFillLevelFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables the transmit FIFO outlet to allow transmission
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable/disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableTxFifoOutlet(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables Tx FIFO overflow flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableTxFifoOverflowFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables transmit header end flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableTxHeaderEndFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Enables/disables transmit response end flag
 * \param asclin pointer to ASCLIN registers
 * \param enable choice (enable / disable)
 * \return None
 */
IFX_INLINE void IfxAsclin_enableTxResponseEndFlag(Ifx_ASCLIN *asclin, boolean enable);

/** \brief Selects the clock polarity
 * \param asclin pointer to ASCLIN registers
 * \param cpol CPOL selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setClockPolarity(Ifx_ASCLIN *asclin, IfxAsclin_ClockPolarity cpol);

/** \brief Selects the CTS input pin
 * \param asclin pointer to ASCLIN registers
 * \param ctsi CTS input pin selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setCtsInput(Ifx_ASCLIN *asclin, IfxAsclin_CtsInputSelect ctsi);

/** \brief Selects the module's mode of operation
 * \param asclin pointer to ASCLIN registers
 * \param mode mode selction
 * \return None
 */
IFX_INLINE void IfxAsclin_setFrameMode(Ifx_ASCLIN *asclin, IfxAsclin_FrameMode mode);

/** \brief Selects the RTS/CTS polarity
 * \param asclin pointer to ASCLIN registers
 * \param rcpol RCPOL selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setRtsCtsPolarity(Ifx_ASCLIN *asclin, IfxAsclin_RtsCtsPolarity rcpol);

/** \brief Selects the alternate input for Rx signal
 * \param asclin pointer to ASCLIN registers
 * \param alti alternate input selection of Rx signal
 * \return None
 */
IFX_INLINE void IfxAsclin_setRxInput(Ifx_ASCLIN *asclin, IfxAsclin_RxInputSelect alti);

/** \brief Selects the slave polarity
 * \param asclin pointer to ASCLIN registers
 * \param spol SPOL selection
 * \return None
 */
IFX_INLINE void IfxAsclin_setSlavePolarity(Ifx_ASCLIN *asclin, IfxAsclin_SlavePolarity spol);

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Set the bit timing
 *
 * \note this function required FA source and the PD frequency to be set to their final values
 * \param asclin pointer to ASCLIN registers
 * \param baudrate Required baudrate
 * \param oversampling The oversampling factor
 * \param samplepoint The sample point position
 * \param medianFilter Number of samples per bit (median filter)
 * \return TRUE if configuration was successfull
 */
IFX_EXTERN boolean IfxAsclin_setBitTiming(Ifx_ASCLIN *asclin, float32 baudrate, IfxAsclin_OversamplingFactor oversampling, IfxAsclin_SamplePointPosition samplepoint, IfxAsclin_SamplesPerBit medianFilter);

/** \} */

/** \addtogroup IfxLld_Asclin_Std_External
 * \{ */

/******************************************************************************/
/*-------------------------Global Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Enables the ASC error interrupt Flags
 * \param asclin pointer to ASCLIN registers
 * \param parEnable parity error
 * \param rfoEnable Rx FIFO overflow error
 * \return None
 */
IFX_EXTERN void IfxAsclin_enableAscErrorFlags(Ifx_ASCLIN *asclin, boolean parEnable, boolean rfoEnable);

/** \brief Enables the module
 * \param asclin pointer to ASCLIN registers
 * \return None
 */
IFX_EXTERN void IfxAsclin_enableModule(Ifx_ASCLIN *asclin);

/** \brief Returns the module Index
 * \param asclin pointer to ASCLIN registers
 * \return module index
 */
IFX_EXTERN sint32 IfxAsclin_getIndex(Ifx_ASCLIN *asclin);

/** \brief Returns the SRC pointer for ERR
 * \param asclin pointer to ASCLIN registers
 */
IFX_EXTERN volatile Ifx_SRC_SRCR *IfxAsclin_getSrcPointerEr(Ifx_ASCLIN *asclin);

/** \brief Returns the  SRC pointer for Rx
 * \param asclin pointer to ASCLIN registers
 */
IFX_EXTERN volatile Ifx_SRC_SRCR *IfxAsclin_getSrcPointerRx(Ifx_ASCLIN *asclin);

/** \brief Returns the SRC pointer for Tx
 * \param asclin pointer to ASCLIN registers
 */
IFX_EXTERN volatile Ifx_SRC_SRCR *IfxAsclin_getSrcPointerTx(Ifx_ASCLIN *asclin);

/** \brief Reads data up to 16 bits
 * \param asclin pointer to ASCLIN registers
 * \param data Array where the read data shall be stored
 * \param count number of items to read
 * \return number of items that could not be read
 */
IFX_EXTERN uint32 IfxAsclin_read16(Ifx_ASCLIN *asclin, uint16 *data, uint32 count);

/** \brief Reads data up to 32 bits
 * \param asclin pointer to ASCLIN registers
 * \param data Array where the read data shall be stored
 * \param count number of items to read
 * \return number of items that could not be read
 */
IFX_EXTERN uint32 IfxAsclin_read32(Ifx_ASCLIN *asclin, uint32 *data, uint32 count);

/** \brief Reads data up to 8 bits
 * \param asclin pointer to ASCLIN registers
 * \param data Array where the read data shall be stored
 * \param count number of items to read
 * \return number of items that could not be read
 */
IFX_EXTERN uint32 IfxAsclin_read8(Ifx_ASCLIN *asclin, uint8 *data, uint32 count);

/** \brief Sets the baudrate bit fields
 * \param asclin pointer to ASCLIN registers
 * \param prescaler division ratio of the predivider
 * \param numerator numerator of the fractional divider
 * \param denominator denominator of the fractional divider
 * \param oversampling division ratio of the baudrate postdivider
 * \return None
 */
IFX_EXTERN void IfxAsclin_setBaudrateBitFields(Ifx_ASCLIN *asclin, uint16 prescaler, uint16 numerator, uint16 denominator, IfxAsclin_OversamplingFactor oversampling);

/** \brief Sets the  clock source
 * \param asclin pointer to ASCLIN registers
 * \param clockSource clock source selection
 * \return None
 */
IFX_EXTERN void IfxAsclin_setClockSource(Ifx_ASCLIN *asclin, IfxAsclin_ClockSource clockSource);

/** \brief Writes data up to 16 bits
 * \param asclin pointer to ASCLIN registers
 * \param data Array of data to be send
 * \param count number of items to be send
 * \return the number of items that could not be send
 */
IFX_EXTERN uint32 IfxAsclin_write16(Ifx_ASCLIN *asclin, uint16 *data, uint32 count);

/** \brief Writes data up to 32 bits
 * \param asclin pointer to ASCLIN registers
 * \param data Array of data to be send
 * \param count number of items to be send
 * \return the number of items that could not be send
 */
IFX_EXTERN uint32 IfxAsclin_write32(Ifx_ASCLIN *asclin, uint32 *data, uint32 count);

/** \brief Writes data up to 8 bits
 * \param asclin pointer to ASCLIN registers
 * \param data Array of data to be send
 * \param count number of items to be send
 * \return the number of items that could not be send
 */
IFX_EXTERN uint32 IfxAsclin_write8(Ifx_ASCLIN *asclin, uint8 *data, uint32 count);

/** \} */

/** \addtogroup IfxLld_Asclin_Std_IO
 * \{ */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/** \brief Initializes a CTS input
 *
 * Attention: the kernel clock has to be disabled whenever the input multiplexer is changed.
 *
 * This can be done with IfxAsclin_setClockSource(asclinSFR, IfxAsclin_ClockSource_noClock);
 *
 * After the function call, the current clock can be enabled again.
 * \param cts the CTS Pin which should be configured
 * \param inputMode the pin input mode which should be configured
 * \return None
 */
IFX_INLINE void IfxAsclin_initCtsPin(const IfxAsclin_Cts_In *cts, IfxPort_InputMode inputMode);

/** \brief Initializes a RTS output
 * \param rts the RTS Pin which should be configured
 * \param outputMode the pin output mode which should be configured
 * \param padDriver the pad driver mode which should be configured
 * \return None
 */
IFX_INLINE void IfxAsclin_initRtsPin(const IfxAsclin_Rts_Out *rts, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver);

/** \brief Initializes a RX input
 *
 * Attention: the kernel clock has to be disabled whenever the input multiplexer is changed.
 *
 * This can be done with IfxAsclin_setClockSource(asclinSFR, IfxAsclin_ClockSource_noClock);
 *
 * After the function call, the current clock can be enabled again.
 * \param rx the RX Pin which should be configured
 * \param inputMode the pin input mode which should be configured
 * \return None
 */
IFX_INLINE void IfxAsclin_initRxPin(const IfxAsclin_Rx_In *rx, IfxPort_InputMode inputMode);

/** \brief Initializes a SCLK output
 * \param sclk the SCLK Pin which should be configured
 * \param outputMode the pin output mode which should be configured
 * \param padDriver the pad driver mode which should be configured
 * \return None
 */
IFX_INLINE void IfxAsclin_initSclkPin(const IfxAsclin_Sclk_Out *sclk, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver);

/** \brief Initializes a SLSO output
 * \param slso the SLSO Pin which should be configured
 * \param outputMode the pin output mode which should be configured
 * \param padDriver the pad driver mode which should be configured
 * \return None
 */
IFX_INLINE void IfxAsclin_initSlsoPin(const IfxAsclin_Slso_Out *slso, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver);

/** \brief Initializes a TX output
 * \param tx the TX Pin which should be configured
 * \param outputMode the pin output mode which should be configured
 * \param padDriver the pad driver mode which should be configured
 * \return None
 */
IFX_INLINE void IfxAsclin_initTxPin(const IfxAsclin_Tx_Out *tx, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver);

/** \} */

/******************************************************************************/
/*-------------------------Inline Function Prototypes-------------------------*/
/******************************************************************************/

/**
 * \param asclin pointer to ASCLIN registers
 * \return returns the value of RxDATA register
 */
IFX_INLINE uint32 IfxAsclin_readRxData(Ifx_ASCLIN *asclin);

/**
 * \param asclin pointer to ASCLIN registers
 * \param data data to be written
 * \return None
 */
IFX_INLINE void IfxAsclin_writeTxData(Ifx_ASCLIN *asclin, uint32 data);

/******************************************************************************/
/*---------------------Inline Function Implementations------------------------*/
/******************************************************************************/

IFX_INLINE void IfxAsclin_clearAllFlags(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.U = 0xFFFFFFFF;
}


IFX_INLINE void IfxAsclin_clearBreakDetectedFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.BDC = 1;
}


IFX_INLINE void IfxAsclin_clearCollisionDetectionErrorFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.CEC = 1;
}


IFX_INLINE void IfxAsclin_clearFallingEdgeDetectedFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.FEDC = 1;
}


IFX_INLINE void IfxAsclin_clearFrameErrorFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.FEC = 1;
}


IFX_INLINE void IfxAsclin_clearHeaderTimeoutFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.HTC = 1;
}


IFX_INLINE void IfxAsclin_clearKernelResetStatus(Ifx_ASCLIN *asclin)
{
    asclin->KRSTCLR.B.CLR = 1;
}


IFX_INLINE void IfxAsclin_clearLinAutoBaudDetectionErrorFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.LAC = 1;
}


IFX_INLINE void IfxAsclin_clearLinChecksumErrorFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.LCC = 1;
}


IFX_INLINE void IfxAsclin_clearLinParityErrorFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.LPC = 1;
}


IFX_INLINE void IfxAsclin_clearParityErrorFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.PEC = 1;
}


IFX_INLINE void IfxAsclin_clearRaisingEdgeDetectedFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.REDC = 1;
}


IFX_INLINE void IfxAsclin_clearResponseTimeoutFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.RTC = 1;
}


IFX_INLINE void IfxAsclin_clearRxFifoFillLevelFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.RFLC = 1;
}


IFX_INLINE void IfxAsclin_clearRxFifoOverflowFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.RFOC = 1;
}


IFX_INLINE void IfxAsclin_clearRxFifoUnderflowFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.RFUC = 1;
}


IFX_INLINE void IfxAsclin_clearRxHeaderEndFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.RHC = 1;
}


IFX_INLINE void IfxAsclin_clearRxResponseEndFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.RRC = 1;
}


IFX_INLINE void IfxAsclin_clearTransmissionCompletedFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.TCC = 1;
}


IFX_INLINE void IfxAsclin_clearTxFifoFillLevelFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.TFLC = 1;
}


IFX_INLINE void IfxAsclin_clearTxFifoOverflowFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.TFOC = 1;
}


IFX_INLINE void IfxAsclin_clearTxHeaderEndFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.THC = 1;
}


IFX_INLINE void IfxAsclin_clearTxResponseEndFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSCLEAR.B.TRC = 1;
}


IFX_INLINE void IfxAsclin_disableAllFlags(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSENABLE.U = 0x00000000;
}


IFX_INLINE void IfxAsclin_enableAutoBaudrateDetection(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->LIN.CON.B.ABD = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableBreakDetectedFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.BDE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableCollisionDetection(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FRAMECON.B.CEN = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableCollisionDetectionErrorFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.CEE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableCts(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->IOCR.B.CTSEN = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableFallingEdgeDetectedFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.FEDE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableFrameErrorFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.FEE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableHardwareChecksum(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->LIN.CON.B.CSEN = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableHeaderTimeoutFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.HTE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableLinAutoBaudDetectionErrorFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.ABE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableLinChecksumErrorFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.LCE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableLinParityErrorFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.LPE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableLoopBackMode(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->IOCR.B.LB = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableParity(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FRAMECON.B.PEN = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableParityErrorFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.PEE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableRaisingEdgeDetectedFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.REDE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableResponseTimeoutFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.RTE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableRxFifoFillLevelFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.RFLE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableRxFifoInlet(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->RXFIFOCON.B.ENI = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableRxFifoOverflowFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.RFOE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableRxFifoUnderflowFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.RFUE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableRxHeaderEndFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.RHE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableRxResponseEndFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.RRE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableTransmissionCompletedFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.TCE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableTxFifoFillLevelFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.TFLE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableTxFifoOutlet(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->TXFIFOCON.B.ENO = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableTxFifoOverflowFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.TFOE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableTxHeaderEndFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.THE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_enableTxResponseEndFlag(Ifx_ASCLIN *asclin, boolean enable)
{
    asclin->FLAGSENABLE.B.TRE = enable ? 1 : 0;
}


IFX_INLINE void IfxAsclin_flushRxFifo(Ifx_ASCLIN *asclin)
{
    asclin->RXFIFOCON.B.FLUSH = 1;
}


IFX_INLINE void IfxAsclin_flushTxFifo(Ifx_ASCLIN *asclin)
{
    asclin->TXFIFOCON.B.FLUSH = 1;
}


IFX_INLINE boolean IfxAsclin_getBreakDetectedFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.BD;
}


IFX_INLINE uint8 IfxAsclin_getClockSource(Ifx_ASCLIN *asclin)
{
    return asclin->CSR.B.CLKSEL;
}


IFX_INLINE boolean IfxAsclin_getClockStatus(Ifx_ASCLIN *asclin)
{
    return asclin->CSR.B.CON;
}


IFX_INLINE boolean IfxAsclin_getCollisionDetectionErrorFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.CE;
}


IFX_INLINE boolean IfxAsclin_getFallingEdgeDetectedFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.FED;
}


IFX_INLINE boolean IfxAsclin_getFrameErrorFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.FE;
}


IFX_INLINE boolean IfxAsclin_getHeaderTimeoutFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.HT;
}


IFX_INLINE boolean IfxAsclin_getKernelResetStatus(Ifx_ASCLIN *asclin)
{
    return asclin->KRST0.B.RSTSTAT;
}


IFX_INLINE boolean IfxAsclin_getLinAutoBaudDetectionErrorFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.LA;
}


IFX_INLINE boolean IfxAsclin_getLinChecksumErrorFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.LC;
}


IFX_INLINE boolean IfxAsclin_getLinParityErrorFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.LP;
}


IFX_INLINE boolean IfxAsclin_getModuleStatus(Ifx_ASCLIN *asclin)
{
    return asclin->CLC.B.DISS;
}


IFX_INLINE boolean IfxAsclin_getParityErrorFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.PE;
}


IFX_INLINE uint16 IfxAsclin_getPrescaler(Ifx_ASCLIN *asclin)
{
    return asclin->BITCON.B.PRESCALER + 1;
}


IFX_INLINE boolean IfxAsclin_getRaisingEdgeDetectedFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.RED;
}


IFX_INLINE boolean IfxAsclin_getReceiveSignalStatus(Ifx_ASCLIN *asclin)
{
    return asclin->IOCR.B.RXM;
}


IFX_INLINE boolean IfxAsclin_getResponseTimeoutFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.RT;
}


IFX_INLINE uint8 IfxAsclin_getRxFifoFillLevel(Ifx_ASCLIN *asclin)
{
    return asclin->RXFIFOCON.B.FILL;
}


IFX_INLINE boolean IfxAsclin_getRxFifoFillLevelFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.RFL;
}


IFX_INLINE uint8 IfxAsclin_getRxFifoOutletWidth(Ifx_ASCLIN *asclin)
{
    return asclin->RXFIFOCON.B.OUTW;
}


IFX_INLINE boolean IfxAsclin_getRxFifoOverflowFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.RFO;
}


IFX_INLINE boolean IfxAsclin_getRxFifoUnderflowFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.RFU;
}


IFX_INLINE boolean IfxAsclin_getRxHeaderEndFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.RH;
}


IFX_INLINE boolean IfxAsclin_getRxResponseEndFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.RR;
}


IFX_INLINE boolean IfxAsclin_getTransmissionCompletedFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.TC;
}


IFX_INLINE boolean IfxAsclin_getTransmitSignalStatus(Ifx_ASCLIN *asclin)
{
    return asclin->IOCR.B.TXM;
}


IFX_INLINE uint8 IfxAsclin_getTxFifoFillLevel(Ifx_ASCLIN *asclin)
{
    return asclin->TXFIFOCON.B.FILL;
}


IFX_INLINE boolean IfxAsclin_getTxFifoFillLevelFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.TFL;
}


IFX_INLINE uint8 IfxAsclin_getTxFifoInletWidth(Ifx_ASCLIN *asclin)
{
    return asclin->TXFIFOCON.B.INW;
}


IFX_INLINE boolean IfxAsclin_getTxFifoOverflowFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.TFO;
}


IFX_INLINE boolean IfxAsclin_getTxHeaderEndFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.TH;
}


IFX_INLINE boolean IfxAsclin_getTxResponseEndFlagStatus(Ifx_ASCLIN *asclin)
{
    return asclin->FLAGS.B.TR;
}


IFX_INLINE void IfxAsclin_initCtsPin(const IfxAsclin_Cts_In *cts, IfxPort_InputMode inputMode)
{
    IfxPort_setPinModeInput(cts->pin.port, cts->pin.pinIndex, inputMode);
    IfxAsclin_enableCts(cts->module, TRUE);
    IfxAsclin_setCtsInput(cts->module, (IfxAsclin_CtsInputSelect)cts->select);
}


IFX_INLINE void IfxAsclin_initRtsPin(const IfxAsclin_Rts_Out *rts, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeOutput(rts->pin.port, rts->pin.pinIndex, outputMode, rts->select);
    IfxPort_setPinPadDriver(rts->pin.port, rts->pin.pinIndex, padDriver);
}


IFX_INLINE void IfxAsclin_initRxPin(const IfxAsclin_Rx_In *rx, IfxPort_InputMode inputMode)
{
    IfxPort_setPinModeInput(rx->pin.port, rx->pin.pinIndex, inputMode);
    IfxAsclin_setRxInput(rx->module, (IfxAsclin_RxInputSelect)rx->select);
}


IFX_INLINE void IfxAsclin_initSclkPin(const IfxAsclin_Sclk_Out *sclk, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeOutput(sclk->pin.port, sclk->pin.pinIndex, outputMode, sclk->select);
    IfxPort_setPinPadDriver(sclk->pin.port, sclk->pin.pinIndex, padDriver);
}


IFX_INLINE void IfxAsclin_initSlsoPin(const IfxAsclin_Slso_Out *slso, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeOutput(slso->pin.port, slso->pin.pinIndex, outputMode, slso->select);
    IfxPort_setPinPadDriver(slso->pin.port, slso->pin.pinIndex, padDriver);
}


IFX_INLINE void IfxAsclin_initTxPin(const IfxAsclin_Tx_Out *tx, IfxPort_OutputMode outputMode, IfxPort_PadDriver padDriver)
{
    IfxPort_setPinModeOutput(tx->pin.port, tx->pin.pinIndex, outputMode, tx->select);
    IfxPort_setPinPadDriver(tx->pin.port, tx->pin.pinIndex, padDriver);
}


IFX_INLINE uint32 IfxAsclin_readRxData(Ifx_ASCLIN *asclin)
{
    return asclin->RXDATA.U;
}


IFX_INLINE void IfxAsclin_setBrdLowerlimt(Ifx_ASCLIN *asclin, uint8 limit)
{
    asclin->BRD.B.LOWERLIMIT = limit;
}


IFX_INLINE void IfxAsclin_setBrdUpperlimt(Ifx_ASCLIN *asclin, uint8 limit)
{
    asclin->BRD.B.UPPERLIMIT = limit;
}


IFX_INLINE void IfxAsclin_setChecksumInjection(Ifx_ASCLIN *asclin, IfxAsclin_ChecksumInjection csi)
{
    asclin->LIN.CON.B.CSI = csi;
}


IFX_INLINE void IfxAsclin_setChecksumMode(Ifx_ASCLIN *asclin, IfxAsclin_Checksum mode)
{
    asclin->DATCON.B.CSM = mode;
}


IFX_INLINE void IfxAsclin_setClockPolarity(Ifx_ASCLIN *asclin, IfxAsclin_ClockPolarity cpol)
{
    asclin->IOCR.B.CPOL = cpol;
}


IFX_INLINE void IfxAsclin_setCtsInput(Ifx_ASCLIN *asclin, IfxAsclin_CtsInputSelect ctsi)
{
    asclin->IOCR.B.CTS = ctsi;
}


IFX_INLINE void IfxAsclin_setDataLength(Ifx_ASCLIN *asclin, IfxAsclin_DataLength length)
{
    asclin->DATCON.B.DATLEN = length;
}


IFX_INLINE void IfxAsclin_setDenominator(Ifx_ASCLIN *asclin, uint16 denominator)
{
    asclin->BRG.B.DENOMINATOR = denominator;
}


IFX_INLINE void IfxAsclin_setDisableModuleRequest(Ifx_ASCLIN *asclin)
{
    asclin->CLC.B.DISR = 1;
}


IFX_INLINE void IfxAsclin_setEnableModuleRequest(Ifx_ASCLIN *asclin)
{
    asclin->CLC.B.DISR = 0;
}


IFX_INLINE void IfxAsclin_setFilterDepth(Ifx_ASCLIN *asclin, uint8 depth)
{
    asclin->IOCR.B.DEPTH = __minu(depth, 63);
}


IFX_INLINE void IfxAsclin_setFrameMode(Ifx_ASCLIN *asclin, IfxAsclin_FrameMode mode)
{
    asclin->FRAMECON.B.MODE = mode;
}


IFX_INLINE void IfxAsclin_setHeaderResponseSelect(Ifx_ASCLIN *asclin, IfxAsclin_HeaderResponseSelect type)
{
    asclin->DATCON.B.HO = type;
}


IFX_INLINE void IfxAsclin_setIdleDelay(Ifx_ASCLIN *asclin, IfxAsclin_IdleDelay delay)
{
    asclin->FRAMECON.B.IDLE = delay;
}


IFX_INLINE void IfxAsclin_setKernelResetOne(Ifx_ASCLIN *asclin)
{
    asclin->KRST1.B.RST = 1;
}


IFX_INLINE void IfxAsclin_setKernelResetZero(Ifx_ASCLIN *asclin)
{
    asclin->KRST0.B.RST = 1;
}


IFX_INLINE void IfxAsclin_setLeadDelay(Ifx_ASCLIN *asclin, IfxAsclin_LeadDelay delay)
{
    asclin->FRAMECON.B.LEAD = delay;
}


IFX_INLINE void IfxAsclin_setLinBreakLength(Ifx_ASCLIN *asclin, uint8 length)
{
    asclin->LIN.BTIMER.B.BREAK = length;
}


IFX_INLINE void IfxAsclin_setLinHeaderTimeout(Ifx_ASCLIN *asclin, uint8 timeout)
{
    asclin->LIN.HTIMER.B.HEADER = timeout;
}


IFX_INLINE void IfxAsclin_setLinMode(Ifx_ASCLIN *asclin, IfxAsclin_LinMode mode)
{
    asclin->LIN.CON.B.MS = mode;
}


IFX_INLINE void IfxAsclin_setLinResponseTimeoutMode(Ifx_ASCLIN *asclin, IfxAsclin_LinResponseTimeoutMode mode)
{
    asclin->DATCON.B.RM = mode;
}


IFX_INLINE void IfxAsclin_setLinResponseTimeoutThreshold(Ifx_ASCLIN *asclin, uint16 threshold)
{
    asclin->DATCON.B.RESPONSE = __minu(threshold, 256);
}


IFX_INLINE void IfxAsclin_setNumerator(Ifx_ASCLIN *asclin, uint16 numerator)
{
    asclin->BRG.B.NUMERATOR = numerator;
}


IFX_INLINE void IfxAsclin_setOversampling(Ifx_ASCLIN *asclin, IfxAsclin_OversamplingFactor ovsFactor)
{
    asclin->BITCON.B.OVERSAMPLING = ovsFactor;
}


IFX_INLINE void IfxAsclin_setParityType(Ifx_ASCLIN *asclin, IfxAsclin_ParityType type)
{
    asclin->FRAMECON.B.ODD = type;
}


IFX_INLINE void IfxAsclin_setPrescaler(Ifx_ASCLIN *asclin, uint16 prescaler)
{
    asclin->BITCON.B.PRESCALER = prescaler - 1;
}


IFX_INLINE void IfxAsclin_setRtsCtsPolarity(Ifx_ASCLIN *asclin, IfxAsclin_RtsCtsPolarity rcpol)
{
    asclin->IOCR.B.RCPOL = rcpol;
}


IFX_INLINE void IfxAsclin_setRxBufferMode(Ifx_ASCLIN *asclin, IfxAsclin_ReceiveBufferMode mode)
{
    asclin->RXFIFOCON.B.BUF = mode;
}


IFX_INLINE void IfxAsclin_setRxFifoInterruptLevel(Ifx_ASCLIN *asclin, IfxAsclin_RxFifoInterruptLevel level)
{
    asclin->RXFIFOCON.B.INTLEVEL = __minu(level, 15);
}


IFX_INLINE void IfxAsclin_setRxFifoOutletWidth(Ifx_ASCLIN *asclin, IfxAsclin_RxFifoOutletWidth width)
{
    asclin->RXFIFOCON.B.OUTW = width;
}


IFX_INLINE void IfxAsclin_setRxInput(Ifx_ASCLIN *asclin, IfxAsclin_RxInputSelect alti)
{
    asclin->IOCR.B.ALTI = alti;
}


IFX_INLINE void IfxAsclin_setSampleMode(Ifx_ASCLIN *asclin, IfxAsclin_SamplesPerBit medianFilter)
{
    asclin->BITCON.B.SM = medianFilter;
}


IFX_INLINE void IfxAsclin_setSamplePointPosition(Ifx_ASCLIN *asclin, IfxAsclin_SamplePointPosition spPosition)
{
    asclin->BITCON.B.SAMPLEPOINT = __minu(spPosition, asclin->BITCON.B.OVERSAMPLING);
}


IFX_INLINE void IfxAsclin_setShiftDirection(Ifx_ASCLIN *asclin, IfxAsclin_ShiftDirection dir)
{
    asclin->FRAMECON.B.MSB = dir;
}


IFX_INLINE void IfxAsclin_setSlavePolarity(Ifx_ASCLIN *asclin, IfxAsclin_SlavePolarity spol)
{
    asclin->IOCR.B.SPOL = spol;
}


IFX_INLINE void IfxAsclin_setSleepMode(Ifx_ASCLIN *asclin, IfxAsclin_SleepMode mode)
{
    uint16 passwd = IfxScuWdt_getCpuWatchdogPassword();
    IfxScuWdt_clearCpuEndinit(passwd);
    asclin->CLC.B.EDIS = mode;
    IfxScuWdt_setCpuEndinit(passwd);
}


IFX_INLINE void IfxAsclin_setStopBit(Ifx_ASCLIN *asclin, IfxAsclin_StopBit stopBit)
{
    asclin->FRAMECON.B.STOP = stopBit;
}


IFX_INLINE void IfxAsclin_setTransmitHeaderRequestFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSSET.B.THRQS = 1;
}


IFX_INLINE void IfxAsclin_setTransmitResponseRequestFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSSET.B.TRRQS = 1;
}


IFX_INLINE void IfxAsclin_setTransmitWakeRequestFlag(Ifx_ASCLIN *asclin)
{
    asclin->FLAGSSET.B.TWRQS = 1;
}


IFX_INLINE void IfxAsclin_setTxFifoInletWidth(Ifx_ASCLIN *asclin, IfxAsclin_TxFifoInletWidth width)
{
    asclin->TXFIFOCON.B.INW = width;
}


IFX_INLINE void IfxAsclin_setTxFifoInterruptLevel(Ifx_ASCLIN *asclin, IfxAsclin_TxFifoInterruptLevel level)
{
    asclin->TXFIFOCON.B.INTLEVEL = __minu(level, 15);
}


IFX_INLINE void IfxAsclin_writeTxData(Ifx_ASCLIN *asclin, uint32 data)
{
    asclin->TXDATA.U = data;
}


#endif /* IFXASCLIN_H */
