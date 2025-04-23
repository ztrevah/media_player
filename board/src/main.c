/* Including necessary configuration files. */
#include "sdk_project_config.h"

volatile int exit_code = 0;

/* User includes */

#include "S32K144.h"
#include "interrupt_manager.h"
#include "device_registers.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

/* User library */
#include "sys_clock.h"
#include "timer_interrupt.h"
#include "GPIO.h"
#include "LPUART.h"
#include "PWM.h"
#include "ADC.h"

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/


uint32_t adcResultInPer_new = 0;
uint32_t adcResultInPer_old = 0;

void WDOG_disable (void)
{
  WDOG->CNT = 0xD928C520;     /* Unlock watchdog */
  WDOG->TOVAL = 0x0000FFFF;   /* Maximum timeout value */
  WDOG->CS = 0x00002100;    /* Disable watchdog */
}

int main(void)
{
    /* Write your code here */
	WDOG_disable();
	Init_Clock_System();

	Init_GPIO();
	Init_PORTC_IRQ();

//	LPTMR0_Init();
//	PTD->PTOR |= (1 << 16);

//	NVIC_Init_IRQs();

	PORT_init();
	LPUART1_init();

//	Edge_Align_PWM_Init();

//	PORT_ADC_init();
	ADC_init();
	char adcResultInStr[20];

	while (1) {
		convertAdcChan(12); /* Convert Channel AD12 to pot on EVB */

		while(adc_complete()==0){} /* Wait for conversion complete flag */

		adcResultInPer_new = read_adc_chx(); /* Get channel's conversion results in percentage */

		if(adcResultInPer_new - adcResultInPer_old >= 1) {
			sprintf(adcResultInStr, "%lu",100 - adcResultInPer_new);
			LPUART1_transmit_string(strcat(adcResultInStr, "."));
		}

		adcResultInPer_old = adcResultInPer_new;
		delay_ms(100);
	}
    return exit_code;
}

/* END main */
/*!
** @}
*/
