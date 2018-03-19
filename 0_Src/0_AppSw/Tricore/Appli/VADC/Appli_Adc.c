#include <Ifx_TypesReg.h>
#include "Appli_Adc.h"
#include "Tricore\Appli\Entry\Appli_Entry.h"

#define	QUEUE_TEST_CH_N	4

int adc_count;

// VADC handle
IfxVadc_Adc vadc;
IfxVadc_Adc_Config adcConfig;
IfxVadc_Adc_Group adcGroup;
// create group config
IfxVadc_Adc_GroupConfig adcGroupConfig;

Ifx_VADC_RES resultTrace[QUEUE_TEST_CH_N*10];
Ifx_VADC_RES conversionResult;
// create channel config
IfxVadc_Adc_ChannelConfig adcChannelConfig[QUEUE_TEST_CH_N];
IfxVadc_Adc_Channel adcChannel[QUEUE_TEST_CH_N];

extern int demo_item;

void Appli_AdcModule_Init(void)
{
	// create configuration
	IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

	// initialize module
	IfxVadc_Adc_initModule(&vadc, &adcConfig);

	IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &vadc);

	adcGroupConfig.groupId = IfxVadc_GroupId_0;
	// IMPORTANT: usually we use the same group as master!
	adcGroupConfig.master = adcGroupConfig.groupId;
	// enable all arbiter request sources
	adcGroupConfig.arbiter.requestSlotQueueEnabled          = TRUE;
	// enable Queue mode
	adcGroupConfig.arbiter.requestSlotScanEnabled           = TRUE;
	// enable Scan mode
	adcGroupConfig.arbiter.requestSlotBackgroundScanEnabled = TRUE;

	// enable Background scan// enable all gates in "always" mode (no edge detection)
	adcGroupConfig.queueRequest.triggerConfig.gatingMode          = IfxVadc_GatingMode_always;
	adcGroupConfig.scanRequest.triggerConfig.gatingMode           = IfxVadc_GatingMode_always;
	adcGroupConfig.backgroundScanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_always;

	// initialize the group
	IfxVadc_Adc_initGroup(&adcGroup, &adcGroupConfig);
}

void Appli_AdcQueuedInit(void)
{
	int chnIx;

	// IMPORTANT: for deterministic results we have to disable the queue gate
	// while filling the queue, otherwise results could be output in the wrong order
	unsigned int savedGate = adcGroup.module.vadc->G[adcGroup.groupId].QMR0.B.ENGT;
	adcGroup.module.vadc->G[adcGroup.groupId].QMR0.B.ENGT = 0;

	for(chnIx = 0 ; chnIx < QUEUE_TEST_CH_N; ++chnIx)
	{
		IfxVadc_Adc_initChannelConfig(&adcChannelConfig[chnIx], &adcGroup);
		adcChannelConfig[chnIx].channelId = (IfxVadc_ChannelId)(chnIx);
		adcChannelConfig[chnIx].resultRegister = IfxVadc_ChannelResult_1;

		// use result register #1 for all channels
		// initialize the channel
		IfxVadc_Adc_initChannel(&adcChannel[chnIx], &adcChannelConfig[chnIx]);
		// Add channel to queue with refill enabled
		IfxVadc_Adc_addToQueue(&adcChannel[chnIx], IFXVADC_QUEUE_REFILL);
	}

	// restore previous gate config
	adcGroup.module.vadc->G[adcGroup.groupId].QMR0.B.ENGT = savedGate;

}


void Appli_AdcScanInit(void)
{
	int chnIx;

	// now with group 0
	adcGroupConfig.groupId = IfxVadc_GroupId_7;
	adcGroupConfig.master = adcGroupConfig.groupId;
	adcGroupConfig.backgroundScanRequest.autoBackgroundScanEnabled = TRUE;
	adcGroupConfig.backgroundScanRequest.triggerConfig.gatingMode  = IfxVadc_GatingMode_always;
	adcGroupConfig.arbiter.requestSlotBackgroundScanEnabled        = TRUE;

	// initialize the group//IfxVadc_Adc_Group adcGroup; // no need to create a new one
	IfxVadc_Adc_initGroup(&adcGroup, &adcGroupConfig);

	{
		// create channel config
		for(chnIx = 0; chnIx < QUEUE_TEST_CH_N; ++chnIx)
		{
			IfxVadc_Adc_initChannelConfig(&adcChannelConfig[chnIx], &adcGroup);
			adcChannelConfig[chnIx].channelId = (IfxVadc_ChannelId)(chnIx);
			adcChannelConfig[chnIx].resultRegister = (IfxVadc_ChannelResult)(chnIx);
			// use dedicated result register

			// initialize the channel
			IfxVadc_Adc_initChannel(&adcChannel[chnIx], &adcChannelConfig[chnIx]);

			// add to autoscan
			unsigned int channels = (1 << adcChannelConfig[chnIx].channelId);
			unsigned int mask = channels;        boolean continuous = TRUE;
			IfxVadc_Adc_setScan(&adcGroup, channels, mask);
		}
	}
}

void Appli_AdcBackgroundInit(void)
{
	int chnIx;

	for(chnIx = 0; chnIx < 2; ++chnIx)
	{
		IfxVadc_Adc_initChannelConfig(&adcChannelConfig[chnIx], &adcGroup);

		adcChannelConfig[chnIx].channelId = (IfxVadc_ChannelId)(chnIx + 2);
		adcChannelConfig[chnIx].resultRegister = (IfxVadc_ChannelResult)(2 + chnIx);
		// use register #2 and 3 for results
		adcChannelConfig[chnIx].backgroundChannel = TRUE;

		// initialize the channel
		IfxVadc_Adc_initChannel(&adcChannel[chnIx], &adcChannelConfig[chnIx]);
		// add to background scan
		unsigned int channels = (1 << adcChannelConfig[chnIx].channelId);
		unsigned int mask = channels;
		IfxVadc_Adc_setBackgroundScan(&vadc, &adcGroup, channels, mask);
	}   // start autoscan

}

void Appli_AdcInit(void)
{
	Appli_AdcModule_Init();

	if(demo_item == 1)
	{
		Appli_AdcQueuedInit();
	}
	else if(demo_item == 2)
		Appli_AdcScanInit();
	else if(demo_item == 3)
		Appli_AdcBackgroundInit();
}

void Appli_AdcDemo_Queued(void)
{
	int chnIx;
	Ifx_VADC_RES conversionResult;

	Appli_AdcQueuedInit();

	// start the Queue
	IfxVadc_Adc_startQueue(&adcGroup);
	// just for the case that somebody copy&pastes the code -
	//the queue has already been started in previous test

	// get 10 results for all channels and store in temporary buffer
	// (the usage of a buffer is required, since the print statements used by the checks take
	// more time than the conversions)
	for(chnIx = 0; chnIx < QUEUE_TEST_CH_N; ++chnIx)
	{
		do {
			conversionResult = IfxVadc_Adc_getResult(&adcChannel[chnIx]);
		} while( !conversionResult.B.VF );

		// store result
		resultTrace[chnIx] = conversionResult;

	}
	printf("%u\r\n",
			resultTrace[3].B.RESULT);
	//	printf("%u\t%u\t%u\t%u\r\n",
	//			resultTrace[0].B.RESULT,
	//			resultTrace[1].B.RESULT,
	//			resultTrace[2].B.RESULT,
	//			resultTrace[3].B.RESULT);

	//	printf("CH0 :%u\t", resultTrace[0].B.RESULT);
	//	printf("CH1 :%u\t", resultTrace[1].B.RESULT);
	//	printf("CH2 :%u\t", resultTrace[2].B.RESULT);
	//	printf("CH3 :%u\r\n", resultTrace[3].B.RESULT);

	// stop the queue
	IfxVadc_Adc_clearQueue(&adcGroup);

	// check results in buffer
	// ...

}

void Appli_AdcDemo_Scan(void)
{
	int chnIx;
	IfxVadc_GroupId grp_id;
	IfxVadc_ChannelId ch_id;
	char str_adc[10];

	// start autoscan
	IfxVadc_Adc_startScan(&adcGroup);
	{
		// check results
		for(chnIx=0; chnIx<QUEUE_TEST_CH_N; ++chnIx)
		{
			grp_id = adcChannel[chnIx].group->groupId;
			ch_id = adcChannel[chnIx].channel;

			// wait for valid result
			do {
				conversionResult = IfxVadc_Adc_getResult(&adcChannel[chnIx]);
			} while( !conversionResult.B.VF );

			// print result, check with expected value
			resultTrace[chnIx] = conversionResult;
		}
		printf("%u\t%u\t%u\t%u\r\n",
				resultTrace[0].B.RESULT,
				resultTrace[1].B.RESULT,
				resultTrace[2].B.RESULT,
				resultTrace[3].B.RESULT);
		//		printf("CH0 :%u\t", resultTrace[0].B.RESULT);
		//		printf("CH1 :%u\t", resultTrace[1].B.RESULT);
		//		printf("CH2 :%u\t", resultTrace[2].B.RESULT);
		//		printf("CH3 :%u\r\n", resultTrace[3].B.RESULT);
	}
}

void Appli_AdcDemo_Background(void)
{
	int chnIx;
	IfxVadc_GroupId grp_id;
	IfxVadc_ChannelId ch_id;
	char str_adc[10];

	//boolean continuous = TRUE;
	IfxVadc_Adc_startBackgroundScan(&vadc);

	// check results
	for(chnIx = 0; chnIx < 2; ++chnIx)
	{
		grp_id = adcChannel[chnIx].group->groupId;
		ch_id = adcChannel[chnIx].channel;

		// wait for valid result
		do
		{
			conversionResult = IfxVadc_Adc_getResult(&adcChannel[chnIx]);
		} while( !conversionResult.B.VF );

		// check with expected value
		// ...
		resultTrace[chnIx] = conversionResult;
	}

	printf("CH2 :%u\t", resultTrace[0].B.RESULT);
	printf("CH2 :%u\r\n", resultTrace[1].B.RESULT);
}

void Appli_AdcCyclic(void)
{
	adc_count++;

	if(adc_count > 2)
	{
		adc_count = 0;

		if(demo_item == 1)
		{
			Appli_AdcDemo_Queued();
		}
		else if(demo_item == 2)
		{
			Appli_AdcDemo_Scan();
		}
		else if(demo_item == 3)
		{
			Appli_AdcDemo_Background();
		}
	}
}

