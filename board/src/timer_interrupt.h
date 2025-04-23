#ifndef TIMER_INTERRUPT_H
#define TIMER_INTERRUPT_H

void NVIC_Init_IRQs(void);

void LPTMR0_IRQHandler(void);

void PORTC_IRQHandler(void);

void LPTMR0_Init(void);

#endif
