/*******************************************************************************
Copyright (c) 2014 - Analog Devices Inc. All Rights Reserved.
This software is proprietary & confidential to Analog Devices, Inc.
and its licensors.
******************************************************************************
 * @file:    adi_a2b_sys.c
 * @brief:   System component APIs are defined in this file
 * @version: $Revision: 6145 $
 * @date:    $Date: 2014-12-18 15:15:45 +0530 (Thu, 18 Dec 2014) $
 * Developed by: Automotive Software and Systems team, Bangalore, India
*****************************************************************************/
/*! \addtogroup ADI_A2B_SYS
 *  @{
 */

/*============= I N C L U D E S =============*/

#include <sys/adi_core.h>
#include <drivers/twi/adi_twi.h>

#include "adi_a2b_pal.h"
#include "adi_a2b_sys.h"
#include "../../Commons/ComADAU_1978Common.h"

/*============= D E F I N E S =============*/

int ADAU_1978_init(void);

static int ADAU_1978_Pllinit(void);
static void Write_TWI_8bit_Reg(unsigned char Reg_ID, unsigned char Tx_Data);
static unsigned char Read_TWI_8bit_Reg(unsigned char Reg_ID);

static ADI_A2B_SYS_RESULT CODEC_Init(void);
static ADI_PWR_RESULT CheckClock(ADI_A2B_SYS_POWER_CONFIG *pSysPowerConfig);


/*============= C O D E =============*/

/*
** Function Prototype section
** (static-scoped functions)
*/

#define PORT_CODEC_RESET    (ADI_A2B_HAL_GPIO_PORT_B)            /*!< Port used for resetting the ADC and DAC. This is
                                                                     a hard wired port number for CODEC resetting. */
#define PIN_CODEC_RESET     (ADI_A2B_HAL_GPIO_PIN_2)            /*!< Pin used for resetting the ADC and DAC. This
                                                                     is a hard wired pin number for CODEC resetting. */
#ifndef _TESSY_
#define WAIT_CLOCK_CYCLES(n)      \
        do                        \
        {                         \
			do                    \
			{                     \
            (n)--;                \
			}while((n)>0);        \
        }while(0)
#else	/* _TESSY_ */
#define WAIT_CLOCK_CYCLES(n)      \
			do                    \
			{                     \
            (n)--;                \
			}while((n)>0);
#endif	/* _TESSY_ */

#define CODEC_RESET_DELAY    (100000U)

/*============= D A T A =============*/

struct Config_Table
{
	short Reg_Add;
	char  Value;
};

char Config_read_ADC[16];

/* Dev buffer for configuring ADC-DAC through TWI*/
static uint8_t devBuffer[BUFFER_SIZE];

struct Config_Table Config_array_ADC1[16] = {

		 {ADAU1978_REG_BOOST			,	0x00},
		 {ADAU1978_REG_MICBIAS			,	0x00},
		 {ADAU1978_REG_BLOCK_POWER_SAI	,	0x70}, /* LRCLK high then low,data changes on rising edge   */
		 {ADAU1978_REG_SAI_CTRL0		,	0x1A}, /* 1 BCLK delay, TDM8, 48kHz                         */
		 {ADAU1978_REG_SAI_CTRL1		,	0x00}, /* slot width 32bit, 24 bit data, 50% duty,MSB first */
		 {ADAU1978_REG_CMAP12			,	0x10},
		 {ADAU1978_REG_CMAP34			,	0x32},
		 {ADAU1978_REG_SAI_OVERTEMP		,	0xf0},
		 {ADAU1978_REG_POST_ADC_GAIN1 	,	0xA0},
		 {ADAU1978_REG_POST_ADC_GAIN2 	,	0xA0},
		 {ADAU1978_REG_POST_ADC_GAIN3 	,	0xA0},
		 {ADAU1978_REG_POST_ADC_GAIN4 	,	0xA0},
		 {ADAU1978_REG_ADC_CLIP			,	0x00},
		 {ADAU1978_REG_DC_HPF_CAL		,	0x00},
		 {ADAU1978_REG_BLOCK_POWER_SAI	,	0x3f},
		 {ADAU1978_REG_MISC_CONTROL		,	0x00}
};

/*
 * Function Definition section
 */

/*!
    @brief        This function allocates memory for the sys components and initializes them.
    @param        pMemBlock    Pointer to blocks of memory required for the creation of
                               system component
    @param        pSysConfig   System component configuration structure.
    @return       Return code of ADI_A2B_SYS_RESULT type.
                   - #ADI_A2B_SYS_SUCCESS   : System component initialized successfully
                   - #ADI_A2B_SYS_FAILURE   : System initialization failed
                   - #ADI_A2B_SYS_INSUFFICIENT_MEM : Insufficient memory for initialization of system component
                   - #ADI_A2B_SYS_CORE_SECONDARY : Secondary core configured for system initialization. However system
                                                                          initialization must happen from primary core

    <b> Global Variables Used: <b>
                None

   @note        None.
 */
ADI_A2B_SYS_RESULT adi_a2b_sys_Init(void *pMemBlock, uint32_t const  nMemorySize,ADI_A2B_SYS_CONFIG *pSysConfig)
{

    uint32_t *pMemPtr;
    ADI_A2B_SYS_RESULT eResult;


    if(pSysConfig->bProcMaster == true)
    {
        if(nMemorySize < ADI_A2B_SYS_MEMSIZE_BYTES)
        {
            return ADI_A2B_SYS_INSUFFICIENT_MEM;
        }

        eResult = CODEC_Init();   //codec reset

        pMemPtr = (uint32_t*)pMemBlock;


    }
    else
    {
        return ADI_A2B_SYS_CORE_SECONDARY;
    }

    return ADI_A2B_SYS_SUCCESS;
}



/*!
   @brief      This function resets ADC and DAC.
   @param      None
   @return     return code of ADI_A2B_SYS_RESULT type
                   - ADI_A2B_SYS_SUCCESS:  The call to this function is successful.
                   - ADI_A2B_SYS_FAILURE : The call to this function failed.

   <b> Global Variables Used: <b>
                   None.

  @note        None.
*/
static ADI_A2B_SYS_RESULT CODEC_Init(void)
{
    ADI_A2B_HAL_GPIO_STATUS eGpioRetVal;
    uint32_t nDelay;

    eGpioRetVal = adi_a2b_hal_gpio_SetDirection(PORT_CODEC_RESET, PIN_CODEC_RESET, ADI_A2B_HAL_GPIO_DIRECTION_OUTPUT);
    if(eGpioRetVal != ADI_A2B_HAL_GPIO_STATUS_SUCCESS)
    {
        return ADI_A2B_SYS_FAILURE;
    }

    eGpioRetVal = adi_a2b_hal_gpio_Clear(PORT_CODEC_RESET, PIN_CODEC_RESET);
    if(eGpioRetVal != ADI_A2B_HAL_GPIO_STATUS_SUCCESS)
    {
        return ADI_A2B_SYS_FAILURE;
    }

    /* Wait for a duration of 1000 clock cycles */
    nDelay = CODEC_RESET_DELAY;
    WAIT_CLOCK_CYCLES(nDelay);

    eGpioRetVal = adi_a2b_hal_gpio_Set(PORT_CODEC_RESET, PIN_CODEC_RESET);
    if(eGpioRetVal != ADI_A2B_HAL_GPIO_STATUS_SUCCESS)
    {
        return ADI_A2B_SYS_FAILURE;
    }

    /* Wait for a duration of 1000 clock cycles */
    nDelay = CODEC_RESET_DELAY;
    WAIT_CLOCK_CYCLES(nDelay);

    return ADI_A2B_SYS_SUCCESS;
}



/*!
    @brief     Function to check the set clock values.
    @param     pSysPowerConfig  Configuration structure for power management.se.
    @return    Return code of ADI_PWR_RESULT type.
                    - ADI_PWR_SUCCESS  :  The call to this function is successful.
                    - ADI_PWR_FAILURE  :  The call to this function failed.

    <b> Global Variables Used: <b>
                None.

   @note        None.
*/

static ADI_PWR_RESULT CheckClock(ADI_A2B_SYS_POWER_CONFIG *pSysPowerConfig)
{
    uint32_t    fcclk    = 0U;
    uint32_t    fsysclk  = 0U;
    uint32_t    fsclk0   = 0U;
    uint32_t    fsclk1   = 0U;
	ADI_PWR_RESULT  eResult = ADI_PWR_SUCCESS;

    eResult = adi_pwr_GetCoreClkFreq((uint32_t)pSysPowerConfig->eCGUDev, &fcclk);
	eResult = adi_pwr_GetSystemFreq((uint32_t)pSysPowerConfig->eCGUDev, &fsysclk, &fsclk0, &fsclk1);


	if(eResult == ADI_PWR_SUCCESS)
	{
		if((fcclk < (pSysPowerConfig->nCoreClk-ADI_A2B_SYS_POWER_CLKIN)) ||
		   (fsysclk < (pSysPowerConfig->nSysClk-ADI_A2B_SYS_POWER_CLKIN)))
		{
			eResult = ADI_PWR_FAILURE;
		}
	}

	return eResult;
}



/*****************************************************************************************************************************/

static void Write_TWI_8bit_Reg(unsigned char Reg_ID, unsigned char Tx_Data)
{
	devBuffer[0] = Reg_ID;
	devBuffer[1] = Tx_Data;

	adi_twi_Write(adi_twi_hDevice, devBuffer, 2u, false);
}

static unsigned char Read_TWI_8bit_Reg(unsigned char Reg_ID)
{
	ADI_TWI_RESULT eResult;
	unsigned char  Rx_Data;

	/* write register address */
	devBuffer[0] = Reg_ID;
	eResult = adi_twi_Write(adi_twi_hDevice, devBuffer, 1u, true);
	if( eResult != ADI_TWI_SUCCESS )
	{
		REPORT_ERROR("TWI write failed 0x%08X\n", eResult);
	}

	/* read register value */
	eResult = adi_twi_Read(adi_twi_hDevice, &Rx_Data, 1u, false);
	if(eResult!=ADI_TWI_SUCCESS)
	{
		REPORT_ERROR("TWI Read failed 0x%08X\n", eResult);
	}

	return Rx_Data;
}

static int ADAU_1978_Pllinit(void)
{

	int status,delay1=0xffff;
	ADI_TWI_RESULT eResult;

   	eResult = adi_twi_SetHardwareAddress(adi_twi_hDevice, TARGETADDR_1978);
	if(eResult != ADI_TWI_SUCCESS)
	{
		REPORT_ERROR("TWI Set Hw address failed 0x%08X\n", eResult);
	}

   	Write_TWI_8bit_Reg(ADAU1978_REG_POWER,0x01);
	Write_TWI_8bit_Reg(ADAU1978_REG_PLL,0x11);

	status = Read_TWI_8bit_Reg(ADAU1978_REG_PLL);
	while(delay1--)
	{
		asm("nop;");
	}

	while(!((status & 0x80)>>7) )
	{
		status = Read_TWI_8bit_Reg(ADAU1978_REG_PLL);
		asm("nop;");
	}

	return eResult;

}


int ADAU_1978_init(void)
{
	int i;

	ADAU_1978_Pllinit();

	for(i=0;i<16;i++)
	{
		Write_TWI_8bit_Reg(Config_array_ADC1[i].Reg_Add, Config_array_ADC1[i].Value);
		Config_read_ADC[i] = Read_TWI_8bit_Reg(Config_array_ADC1[i].Reg_Add);

		if(Config_array_ADC1[i].Value != Config_read_ADC[i])
		{
			REPORT_ERROR("\n Configuring ADAU_1978 failed");
			return 1;
		}
	}

	return 0;
}




/*
 *
 * EOF: $URL:  $
 *
*/
