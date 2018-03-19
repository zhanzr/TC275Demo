/**
 * \file CpuPerfCounterDemo.c
 * \brief Cpu Performance counter Demo
 *
 * \version iLLD_Demos_1_0_1_3_0
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
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
 */

/******************************************************************************/
/*----------------------------------Includes----------------------------------*/
/******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <Cpu/Std/IfxCpu.h>
#include <IfxCbs_reg.h>
#include "CpuPerfCounterDemo.h"

/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*--------------------------------Enumerations--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-----------------------------Data Structures--------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*------------------------------Global variables------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Prototypes--------------------------------*/
/******************************************************************************/
static void TestCodeForPerfomanceEval(void);

/******************************************************************************/
/*------------------------Private Variables/Constants-------------------------*/
/******************************************************************************/

/******************************************************************************/
/*-------------------------Function Implementations---------------------------*/
/******************************************************************************/
/** \brief Sample code for performance evaluation
 *
 */
#define size    8190

bool flags[size + 1];

void TestFunct1(void)
{
    int     i,prime,k,count,iter;

    uint32_t Begin_Time = (uint32_t) schd_GetTick();

    for( iter = 1; iter <= 10; iter++ )     /* do program 10 times  */
    {
            count = 0;                      /* prime counter        */
            for( i = 0; i <= size; i++ )    /* set all flags true   */
                    flags[i] = true;
            for( i = 0; i <= size; i++ )
            {
                    if( flags[i] )          /* found a prime        */
                    {
                            prime = i + i + 3; /* twice index + 3   */
                            for(k=i+prime; k<=size; k+=prime)
                                    flags[k] = false;
                                            /* kill all multiples   */
                            count++;        /* primes found         */
                    }
            }
    }

    uint32_t End_Time = (unsigned int) schd_GetTick();

    printf("%s-%u\n", __func__, (End_Time - Begin_Time));
}

/** \brief Sample code for performance evaluation
 *
 */
void TestFunct2(void)
{
    int     i,prime,k,count,iter;

    uint32_t Begin_Time = (uint32_t) schd_GetTick();

    for( iter = 1; iter <= 20; iter++ )     /* do program 10 times  */
    {
            count = 0;                      /* prime counter        */
            for( i = 0; i <= size; i++ )    /* set all flags true   */
                    flags[i] = true;
            for( i = 0; i <= size; i++ )
            {
                    if( flags[i] )          /* found a prime        */
                    {
                            prime = i + i + 3; /* twice index + 3   */
                            for(k=i+prime; k<=size; k+=prime)
                                    flags[k] = false;
                                            /* kill all multiples   */
                            count++;        /* primes found         */
                    }
            }
    }

    uint32_t End_Time = (unsigned int) schd_GetTick();

    printf("%s-%u\n", __func__, (End_Time - Begin_Time));
}

/** \brief Demo init API
 *
 * This function is called from main during initialization phase
 */
void IfxCpuPerfCounterDemo_init(void)
{
    printf("%s called\n", __func__);

    /* Enable OCDS */
    CBS_OEC.U = 0xa1;
    CBS_OEC.U = 0x5e;
    CBS_OEC.U = 0xa1;
    CBS_OEC.U = 0x5e;
}


/** \brief Demo run API
 *
 * This function is called from main, background loop
 */
void IfxCpuPerfCounterDemo_run(void)
{
    IfxCpu_Perf perfCounts = {0};
    uint32_t tmpTick;

    tmpTick = schd_GetTick();
    /* With Normal mode, the counters increment on their respective triggers */
    IfxCpu_resetAndStartCounters(IfxCpu_CounterMode_normal);

    /* Call the code for which the performance is to be measured*/
    TestFunct1();

    /* Now, perfCounts will have all the counter + stick bit status */
    perfCounts = IfxCpu_stopCounters();
    tmpTick = schd_GetTick() - tmpTick;

    printf("TestFunct1[%u], Performance counters: %u, %u, %u, %u, %u\n",
    		tmpTick,
			perfCounts.instruction,
    		perfCounts.clock,
			perfCounts.counter1,
			perfCounts.counter2,
			perfCounts.counter3);

    memset((void*)&perfCounts, 0, sizeof(perfCounts));

    tmpTick = schd_GetTick();
    /* With Normal mode, the counters increment on their respective triggers */
    IfxCpu_resetAndStartCounters(IfxCpu_CounterMode_normal);

    /* Call the code for which the performance is to be measured*/
    TestFunct2();

    /* Now, perfCounts will have all the counter + stick bit status */
    perfCounts = IfxCpu_stopCounters();
    tmpTick = schd_GetTick() - tmpTick;

    printf("TestFunct2[%u], Performance counters: %u, %u, %u, %u, %u\n",
    		tmpTick,
			perfCounts.instruction,
    		perfCounts.clock,
			perfCounts.counter1,
			perfCounts.counter2,
			perfCounts.counter3);
}
