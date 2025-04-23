#include "S32K144.h"
#include "PWM.h"
#include "interrupt_manager.h"

int countPWM = 0;

void Edge_Align_PWM_Init(void)
{
	/* Enable clock for PORTD */
	PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK;

	/* Select and enable clock for FTM0 */
	PCC->PCCn[PCC_FTM0_INDEX] = PCC_PCCn_PCS(6) | PCC_PCCn_CGC_MASK;

	/* Set PORTD pins for FTM0 */
	PORTD->PCR[15] = PORT_PCR_MUX(2); // FTM0, Channel0
	PORTD->PCR[16] = PORT_PCR_MUX(2); // FTM0, Channel1
	PORTD->PCR[0] = PORT_PCR_MUX(2); // FTM0, Channel2

	/* Enable registers updating from write buffers */
	FTM0->MODE = FTM_MODE_FTMEN_MASK;

	/* Enable sync, combine mode and dead-time for pair channel n=0 and n=1 */
	FTM0->COMBINE = FTM_COMBINE_COMP0_MASK | FTM_COMBINE_DTEN0_MASK
					| FTM_COMBINE_COMP1_MASK | FTM_COMBINE_DTEN1_MASK;

	/* Set Modulo in initialization stage (8kHz PWM frequency @112MHz system clock) */
	FTM0->MOD = FTM_MOD_MOD(8000-1);

	/* Set CNTIN in initialization stage */
	FTM0->CNTIN = FTM_CNTIN_INIT(0);

	/* Enable high-true pulses of PWM signals */
	FTM0->CONTROLS[0].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	FTM0->CONTROLS[1].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	FTM0->CONTROLS[2].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;

	/* Set channel value in initialization stage */
	FTM0->CONTROLS[0].CnV=FTM_CnV_VAL(1000); // 50% duty cycle
	FTM0->CONTROLS[1].CnV=FTM_CnV_VAL(7000); // 50% duty cycle
	FTM0->CONTROLS[2].CnV=FTM_CnV_VAL(1000); // 25% duty cycle

	/* Reset FTM counter */
	FTM0->CNT = 0;

	/* Insert deadtime (1us) */
	FTM0->DEADTIME = FTM_DEADTIME_DTPS(3) | FTM_DEADTIME_DTVAL(7);

	/* Select clock, enable reload opportunity interrupt */
	FTM0->SC |= FTM_SC_CLKS(1) | FTM_SC_RIE_MASK;

	/* Enable reload opportunity interrupt when FTM counter reach CNTMAX value */
	FTM0->SYNC |= FTM_SYNC_SYNCHOM_MASK | FTM_SYNC_CNTMAX_MASK;

	/* Allow each fourth reload opportunity interrupt */
	FTM0->CONF = FTM_CONF_LDFQ(3);

	/* Enable software synchronization */
	FTM0->SYNCONF = FTM_SYNCONF_SYNCMODE_MASK | FTM_SYNCONF_SWWRBUF_MASK | FTM_SYNCONF_SWOM_MASK |
						FTM_SYNCONF_SWRSTCNT_MASK;

	/* Clock selection and enabling PWM generation */
	FTM0->SC |= FTM_SC_CLKS(1) | FTM_SC_PWMEN0_MASK | FTM_SC_PWMEN1_MASK | FTM_SC_PWMEN2_MASK |
			FTM_SC_RIE_MASK;

	INT_SYS_EnableIRQ(FTM0_Ovf_Reload_IRQn);
}

void FTM0_Ovf_Reload_IRQHandler(void)
{
	countPWM++;
	if(countPWM > 7999) countPWM = 0;
	/* Set FTM0_CH0/FTM0_CH1 output polarity to low */
	FTM0->POL &= (~FTM_POL_POL0_MASK) & (~FTM_POL_POL1_MASK);

	/* Disable FTM0_CH0/FTM0_CH1 mask */
	FTM0->OUTMASK &= (~FTM_OUTMASK_CH0OM_MASK) & (~FTM_OUTMASK_CH1OM_MASK);
	FTM0->CONTROLS[0].CnV=FTM_CnV_VAL(countPWM);
	FTM0->CONTROLS[1].CnV=FTM_CnV_VAL(countPWM);
	FTM0->CONTROLS[2].CnV=FTM_CnV_VAL(countPWM);

	FTM0->SYNC |= FTM_SYNC_SWSYNC_MASK; // Software sync
	FTM0->SC &= ~FTM_SC_RF_MASK; // Clear Reload Flag bit
}
