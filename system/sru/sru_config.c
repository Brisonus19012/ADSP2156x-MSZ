/*
 **
 ** sru_config.c source file generated on ÎåÔÂ 27, 2020 at 10:44:55.	
 **
 ** Copyright (C) 2014-2020 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the SRU Configuration editor. Changes to the SRU configuration should
 ** be made by changing the appropriate options rather than editing this file.
 **
 ** Only registers with non-default values are written to this file.
 **
 */
 
#include <stdint.h>
#include <sys/platform.h>

int32_t adi_SRU_Init(void);

/*
 * Initialize the Signal Routing Unit
 */
int32_t adi_SRU_Init(void)
{
    /* SPT2B_CLK_I, SPT0B_CLK_I, SPT1B_CLK_I, SPT1A_CLK_I, SPT0A_CLK_I, SPT2A_CLK_I */
    *pREG_DAI0_CLK0 = (unsigned int) 0x252630c5;

    /* SPT1A_D0_I, SPT0B_D0_I, SPT0B_D1_I, SPT0A_D0_I, SPT0A_D1_I */
    *pREG_DAI0_DAT0 = (unsigned int) 0x08144083;

    /* SPT1A_FS_I, SPT2A_FS_I, SPT0B_FS_I, SPT2B_FS_I, SPT0A_FS_I, SPT1B_FS_I */
    *pREG_DAI0_FS0 = (unsigned int) 0x2736b4e4;

    /* DAI0_PB02_I, DAI0_PB01_I, DAI0_PB04_I, DAI0_PB03_I */
    *pREG_DAI0_PIN0 = (unsigned int) 0x0fc54a7e;

    /* DAI0_PB06_I, DAI0_PB05_I, DAI0_PB08_I, DAI0_PB07_I */
    *pREG_DAI0_PIN1 = (unsigned int) 0x04e85c3a;

    /* DAI0_PBEN07_I, DAI0_PBEN06_I, DAI0_PBEN09_I, DAI0_PBEN08_I, DAI0_PBEN10_I */
    *pREG_DAI0_PBEN1 = (unsigned int) 0x1348d301;

    /* DAI0_PBEN03_I, DAI0_PBEN02_I, DAI0_PBEN01_I, DAI0_PBEN05_I, DAI0_PBEN04_I */
    *pREG_DAI0_PBEN0 = (unsigned int) 0x0128b000;

    /* SPT4A_CLK_I, SPT5A_CLK_I, SPT4B_CLK_I, SPT5B_CLK_I, SPT6B_CLK_I, SPT6A_CLK_I */
    *pREG_DAI1_CLK0 = (unsigned int) 0x25261cc7;

    /* SPT5A_D1_I, SPT5B_D0_I, SPT6A_D0_I, SPT5B_D1_I, SPT6A_D1_I */
    *pREG_DAI1_DAT1 = (unsigned int) 0x0f38b281;

    /* SPT4B_D1_I, SPT4B_D0_I, SPT5A_D0_I, SPT4A_D1_I, SPT4A_D0_I */
    *pREG_DAI1_DAT0 = (unsigned int) 0x00144248;

    /* SPT6B_FS_I, SPT4B_FS_I, SPT5A_FS_I, SPT6A_FS_I, SPT5B_FS_I, SPT4A_FS_I */
    *pREG_DAI1_FS0 = (unsigned int) 0x273698e6;

    /* DAI1_PB19_I, DAI1_PB18_I, DAI1_PB17_I, DAI1_PB20_I */
    *pREG_DAI1_PIN4 = (unsigned int) 0x0fc94f9e;

    /* DAI1_PB09_I, DAI1_PB12_I, DAI1_PB11_I, DAI1_PB10_I */
    *pREG_DAI1_PIN2 = (unsigned int) 0x03668a94;

    /* DAI1_PB04_I, DAI1_PB02_I, DAI1_PB03_I, DAI1_PB01_I */
    *pREG_DAI1_PIN0 = (unsigned int) 0x0fdf8c98;

    /* DAI1_PB08_I, DAI1_PB06_I, DAI1_PB07_I, DAI1_PB05_I */
    *pREG_DAI1_PIN1 = (unsigned int) 0x070ebf7e;

    /* DAI1_PBEN20_I, DAI1_PBEN17_I, DAI1_PBEN16_I, DAI1_PBEN19_I, DAI1_PBEN18_I */
    *pREG_DAI1_PBEN3 = (unsigned int) 0x0171f79b;

    /* DAI1_PBEN10_I, DAI1_PBEN09_I, DAI1_PBEN06_I, DAI1_PBEN08_I, DAI1_PBEN07_I */
    *pREG_DAI1_PBEN1 = (unsigned int) 0x0b28104f;

    /* DAI1_PBEN05_I, DAI1_PBEN01_I, DAI1_PBEN02_I, DAI1_PBEN03_I, DAI1_PBEN04_I */
    *pREG_DAI1_PBEN0 = (unsigned int) 0x0e0004d2;


    return 0;
}

