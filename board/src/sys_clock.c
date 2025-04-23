#include "S32K144.h"

void Enable_FIRC(void){
	SCG->FIRCCSR |= SCG_FIRCCSR_FIRCEN_MASK; //enable firc o bit 0
	while (!(SCG->FIRCCSR & SCG_FIRCCSR_FIRCEN_MASK)); // cho firc on dinh
}

void Enable_SOSC_8MHz(void) {
	SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV1(1)
				  | SCG_SOSCDIV_SOSCDIV2(1); // Chia cho 1

    SCG->SOSCCSR &= ~SCG_SOSCCSR_SOSCEN_MASK;  // Tat SOSC truoc khi cau hinh

    SCG->SOSCCFG = SCG_SOSCCFG_RANGE(2)  // Chon dai tan 8-40MHz
                 | SCG_SOSCCFG_EREFS_MASK; // Dung thach anh ngoai

    while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK); // Cho cho CSR hoat dong
    SCG->SOSCCSR = SCG_SOSCCSR_SOSCEN_MASK;  // Bat SOSC
    while (!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK));  // Cho SOSC on dinh
}

void Enable_SPLL_160MHz(void) {
	while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); // Cho cho CSR hoat dong
    SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLEN_MASK;  // Tat SPLL truoc khi cau hinh

    SCG->SPLLDIV |= SCG_SPLLDIV_SPLLDIV1(2) // SPLL div by 2
    				| SCG_SPLLDIV_SPLLDIV2(3); // SPLL div by 3

    SCG->SPLLCSR = (SCG->SPLLCSR & ~(1 << 2)) | (0 << 2); //Chon SOSC lam nguon clock
//    SCG->SPLLCSR = (SCG->SPLLCSR & ~(1 << 2)) | (2 << 2); //Chon FIRC lam nguon clock

    SCG->SPLLCFG = SCG_SPLLCFG_MULT(24)  // Nhan thach anh 8MHz len 112MHz
                 | SCG_SPLLCFG_PREDIV(1); // Chia truoc 1

    while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); // Cho cho CSR hoat dong
    SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK;  // Bat SPLL
    while (!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK));  // Cho SPLL on dinh
}

void Set_System_Clock(void) {
	SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(1)
				  | SCG_SIRCDIV_SIRCDIV2(1);

    SCG->RCCR = SCG_RCCR_SCS(6)    // Chon SPLL lam clock chinh
              | SCG_RCCR_DIVCORE(1) // Core Clock = 80MHz
              | SCG_RCCR_DIVBUS(1)  // Bus Clock = 80MHz
              | SCG_RCCR_DIVSLOW(2);// Slow Clock = 40MHz

    while(((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) != 6); // Cho cho sys clk = SPLL
}

void Init_Clock_System(void) {
//    Enable_FIRC();        // B1: Bat FIRC (48MHz)
    Enable_SOSC_8MHz();        // B2: Bat thach anh ngoai (8MHz)
    Enable_SPLL_160MHz();        // B3: Bat PLL (80MHz)
    Set_System_Clock();   // B4: bat clock he thong
}
