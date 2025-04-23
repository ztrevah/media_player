#ifndef GPIO_H
#define GPIO_H

void Init_GPIO(void);

void Init_PORTC_IRQ(void);

void delay_ms(uint32_t ms);

void PORTC_IRQHandler(void);

#endif
