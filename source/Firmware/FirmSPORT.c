/*! \addtogroup Target_Dependent Target Dependent
 *  @{
 */

/** @defgroup SPORT
 *
 * This module controls the SPORTs configuration for Transmit and Receive. The
 * framework will use the SPORTs to transfer audio data from and to Codec and
 * AD2410.
 */

/*! \addtogroup SPORT
 *  @{
 */

/*============= I N C L U D E S =============*/

#include <stdio.h>
#include <sys/platform.h>
#include "FirmSPORT.h"

/*============= D E F I N E S =============*/

uint32_t ulfSPortInit(void);
void vfProcessAudioBlocks(void);

static void prvfProcessBuffers(int32_t *adcbuf,int32_t *dacbuf,int32_t* a2bbuf);

/* Prepares descriptors for SPORT DMA */
static void prvfRXPrepareDescriptors (void);
static void prvfTXPrepareDescriptors (void);

/* Count to track the number of callBacks for SPORT transfer */
volatile uint8_t prvCallbackCount = 0;

uint8_t ucfInputReady = 0;  /* ADC Digital signal ready flag */

/* Source SPORT PDMA Lists */
static ADI_PDMA_DESC_LIST iDESC_LIST_1_SP0A;
static ADI_PDMA_DESC_LIST iDESC_LIST_2_SP0A;

/* Destination SPORT PDMA Lists */
static ADI_PDMA_DESC_LIST iDESC_LIST_1_SP4A;
static ADI_PDMA_DESC_LIST iDESC_LIST_2_SP4A;

static ADI_PDMA_DESC_LIST iSRC_LIST_1_SP5A;
static ADI_PDMA_DESC_LIST iSRC_LIST_2_SP5A;

/* Memory required for SPORT */
static uint8_t prvSPORTMemory0A[ADI_SPORT_MEMORY_SIZE];
static uint8_t prvSPORTMemory4A[ADI_SPORT_MEMORY_SIZE];
static uint8_t prvSPORTMemory5A[ADI_SPORT_MEMORY_SIZE];

/* SPORT Handle */
static ADI_SPORT_HANDLE prvSPORTDev0ATx;//TX
static ADI_SPORT_HANDLE prvSPORTDev4ATx;//TX
static ADI_SPORT_HANDLE prvSPORTDev5ARx;//RX

#pragma section("seg_l1_block1")
ADI_CACHE_ALIGN int32_t lfSP4ABuffer1[DAC_BUFFER_SIZE];
#pragma section("seg_l1_block1")
ADI_CACHE_ALIGN int32_t lfSP4ABuffer2[DAC_BUFFER_SIZE];
#pragma section("seg_l1_block1")
ADI_CACHE_ALIGN int32_t lfSP5ABuffer1[ADC_BUFFER_SIZE];
#pragma section("seg_l1_block1")
ADI_CACHE_ALIGN int32_t lfSP5ABuffer2[ADC_BUFFER_SIZE];
#pragma section("seg_l1_block1")
ADI_CACHE_ALIGN int32_t lfSP0ABuffer1[A2B_BUFFER_SIZE];
#pragma section("seg_l1_block1")
ADI_CACHE_ALIGN int32_t lfSP0ABuffer2[A2B_BUFFER_SIZE];

/*============= C O D E =============*/

/**
 * @Function    SPORTCallback()
 * @brief       SPORT interrupt callback
 * @details     This is the ISR for servicing the SPORT Rx interrupt callback
 *
 * @param[in]	pAppHandle		Handle to the SPORT device
 * @param[in]	nEvent			Event type
 *
 * @return
 *
 **/
static void prvfSPORTCallback(void *pAppHandle, uint32_t nEvent, void *pArg)
{

    switch (nEvent)                               /* CASEOF (event type)     */
    {
        case ADI_SPORT_EVENT_RX_BUFFER_PROCESSED: /* CASE (buffer processed) */

        		prvCallbackCount ++;
        		ucfInputReady = 1;

        		break;
        default:
        	 break;
    }

}

/**
 * TODO:[Tips]Put your algorithm main function here
 */
/**
 * @Function    prvfProcessBuffers(void)
 * @brief       Handler function
 * @details     Audio digital signal processing
 * @param[in]   adcbuf      SPORT5A received digital signal                    --lfSP5ABufferx
 * @param[in]   dacbuf      Digital signal sent from SPORT4A to TDA7803        --lfSP4ABufferx
 * @param[in]   a2bbuf      Digital signal sent from SPORT0A to A2B downstream --lfSP0ABufferx
 *
 * @return
 **/
void prvfProcessBuffers(int32_t* adcbuf,int32_t* dacbuf,int32_t* a2bbuf)
{
	/* Replace the test code with your algorithm function */
	/* test code : Copy from input to output */
	int  i;
	for(i=0;i<24;i++)
	{
	   /*Copy ADC buffer to DAC buffer */
		a2bbuf[24*i]   = adcbuf[8*i];
		a2bbuf[24*i+1] = adcbuf[8*i+1];
		a2bbuf[24*i+2] = adcbuf[8*i+2];
		a2bbuf[24*i+3] = adcbuf[8*i+3];
/*		a2bbuf[0+i] = 0;
		a2bbuf[24+i] = 111161<<8;
		a2bbuf[48+i] = 214748<<8;
		a2bbuf[72+i] = 303700<<8;
		a2bbuf[96+i] = 371955<<8;
		a2bbuf[120+i] = 414861<<8;
		a2bbuf[144+i] = 429496<<8;
		a2bbuf[168+i] = 414861<<8;
		a2bbuf[192+i] = 371955<<8;
		a2bbuf[216+i] = 303700<<8;
		a2bbuf[240+i] = 214748<<8;
		a2bbuf[264+i] = 111161<<8;
		a2bbuf[288+i] = 0;
		a2bbuf[312+i] = -111161<<8;
		a2bbuf[336+i] = -214748<<8;
		a2bbuf[360+i] = -303700<<8;
		a2bbuf[384+i] = -371955<<8;
		a2bbuf[408+i] = -414861<<8;
		a2bbuf[432+i] = -429496<<8;
		a2bbuf[456+i] = -414861<<8;
		a2bbuf[480+i] = -371955<<8;
		a2bbuf[504+i] = -303700<<8;
		a2bbuf[528+i] = -214748<<8;
		a2bbuf[552+i] = -111161<<8;*/

	}
	for(i=0;i<24*8;i++)
	{
		dacbuf[i] = adcbuf[i];
/*		dacbuf[0+i] = 0;
		dacbuf[8+i] = 111161<<8;
		dacbuf[16+i] = 214748<<8;
		dacbuf[24+i] = 303700<<8;
		dacbuf[32+i] = 371955<<8;
		dacbuf[40+i] = 414861<<8;
		dacbuf[48+i] = 429496<<8;
		dacbuf[56+i] = 414861<<8;
		dacbuf[64+i] = 371955<<8;
		dacbuf[72+i] = 303700<<8;
		dacbuf[80+i] = 214748<<8;
		dacbuf[88+i] = 111161<<8;
		dacbuf[96+i] = 0;
		dacbuf[104+i] = -111161<<8;
		dacbuf[112+i] = -214748<<8;
		dacbuf[120+i] = -303700<<8;
		dacbuf[128+i] = -371955<<8;
		dacbuf[136+i] = -414861<<8;
		dacbuf[144+i] = -429496<<8;
		dacbuf[152+i] = -414861<<8;
		dacbuf[160+i] = -371955<<8;
		dacbuf[168+i] = -303700<<8;
		dacbuf[176+i] = -214748<<8;
		dacbuf[184+i] = -111161<<8;*/
	}
	/* test end */
}

/**
 * @Function    vfProcessAudioBlocks(void)
 * @brief       Main process buffer task
 * @details     Ping-Pong buffer alternate transmission
 *
 * @param[in]
 * @return
 **/
void vfProcessAudioBlocks(void)
{
	if (prvCallbackCount == 1)  /* Process Ping buffer */
	{
		prvfProcessBuffers(lfSP5ABuffer1, lfSP4ABuffer1, lfSP0ABuffer1);
	}
	if (prvCallbackCount == 2)  /* Process Pong buffer */
	{
		prvfProcessBuffers(lfSP5ABuffer2, lfSP4ABuffer2, lfSP0ABuffer2);
		prvCallbackCount = 0;
	}
}


/**
 * @Function    prvfRXPrepareDescriptors(void)
 * @brief		Configuring all the parameters of RX
 * @details
 *
 * @param[in]
 * @return
 **/
static void prvfRXPrepareDescriptors (void)
{
	iSRC_LIST_1_SP5A.pStartAddr		=(int *)lfSP5ABuffer1;     /* Start address of current buffer */
	iSRC_LIST_1_SP5A.Config			= ENUM_DMA_CFG_XCNT_INT;   /* DMA Configuration register      */
	iSRC_LIST_1_SP5A.XCount			= ADC_BUFFER_SIZE;         /* Inner loop count start value in number of MSIZE data transfers */
	iSRC_LIST_1_SP5A.XModify		= 4;                       /* Inner loop address increment, in bytes */
	iSRC_LIST_1_SP5A.YCount			= 0;
	iSRC_LIST_1_SP5A.YModify		= 0;
	iSRC_LIST_1_SP5A.pNxtDscp		= &iSRC_LIST_2_SP5A;       /* Pointer to next Initial Descriptor */

	iSRC_LIST_2_SP5A.pStartAddr		=(int *)lfSP5ABuffer2;
	iSRC_LIST_2_SP5A.Config			= ENUM_DMA_CFG_XCNT_INT;
	iSRC_LIST_2_SP5A.XCount			= ADC_BUFFER_SIZE;
	iSRC_LIST_2_SP5A.XModify		= 4;
	iSRC_LIST_2_SP5A.YCount			= 0;
	iSRC_LIST_2_SP5A.YModify		= 0;
	iSRC_LIST_2_SP5A.pNxtDscp		= &iSRC_LIST_1_SP5A;
}

/**
 * @Function    prvfTXPrepareDescriptors(void)
 * @brief		Configuring all the parameters of TX
 * @details
 *
 * @param[in]
 * @return
 **/
static void prvfTXPrepareDescriptors (void)
{
	iDESC_LIST_1_SP0A.pStartAddr	=(int *)lfSP0ABuffer1;
	iDESC_LIST_1_SP0A.Config		= ENUM_DMA_CFG_XCNT_INT ;
	iDESC_LIST_1_SP0A.XCount		= A2B_BUFFER_SIZE;
	iDESC_LIST_1_SP0A.XModify		= 4;
	iDESC_LIST_1_SP0A.YCount		= 0;
	iDESC_LIST_1_SP0A.YModify		= 0;
	iDESC_LIST_1_SP0A.pNxtDscp		= &iDESC_LIST_2_SP0A;

	iDESC_LIST_2_SP0A.pStartAddr	=(int *)lfSP0ABuffer2;
	iDESC_LIST_2_SP0A.Config		= ENUM_DMA_CFG_XCNT_INT;
	iDESC_LIST_2_SP0A.XCount		= A2B_BUFFER_SIZE;
	iDESC_LIST_2_SP0A.XModify		= 4;
	iDESC_LIST_2_SP0A.YCount		= 0;
	iDESC_LIST_2_SP0A.YModify		= 0;
	iDESC_LIST_2_SP0A.pNxtDscp		= &iDESC_LIST_1_SP0A;

	iDESC_LIST_1_SP4A.pStartAddr	= (int *)lfSP4ABuffer1;
	iDESC_LIST_1_SP4A.Config		= ENUM_DMA_CFG_XCNT_INT ;
	iDESC_LIST_1_SP4A.XCount		= DAC_BUFFER_SIZE;
	iDESC_LIST_1_SP4A.XModify		= 4;
	iDESC_LIST_1_SP4A.YCount		= 0;
	iDESC_LIST_1_SP4A.YModify		= 0;
	iDESC_LIST_1_SP4A.pNxtDscp		= &iDESC_LIST_2_SP4A;

	iDESC_LIST_2_SP4A.pStartAddr	= (int *)lfSP4ABuffer2;
	iDESC_LIST_2_SP4A.Config		= ENUM_DMA_CFG_XCNT_INT ;
	iDESC_LIST_2_SP4A.XCount		= DAC_BUFFER_SIZE;
	iDESC_LIST_2_SP4A.XModify		= 4;
	iDESC_LIST_2_SP4A.YCount		= 0;
	iDESC_LIST_2_SP4A.YModify		= 0;
	iDESC_LIST_2_SP4A.pNxtDscp		= &iDESC_LIST_1_SP4A;
}

/**
 * @Function    ulfSPortInit(void)
 * @brief       SPORT initialization
 * @details     SPORT5A direction : RX <- ADAU1978 <- MIC IN
 * 				SPORT4A direction : TX -> TDA7803 -> PA OUT
 * 				SPORT0A direction : TX -> A2B Downstream
 *				RX Callback
 * @param[in]
 * @return
 **/
uint32_t ulfSPortInit(void)
{
    /* SPORT return code */
    ADI_SPORT_RESULT    eResult;

    /* Open the SPORT Device 0A*/
	eResult = adi_sport_Open(SPORT_DEVICE_5A,ADI_HALF_SPORT_A,ADI_SPORT_DIR_RX, ADI_SPORT_MC_MODE, prvSPORTMemory5A,ADI_SPORT_MEMORY_SIZE,&prvSPORTDev5ARx);

	/* Configure the data,clock,frame sync and MCTL of SPORT Device 5A*/
	eResult = adi_sport_ConfigData(prvSPORTDev5ARx,ADI_SPORT_DTYPE_SIGN_FILL,31,false,false,false);

	eResult = adi_sport_ConfigClock(prvSPORTDev5ARx,100,false,false,false);

	eResult = adi_sport_ConfigFrameSync(prvSPORTDev5ARx,0,false,false,false,false,false,true);

	eResult = adi_sport_ConfigMC(prvSPORTDev5ARx,1u,7u,0u,false);

	eResult = adi_sport_SelectChannel(prvSPORTDev5ARx,0u,7u);

	eResult = adi_sport_RegisterCallback(prvSPORTDev5ARx,prvfSPORTCallback,NULL);

	/* Open the SPORT Device 4A */
	eResult = adi_sport_Open(SPORT_DEVICE_4A,ADI_HALF_SPORT_A,ADI_SPORT_DIR_TX, ADI_SPORT_MC_MODE, prvSPORTMemory4A,ADI_SPORT_MEMORY_SIZE,&prvSPORTDev4ATx);

	/* Configure the data,clock,frame sync and MCTL of SPORT Device 4A*/
	eResult = adi_sport_ConfigData(prvSPORTDev4ATx,ADI_SPORT_DTYPE_SIGN_FILL,31,false,false,false);

	eResult = adi_sport_ConfigClock(prvSPORTDev4ATx,100,false,false,false);

	eResult = adi_sport_ConfigFrameSync(prvSPORTDev4ATx,0,false,false,false,true,false,true);

	eResult = adi_sport_ConfigMC(prvSPORTDev4ATx,1u,7u,0u,false);

	eResult = adi_sport_SelectChannel(prvSPORTDev4ATx,0u,7u);

	/* Open the SPORT Device 0A*/
	eResult = adi_sport_Open(SPORT_DEVICE_0A,ADI_HALF_SPORT_A,ADI_SPORT_DIR_TX, ADI_SPORT_MC_MODE, prvSPORTMemory0A,ADI_SPORT_MEMORY_SIZE,&prvSPORTDev0ATx);

	/* Configure the data,clock,frame sync and MCTL of SPORT Device 0A*/
	eResult = adi_sport_ConfigData(prvSPORTDev0ATx,ADI_SPORT_DTYPE_SIGN_FILL,31,false,false,false);

	eResult = adi_sport_ConfigClock(prvSPORTDev0ATx,100,false,false,false);

	eResult = adi_sport_ConfigFrameSync(prvSPORTDev0ATx,0,false,false,false,true,false,true);

	eResult = adi_sport_ConfigMC(prvSPORTDev0ATx,1u,23u,0u,false);

	eResult = adi_sport_SelectChannel(prvSPORTDev0ATx,0u,23u);

//	eResult = adi_sport_RegisterCallback(hSPORTDev0ARx,SPORTCallback,NULL);

	/* Prepare descriptors */
	prvfTXPrepareDescriptors();
	prvfRXPrepareDescriptors();

	/* Submit the first buffer for Rx.  */
	eResult = adi_sport_DMATransfer(prvSPORTDev5ARx,&iSRC_LIST_1_SP5A,(DMA_NUM_DESC),ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);

	/* Submit the first buffer for Rx.  */
	eResult = adi_sport_DMATransfer(prvSPORTDev0ATx,&iDESC_LIST_1_SP0A,(DMA_NUM_DESC),ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);

	/* Submit the first buffer for Tx.  */
	eResult = adi_sport_DMATransfer(prvSPORTDev4ATx,&iDESC_LIST_1_SP4A,(DMA_NUM_DESC),ADI_PDMA_DESCRIPTOR_LIST, ADI_SPORT_CHANNEL_PRIM);

	/*Enable the Sport Device 0A */
	eResult = adi_sport_Enable(prvSPORTDev5ARx,true);

	/*Enable the Sport Device 0A */
	eResult = adi_sport_Enable(prvSPORTDev0ATx,true);

	/*Enable the Sport Device 4A */
	eResult = adi_sport_Enable(prvSPORTDev4ATx,true);

	return (uint32_t)eResult;

}

/**
 * @Function    ulfSPortClose(void)
 * @brief       SPORT stop
 * @details     Close SPORT, stop all actions of SPORT
 * @param[in]
 * @return
 **/
uint32_t ulfSPortClose(void)
{
	/* SPORT return code */
	ADI_SPORT_RESULT xResult;

	xResult = adi_sport_StopDMATransfer(prvSPORTDev5ARx);
	xResult = adi_sport_StopDMATransfer(prvSPORTDev4ATx);
	xResult = adi_sport_StopDMATransfer(prvSPORTDev0ATx);

	xResult = adi_sport_Close(prvSPORTDev5ARx);
	xResult = adi_sport_Close(prvSPORTDev4ATx);
	xResult = adi_sport_Close(prvSPORTDev0ATx);

	return xResult;
}



/*
**
** EOF: $URL$
**
*/

