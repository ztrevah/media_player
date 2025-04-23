#include "ADC.h"
#include "S32K144.h"
#include "stdint.h"

void ADC_init(void) {
	PCC->PCCn[PCC_ADC0_INDEX] &=~ PCC_PCCn_CGC_MASK; /* Disable clock to change PCS */
	PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(1) /* PCS=1: Select SOSCDIV2 */
								|PCC_PCCn_CGC_MASK; /* Enable bus clock in ADC */

	ADC0->SC1[0] = ADC_SC1_ADCH_MASK; /* ADCH=1F: Module is disabled for conversions*/
							/* AIEN=0: Interrupts are disabled */

	ADC0->CFG1 = ADC_CFG1_MODE(1); /* ADICLK=0: Input clk=ALTCLK1=SOSCDIV2 */
								/* ADIV=0: Prescaler=1 */
								/* MODE=1: 12-bit conversion */
	ADC0->CFG2 = ADC_CFG2_SMPLTS(12); /* SMPLTS=12(default): sample time is 13 ADC clks */

	ADC0->SC2 = ADC_SC2_REFSEL(0); /* ADTRG=0: SW trigger */
							/* ACFE,ACFGT,ACREN=0: Compare func disabled */
							/* DMAEN=0: DMA disabled */
							/* REFSEL=0: Voltage reference pins= VREFH, VREEFL */
	ADC0->SC3 = 0x00000000; /* CAL=0: Do not start calibration sequence */
							/* ADCO=0: One conversion performed */
							/* AVGE,AVGS=0: HW average function disabled */
}

void convertAdcChan(uint16_t adcChan) { /* For SW trigger mode, SC1[0] is used */
	ADC0->SC1[0]&=~ADC_SC1_ADCH_MASK; /* Clear prior ADCH bits */
	ADC0->SC1[0] = ADC_SC1_ADCH(adcChan); /* Initiate Conversion*/
}

uint8_t adc_complete(void) {
	return (uint8_t) ((ADC0->SC1[0] & ADC_SC1_COCO_MASK)>>ADC_SC1_COCO_SHIFT); /* Wait for completion */
}

uint32_t read_adc_chx(void) {
	uint16_t adc_result=0;
	adc_result=ADC0->R[0]; /* For SW trigger mode, R[0] is used */
	return (uint32_t) ((100*adc_result)/0xFFF); /* Convert result to percentage for 0-100 range */
}

void PORT_ADC_init (void) {
	PCC->PCCn[PCC_PORTD_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTD */
	PORTD->PCR[0] = 1 << 8; /* Port D0: MUX = GPIO */
	PORTD->PCR[15] = 1 << 8; /* Port D15: MUX = GPIO */
	PORTD->PCR[16] = 1 << 8; /* Port D16: MUX = GPIO */
	PTD->PDDR |= 1 << 0; /* Port D0: Data Direction= output */
	PTD->PDDR |= 1 << 15; /* Port D15: Data Direction= output */
	PTD->PDDR |= 1 << 16; /* Port D16: Data Direction= output */
}
