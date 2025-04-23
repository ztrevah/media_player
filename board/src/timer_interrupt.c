#include "S32K144.h"

volatile int count = 0;

void NVIC_Init_IRQs(void){
	// Dang ky ngat timer
	S32_NVIC->ICPR[1] = 1 << (58 % 32); //xoa trang thai pending cua ngat 58: LPTMR0
	S32_NVIC->ISER[1] = 1 << (58 % 32); //cho phep ngat 58
	S32_NVIC->IP[58] = 0x02; //dat muc uu tien cho ngat 58: 10 (1-15)

	// Dang ky ngat input
	S32_NVIC->ICPR[1] = 1 << (61 % 32);
	S32_NVIC->ISER[1] = 1 << (61 % 32);
	S32_NVIC->IP[61] = 0x0A;
}

void LPTMR0_IRQHandler(void){
    /* Xoa co ngat */
	LPTMR0->CSR |= (1 << 7);// TCF = 1 (Clear Interrupt Flag)
    switch (count){
    case 0: {
    	PTD->PTOR = (1 << 15);
    	break;
    }
    case 1: {
    	PTD->PTOR = (1 << 16);
    	break;
    }
    case 2: {
    	PTD->PTOR = ((1 << 15) | (1 << 16));
    	break;
    }
    default: {
    	break;
    }
    }
    count = (count + 1) % 3;
}

//void PORTC_IRQHandler(void){
//	if(PORTC->ISFR & (1 << 13)){ // Kiem tra chan 13 co kich hoat ngat k
//		PORTC->ISFR |= (1 << 13); // Xoa co ngat
//		LPTMR0->CSR &= ~(1 << 0); // Tat LPTMR0 truoc khi thay doi clock
//		LPTMR0->CMR = 5000; // Cap nhat clock
//		LPTMR0->CSR |= (1 << 0); // Bat lai LPTMR0
//	}
//	if(PORTC->ISFR & (1 << 12)){
//		PORTC->ISFR |= (1 << 12);
//		LPTMR0->CSR &= ~(1 << 0);
//		LPTMR0->CMR = 2000;
//		LPTMR0->CSR |= (1 << 0);
//	}
//}

void LPTMR0_Init(void){
	PCC->PCCn[PCC_LPTMR0_INDEX] |= (1 << 30); // bat clock cho LPTMR0
	PCC->PCCn[PCC_LPTMR0_INDEX] |= PCC_PCCn_PCS(3);  /* Chon nguon clock 1 kHz (LPO_CLK) */

	LPTMR0->CSR &= ~(1 << 0); //tat timer truoc khi cau hinh

	LPTMR0->PSR = (1 << 2) | (1 << 0); // chon clock LPO 1kHz

	LPTMR0->CMR = 2000; //dinh thoi 1000ms

    LPTMR0->CSR |= (1 << 6);  // Cho phep ngat
    LPTMR0->CSR |= (1 << 0);  // Bat Timer (TEN = 1)
}
