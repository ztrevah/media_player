#include "S32K144.h"
#include "LPUART.h"

void Init_GPIO(void){
	PCC->PCCn[PCC_PORTC_INDEX] = (1 << 30);
//	PCC->PCCn[PCC_PORTD_INDEX] = (1 << 30);

//	PORTD->PCR[0] |= (1 << 8);
//	PORTD->PCR[15] |= (1 << 8);
//	PORTD->PCR[16] |= (1 << 8);

	PORTC->PCR[13] |= (1 << 8) | // Chon lam GPIO
					PORT_PCR_PE_MASK | //Bat Pull-up / Pull-down
					PORT_PCR_PS_MASK | // Chon Pull-up
					PORT_PCR_IRQC(0xA); // Ngat suon xuong

	PORTC->PCR[12] |= (1 << 8) | //chon lam GPIO
					PORT_PCR_PE_MASK | // Bat Pull-up / Pull-down
					PORT_PCR_PS_MASK | // Chon Pull-up
					PORT_PCR_IRQC(0xA); // Ngat suon xuong

//	PTD->PDDR |= (1 << 0) | (1 << 15) | (1 << 16); // Chon PORTC chan 0, 15, 16 lam output: 1

	PTC->PDDR &= ~(1 << 13); // Chon PORTC chan 13 lam input: 0
	PTC->PDDR &= ~(1 << 12); // Chon PORTC chan 12 lam input: 0

//	PTD->PSOR |= (1 << 0) | (1 << 15) | (1 << 16);
}

void Init_PORTC_IRQ(void){
	// Dang ky ngat input cho PORTC
	S32_NVIC->ICPR[1] = 1 << (61 % 32);
	S32_NVIC->ISER[1] = 1 << (61 % 32);
	S32_NVIC->IP[61] = 0x0A;
}

void delay_ms(uint32_t ms) {
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 5000; j++) {
            __asm("nop");
        }
    }
}

void PORTC_IRQHandler(void) {
    static uint8_t flag_btn12 = 0;
    static uint8_t flag_btn13 = 0;

    if(PORTC->ISFR & (1 << 13)) {
        PORTC->ISFR |= (1 << 13);

        if (flag_btn13 == 0) {
            flag_btn13 = 1;
            delay_ms(200);
            if (PORTC->ISFR & (1 << 13)) {
                PORTC->ISFR |= (1 << 13);
                LPUART1_transmit_string("T.");
            } else {
                LPUART1_transmit_string("N.");
            }
            flag_btn13 = 0;
        }
    }

    if(PORTC->ISFR & (1 << 12)) {
        PORTC->ISFR |= (1 << 12);

        if (flag_btn12 == 0) {
            flag_btn12 = 1;
            delay_ms(200);
            if (PORTC->ISFR & (1 << 12)) {
                PORTC->ISFR |= (1 << 12);
                LPUART1_transmit_string("S.");
            } else {
                LPUART1_transmit_string("P.");
            }
            flag_btn12 = 0;
        }
    }
}
