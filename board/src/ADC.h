#ifndef ADC_H
#define ADC_H

#include "stdint.h"

void ADC_init(void);

void convertAdcChan(uint16_t adcChan);

uint8_t adc_complete(void);

uint32_t read_adc_chx(void);

void PORT_ADC_init (void);

#endif
