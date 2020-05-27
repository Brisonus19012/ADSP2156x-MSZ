/**
 *
 */

#ifndef __FIRMSPORT_H__
#define __FIRMSPORT_H__

/*============= I N C L U D E S =============*/

 /*!< ADI SPORT(Serial Port) Device driver definitions include file */
#include <drivers/sport/adi_sport.h>
#include <services/int/adi_int.h>  /* Interrupt Handler API header. */

/*============== D E F I N E S ===============*/

#define SAMPLE_RATE   			        (48000u)       /* ADC/DAC sample rate */

/**
 * TODO:[Tips]If you want to modify the sampling rate of 2khz
 * 			  please modify REFERENCE_FREQ
 */
#define REFERENCE_FREQ 				    (2000u)        /* uint : hz */

 /* Number of sampling points per channel */
#define SAMPLES_PER_PERIOD 			    (SAMPLE_RATE) / (REFERENCE_FREQ)

#define TxNUM_CHANNELS				    (24u)          /* Number of A2B slot     */
#define RxNUM_CHANNELS				    (8u)           /* Number of ADC/DAC slot */

/* Macro to set buffer size */
#define A2B_BUFFER_SIZE 	            (SAMPLES_PER_PERIOD * TxNUM_CHANNELS)
#define DAC_BUFFER_SIZE 	            (SAMPLES_PER_PERIOD * RxNUM_CHANNELS)
#define ADC_BUFFER_SIZE 	            (SAMPLES_PER_PERIOD * RxNUM_CHANNELS)

#define SPORT_DEVICE_5A 			    (5u)			/* SPORT device number */
#define SPORT_DEVICE_4A 			    (4u)			/* SPORT device number */
#define SPORT_DEVICE_0A 			    (0u)			/* SPORT device number */

#define DMA_NUM_DESC 				    (2u)

extern uint8_t ucfInputReady;

void vfProcessAudioBlocks(void);
uint32_t ulfSPortInit(void);
uint32_t ulfSPortClose(void);


#endif /* __FIRMSPORT_H__ */

