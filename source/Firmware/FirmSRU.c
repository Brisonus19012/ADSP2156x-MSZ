/**
 *
 */

#include "FirmSRU.h"


void vfSRUInit(void);


/**
 * @Function    vfSRUInit(void)
 * @brief       DAI signal routing
 * @details     SPORT signal routing,ADC & DAC share clock
 *              SPORT5A receive ADC digital signal                 ------( TDM8 )
 *              SPORT4A transmit digital signal to TDA7803         ------( TDM8 )
 *              SPORT0A transmit digital signal to A2B downstream  ------( TDM24)
 *
 * @param[in]
 *
 * @return
 *
 **/
void vfSRUInit(void)
{
	*pREG_PADS0_DAI0_IE = 0x1fffff;
	*pREG_PADS0_DAI1_IE = 0x1fffff;

	SRU2(HIGH, DAI1_PBEN08_I);      /* DAI1_PBEN08 set as output */
	SRU2(HIGH, DAI1_PBEN07_I);      /* DAI1_PBEN07 set as output */

	SRU2(DAI1_PB08_O, SPT5_ACLK_I); /* DAC clock to SPORT 5A     */
	SRU2(DAI1_PB07_O, SPT5_AFS_I);  /* DAC FS to SPORT 5A        */
	SRU2(DAI1_PB01_O, SPT5_AD0_I);  /* ADC digital to SPORT 5A   */
	SRU2(LOW, DAI1_PBEN01_I);       /* DAI1_PBEN01 set as input  */

	SRU2(DAI1_PB08_O, SPT4_ACLK_I); /* DAC clock to SPORT 4A     */
	SRU2(DAI1_PB07_O, SPT4_AFS_I);  /* DAC FS to SPORT 4A        */
	SRU2(SPT4_AD0_O, DAI1_PB09_I);  /* SPORT 4A to DAC           */
	SRU2(HIGH, DAI1_PBEN09_I);      /* DAI1_PBEN09 set as output */

	SRU(DAI0_PB06_O, SPT0_ACLK_I);  /* PCGA to SPORT0 CLK (CLK)  */
	SRU(DAI0_PB05_O, SPT0_AFS_I);   /* PCGA to SPORT0 FS (FS)    */
	SRU(SPT0_AD0_O, DAI0_PB02_I);   /* SPORT 0A digital to A2B   */
	SRU(HIGH, DAI0_PBEN02_I);       /* DAI0_PBEN02 set as output */
}
