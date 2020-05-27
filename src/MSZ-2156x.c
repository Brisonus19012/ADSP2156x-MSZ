/*****************************************************************************
 * RNC-2156x.c
 *****************************************************************************/

/*=============  I N C L U D E S   =============*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/adi_core.h>
#include <adi_initialize.h>
#include <services/int/adi_int.h>

#include "MSZ-2156x.h"


a2b_App_t gApp_Info;

/* The currently searched node number,
 * indicates that there is a problem with the connection
 * between the current node and the next node */
a2b_UInt8 CurrNode;

void main(int argc, char *argv[])
{
	/**
	 * Initialize managed drivers and/or services that have been added to
	 * the project.
	 * @return zero on success
	 */
 	uint32_t Result=0;

	adi_initComponents();

	/* Begin adding your custom code here */
	/**
	 * TODO:[Step1]Put your initialization code in the Suitable location
	 */
	vfPWRInit();           			/* initialization CCLK 750Mhz,SYSCLK 300Mhz */

	vfSRUInit();                    /* DAI signal routing                       */

	Result = adi_a2b_SystemInit();  /* system/platform specific initialization  */
	if(Result != 0)
	{
		REPORT_ERROR("Failed to initialize a2b system\n");
	}

	Result = a2b_setup(&gApp_Info); /* A2B Network Setup. Performs discovery and configuration of A2B nodes and its peripherals */
	if (Result != 0)
	{
		CurrNode = gApp_Info.faultNode;
		REPORT_ERROR("Currently found node number is:%d\n", CurrNode);
		assert(Result == 0);        /* failed to setup A2B network */
	}

	Result = ulfSPortInit();
	if(Result != 0)
	{
		REPORT_ERROR("Failed to initialize SPORT\n");
	}

	while(1)
	{
		if(ucfInputReady)
		{
			ucfInputReady = 0;
			/* TODO:[Step2]Main data processing function,make changes inside the function */
			vfProcessAudioBlocks();
		}

		Result = a2b_fault_monitor(&gApp_Info);/* Monitor a2b network for faults and initiate re-discovery if enabled */
		if (Result != 0)                       /* Condition to exit the program */
		{
			DEBUG_INFORMATION("A2B Network failed.\n");
			assert(Result == 0);               /* failed to setup A2B network   */
		}
//		a2b_stackTick(gApp_Info.ctx);          /* A2B Diagnostic scheduler      */
	}

}



