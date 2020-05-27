/*
 **
 ** Source file generated on February 2, 2018 at 20:06:04.
 **
 ** Copyright(c) 2019 Analog Devices, Inc. All Rights Reserved.
 ** This software is proprietary and confidential. By using this software you agree
 ** to the terms of the associated Analog Devices License Agreement.
 **
 ** This file is generated automatically based upon the options selected in
 ** the CGU Initialization configuration dialog. Changes to this configuration should be made by
 ** changing the appropriate options rather than editing this file.
 **
 */
#include "FirmPWRConfig.h"

/**********************************************************************************************
 *						CGU	Configuration Number 0
 **********************************************************************************************/
/*
Configuration Number	: 0
SDRAM Mode				: DDR3
SYS_CLKIN0 (MHz)		: 25
Use CGU1 ?				: Yes
Reset DMC DLL ? 		: Yes

//CDU Initialization Options
SPI(0-2) CLOCK SOURCE (CLKO0)  :   SCLK0_0		:	150 MHz
DDR CLOCK	  		  (CLKO1)  :   DCLK0_1		:	800 MHz

//CGU0 Initialization Options
fVCO (Desired)			:	2000 MHz
fVCO (Actual)			: 	2000 MHz
CCLK					:	1000 MHz
SYSCLK					:	500  MHz
SCLK0					:	125  MHz
SCLK1(SPDIF RX)			:	250	 MHz
DCLK					:	666.66 MHz
OCLK (LP CLOCK)			:	125  MHz

MSEL					: 	80
Use DF?					: 	No
DF						: 	0
CSEL					: 	2
CCLK to SYSCLK Ratio	: 	2:1
S0SEL					: 	4
S1SEL					: 	2
DSEL					: 	3
OSEL					: 	16
Use S1SELEX?			: 	No
S1SELEX					: 	0

//CGU1 Initialization Options
fVCO (Desired)			:	1500 MHz
fVCO (Actual)			: 	1500 MHz
DCLK0_1					:	750  MHz

MSEL					: 	64
Use DF?					: 	No
DF						: 	0
DSEL					: 	2
*/


/**********************************************************************************************
 *						CGU	Configuration Number 0
 **********************************************************************************************/

/* CLKIN External crystal */
#define CFG0_BIT_CGU0_CLKIN						25000000

/* SPI(0-2) CLOCK SOURCE */
#define CDU0_CFG0_SEL_VALUE						(0u)

/* DDR CLOCK */
#define CDU0_CFG1_SEL_VALUE						(2u)

/**********************************************************************************************
 *						CGU	Configuration Number 0 Register Values
 **********************************************************************************************/

/*****************************************CGU0_CTL**********************************************/

#define CFG0_BIT_CGU0_CTL_DF					(0u)   /* CLKIN Frequency division factor      */
#define CFG0_BIT_CGU0_CTL_MSEL					(60u)  /* CLKIN Frequency multiplication factor*/


/*****************************************CGU0_DIV**********************************************/

#define CFG0_BIT_CGU0_DIV_CSEL					(2u)   /* CCLK division   */
#define CFG0_BIT_CGU0_DIV_SYSSEL				(5u)   /* SYSCLK division */
#define CFG0_BIT_CGU0_DIV_S0SEL					(2u)   /* SCLK0 division  */
#define CFG0_BIT_CGU0_DIV_S1SEL					(2u)   /* SCLK1 division  */
#define CFG0_BIT_CGU0_DIV_DSEL					(3u)   /* DCLK division   */
#define CFG0_BIT_CGU0_DIV_OSEL					(40u)  /* OCLK division   */


/*****************************************CGU0_DIVEX**********************************************/

#define CFG0_BIT_CGU0_DIV_S1SELEX				(32u)

/*****************************************CGU1_CTL**********************************************/

#define CFG0_BIT_CGU1_CTL_DF					(0u)
#define CFG0_BIT_CGU1_CTL_MSEL					(60u)

/*****************************************CGU1_DIV**********************************************/

#define CFG0_BIT_CGU1_DIV_DSEL					(4u)

/*****************************************DMC SELF REFESH***************************************/

#define DDR_SELF_REFRESH    					(0u)

/**************************************************************************************************/


void vfPWRInit(void);

/* Structure pointer for CGU0 and CGU1 parameters*/
static ADI_PWR_CGU_PARAM_LIST prvfCGUParamList;

/* Structure pointer for CDU parameters*/
static ADI_PWR_CDU_PARAM_LIST prvfCDUParamList;

/**
 * @Function    adi_pwr_cfg0_init0(void)
 * @brief       Initialize the CGU0 & CGU1
 * @details     PLL CLK: 1500Mhz
 *				CCLK   : 750 Mhz( maximum 800Mhz )
 *				SYSCLK : 300 Mhz
 *				SCLK0  : 150 Mhz( TWI & PCG reference clock,Derived from SYSCLK frequency division )
 *			    SCLK1  : 150 Mhz
 * @param[in]
 *
 * @return
 *
 **/
void vfPWRInit(void)
{
	/* CDU */
	prvfCDUParamList.cdu_settings[0].cfg_SEL 				      = 	(ADI_PWR_CDU_CLKIN)CDU0_CFG0_SEL_VALUE;
	prvfCDUParamList.cdu_settings[0].cfg_EN  				      = 	true;

	prvfCDUParamList.cdu_settings[1].cfg_SEL 				      = 	(ADI_PWR_CDU_CLKIN)CDU0_CFG1_SEL_VALUE;
	prvfCDUParamList.cdu_settings[1].cfg_EN  				      = 	true;

	/* CGU0 */
	prvfCGUParamList.cgu0_settings.clocksettings.ctl_MSEL	      =		(uint32_t)CFG0_BIT_CGU0_CTL_MSEL;
	prvfCGUParamList.cgu0_settings.clocksettings.ctl_DF		      =		(uint32_t)CFG0_BIT_CGU0_CTL_DF;
	prvfCGUParamList.cgu0_settings.clocksettings.div_CSEL	      =		(uint32_t)CFG0_BIT_CGU0_DIV_CSEL;
	prvfCGUParamList.cgu0_settings.clocksettings.div_SYSSEL	      =		(uint32_t)CFG0_BIT_CGU0_DIV_SYSSEL;
	prvfCGUParamList.cgu0_settings.clocksettings.div_S0SEL	      =		(uint32_t)CFG0_BIT_CGU0_DIV_S0SEL;
	prvfCGUParamList.cgu0_settings.clocksettings.div_S1SEL	      =		(uint32_t)CFG0_BIT_CGU0_DIV_S1SEL;
	prvfCGUParamList.cgu0_settings.clocksettings.divex_S1SELEX    =		(uint32_t)CFG0_BIT_CGU0_DIV_S1SELEX;
	prvfCGUParamList.cgu0_settings.clocksettings.div_DSEL	      =		(uint32_t)CFG0_BIT_CGU0_DIV_DSEL;
	prvfCGUParamList.cgu0_settings.clocksettings.div_OSEL	      =		(uint32_t)CFG0_BIT_CGU0_DIV_OSEL;
	prvfCGUParamList.cgu0_settings.clkin 					      = 	(uint32_t)CFG0_BIT_CGU0_CLKIN;
	prvfCGUParamList.cgu0_settings.enable_IDLE				      = 	false;
	prvfCGUParamList.cgu0_settings.enable_SCLK1ExDiv			  = 	false;

	/* CGU1 */
	prvfCGUParamList.cgu1_settings.clkin 					      = 	(uint32_t)CFG0_BIT_CGU0_CLKIN;
	prvfCGUParamList.cgu1_settings.clocksettings.ctl_MSEL	      =		(uint32_t)CFG0_BIT_CGU1_CTL_MSEL;
	prvfCGUParamList.cgu1_settings.clocksettings.ctl_DF		      =		(uint32_t)CFG0_BIT_CGU1_CTL_DF;
	prvfCGUParamList.cgu1_settings.clocksettings.div_DSEL	      =		(uint32_t)CFG0_BIT_CGU1_DIV_DSEL;
	prvfCGUParamList.cgu1_settings.enable_IDLE				      = 	false;

	/* Set DDR DLL Reset*/
	int delay = 9000;
	while(delay--);

	/* Initialize all the clocks*/
	adi_pwr_ClockInit(&prvfCGUParamList, &prvfCDUParamList);

	delay = 9000;
	while(delay--);

}



